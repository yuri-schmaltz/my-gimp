#!/bin/sh

set -eu

baseline_dir="${UI_VISUAL_BASELINE_DIR:-devel-docs/reference/gimp-ui/images}"
current_dir="${UI_VISUAL_CURRENT_DIR:-_log/ui-visual/current}"
report_dir="${UI_VISUAL_REPORT_DIR:-_log/ui-visual/report}"
ssim_min="${UI_VISUAL_SSIM_MIN:-0.995}"
require_current="${UI_VISUAL_REQUIRE_CURRENT:-0}"

if [ ! -d "$baseline_dir" ]; then
  echo "FAIL: baseline dir not found: $baseline_dir"
  exit 1
fi

if [ ! -d "$current_dir" ]; then
  if [ "$require_current" = "1" ]; then
    echo "FAIL: current screenshots dir not found: $current_dir"
    exit 1
  fi

  echo "No current screenshots found at $current_dir; skipping visual diff."
  exit 0
fi

if ! command -v ffmpeg >/dev/null 2>&1; then
  if [ "$require_current" = "1" ]; then
    echo "FAIL: ffmpeg command not found and visual diff is required."
    exit 1
  fi

  echo "ffmpeg not found; skipping visual diff."
  exit 0
fi

tmp_images="$(mktemp)"
cleanup() {
  rm -f "$tmp_images"
}
trap cleanup EXIT INT TERM

find "$baseline_dir" -maxdepth 1 -type f -name '*.png' \
  | sed -E 's#^.*/##' \
  | sort -u > "$tmp_images"

image_count="$(wc -l < "$tmp_images" | tr -d ' ')"
if [ "$image_count" -eq 0 ]; then
  echo "FAIL: no PNG images found in baseline dir: $baseline_dir"
  exit 1
fi

mkdir -p "$report_dir"
report_csv="$report_dir/ssim.csv"
printf "image;ssim;status\n" > "$report_csv"

echo "UI visual diff gate"
echo "Baseline dir: $baseline_dir"
echo "Current dir: $current_dir"
echo "SSIM threshold: $ssim_min"
echo

total=0
missing=0
below=0
fail=0

while IFS= read -r image; do
  [ -n "$image" ] || continue

  total=$((total + 1))
  baseline_file="$baseline_dir/$image"
  current_file="$current_dir/$image"

  if [ ! -f "$current_file" ]; then
    printf "MISS  %s (not found in current dir)\n" "$image"
    printf "%s;NA;missing-current\n" "$image" >> "$report_csv"
    missing=$((missing + 1))
    if [ "$require_current" = "1" ]; then
      fail=1
    fi
    continue
  fi

  if ffmpeg_output="$(ffmpeg -hide_banner -nostdin \
                      -i "$baseline_file" \
                      -i "$current_file" \
                      -lavfi ssim \
                      -f null - 2>&1)"; then
    ssim_value="$(printf '%s\n' "$ffmpeg_output" \
      | sed -n 's/.*All:\([0-9.]*\).*/\1/p' \
      | tail -n 1)"
  else
    printf "FAIL  %s (ffmpeg compare error)\n" "$image"
    printf "%s;NA;ffmpeg-error\n" "$image" >> "$report_csv"
    fail=1
    continue
  fi

  if [ -z "$ssim_value" ]; then
    printf "FAIL  %s (unable to parse SSIM)\n" "$image"
    printf "%s;NA;parse-error\n" "$image" >> "$report_csv"
    fail=1
    continue
  fi

  if awk "BEGIN { exit !($ssim_value >= $ssim_min) }"; then
    printf "PASS  %s (SSIM=%s)\n" "$image" "$ssim_value"
    printf "%s;%s;pass\n" "$image" "$ssim_value" >> "$report_csv"
  else
    printf "FAIL  %s (SSIM=%s < %s)\n" "$image" "$ssim_value" "$ssim_min"
    printf "%s;%s;below-threshold\n" "$image" "$ssim_value" >> "$report_csv"
    below=$((below + 1))
    fail=1
  fi
done < "$tmp_images"

echo
echo "Compared images: $total"
echo "Missing current images: $missing"
echo "Below-threshold images: $below"
echo "Report: $report_csv"

if [ "$fail" -ne 0 ]; then
  echo "FAIL: visual diff gate failed."
  exit 1
fi

echo "PASS: visual diff gate passed."
