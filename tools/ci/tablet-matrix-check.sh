#!/bin/sh

set -eu

doc="devel-docs/tablet-input-validation-matrix.md"
require_results="${TABLET_MATRIX_REQUIRE_RESULTS:-0}"
fail=0

if [ ! -f "$doc" ]; then
  echo "FAIL: tablet matrix doc not found: $doc"
  exit 1
fi

check_pattern() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$doc" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (pattern: %s)\n" "$label" "$pattern"
      fail=1
    fi
  else
    if grep -En "$pattern" "$doc" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (pattern: %s)\n" "$label" "$pattern"
      fail=1
    fi
  fi
}

echo "Tablet matrix checklist"
echo

check_pattern "Linux" "Platform Linux present"
check_pattern "Windows" "Platform Windows present"
check_pattern "Pressure" "Pressure coverage present"
check_pattern "Tilt" "Tilt coverage present"
check_pattern "Eraser" "Eraser coverage present"
check_pattern "Stylus button" "Stylus button coverage present"
check_pattern "Expected result" "Expected result column present"
check_pattern "Execution log template" "Execution log template present"
check_pattern "Exit criteria" "Exit criteria present"

if command -v rg >/dev/null 2>&1; then
  case_count="$(rg -o "TBL-[0-9]{3}" "$doc" | sort -u | wc -l | tr -d ' ')"
else
  case_count="$(grep -oE "TBL-[0-9]{3}" "$doc" | sort -u | wc -l | tr -d ' ')"
fi

echo "Detected unique tablet case IDs: $case_count"
if [ "$case_count" -lt 12 ]; then
  echo "FAIL: expected at least 12 unique tablet case IDs."
  fail=1
fi

if [ "$require_results" = "1" ]; then
  echo "Strict mode enabled: TABLET_MATRIX_REQUIRE_RESULTS=1"
  if command -v rg >/dev/null 2>&1; then
    if rg -n "PENDING" "$doc" >/dev/null 2>&1; then
      echo "FAIL: matrix still contains PENDING entries in strict mode."
      fail=1
    fi
  else
    if grep -n "PENDING" "$doc" >/dev/null 2>&1; then
      echo "FAIL: matrix still contains PENDING entries in strict mode."
      fail=1
    fi
  fi
fi

echo
if [ "$fail" -ne 0 ]; then
  echo "FAIL: tablet matrix checklist failed."
  exit 1
fi

echo "PASS: tablet matrix checklist passed."
