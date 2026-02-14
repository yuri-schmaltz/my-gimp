#!/bin/sh

set -eu

TARGETS="app libgimp libgimpwidgets modules plug-ins"
MESON_FILES="meson.build app/widgets/meson.build libgimpwidgets/meson.build"
MESON_OPTIONS_FILE="meson_options.txt"
fail=0

count_matches() {
  pattern="$1"

  if command -v rg >/dev/null 2>&1; then
    # shellcheck disable=SC2086
    rg -n --glob '*.[ch]' --glob '*.cc' --glob '*.cpp' --glob '*.hpp' \
      "$pattern" $TARGETS 2>/dev/null | wc -l | tr -d ' '
  else
    grep -RInE "$pattern" $TARGETS \
      --include='*.c' --include='*.h' --include='*.cc' --include='*.cpp' --include='*.hpp' \
      2>/dev/null | wc -l | tr -d ' '
  fi
}

check_dep_present() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" $MESON_FILES >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing in Meson files)\n" "$label"
      fail=1
    fi
  else
    if grep -En "$pattern" $MESON_FILES >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing in Meson files)\n" "$label"
      fail=1
    fi
  fi
}

check_dep_absent() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" $MESON_FILES >/dev/null 2>&1; then
      printf "FAIL  %s (still present in Meson files)\n" "$label"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  else
    if grep -En "$pattern" $MESON_FILES >/dev/null 2>&1; then
      printf "FAIL  %s (still present in Meson files)\n" "$label"
      fail=1
    else
      printf "PASS  %s\n" "$label"
    fi
  fi
}

check_option_present() {
  pattern="$1"
  label="$2"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$MESON_OPTIONS_FILE" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing in %s)\n" "$label" "$MESON_OPTIONS_FILE"
      fail=1
    fi
  else
    if grep -En "$pattern" "$MESON_OPTIONS_FILE" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (missing in %s)\n" "$label" "$MESON_OPTIONS_FILE"
      fail=1
    fi
  fi
}

echo "GTK4 complete migration gate (strict)"
echo "Scope: $TARGETS"
echo

echo "Dependency checks:"
check_option_present "option\\('gtk-toolkit'" "Meson option gtk-toolkit exists"
check_dep_present "get_option\\('gtk-toolkit'\\)|gtk4_minver|gtk_dep_name\\s*=\\s*'gtk4'" "GTK4 selection path exists"
check_dep_absent  "dependency\\('gtk\\+-3\\.0'" "Hardcoded GTK3 dependency removed from root Meson"
echo

echo "Legacy API checks (must be zero):"
printf "%-4s %-7s %-7s %s\n" "ID" "COUNT" "STATUS" "PATTERN"
while IFS=';' read -r id pattern; do
  [ -n "$id" ] || continue

  current="$(count_matches "$pattern")"
  status="PASS"
  if [ "$current" -gt 0 ]; then
    status="FAIL"
    fail=1
  fi

  printf "%-4s %-7s %-7s %s\n" "$id" "$current" "$status" "$pattern"
done <<'EOF'
L01;gtk_box_pack_start
L02;gtk_box_pack_end
L03;gtk_container_add
L04;GtkContainer|gtk_container_
L05;GtkTreeView|gtk_tree_view_
L06;GtkTreeSelection|gtk_tree_selection_
L07;GtkFileChooserDialog
L08;gtk_file_chooser_
L09;GtkClipboard|gtk_clipboard_
L10;gtk_dialog_add_buttons
L11;gtk_drag_dest_set
L12;gtk_drag_source_set
L13;gtk_selection_data_
L14;GdkWindow|gdk_window_|gtk_widget_get_window
EOF
echo

echo "GTK4 API evidence checks (must be non-zero):"
printf "%-4s %-7s %-7s %s\n" "ID" "COUNT" "STATUS" "PATTERN"
while IFS=';' read -r id pattern; do
  [ -n "$id" ] || continue

  current="$(count_matches "$pattern")"
  status="PASS"
  if [ "$current" -eq 0 ]; then
    status="FAIL"
    fail=1
  fi

  printf "%-4s %-7s %-7s %s\n" "$id" "$current" "$status" "$pattern"
done <<'EOF'
N01;GtkListView|gtk_list_view_
N02;GtkColumnView|gtk_column_view_
N03;gtk_box_append|gtk_box_prepend
N04;gtk_window_set_child
N05;GtkDropTarget|gtk_drop_target_
N06;GtkDragSource|gtk_drag_source_new|gtk_drag_source_set_actions
N07;GdkClipboard|gdk_clipboard_
N08;GtkFileDialog|gtk_file_dialog_
N09;gtk_widget_add_controller|GtkEventController
EOF

echo
if [ "$fail" -ne 0 ]; then
  echo "FAIL: GTK4 complete migration is NOT confirmed."
  exit 1
fi

echo "PASS: GTK4 complete migration confirmed."
