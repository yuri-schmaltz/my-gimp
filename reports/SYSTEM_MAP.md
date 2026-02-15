# SYSTEM MAP

## Componentes principais

- Core app/UI C/GTK: `app/`, `libgimpwidgets/`, `libgimp/`.
- Build/orquestração: `meson.build`, `meson_options.txt`, `_build-local/`.
- Plug-ins e extensões: `plug-ins/`, `extensions/`.
- Dados/submódulos: `gimp-data`, `_deps/*`.
- Qualidade/CI local: `tools/ci/*.sh` + jobs em `.gitlab-ci.yml`.

## Fluxos E2E críticos (3–5)

1. Fluxo de arquivo: abrir/salvar/exportar.
- Evidência: PASS em `reports/EVIDENCE/16_baseline_e2e_core.log`.

2. Fluxo de edição: undo/redo + alternância de ferramentas.
- Evidência: PASS em `reports/EVIDENCE/16_baseline_e2e_core.log`.

3. Fluxo de janelas/layout: ocultar/restaurar docks e modo de janela.
- Evidência: PASS em `reports/EVIDENCE/16_baseline_e2e_core.log`.

4. Fluxo de regressão visual dos widgets base.
- Evidência: 31/31 imagens com SSIM=1.0 em `reports/EVIDENCE/18_baseline_ui_visual_smoke.log`.

5. Fluxo de fidelidade de cor ICC (smoke).
- Evidência: 2/2 casos PASS em `reports/EVIDENCE/19_baseline_color_icc_smoke.log`.
