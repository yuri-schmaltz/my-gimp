# BASELINE

## Descoberto

- Repositório raiz: `/home/yurix/Documentos/my-gimp` (`reports/EVIDENCE/00_repo_root.txt`).
- Stack principal: C + Meson + GTK, com opção de toolkit `gtk3|gtk4` (`meson_options.txt:75`, `meson.build:492-503`, `reports/EVIDENCE/24_key_lines.txt`).
- CI com gates de migração GTK4 e regressão visual já existentes (`.gitlab-ci.yml:820`, `.gitlab-ci.yml:838`, `.gitlab-ci.yml:1002`, `reports/EVIDENCE/24_key_lines.txt`).
- Scripts de validação local disponíveis em `tools/ci/` (`reports/EVIDENCE/04_ci_inventory.txt`).
- Empacotamento detectado em CI: AppImage/Flatpak (`reports/EVIDENCE/03_stack_indicators.txt`).
- Submódulos `_deps/*` e `gimp-data` mapeados em `.gitmodules` (`reports/EVIDENCE/05_submodules.txt`).

## ASSUMIDO

- As execuções de `tools/ci/*.sh` foram usadas como baseline funcional/e2e por serem os guardrails oficiais já integrados no repo.
- A medição de performance foi baseada em tempo/RSS dos gates e build incremental, não no startup completo do binário final.

## NÃO VERIFICADO

- Startup end-to-end do executável final em ambiente limpo (sem resíduos de build local).
- Validação de tablet/stylus com hardware real; o check atual valida matriz/checklist, não telemetria de dispositivo físico (`reports/EVIDENCE/20_baseline_tablet_matrix.log`).
- Segurança de segredos em nível de política organizacional (scan regex local gerou ruído de “token” sem evidência de segredo exposto) (`reports/EVIDENCE/23_security_desktop_scan.log`).

## Baseline automático (antes da onda)

Fonte: `reports/EVIDENCE/30_baseline_summary.tsv`

| Check | Exit | Elapsed(s) | Max RSS(KB) |
|---|---:|---:|---:|
| gtk4-readiness | 0 | 0.21 | 6400 |
| b04-file-dialog-backend-check | 0 | 0.04 | 4480 |
| b05-clipboard-compat-check | 0 | 0.05 | 4480 |
| b06-path-editor-modernization | 0 | 0.04 | 4160 |
| b06-gtk4-listview-gate | 0 | 0.01 | 5440 |
| e2e-core-checklist | 0 | 0.03 | 4160 |
| ui-test-metrics | 0 | 0.07 | 2560 |
| ui-visual-diff-smoke | 0 | 3.04 | 57812 |
| color-icc-diff-smoke | 0 | 0.50 | 55992 |
| tablet-matrix-check | 0 | 0.02 | 4160 |
| gtk4-complete-migration-check | 1 | 0.37 | 6400 |
| build libgimpwidgets target | 0 | 0.04 | 16320 |

## Metodologia

- Execução não interativa com `/usr/bin/time` (elapsed/RSS/CPU) por check.
- Registro integral de comando, saída e exit code em `reports/EVIDENCE/*_baseline_*.log`.
- Consolidação em `reports/EVIDENCE/30_baseline_summary.tsv`.

## Baseline incremental (rodada 3)

Fonte: `reports/EVIDENCE/69_before_wave3_summary.tsv`

| Check | Exit | Elapsed(s) | Max RSS(KB) |
|---|---:|---:|---:|
| build libgimpwidgets target | 0 | 0.05 | 16320 |
| gtk4-readiness | 0 | 0.19 | 6400 |
| gtk4-complete-migration-check | 1 | 0.37 | 6400 |
| e2e-core-checklist | 0 | 0.03 | 4160 |
| ui-visual-diff-smoke | 0 | 3.11 | 57224 |

## Endurecimento da suite (rodada 4)

- Execução exaustiva antes do ajuste: `reports/EVIDENCE/83_gtk4_functional_suite_after_hardening.log`.
- Execução exaustiva depois do ajuste: `reports/EVIDENCE/84_gtk4_functional_suite_after_runtime_fix.log`.
- Delta de falhas: `2 -> 1` (`reports/EVIDENCE/88_hardening_suite_delta.tsv`).
- Falha remanescente esperada: `gtk4-complete-migration-check`.

## Baseline incremental (rodada 5)

- Gate estrito pós-migração de widgets de cor:
  - `reports/EVIDENCE/89_gtk4_complete_migration_after_color_widgets.log`
  - Exit: `1` (esperado nesta fase)
  - Elapsed: `0.37s`
  - Max RSS: `7040 KB`
- Readiness pós-migração de widgets de cor:
  - `reports/EVIDENCE/100_gtk4_readiness_after_color_widgets.log`
  - Exit: `0`
  - Elapsed: `0.20s`
  - Max RSS: `6720 KB`
- Suite exaustiva pós-migração de widgets de cor:
  - `reports/EVIDENCE/90_gtk4_functional_suite_after_color_widgets.log`
  - Exit: `1` (única falha: `gtk4-complete-migration-check`)
  - Elapsed: `10.79s`
  - Max RSS: `98880 KB`
- Delta de superfície legada nesta rodada:
  - `reports/EVIDENCE/91_legacy_api_delta_after_color_widgets.tsv`
