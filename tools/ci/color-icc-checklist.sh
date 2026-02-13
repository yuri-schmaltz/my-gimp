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

echo "Color/ICC checklist (static coverage guard)"
echo

check_pattern "app/pdb/image-color-profile-cmds.c" "\"gimp-image-set-color-profile\"" "PDB: set color profile"
check_pattern "app/pdb/image-color-profile-cmds.c" "\"gimp-image-convert-color-profile\"" "PDB: convert color profile"
check_pattern "app/pdb/image-color-profile-cmds.c" "\"gimp-image-set-simulation-profile\"" "PDB: set simulation profile"
check_pattern "app/pdb/internal-procs.c" "register_image_color_profile_procs" "Registro PDB de color profile"
check_pattern "app/core/gimpimage-color-profile.c" "gimp_image_assign_color_profile" "Core: assign profile"
check_pattern "app/core/gimpimage-color-profile.c" "gimp_image_convert_color_profile" "Core: convert profile"
check_pattern "app/display/gimpdisplayshell-profile.c" "gimp_display_shell_profile_update" "Display: profile update"
check_pattern "app/display/gimpdisplayshell-render.c" "gimp_color_transform_process_buffer" "Display: transform process"

echo

if [ "$fail" -ne 0 ]; then
  echo "FAIL: checklist Color/ICC incompleto."
  exit 1
fi

echo "PASS: checklist Color/ICC atendido."
