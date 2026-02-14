#!/bin/sh

set -eu

manifest="${ICC_MANIFEST:-tools/ci/fixtures/color-icc/corpus.txt}"
expected_dir="${ICC_EXPECTED_DIR:-tools/ci/fixtures/color-icc/expected}"
actual_dir="${ICC_ACTUAL_DIR:-_log/color-icc/actual}"
report_dir="${ICC_REPORT_DIR:-_log/color-icc/report}"
ssim_min="${ICC_SSIM_MIN:-0.999}"
require_actual="${ICC_REQUIRE_ACTUAL:-0}"

if [ ! -f "$manifest" ]; then
  echo "FAIL: ICC manifest not found: $manifest"
  exit 1
fi

if [ ! -d "$expected_dir" ]; then
  echo "FAIL: ICC expected dir not found: $expected_dir"
  exit 1
fi

if [ ! -d "$actual_dir" ]; then
  if [ "$require_actual" = "1" ]; then
    echo "FAIL: ICC actual dir not found: $actual_dir"
    exit 1
  fi

  echo "No ICC actual dir found at $actual_dir; skipping dynamic ICC diff."
  exit 0
fi

if ! command -v ffmpeg >/dev/null 2>&1; then
  if [ "$require_actual" = "1" ]; then
    echo "FAIL: ffmpeg command not found and ICC diff is required."
    exit 1
  fi

  echo "ffmpeg not found; skipping dynamic ICC diff."
  exit 0
fi

tmp_cases="$(mktemp)"
cleanup() {
  rm -f "$tmp_cases"
}
trap cleanup EXIT INT TERM

grep -v '^[[:space:]]*#' "$manifest" \
  | sed '/^[[:space:]]*$/d' > "$tmp_cases"

case_count="$(wc -l < "$tmp_cases" | tr -d ' ')"
if [ "$case_count" -eq 0 ]; then
  if [ "$require_actual" = "1" ]; then
    echo "FAIL: ICC manifest has no enabled cases: $manifest"
    exit 1
  fi

  echo "No ICC cases enabled in $manifest; skipping dynamic ICC diff."
  exit 0
fi

mkdir -p "$report_dir"
report_csv="$report_dir/ssim.csv"
printf "case;expected;actual;ssim;status\n" > "$report_csv"

echo "ICC dynamic diff gate"
echo "Manifest: $manifest"
echo "Expected dir: $expected_dir"
echo "Actual dir: $actual_dir"
echo "SSIM threshold: $ssim_min"
echo

fail=0
total=0
below=0
missing=0

while IFS= read -r line; do
  [ -n "$line" ] || continue

  set -- $line

  if [ "$#" -ne 3 ]; then
    echo "FAIL: invalid ICC case line (expected 3 fields): $line"
    fail=1
    continue
  fi

  case_id="$1"
  expected_rel="$2"
  actual_rel="$3"
  expected_file="$expected_dir/$expected_rel"
  actual_file="$actual_dir/$actual_rel"

  total=$((total + 1))

  if [ ! -f "$expected_file" ]; then
    printf "FAIL  %s (missing expected image: %s)\n" "$case_id" "$expected_file"
    printf "%s;%s;%s;NA;missing-expected\n" "$case_id" "$expected_rel" "$actual_rel" >> "$report_csv"
    fail=1
    continue
  fi

  if [ ! -f "$actual_file" ]; then
    printf "FAIL  %s (missing actual image: %s)\n" "$case_id" "$actual_file"
    printf "%s;%s;%s;NA;missing-actual\n" "$case_id" "$expected_rel" "$actual_rel" >> "$report_csv"
    missing=$((missing + 1))
    fail=1
    continue
  fi

  if ffmpeg_output="$(ffmpeg -hide_banner -nostdin \
                      -i "$expected_file" \
                      -i "$actual_file" \
                      -lavfi ssim \
                      -f null - 2>&1)"; then
    ssim_value="$(printf '%s\n' "$ffmpeg_output" \
      | sed -n 's/.*All:\([0-9.]*\).*/\1/p' \
      | tail -n 1)"
  else
    printf "FAIL  %s (ffmpeg compare error)\n" "$case_id"
    printf "%s;%s;%s;NA;ffmpeg-error\n" "$case_id" "$expected_rel" "$actual_rel" >> "$report_csv"
    fail=1
    continue
  fi

  if [ -z "$ssim_value" ]; then
    printf "FAIL  %s (unable to parse SSIM)\n" "$case_id"
    printf "%s;%s;%s;NA;parse-error\n" "$case_id" "$expected_rel" "$actual_rel" >> "$report_csv"
    fail=1
    continue
  fi

  if awk "BEGIN { exit !($ssim_value >= $ssim_min) }"; then
    printf "PASS  %s (SSIM=%s)\n" "$case_id" "$ssim_value"
    printf "%s;%s;%s;%s;pass\n" "$case_id" "$expected_rel" "$actual_rel" "$ssim_value" >> "$report_csv"
  else
    printf "FAIL  %s (SSIM=%s < %s)\n" "$case_id" "$ssim_value" "$ssim_min"
    printf "%s;%s;%s;%s;below-threshold\n" "$case_id" "$expected_rel" "$actual_rel" "$ssim_value" >> "$report_csv"
    below=$((below + 1))
    fail=1
  fi
done < "$tmp_cases"

echo
echo "Compared ICC cases: $total"
echo "Missing actual images: $missing"
echo "Below-threshold cases: $below"
echo "Report: $report_csv"

if [ "$fail" -ne 0 ]; then
  echo "FAIL: ICC dynamic diff gate failed."
  exit 1
fi

echo "PASS: ICC dynamic diff gate passed."
