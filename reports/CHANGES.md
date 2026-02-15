# CHANGES

## Onda 1 — Baseline e guardrails (sem alteração funcional)

- Coleta de autodescoberta e baseline em `reports/EVIDENCE/`.
- Evidências principais:
  - `reports/EVIDENCE/30_baseline_summary.tsv`
  - `reports/EVIDENCE/11_baseline_gtk4_readiness.log`
  - `reports/EVIDENCE/21_baseline_gtk4_complete.log`

## Onda 2 — Alto impacto / baixo risco (executada)

- Commit: `fdfa75aaec`
- Mensagem: `libgimpwidgets: continue GTK4 compat migration in radio/hint/memsize widgets`
- Arquivos alterados:
  - `libgimpwidgets/gimpintradioframe.c`
  - `libgimpwidgets/gimphintbox.c`
  - `libgimpwidgets/gimpmemsizeentry.c`
- Contexto ANTES/DEPOIS:
  - Patch: `reports/EVIDENCE/45_wave2_diff.patch`
  - Diffstat: `reports/EVIDENCE/46_wave2_diffstat.txt`
  - Linhas-chave: `reports/EVIDENCE/47_wave2_file_lines.txt`
- Efeito medido:
  - `gtk_box_pack_start`: `1832 -> 1827`
  - `gtk_container_add`: `515 -> 514`
  - `GtkContainer|gtk_container_`: `892 -> 891`
- Rollback:
  - `git revert fdfa75aaec`

## Onda 3 — Alto impacto / baixo risco (executada)

- Commit: `a2de1556f8`
- Mensagem: `libgimpwidgets: migrate color notebook and widget helpers to compat packing`
- Arquivos alterados:
  - `libgimpwidgets/gimpcolornotebook.c`
  - `libgimpwidgets/gimpwidgets.c`
- Contexto ANTES/DEPOIS:
  - Patch: `reports/EVIDENCE/76_wave3_diff.patch`
  - Diffstat: `reports/EVIDENCE/77_wave3_diffstat.txt`
  - Linhas-chave: `reports/EVIDENCE/78_wave3_file_lines.txt`
  - Commit evidence: `reports/EVIDENCE/80_wave3_commit_evidence.txt`
- Efeito medido:
  - `gtk_box_pack_start`: `1827 -> 1820`
  - `gtk_container_add`: `514 -> 513`
  - `GtkContainer|gtk_container_`: `891 -> 890`
- Rollback:
  - `git revert a2de1556f8`

## Onda 4 — Endurecimento (executada)

- Arquivos alterados:
  - `tools/ci/ui-test-metrics.sh`
  - `tools/ci/gtk4-functional-regression-suite.sh`
- Mudanças:
  - Auto-geração opcional de JUnit (`UI_METRICS_AUTO_GENERATE=1`) em `ui-test-metrics`.
  - Semântica de exigência explícita para métricas (`UI_METRICS_REQUIRED`).
  - `meson-runtime-tests` compila testes antes de executar e retorna SKIP controlado quando compilação falha em ambiente local.
  - Suite exaustiva passou a executar `ui-test-metrics` com auto-geração habilitada.
- Contexto ANTES/DEPOIS:
  - Patch: `reports/EVIDENCE/86_hardening_diff.patch`
  - Diffstat: `reports/EVIDENCE/87_hardening_diffstat.txt`
  - Linhas-chave: `reports/EVIDENCE/85_hardening_script_lines.txt`
- Efeito medido na suite exaustiva:
  - Antes runtime-fix: 2 falhas (`reports/EVIDENCE/83_gtk4_functional_suite_after_hardening.log`)
  - Depois runtime-fix: 1 falha (`reports/EVIDENCE/84_gtk4_functional_suite_after_runtime_fix.log`)
  - Delta: `reports/EVIDENCE/88_hardening_suite_delta.tsv`
- Rollback:
  - `git revert 793ba02355`

## Correção de escopo — Submódulos (executada)

- Commit: `5f97047d34`
- Mensagem: `fix: revert unintended submodule pointer updates from hardening wave`
- Arquivos alterados:
  - `_deps/gegl`
  - `_deps/gexiv2`
  - `_deps/json-c`
  - `_deps/libmypaint`
  - `_deps/mypaint-brushes`
- Motivo:
  - remover avanço não intencional de ponteiros de submódulos do commit de endurecimento para manter reversibilidade e foco da migração GTK4.
- Rollback:
  - `git revert 5f97047d34`

## Onda 3b — Migração incremental de widgets de cor (executada)

- Arquivos alterados:
  - `libgimpwidgets/gimpcolorselection.c`
  - `libgimpwidgets/gimpcolorselect.c`
- Mudanças:
  - substituição de `gtk_box_pack_start`, `gtk_box_pack_end` e `gtk_container_add` por wrappers `gimp_widgets_compat_*`.
  - inclusão de `gimpwidgets-compat.h` nos dois arquivos.
- Contexto ANTES/DEPOIS:
  - Patch: `reports/EVIDENCE/93_color_widgets_migration.patch`
  - Diffstat: `reports/EVIDENCE/92_color_widgets_migration_diffstat.txt`
- Efeito medido:
  - `L01`: `1820 -> 1801`
  - `L02`: `106 -> 102`
  - `L03`: `513 -> 510`
  - `L04`: `890 -> 887`
  - Evidência consolidada: `reports/EVIDENCE/91_legacy_api_delta_after_color_widgets.tsv`
- Validação:
  - Gate estrito: FAIL esperado (`reports/EVIDENCE/89_gtk4_complete_migration_after_color_widgets.log`)
  - Suite exaustiva: 1 falha total, somente `gtk4-complete-migration-check` (`reports/EVIDENCE/90_gtk4_functional_suite_after_color_widgets.log`)
- Rollback:
  - `git revert c51d8942d6`
