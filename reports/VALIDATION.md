# VALIDATION

## Critérios e resultado geral

- Equivalência funcional nas ondas aplicadas: PASS (sem perda observada nos checks executados).
- Build incremental do alvo impactado: PASS.
- Regressão visual smoke/strict: PASS.
- Gate de migração completa GTK4: FAIL (esperado; redução parcial contínua da superfície legada).

## Rodada 3 — Código (antes vs depois)

| Validação | Antes (wave3) | Depois (wave3) | Resultado |
|---|---|---|---|
| Build `libgimpwidgets` | PASS (`60_before_wave3_build`) | PASS (`71_after_wave3_build`) | Mantido |
| gtk4-readiness | PASS (R01=1827, R03=514, R04=891) | PASS (R01=1820, R03=513, R04=890) | Melhorou |
| gtk4-complete-migration-check | FAIL (`62_before_wave3_complete`) | FAIL (`73_after_wave3_complete`) | Inalterado (esperado) |
| e2e-core-checklist | PASS (`63_before_wave3_e2e`) | PASS (`74_after_wave3_e2e`) | Mantido |
| ui-visual-diff-smoke | PASS (`64_before_wave3_visual`) | PASS (`75_after_wave3_visual`) | Mantido |

Referências:
- `reports/EVIDENCE/69_before_wave3_summary.tsv`
- `reports/EVIDENCE/79_after_wave3_summary.tsv`

## Rodada 4 — Endurecimento de suíte

| Validação | Antes hardening | Depois hardening | Resultado |
|---|---|---|---|
| gtk4-functional-regression-suite | 2 falhas | 1 falha | Melhorou |
| Falhas remanescentes | complete + runtime | complete | Melhorou |

Referências:
- Antes: `reports/EVIDENCE/83_gtk4_functional_suite_after_hardening.log`
- Depois: `reports/EVIDENCE/84_gtk4_functional_suite_after_runtime_fix.log`
- Delta: `reports/EVIDENCE/88_hardening_suite_delta.tsv`

## Rodada 5 — Migração incremental de widgets de cor

| Validação | Antes (rodada 4) | Depois (rodada 5) | Resultado |
|---|---|---|---|
| gtk4-complete-migration-check | FAIL (`84_..._runtime_fix`) | FAIL (`89_gtk4_complete_migration_after_color_widgets.log`) | Melhorou contagens |
| L01 gtk_box_pack_start | 1820 | 1801 | Melhorou |
| L02 gtk_box_pack_end | 106 | 102 | Melhorou |
| L03 gtk_container_add | 513 | 510 | Melhorou |
| L04 GtkContainer/gtk_container_ | 890 | 887 | Melhorou |
| gtk4-readiness | PASS (`R01=1820`, `R03=513`, `R04=890`) | PASS (`R01=1801`, `R03=510`, `R04=887`) | Melhorou |
| Build incremental do alvo impactado | PASS (build anterior) | PASS (`99_build_libgimpwidgets_target_after_color_widgets.log`) | Mantido |
| gtk4-functional-regression-suite | 1 falha (`84_..._runtime_fix`) | 1 falha (`90_gtk4_functional_suite_after_color_widgets.log`) | Mantido |

Referências:
- `reports/EVIDENCE/89_gtk4_complete_migration_after_color_widgets.log`
- `reports/EVIDENCE/90_gtk4_functional_suite_after_color_widgets.log`
- `reports/EVIDENCE/91_legacy_api_delta_after_color_widgets.tsv`
- `reports/EVIDENCE/99_build_libgimpwidgets_target_after_color_widgets.log`
- `reports/EVIDENCE/100_gtk4_readiness_after_color_widgets.log`

## AEGIS UI / A11y

- Regressão visual smoke e strict sem divergências no conjunto atual.
- Navegação/foco/teclado em runtime interativo completo: NÃO VERIFICADO nesta rodada.

## Rollback validado

- Onda 4 scripts: `git revert 793ba02355`
- Onda 3 código: `git revert a2de1556f8`
- Onda 2 código: `git revert fdfa75aaec`
