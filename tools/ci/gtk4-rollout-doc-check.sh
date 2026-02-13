#!/bin/sh

set -eu

doc="devel-docs/gtk4-rollout-rollback-plan.md"

if [ ! -f "$doc" ]; then
  echo "FAIL: rollout/rollback document not found: $doc"
  exit 1
fi

fail=0

check_doc() {
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

echo "GTK4 rollout/rollback documentation checklist"
echo

check_doc "^## Waves" "Section: Waves"
check_doc "^### Wave 1" "Wave 1"
check_doc "^### Wave 2" "Wave 2"
check_doc "^### Wave 3" "Wave 3"
check_doc "^### Wave 4" "Wave 4"
check_doc "^## Rollback triggers" "Section: rollback triggers"
check_doc "^## Rollback actions by channel" "Section: rollback actions"
check_doc "Flatpak" "Channel: Flatpak"
check_doc "Windows installer" "Channel: Windows installer"
check_doc "MS Store" "Channel: MS Store"
check_doc "^## Release gate checklist" "Section: release gate checklist"

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: rollout/rollback documentation checklist incomplete."
  exit 1
fi

echo "PASS: rollout/rollback documentation checklist complete."
