# BACKLOG

## Priorização consolidada

Score = (Impacto × Probabilidade) ÷ Esforço

| ID | Categoria | Severidade | Impacto | Prob. | Esforço | Risco Regressão | Urgência | Score |
|---|---|---|---:|---:|---:|---|---|---:|
| B001 | GTK4 migração completa | Alta | 10 | 10 | 7 | Médio | Alta | 14.29 |
| B002 | Migração TreeView/Selection | Alta | 8 | 9 | 5 | Médio | Alta | 14.40 |
| B003 | Observabilidade de testes | Média | 6 | 8 | 2 | Baixo | Média | 24.00 |
| B004 | Segurança/segredos | Média | 7 | 4 | 3 | Baixo | Média | 9.33 |
| B005 | Desktop tablet real | Média | 6 | 6 | 6 | Médio | Média | 6.00 |

## Achados detalhados

### B001 — Migração GTK4 completa ainda não confirmada
- Categoria: Migração GTK4.
- Evidência:
  - `sh tools/ci/gtk4-complete-migration-check.sh` => FAIL (`reports/EVIDENCE/73_after_wave3_complete.log`).
  - Contagens legadas atuais relevantes: `L01=1820`, `L03=513`, `L04=890`, `L05=665`.
- Impacto: bloqueia fechamento formal da migração completa para GTK4.
- Causa provável: superfície legada histórica ainda ampla em widgets/core.
- Recomendação: manter ondas incrementais por arquivo, começando por baixa cardinalidade remanescente (`gimpenumwidgets.c`, `gimppropwidgets.c`, `gimpcolorscales.c`).
- Validação atual: FAIL.
- Risco + mitigação: risco de regressão de UI; mitigar com build alvo + `gtk4-readiness` + `e2e-core-checklist` + `ui-visual-diff-smoke` por onda.
- Rollback: `git revert <commit_da_onda>`.

### B002 — Superfície legada GtkTreeView/selection ainda grande
- Categoria: UI/arquitetura de widgets.
- Evidência: `L05=665` e `L06=201` em `reports/EVIDENCE/73_after_wave3_complete.log`.
- Impacto: porta para GTK4 exige transformação estrutural além de wrappers simples.
- Causa provável: dependência histórica de `GtkTreeView`/`GtkTreeSelection`.
- Recomendação: roadmap por domínio (listas simples -> `GtkListView`; estruturas tabulares -> `GtkColumnView`).
- Validação atual: FAIL (gate completo).
- Risco + mitigação: regressão funcional de seleção/edição; mitigar com testes específicos por widget.
- Rollback: revert por commit de onda.

### B003 — Métricas de teste incompletas por ausência de JUnit local
- Categoria: Observabilidade/QA.
- Evidência: `reports/EVIDENCE/17_baseline_ui_metrics.log` (sem `testlog.junit.xml`).
- Impacto: reduz visibilidade de tendência de testes.
- Recomendação: inserir geração local de JUnit antes de `ui-test-metrics.sh`.
- Validação atual: PASS técnico do script, cobertura de métrica parcial.
- Rollback: remoção do ajuste de pipeline local.

### B004 — Verificação de segredos com falso-positivo alto
- Categoria: Segurança.
- Evidência: scan regex com alto ruído em `reports/EVIDENCE/23_security_desktop_scan.log`.
- Impacto: precisão baixa para achar segredo real.
- Recomendação: scanner dedicado com baseline/allowlist.
- Validação atual: NÃO VERIFICADO para exposição real de segredo.
- Rollback: desativar scanner adicional se necessário.

### B005 — Matriz tablet validada apenas por checklist
- Categoria: Desktop/entrada de dispositivo.
- Evidência: `reports/EVIDENCE/20_baseline_tablet_matrix.log`.
- Impacto: cobertura parcial de comportamento real de hardware.
- Recomendação: job opcional com evidência de execução em hardware.
- Validação atual: PASS de checklist, runtime real NÃO VERIFICADO.
- Rollback: desativar job opcional de hardware.
