#!/bin/sh

set -eu

clipboard_file="app/widgets/gimpclipboard.c"
compat_hdr="app/widgets/gimpclipboardcompat.h"
compat_file="app/widgets/gimpclipboardcompat.c"
meson_file="app/widgets/meson.build"
fail=0

check_present() {
  target_file="$1"
  pattern="$2"
  label="$3"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$target_file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s in %s)\n" "$label" "$pattern" "$target_file"
      fail=1
    fi
  else
    if grep -En "$pattern" "$target_file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s in %s)\n" "$label" "$pattern" "$target_file"
      fail=1
    fi
  fi
}

check_absent() {
  target_file="$1"
  pattern="$2"
  label="$3"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$target_file" >/dev/null 2>&1; then
      printf "FAIL  %s (unexpected pattern: %s in %s)\n" "$label" "$pattern" "$target_file"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  else
    if grep -En "$pattern" "$target_file" >/dev/null 2>&1; then
      printf "FAIL  %s (unexpected pattern: %s in %s)\n" "$label" "$pattern" "$target_file"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  fi
}

echo "B05 clipboard compat checklist"
echo

check_present "$clipboard_file" "gimpclipboardcompat\\.h" "Includes clipboard compat header"
check_present "$clipboard_file" "gimp_clipboard_compat_get_default" "Uses compat default clipboard getter"
check_present "$clipboard_file" "gimp_clipboard_compat_owner_is" "Uses compat owner check"
check_present "$clipboard_file" "gimp_clipboard_compat_set_with_owner" "Uses compat set_with_owner"
check_present "$clipboard_file" "gimp_clipboard_compat_wait_for_contents" "Uses compat wait_for_contents"
check_present "$clipboard_file" "gimp_clipboard_compat_set_text" "Uses compat set_text"
check_present "$clipboard_file" "gimp_clipboard_compat_clear" "Uses compat clear"

check_absent "$clipboard_file" "gtk_clipboard_get_for_display\\s*\\(" "No direct gtk_clipboard_get_for_display in clipboard file"
check_absent "$clipboard_file" "gtk_clipboard_set_with_owner\\s*\\(" "No direct gtk_clipboard_set_with_owner in clipboard file"
check_absent "$clipboard_file" "gtk_clipboard_wait_for_contents\\s*\\(" "No direct gtk_clipboard_wait_for_contents in clipboard file"
check_absent "$clipboard_file" "gtk_clipboard_clear\\s*\\(" "No direct gtk_clipboard_clear in clipboard file"
check_absent "$clipboard_file" "gtk_clipboard_store\\s*\\(" "No direct gtk_clipboard_store in clipboard file"
check_absent "$clipboard_file" "gtk_clipboard_set_text\\s*\\(" "No direct gtk_clipboard_set_text in clipboard file"

check_present "$compat_hdr" "gimp_clipboard_compat_get_default" "Compat header exports default getter"
check_present "$compat_hdr" "gimp_clipboard_compat_set_with_owner" "Compat header exports set_with_owner"
check_present "$compat_file" "gimp_clipboard_compat_get_default" "Compat source implements default getter"
check_present "$compat_file" "gimp_clipboard_compat_set_with_owner" "Compat source implements set_with_owner"
check_present "$meson_file" "gimpclipboardcompat\\.c" "Meson includes gimpclipboardcompat source"

echo
if [ "$fail" -ne 0 ]; then
  echo "FAIL: B05 clipboard compat checklist failed."
  exit 1
fi

echo "PASS: B05 clipboard compat checklist passed."
