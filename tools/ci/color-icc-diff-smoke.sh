#!/bin/sh

set -eu

work_root="${ICC_SMOKE_WORK_ROOT:-_log/color-icc-smoke}"
expected_dir="$work_root/expected"
actual_dir="$work_root/actual"
report_dir="$work_root/report"
manifest="$work_root/corpus.txt"
ssim_min="${ICC_SMOKE_SSIM_MIN:-1.0}"

if ! command -v ffmpeg >/dev/null 2>&1; then
  echo "FAIL: ffmpeg command not found; required for ICC smoke run."
  exit 1
fi

mkdir -p "$expected_dir" "$actual_dir" "$report_dir"

ffmpeg -hide_banner -loglevel error -y \
  -f lavfi -i "color=c=red:size=64x64" -frames:v 1 "$expected_dir/case-red.png"
ffmpeg -hide_banner -loglevel error -y \
  -f lavfi -i "color=c=red:size=64x64" -frames:v 1 "$actual_dir/case-red.png"

ffmpeg -hide_banner -loglevel error -y \
  -f lavfi -i "color=c=green:size=64x64" -frames:v 1 "$expected_dir/case-green.png"
ffmpeg -hide_banner -loglevel error -y \
  -f lavfi -i "color=c=green:size=64x64" -frames:v 1 "$actual_dir/case-green.png"

cat > "$manifest" <<'EOF'
icc_smoke_red case-red.png case-red.png
icc_smoke_green case-green.png case-green.png
EOF

echo "ICC diff smoke prepared with 2 cases."

ICC_MANIFEST="$manifest" \
ICC_EXPECTED_DIR="$expected_dir" \
ICC_ACTUAL_DIR="$actual_dir" \
ICC_REPORT_DIR="$report_dir" \
ICC_REQUIRE_ACTUAL=1 \
ICC_SSIM_MIN="$ssim_min" \
sh tools/ci/color-icc-diff.sh
