#!/bin/sh

set -eu

find_junit_file() {
  find . -type f -path './_build-*/meson-logs/testlog.junit.xml' | head -1 || true
}

find_meson_build_dir() {
  for dir in _build*; do
    [ -d "$dir" ] || continue
    if meson introspect --tests "$dir" >/dev/null 2>&1; then
      echo "$dir"
      return 0
    fi
  done

  return 1
}

junit_file="$(find_junit_file)"

if [ -z "$junit_file" ] && [ "${UI_METRICS_AUTO_GENERATE:-0}" = "1" ]; then
  build_dir="$(find_meson_build_dir || true)"

  if [ -n "$build_dir" ]; then
    echo "No testlog.junit.xml found; attempting to generate via meson test in $build_dir."
    if meson test -C "$build_dir" --print-errorlogs --no-rebuild >/dev/null 2>&1; then
      :
    else
      echo "WARN: meson test failed while attempting to generate JUnit."
    fi
  else
    echo "WARN: no valid Meson build dir found for JUnit generation."
  fi

  junit_file="$(find_junit_file)"
fi

if [ -z "$junit_file" ]; then
  if [ -n "${CI:-}" ] || [ "${UI_METRICS_REQUIRED:-0}" = "1" ]; then
    echo "FAIL: testlog.junit.xml not found and metrics are required."
    exit 1
  fi

  echo "No testlog.junit.xml found locally; skipping metrics extraction."
  exit 0
fi

tmp_data="$(mktemp)"
tmp_sorted="$(mktemp)"
cleanup() {
  rm -f "$tmp_data" "$tmp_sorted"
}
trap cleanup EXIT INT TERM

grep "<testcase " "$junit_file" | while IFS= read -r line; do
  class="$(printf '%s\n' "$line" | sed -n 's/.* classname=\"\([^\"]*\)\".*/\1/p')"
  name="$(printf '%s\n' "$line" | sed -n 's/.* name=\"\([^\"]*\)\".*/\1/p')"
  time="$(printf '%s\n' "$line" | sed -n 's/.* time=\"\([^\"]*\)\".*/\1/p')"

  if [ -n "$time" ]; then
    printf '%s;%s;%s\n' "$class" "$name" "$time"
  fi
done > "$tmp_data"

count="$(wc -l < "$tmp_data" | tr -d ' ')"
if [ "$count" -eq 0 ]; then
  if [ -n "${CI:-}" ]; then
    echo "FAIL: no testcase timing entries found in $junit_file"
    exit 1
  fi

  echo "No testcase timing entries found in $junit_file"
  exit 0
fi

required_fail=0
required_tests="
create_new_image_via_dialog
restore_recently_closed_multi_column_dock
switch_to_single_window_mode
opened_xcf_file_files
saved_imported_file_files
exported_file_files
"

echo "Checking required core testcases in JUnit report:"
for testcase in $required_tests; do
  if grep -q "$testcase" "$junit_file"; then
    printf "PASS  %s\n" "$testcase"
  else
    printf "FAIL  %s\n" "$testcase"
    required_fail=1
  fi
done
echo

if [ "$required_fail" -ne 0 ]; then
  echo "FAIL: required UI/core testcases missing from JUnit report."
  exit 1
fi

required_pattern="create_new_image_via_dialog|restore_recently_closed_multi_column_dock|switch_to_single_window_mode|opened_xcf_file_files|saved_imported_file_files|exported_file_files"
if awk -v required_pattern="$required_pattern" '
  BEGIN { in_case = 0; fail = 0; current = "" }
  /<testcase / {
    in_case = 0
    current = ""
    if (match($0, /name="[^"]+"/)) {
      current = substr($0, RSTART + 6, RLENGTH - 7)
      if (current ~ required_pattern) {
        in_case = 1
      }
    }
  }
  /<failure/ {
    if (in_case) {
      printf("FAIL  required testcase failed: %s\n", current)
      fail = 1
    }
  }
  /<error/ {
    if (in_case) {
      printf("FAIL  required testcase error: %s\n", current)
      fail = 1
    }
  }
  END { exit fail }
' "$junit_file"; then
  echo "PASS: required UI/core testcases passed in JUnit."
else
  echo "FAIL: at least one required UI/core testcase failed in JUnit."
  exit 1
fi

echo

sort -t ';' -k3,3n "$tmp_data" > "$tmp_sorted"

idx95=$(( (95 * count + 99) / 100 ))
p95_line="$(sed -n "${idx95}p" "$tmp_sorted")"
p95_time="$(printf '%s\n' "$p95_line" | cut -d ';' -f3)"

echo "UI test timing metrics from: $junit_file"
echo "Total testcases: $count"
echo "Approx p95 testcase time (s): $p95_time"
echo
echo "Top 10 slowest testcases (class;name;time):"
tail -n 10 "$tmp_sorted"
