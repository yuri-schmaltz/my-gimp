# Color ICC Dynamic Corpus

This directory defines inputs for `tools/ci/color-icc-diff.sh`.

## Layout

- `corpus.txt`: enabled comparison cases.
- `expected/`: baseline reference images.

## `corpus.txt` format

Each non-empty/non-comment line has three space-separated fields:

`<case-id> <expected-relative-path> <actual-relative-path>`

Example:

`srgb_to_display expected/srgb_to_display.png srgb_to_display.png`

The `actual` path is resolved against `ICC_ACTUAL_DIR`.
