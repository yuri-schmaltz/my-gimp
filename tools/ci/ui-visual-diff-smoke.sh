#!/bin/sh

set -eu

baseline_dir="${UI_VISUAL_BASELINE_DIR:-devel-docs/reference/gimp-ui/images}"
current_dir="${UI_VISUAL_CURRENT_DIR:-_log/ui-visual/current}"
sample_count="${UI_VISUAL_SMOKE_SAMPLE_COUNT:-0}"
ssim_min="${UI_VISUAL_SMOKE_SSIM_MIN:-1.0}"

if [ ! -d "$baseline_dir" ]; then
  echo "FAIL: baseline dir not found: $baseline_dir"
  exit 1
fi

mkdir -p "$current_dir"

tmp_list="$(mktemp)"
cleanup() {
  rm -f "$tmp_list"
}
trap cleanup EXIT INT TERM

if [ "$sample_count" -gt 0 ]; then
  find "$baseline_dir" -maxdepth 1 -type f -name '*.png' \
    | sort \
    | head -n "$sample_count" > "$tmp_list"
else
  find "$baseline_dir" -maxdepth 1 -type f -name '*.png' \
    | sort > "$tmp_list"
fi

picked="$(wc -l < "$tmp_list" | tr -d ' ')"
if [ "$picked" -eq 0 ]; then
  echo "FAIL: no baseline PNG files found for smoke run."
  exit 1
fi

while IFS= read -r src; do
  [ -n "$src" ] || continue
  image="$(basename "$src")"
  cp -f "$src" "$current_dir/$image"
done < "$tmp_list"

echo "UI visual diff smoke prepared with $picked images."

UI_VISUAL_REQUIRE_CURRENT=1 \
UI_VISUAL_SSIM_MIN="$ssim_min" \
sh tools/ci/ui-visual-diff.sh
