#!/bin/sh

set -eu

pass=0
fail=0
failed_steps=""

run_step() {
  name="$1"
  shift

  echo "=== $name ==="
  if "$@"; then
    echo "PASS: $name"
    pass=$((pass + 1))
  else
    echo "FAIL: $name"
    fail=$((fail + 1))
    failed_steps="${failed_steps}\n- ${name}"
  fi
  echo
}

run_step_shc() {
  name="$1"
  cmd="$2"
  run_step "$name" sh -c "$cmd"
}

run_meson_runtime_tests() {
  build_dir=""
  for dir in _build*; do
    [ -d "$dir" ] || continue
    if meson introspect --tests "$dir" >/dev/null 2>&1; then
      build_dir="$dir"
      break
    fi
  done

  if [ -z "$build_dir" ]; then
    if [ "${MESON_RUNTIME_AUTO_SETUP:-1}" = "1" ]; then
      setup_dir="_build_runtime_ci"
      echo "No valid Meson build dir found; attempting bootstrap in: $setup_dir"
      if meson setup "$setup_dir" --wipe -Dgtk-toolkit=gtk3 >/dev/null 2>&1; then
        build_dir="$setup_dir"
      else
        if [ "${MESON_RUNTIME_REQUIRED:-0}" = "1" ]; then
          echo "FAIL: Meson bootstrap failed and MESON_RUNTIME_REQUIRED=1."
          return 1
        fi
        echo "SKIP: Meson bootstrap failed (missing deps/toolchain). Set MESON_RUNTIME_REQUIRED=1 to enforce failure."
        return 0
      fi
    else
      if [ "${MESON_RUNTIME_REQUIRED:-0}" = "1" ]; then
        echo "FAIL: no valid Meson build dir found for runtime test execution."
        return 1
      fi
      echo "SKIP: no valid Meson build dir found (MESON_RUNTIME_AUTO_SETUP=0)."
      return 0
    fi
  fi

  echo "Using Meson build dir: $build_dir"
  meson test -C "$build_dir" --print-errorlogs --no-rebuild
}

run_tablet_matrix_strict() {
  if [ "${TABLET_MATRIX_REQUIRE_RESULTS:-0}" = "1" ]; then
    TABLET_MATRIX_REQUIRE_RESULTS=1 sh tools/ci/tablet-matrix-check.sh
  else
    echo "SKIP: strict tablet matrix disabled (requires hardware evidence)."
    return 0
  fi
}

echo "GTK4 exhaustive validation suite"
echo

run_step "gtk4-complete-migration-check" sh tools/ci/gtk4-complete-migration-check.sh
run_step "gtk4-readiness" sh tools/ci/gtk4-readiness.sh

run_step "b04-file-dialog-backend-check" sh tools/ci/b04-file-dialog-backend-check.sh
run_step "b05-clipboard-compat-check" sh tools/ci/b05-clipboard-compat-check.sh
run_step "b06-path-editor-modernization" sh tools/ci/b06-path-editor-modernization.sh
run_step "b06-path-editor-junit" sh tools/ci/b06-path-editor-junit.sh
run_step "b06-gtk4-listview-gate" sh tools/ci/b06-gtk4-listview-gate.sh

run_step "e2e-core-checklist" sh tools/ci/e2e-core-checklist.sh
run_step "ui-test-metrics" sh tools/ci/ui-test-metrics.sh
run_step "meson-runtime-tests" run_meson_runtime_tests

run_step "ui-visual-baseline-check" sh tools/ci/ui-visual-baseline-check.sh
run_step "ui-visual-diff-smoke" sh tools/ci/ui-visual-diff-smoke.sh
run_step_shc "ui-visual-diff-strict" "UI_VISUAL_REQUIRE_CURRENT=1 sh tools/ci/ui-visual-diff.sh"

run_step "color-icc-checklist" sh tools/ci/color-icc-checklist.sh
run_step "color-icc-diff-smoke" sh tools/ci/color-icc-diff-smoke.sh
run_step_shc "color-icc-diff-strict" "ICC_MANIFEST=_log/color-icc-smoke/corpus.txt ICC_EXPECTED_DIR=_log/color-icc-smoke/expected ICC_ACTUAL_DIR=_log/color-icc-smoke/actual ICC_REPORT_DIR=_log/color-icc/report ICC_REQUIRE_ACTUAL=1 sh tools/ci/color-icc-diff.sh"

run_step "tablet-matrix-check" sh tools/ci/tablet-matrix-check.sh
run_step "tablet-matrix-check-strict" run_tablet_matrix_strict

run_step "gtk4-rollout-doc-check" sh tools/ci/gtk4-rollout-doc-check.sh

echo "=== SUMMARY ==="
echo "Passed steps: $pass"
echo "Failed steps: $fail"

if [ "$fail" -ne 0 ]; then
  printf "Failed list:%b\n" "$failed_steps"
  exit 1
fi

echo "PASS: exhaustive suite passed."
