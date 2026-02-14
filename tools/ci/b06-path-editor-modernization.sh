#!/bin/sh

set -eu

file="libgimpwidgets/gimppatheditor.c"
test_file="libgimpwidgets/test-path-editor.c"
meson_file="libgimpwidgets/meson.build"
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

echo "B06 path editor modernization checklist"
echo

check_present "$file" "gtk_list_box_new" "Uses GtkListBox widget"
check_present "$file" "gtk_list_box_select_row" "Selection handled by GtkListBox"
check_present "$file" "gtk_list_box_unselect_all" "Supports explicit unselect flow"
check_present "$file" "gimp_widgets_compat_box_pack_start" "Uses compat box-pack wrapper"
check_present "$file" "gimp_widgets_compat_container_add" "Uses compat container-add wrapper"

check_absent "$file" "GtkTreeView" "No GtkTreeView type in pilot file"
check_absent "$file" "gtk_tree_view_" "No gtk_tree_view_ API in pilot file"
check_absent "$file" "GtkTreeSelection" "No GtkTreeSelection type in pilot file"
check_absent "$file" "gtk_tree_selection_" "No gtk_tree_selection_ API in pilot file"
check_absent "$file" "gtk_box_pack_start" "No direct gtk_box_pack_start API in pilot file"
check_absent "$file" "gtk_container_add" "No direct gtk_container_add API in pilot file"

check_present "$test_file" "g_test_add_func \\(\"/gimp-path-editor/roundtrip\"" "Has roundtrip test"
check_present "$test_file" "g_test_add_func \\(\"/gimp-path-editor/reorder-buttons\"" "Has reorder test"
check_present "$test_file" "g_test_add_func \\(\"/gimp-path-editor/add-and-delete\"" "Has add/delete test"
check_present "$test_file" "g_test_add_func \\(\"/gimp-path-editor/writable-toggle\"" "Has writable toggle test"
check_present "$meson_file" "test-path-editor\\.c" "Meson builds test-path-editor"
check_present "$meson_file" "test\\('path-editor'" "Meson registers path-editor test"
check_present "$meson_file" "gimpwidgets-compat\\.c" "Meson builds gimpwidgets-compat source"

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: B06 pilot modernization check failed."
  exit 1
fi

echo "PASS: B06 pilot modernization check passed."
