# RELEASE CHECKLIST

## Pré-release

- [x] Build alvo impactado compila (`71_after_wave3_build.log`).
- [x] Gate de prontidão GTK4 PASS (`72_after_wave3_readiness.log`).
- [x] Smoke funcional core PASS (`74_after_wave3_e2e.log`).
- [x] Smoke visual PASS (`75_after_wave3_visual.log`).
- [x] Smoke ICC PASS (`19_baseline_color_icc_smoke.log`).
- [x] Checklist tablet PASS (estrutura) (`20_baseline_tablet_matrix.log`).

## Riscos conhecidos

- [ ] Migração GTK4 completa ainda FAIL no gate estrito (`73_after_wave3_complete.log`).
- [ ] Cobertura de métricas JUnit local incompleta (`17_baseline_ui_metrics.log`).
- [ ] Validação de tablet com hardware real não executada.

## Rollback

- Commit da onda 3: `a2de1556f8`
- Rollback da onda 3: `git revert a2de1556f8`
- Commit da onda 2: `fdfa75aaec`
- Rollback da onda 2: `git revert fdfa75aaec`
- Revalidação pós-rollback:
  - `sh tools/ci/gtk4-readiness.sh`
  - `sh tools/ci/e2e-core-checklist.sh`
  - `sh tools/ci/ui-visual-diff-smoke.sh`

## Gate recomendado para merge

- Exigir PASS de:
  - build do alvo impactado
  - `gtk4-readiness`
  - `e2e-core-checklist`
  - `ui-visual-diff-smoke`
- Manter `gtk4-complete-migration-check` como objetivo de programa até zerar superfície legada crítica.
