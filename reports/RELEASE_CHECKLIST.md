# RELEASE CHECKLIST

## Pré-release

- [x] Build alvo impactado compila (`71_after_wave3_build.log`).
- [x] Gate de prontidão GTK4 PASS (`72_after_wave3_readiness.log`).
- [x] Smoke funcional core PASS (`74_after_wave3_e2e.log`).
- [x] Smoke visual smoke/strict PASS (`75_after_wave3_visual.log`, `84_..._runtime_fix.log`).
- [x] Smoke ICC PASS (`84_..._runtime_fix.log`).
- [x] Checklist tablet PASS (estrutura) (`20_baseline_tablet_matrix.log`).
- [x] Suite exaustiva endurecida executada com 1 falha remanescente (`84_gtk4_functional_suite_after_runtime_fix.log`).

## Riscos conhecidos

- [ ] Migração GTK4 completa ainda FAIL no gate estrito (`84_..._runtime_fix.log`).
- [ ] JUnit local ainda pode ficar indisponível sem ambiente completo (`82_ui_metrics_autogen_after_hardening.log`).
- [ ] Runtime tests podem cair em SKIP quando toolchain local não compila todos os testes (`84_..._runtime_fix.log`).
- [ ] Validação de tablet com hardware real não executada.

## Rollback

- Onda 4 scripts: `git revert <commit_onda4>`
- Onda 3 código: `git revert a2de1556f8`
- Onda 2 código: `git revert fdfa75aaec`
- Revalidação pós-rollback:
  - `sh tools/ci/gtk4-functional-regression-suite.sh`
  - `sh tools/ci/gtk4-readiness.sh`
  - `sh tools/ci/e2e-core-checklist.sh`
  - `sh tools/ci/ui-visual-diff-smoke.sh`

## Gate recomendado para merge

- Exigir PASS de:
  - build do alvo impactado
  - `gtk4-readiness`
  - `e2e-core-checklist`
  - `ui-visual-diff-smoke`
  - `ui-visual-diff` (strict)
- Manter `gtk4-complete-migration-check` como objetivo de programa até zerar superfície legada crítica.
