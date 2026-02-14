# Tablet Input Validation Matrix (Linux + Windows)

Date: 2026-02-14
Scope: GTK4 migration readiness and regression control for stylus/tablet behavior.

## Goals

- Preserve pressure, tilt, and stylus button behavior during toolkit migration.
- Keep parity between Linux and Windows for core painting workflows.
- Record reproducible evidence for release gates.

## Platforms and devices

| Platform | Device class | Driver/backend | Notes |
|---|---|---|---|
| Linux | USB tablet/stylus | libinput + X11/Wayland | Validate pressure/tilt/button mapping |
| Windows | USB tablet/stylus | WinTab/Windows Ink | Validate pressure/tilt/eraser parity |

## Test cases

| ID | Area | Action | Expected result |
|---|---|---|---|
| TBL-001 | Pressure | Draw stroke with low/high pressure | Stroke width/opacity follows pressure curve |
| TBL-002 | Pressure | Rapid pressure ramps during stroke | No stuck pressure; smooth interpolation |
| TBL-003 | Tilt | Draw with tilt-x changes | Brush orientation/response follows tilt-x |
| TBL-004 | Tilt | Draw with tilt-y changes | Brush orientation/response follows tilt-y |
| TBL-005 | Eraser | Flip stylus to eraser tip | Tool switches to eraser behavior correctly |
| TBL-006 | Stylus button | Press barrel button 1 during paint | Configured action is triggered consistently |
| TBL-007 | Stylus button | Press barrel button 2 during paint | Configured action is triggered consistently |
| TBL-008 | Mapping | Switch monitors / mapping area | Cursor mapping remains stable and accurate |
| TBL-009 | Focus | Paint after dock focus changes | Input focus returns to canvas without lost events |
| TBL-010 | Session | Restart app and retest pressure | Behavior persists; no regression after restart |
| TBL-011 | Undo/redo | Paint with pressure + undo/redo | Replay is stable; no corrupt event stream |
| TBL-012 | Performance | Fast strokes for 30s | No event drop/jank beyond acceptable threshold |

## Execution log template

| Date | Build | Platform | Device | Driver | Cases | Result | Notes |
|---|---|---|---|---|---|---|---|
| PENDING | PENDING | Linux | PENDING | PENDING | TBL-001..TBL-012 | PENDING | Hardware run required |
| PENDING | PENDING | Windows | PENDING | PENDING | TBL-001..TBL-012 | PENDING | Hardware run required |

## Exit criteria

- Linux: all cases TBL-001..TBL-012 pass on at least one supported device.
- Windows: all cases TBL-001..TBL-012 pass on at least one supported device.
- Any failure must include reproduction notes and rollback decision.
