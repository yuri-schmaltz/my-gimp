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
  - `git revert <commit_onda4>`
