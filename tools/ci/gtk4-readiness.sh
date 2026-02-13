#!/bin/sh

set -eu

TARGETS="app libgimp libgimpwidgets modules plug-ins"

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

fail=0

echo "GTK4 readiness gate (legacy GTK3 surface must not increase)"
echo "Scope: $TARGETS"
echo
printf "%-4s %-7s %-7s %-7s %s\n" "ID" "BASE" "CURR" "STATUS" "PATTERN"

while IFS=';' read -r id baseline pattern; do
  [ -n "$id" ] || continue

  current="$(count_matches "$pattern")"
  status="OK"

  if [ "$current" -gt "$baseline" ]; then
    status="FAIL"
    fail=1
  fi

  printf "%-4s %-7s %-7s %-7s %s\n" "$id" "$baseline" "$current" "$status" "$pattern"
done <<'EOF'
R01;1863;gtk_box_pack_start
R02;110;gtk_box_pack_end
R03;535;gtk_container_add
R04;915;GtkContainer|gtk_container_
R05;722;GtkTreeView|gtk_tree_view_
R06;13;GtkFileChooserDialog
R07;12;GtkAccelGroup
R08;19;gtk_drag_dest_set
R09;7;gtk_drag_source_set
R10;80;gtk_selection_data_
R11;66;gtk_clipboard_
R12;325;GdkWindow|gdk_window_|gtk_widget_get_window
EOF

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: at least one legacy GTK3 metric increased over baseline."
  echo "If this is intentional, update baseline values in tools/ci/gtk4-readiness.sh with justification in MR."
  exit 1
fi

echo "PASS: legacy GTK3 surface did not increase."
