#!/bin/sh

set -eu

gallery="devel-docs/reference/gimp-ui/widget-gallery.md"
images_dir="devel-docs/reference/gimp-ui/images"
manifest="devel-docs/reference/gimp-ui/gimp-ui-3.0.toml.in"

if [ ! -f "$gallery" ]; then
  echo "FAIL: visual baseline gallery not found: $gallery"
  exit 1
fi

if [ ! -d "$images_dir" ]; then
  echo "FAIL: visual baseline images dir not found: $images_dir"
  exit 1
fi

if [ ! -f "$manifest" ]; then
  echo "FAIL: visual baseline manifest not found: $manifest"
  exit 1
fi

tmp_refs="$(mktemp)"
cleanup() {
  rm -f "$tmp_refs"
}
trap cleanup EXIT INT TERM

if command -v rg >/dev/null 2>&1; then
  rg -o '\([^)]+\.png\)' "$gallery" | sed -E 's/^\((.*)\)$/\1/' | sort -u > "$tmp_refs"
else
  grep -oE '\([^)]+\.png\)' "$gallery" | sed -E 's/^\((.*)\)$/\1/' | sort -u > "$tmp_refs"
fi

count="$(wc -l < "$tmp_refs" | tr -d ' ')"
if [ "$count" -lt 20 ]; then
  echo "FAIL: only $count visual baseline references found in gallery (expected >= 20)."
  exit 1
fi

echo "UI visual baseline references in gallery: $count"

echo "Checking referenced screenshots exist in $images_dir"
fail=0
while IFS= read -r image; do
  [ -n "$image" ] || continue

  if [ ! -f "$images_dir/$image" ]; then
    echo "FAIL: missing baseline screenshot: $images_dir/$image"
    fail=1
  fi

done < "$tmp_refs"

echo "Checking references are tracked in $manifest"
while IFS= read -r image; do
  [ -n "$image" ] || continue

  if command -v rg >/dev/null 2>&1; then
    if ! rg -n "images/$image" "$manifest" >/dev/null 2>&1; then
      echo "FAIL: screenshot not listed in manifest: images/$image"
      fail=1
    fi
  else
    if ! grep -n "images/$image" "$manifest" >/dev/null 2>&1; then
      echo "FAIL: screenshot not listed in manifest: images/$image"
      fail=1
    fi
  fi

done < "$tmp_refs"

if [ "$fail" -ne 0 ]; then
  echo "FAIL: visual baseline checklist has inconsistencies."
  exit 1
fi

echo "PASS: visual baseline checklist consistent."
