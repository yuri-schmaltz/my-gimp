#!/bin/sh

set -eu

junit_file="$(find . -type f -path './_build-*/meson-logs/testlog.junit.xml' | head -1 || true)"

if [ -z "$junit_file" ]; then
  if [ -n "${CI:-}" ]; then
    echo "FAIL: testlog.junit.xml not found in CI artifacts."
    exit 1
  fi

  echo "No testlog.junit.xml found locally; skipping B06 JUnit validation."
  exit 0
fi

required_tests="
/gimp-path-editor/roundtrip
/gimp-path-editor/reorder-buttons
/gimp-path-editor/add-and-delete
/gimp-path-editor/writable-toggle
"

echo "B06 path editor JUnit validation: $junit_file"
echo

test_fail=0
for testcase in $required_tests; do
  if grep -q "name=\"$testcase\"" "$junit_file"; then
    printf "PASS  testcase present: %s\n" "$testcase"
  else
    printf "FAIL  testcase missing: %s\n" "$testcase"
    test_fail=1
  fi
done

echo
if [ "$test_fail" -ne 0 ]; then
  echo "FAIL: required B06 testcases missing from JUnit report."
  exit 1
fi

if awk '
  BEGIN { in_case = 0; fail = 0; current = "" }
  /<testcase / {
    in_case = 0
    current = ""
    if (match($0, /name="[^"]+"/)) {
      current = substr($0, RSTART + 6, RLENGTH - 7)
      if (current ~ /^\/gimp-path-editor\//) {
        in_case = 1
      }
    }
  }
  /<failure/ {
    if (in_case) {
      printf("FAIL  testcase failed: %s\n", current)
      fail = 1
    }
  }
  /<error/ {
    if (in_case) {
      printf("FAIL  testcase error: %s\n", current)
      fail = 1
    }
  }
  END { exit fail }
' "$junit_file"; then
  echo "PASS: all B06 path-editor testcases passed in JUnit."
else
  echo "FAIL: at least one B06 path-editor testcase failed in JUnit."
  exit 1
fi
