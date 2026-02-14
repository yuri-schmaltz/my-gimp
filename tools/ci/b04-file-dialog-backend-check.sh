#!/bin/sh

set -eu

dialog_file="app/widgets/gimpfiledialog.c"
backend_file="app/widgets/gimpfiledialog-backend.c"
backend_hdr="app/widgets/gimpfiledialog-backend.h"
backend_gtk3_file="app/widgets/gimpfiledialog-backend-gtk3.c"
backend_gtk3_hdr="app/widgets/gimpfiledialog-backend-gtk3.h"
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

echo "B04 file dialog backend checklist"
echo

check_present "$dialog_file" "gimpfiledialog-backend\\.h" "Includes backend header"
check_present "$dialog_file" "gimp_file_dialog_backend_add_default_buttons" "Uses backend for default buttons"
check_present "$dialog_file" "gimp_file_dialog_backend_add_help_button" "Uses backend for help button"
check_present "$dialog_file" "gimp_file_dialog_backend_configure_chooser" "Uses backend chooser config"
check_present "$dialog_file" "gimp_file_dialog_backend_set_extra_widget" "Uses backend extra widget"
check_present "$dialog_file" "gimp_file_dialog_backend_pack_progress" "Uses backend progress pack"
check_present "$dialog_file" "gimp_file_dialog_backend_add_shortcut_folder" "Uses backend shortcut folders"

check_absent "$dialog_file" "gtk_dialog_add_buttons\\s*\\(" "No direct gtk_dialog_add_buttons in dialog file"
check_absent "$dialog_file" "gtk_file_chooser_set_extra_widget\\s*\\(" "No direct gtk_file_chooser_set_extra_widget in dialog file"
check_absent "$dialog_file" "gtk_file_chooser_add_shortcut_folder\\s*\\(" "No direct gtk_file_chooser_add_shortcut_folder in dialog file"

check_present "$backend_hdr" "gimp_file_dialog_backend_add_help_button" "Backend header exports help button API"
check_present "$backend_file" "gimp_file_dialog_backend_add_help_button" "Backend source implements help button API"
check_present "$backend_gtk3_hdr" "gimp_file_dialog_backend_gtk3_add_help_button" "GTK3 backend header exports help API"
check_present "$backend_gtk3_file" "gimp_file_dialog_backend_gtk3_add_help_button" "GTK3 backend source implements help API"
check_present "$meson_file" "gimpfiledialog-backend\\.c" "Meson includes backend shim source"
check_present "$meson_file" "gimpfiledialog-backend-gtk3\\.c" "Meson includes GTK3 backend source"

echo
if [ "$fail" -ne 0 ]; then
  echo "FAIL: B04 file dialog backend checklist failed."
  exit 1
fi

echo "PASS: B04 file dialog backend checklist passed."
