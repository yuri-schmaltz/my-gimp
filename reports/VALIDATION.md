# VALIDATION

## Critérios e resultado geral

- Equivalência funcional nas ondas aplicadas: PASS (sem perda observada nos checks executados).
- Build incremental do alvo impactado: PASS.
- Regressão visual smoke: PASS.
- Gate de migração completa GTK4: FAIL (esperado; redução parcial contínua da superfície legada).

## Rodada 3 — Antes vs Depois

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
- `reports/EVIDENCE/61_before_wave3_readiness.log`
- `reports/EVIDENCE/72_after_wave3_readiness.log`

## AEGIS UI / A11y

- Regressão visual smoke sem divergência observada no conjunto atual (PASS em `reports/EVIDENCE/75_after_wave3_visual.log`).
- Navegação/foco/teclado em runtime interativo completo: NÃO VERIFICADO nesta rodada.

## Rollback validado

- Onda 3:
  - `git revert a2de1556f8`
- Onda 2:
  - `git revert fdfa75aaec`
