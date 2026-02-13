#!/bin/sh

set -eu

file="libgimpwidgets/gimppatheditor.c"
fail=0

check_present() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s)\n" "$label" "$pattern"
      fail=1
    fi
  else
    if grep -En "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s)\n" "$label" "$pattern"
      fail=1
    fi
  fi
}

check_absent() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$file" >/dev/null 2>&1; then
      printf "FAIL  %s (unexpected pattern: %s)\n" "$label" "$pattern"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  else
    if grep -En "$pattern" "$file" >/dev/null 2>&1; then
      printf "FAIL  %s (unexpected pattern: %s)\n" "$label" "$pattern"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  fi
}

echo "B06 path editor modernization checklist"
echo

check_present "gtk_list_box_new" "Uses GtkListBox widget"
check_present "gtk_list_box_select_row" "Selection handled by GtkListBox"
check_present "gtk_list_box_unselect_all" "Supports explicit unselect flow"

check_absent "GtkTreeView" "No GtkTreeView type in pilot file"
check_absent "gtk_tree_view_" "No gtk_tree_view_ API in pilot file"
check_absent "GtkTreeSelection" "No GtkTreeSelection type in pilot file"
check_absent "gtk_tree_selection_" "No gtk_tree_selection_ API in pilot file"

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: B06 pilot modernization check failed."
  exit 1
fi

echo "PASS: B06 pilot modernization check passed."
