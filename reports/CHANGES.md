# CHANGES

## Onda 1 — Baseline e guardrails (sem alteração funcional)

- Coleta de autodescoberta e baseline em `reports/EVIDENCE/`.
- Comandos executados (evidência):
  - `sh tools/ci/gtk4-readiness.sh` (`reports/EVIDENCE/11_baseline_gtk4_readiness.log`)
  - `sh tools/ci/e2e-core-checklist.sh` (`reports/EVIDENCE/16_baseline_e2e_core.log`)
  - `sh tools/ci/ui-visual-diff-smoke.sh` (`reports/EVIDENCE/18_baseline_ui_visual_smoke.log`)
  - `sh tools/ci/color-icc-diff-smoke.sh` (`reports/EVIDENCE/19_baseline_color_icc_smoke.log`)

## Onda 2 — Alto impacto / baixo risco (executada)

- Commit: `fdfa75aaec`
- Mensagem: `libgimpwidgets: continue GTK4 compat migration in radio/hint/memsize widgets`
- Arquivos alterados:
  - `libgimpwidgets/gimpintradioframe.c`
  - `libgimpwidgets/gimphintbox.c`
  - `libgimpwidgets/gimpmemsizeentry.c`
- Contexto ANTES/DEPOIS:
  - Patch completo: `reports/EVIDENCE/45_wave2_diff.patch`
  - Diffstat: `reports/EVIDENCE/46_wave2_diffstat.txt`
  - Linhas-chave: `reports/EVIDENCE/47_wave2_file_lines.txt`
- Efeito medido:
  - `gtk_box_pack_start`: `1832 -> 1827`
  - `gtk_container_add`: `515 -> 514`
  - `GtkContainer|gtk_container_`: `892 -> 891`
  - Evidência: `reports/EVIDENCE/11_baseline_gtk4_readiness.log` vs `reports/EVIDENCE/41_after_gtk4_readiness.log`
- Rollback:
  - `git revert fdfa75aaec`

## Onda 3 — Mudanças complexas

- Não executada nesta rodada.
- Motivo conservador: manter ciclo curto e reversível com validação completa por onda.

## Onda 4 — Endurecimento

- Não executada nesta rodada.
- Próximo passo recomendado: consolidar JUnit local + ampliar cobertura de testes de teclado/foco (a11y) para widgets migrados.
