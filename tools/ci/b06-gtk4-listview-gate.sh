#!/bin/sh

set -eu

meson_root="meson.build"
meson_widgets="libgimpwidgets/meson.build"
file="libgimpwidgets/gimppatheditor.c"
fail=0

check_present() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s in %s)\n" "$label" "$pattern" "$file"
      fail=1
    fi
  else
    if grep -En "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing pattern: %s in %s)\n" "$label" "$pattern" "$file"
      fail=1
    fi
  fi
}

gtk4_enabled=0
if command -v rg >/dev/null 2>&1; then
  if rg -n "gtk4|gtk\\+-4\\.0" "$meson_root" "$meson_widgets" >/dev/null 2>&1; then
    gtk4_enabled=1
  fi
else
  if grep -En "gtk4|gtk\\+-4\\.0" "$meson_root" "$meson_widgets" >/dev/null 2>&1; then
    gtk4_enabled=1
  fi
fi

echo "B06 GTK4 listview gate"
echo

if [ "$gtk4_enabled" -eq 0 ]; then
  echo "GTK4 dependency not enabled in Meson; skipping GTK4 listview gate."
  exit 0
fi

check_present "GtkListView|gtk_list_view_" "GtkListView usage present in path editor"
check_present "GtkColumnView|gtk_column_view_" "GtkColumnView usage present in path editor"

echo
if [ "$fail" -ne 0 ]; then
  echo "FAIL: B06 GTK4 listview gate failed."
  exit 1
fi

echo "PASS: B06 GTK4 listview gate passed."
