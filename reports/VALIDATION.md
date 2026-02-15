# VALIDATION

## Critérios e resultado geral

- Equivalência funcional da onda aplicada: PASS (sem perda observada nos checks executados).
- Build incremental do alvo impactado: PASS.
- Regressão visual smoke: PASS.
- Gate de migração completa GTK4: FAIL (já era FAIL e permaneceu FAIL, com melhora parcial das contagens legadas).

## Antes vs Depois

| Validação | Antes | Depois | Resultado |
|---|---|---|---|
| Build `libgimpwidgets` | PASS (`22_baseline_libgimpwidgets_build`) | PASS (`40_after_build_libgimpwidgets`) | Mantido |
| gtk4-readiness | PASS (R01=1832, R03=515, R04=892) | PASS (R01=1827, R03=514, R04=891) | Melhorou |
| gtk4-complete-migration-check | FAIL | FAIL | Inalterado (esperado) |
| e2e-core-checklist | PASS | PASS | Mantido |
| ui-visual-diff-smoke | PASS | PASS | Mantido |

Referências:
- Antes: `reports/EVIDENCE/30_baseline_summary.tsv`, `reports/EVIDENCE/11_baseline_gtk4_readiness.log`, `reports/EVIDENCE/21_baseline_gtk4_complete.log`
- Depois: `reports/EVIDENCE/49_after_summary.tsv`, `reports/EVIDENCE/41_after_gtk4_readiness.log`, `reports/EVIDENCE/42_after_gtk4_complete.log`

## AEGIS UI / A11y

- Evidência de regressão visual smoke sem divergência: 31/31 imagens SSIM=1.0 (`reports/EVIDENCE/18_baseline_ui_visual_smoke.log`, `reports/EVIDENCE/44_after_ui_visual_smoke.log`).
- Navegação/foco/teclado em nível runtime interativo completo: NÃO VERIFICADO nesta rodada (ambiente local não executou sessão interativa manual/automatizada de a11y completa).

## Rollback validado

- Rollback por commit único da onda:
  - `git revert fdfa75aaec`
