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
  - Evidência: `reports/EVIDENCE/61_before_wave3_readiness.log` vs `reports/EVIDENCE/72_after_wave3_readiness.log`
- Rollback:
  - `git revert a2de1556f8`

## Onda 4 — Endurecimento

- Não executada nesta rodada.
- Próximo passo conservador: ampliar cobertura de `gimpenumwidgets.c` e `gimppropwidgets.c` (baixa/média cardinalidade) com o mesmo ciclo curto de validação.
