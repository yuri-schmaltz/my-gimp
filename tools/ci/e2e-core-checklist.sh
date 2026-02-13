#!/bin/sh

set -eu

fail=0

check_pattern() {
  file="$1"
  pattern="$2"
  label="$3"

  if command -v rg >/dev/null 2>&1; then
    if rg -n "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (pattern: %s in %s)\n" "$label" "$pattern" "$file"
      fail=1
    fi
  else
    if grep -En "$pattern" "$file" >/dev/null 2>&1; then
      printf "PASS  %s\n" "$label"
    else
      printf "FAIL  %s (pattern: %s in %s)\n" "$label" "$pattern" "$file"
      fail=1
    fi
  fi
}

echo "E2E core checklist (static coverage guard)"
echo

check_pattern "app/actions/file-actions.c" "\"file-open\"" "Fluxo arquivo: open"
check_pattern "app/actions/file-actions.c" "\"file-save\"" "Fluxo arquivo: save"
check_pattern "app/actions/file-actions.c" "\"file-export\"" "Fluxo arquivo: export"
check_pattern "app/actions/edit-actions.c" "\"edit-undo\"" "Fluxo edicao: undo"
check_pattern "app/actions/edit-actions.c" "\"edit-redo\"" "Fluxo edicao: redo"
check_pattern "app/actions/windows-actions.c" "\"windows-hide-docks\"" "Fluxo janelas: hide docks"
check_pattern "app/actions/windows-actions.c" "\"windows-use-single-window-mode\"" "Fluxo janelas: single window"
check_pattern "app/actions/filters-actions.c" "\"filters-\"" "Fluxo filtros: acoes registradas"

check_pattern "app/tests/test-ui.c" "ADD_TEST \\(create_new_image_via_dialog\\)" "Teste UI: criar nova imagem"
check_pattern "app/tests/test-ui.c" "ADD_TEST \\(restore_recently_closed_multi_column_dock\\)" "Teste UI: restaurar dock"
check_pattern "app/tests/test-ui.c" "ADD_TEST \\(tab_toggle_dont_change_dock_window_position\\)" "Teste UI: geometria de docks"
check_pattern "app/tests/test-ui.c" "ADD_TEST \\(switch_to_single_window_mode\\)" "Teste UI: alternar modo de janela"
check_pattern "app/tests/test-ui.c" "ADD_TEST \\(swap_tools\\)" "Teste UI: troca de ferramentas"
check_pattern "app/tests/gimp-test-session-utils.c" "gimp_test_session_load_and_write_session_files" "Teste sessao: persistencia sessionrc/dockrc"

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: checklist E2E core incompleto."
  exit 1
fi

echo "PASS: checklist E2E core atendido."
