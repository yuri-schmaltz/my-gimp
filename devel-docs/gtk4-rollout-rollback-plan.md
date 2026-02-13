# GTK4 rollout and rollback plan (GIMP)

Date: 2026-02-13
Scope: Linux + Windows channels currently used by this repository.

## Goals
- Preserve functional parity for core flows (open/save/export, undo/redo, docks, filters, paint/text).
- Roll out GTK4 incrementally by channel with measurable gates.
- Keep rollback paths explicit and fast per channel.

## Channel matrix
- Flatpak (Linux): `build/linux/flatpak/org.gimp.GIMP-nightly.json`
- AppImage (Linux): `.gitlab-ci.yml` jobs gated by `GIMP_CI_APPIMAGE`
- Snap (Linux): `.gitlab-ci.yml` jobs gated by `GIMP_CI_SNAP`
- Windows installer (Inno): `build/windows/installer/base_gimp3264.iss`
- MS Store bundle: `build/windows/store/AppxManifest.xml`

## Waves
### Wave 1: readiness and baseline (no user-facing GTK4 default)
- Keep GTK3 as default build/runtime.
- Enforce CI guards:
  - `tools/ci/gtk4-readiness.sh`
  - `tools/ci/e2e-core-checklist.sh`
  - `tools/ci/ui-test-metrics.sh`
  - `tools/ci/ui-visual-baseline-check.sh`
  - `tools/ci/color-icc-checklist.sh`
- Exit criteria:
  - All checks PASS on merge requests and default branch commits.

### Wave 2: opt-in GTK4 channel (nightly/canary)
- Enable GTK4 variant for one non-stable channel first.
- Keep stable channels on GTK3.
- Exit criteria:
  - No increase in critical crash signals and no loss on E2E checklist.
  - No regressions in visual baseline and color/ICC checklists.

### Wave 3: broaden GTK4 to beta-like channels
- Expand GTK4 builds to more channels after Wave 2 stability window.
- Keep one fallback GTK3 artifact available in CI.
- Exit criteria:
  - Consecutive green pipelines across all checklist jobs.
  - Manual sign-off for tablets/input and color-managed workflows.

### Wave 4: stable default + monitored fallback
- Promote GTK4 as default for stable releases.
- Keep rollback playbook active for one full release cycle.
- Exit criteria:
  - Functional parity and release checklist fully PASS.

## Rollback triggers
- Any blocker regression in open/save/export or undo/redo.
- Dock/session restore regressions.
- Clipboard/DnD interop regressions with external apps.
- Color-management regressions in assign/convert/simulation profile flows.
- Elevated crash reports or startup failures after channel rollout.

## Rollback actions by channel
### Flatpak
- Revert runtime/app commit in `build/linux/flatpak/org.gimp.GIMP-nightly.json`.
- Publish previous stable manifest and tag as rollback candidate.

### AppImage and Snap
- Disable GTK4 variant pipeline switch.
- Rebuild last known-good GTK3 artifact and republish.

### Windows installer and MS Store
- Revert packaging metadata to last known-good runtime set.
- Ship previous signed installer/bundle.

## Operational notes
- Keep update policy controls available (`meson_options.txt`, `app/gimp-update.c`) to stop or slow rollout.
- Keep logs and crash artifacts retained in CI and release channels.
- Do not remove GTK3 fallback until Wave 4 is stable for one full cycle.

## Release gate checklist
- `gtk4-readiness`: PASS
- `e2e-core-checklist`: PASS
- `ui-test-metrics`: PASS with required core tests present
- `ui-visual-baseline-check`: PASS
- `color-icc-checklist`: PASS
- Manual tablet/input smoke on Linux + Windows: PASS
- Rollback package rehearsal on at least one Linux and one Windows channel: PASS
