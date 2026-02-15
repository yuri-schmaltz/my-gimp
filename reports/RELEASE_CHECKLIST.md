# RELEASE CHECKLIST

## Pré-release

- [x] Build alvo impactado compila: `ninja -C _build-local libgimpwidgets/libgimpwidgets-3.0.so.0.200.0 -j4`.
- [x] Gate de prontidão GTK4 (`gtk4-readiness`) PASS.
- [x] Smoke funcional core (`e2e-core-checklist`) PASS.
- [x] Smoke visual (`ui-visual-diff-smoke`) PASS.
- [x] Smoke ICC (`color-icc-diff-smoke`) PASS.
- [x] Checklist tablet PASS (estrutura).

## Riscos conhecidos

- [ ] Migração GTK4 completa ainda FAIL no gate estrito (`gtk4-complete-migration-check`).
- [ ] Cobertura de métricas JUnit local incompleta (sem `testlog.junit.xml`).
- [ ] Validação de tablet com hardware real não executada.

## Rollback

- Commit da onda atual: `fdfa75aaec`
- Comando de rollback: `git revert fdfa75aaec`
- Revalidação pós-rollback: rerun de
  - `sh tools/ci/gtk4-readiness.sh`
  - `sh tools/ci/e2e-core-checklist.sh`
  - `sh tools/ci/ui-visual-diff-smoke.sh`

## Gate final recomendado para merge

- Exigir PASS de:
  - build do alvo impactado
  - `gtk4-readiness`
  - `e2e-core-checklist`
  - `ui-visual-diff-smoke`
- Manter `gtk4-complete-migration-check` como objetivo de programa (não gate bloqueante desta onda incremental).
