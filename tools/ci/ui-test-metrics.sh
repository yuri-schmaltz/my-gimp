#!/bin/sh

set -eu

junit_file="$(find . -type f -path './_build-*/meson-logs/testlog.junit.xml' | head -1 || true)"

if [ -z "$junit_file" ]; then
  if [ -n "${CI:-}" ]; then
    echo "FAIL: testlog.junit.xml not found in CI artifacts."
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
