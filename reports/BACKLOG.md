# BACKLOG

## Priorização consolidada

Score = (Impacto × Probabilidade) ÷ Esforço

| ID | Categoria | Severidade | Impacto | Prob. | Esforço | Risco Regressão | Urgência | Score |
|---|---|---|---:|---:|---:|---|---|---:|
| B001 | GTK4 migração | Alta | 10 | 10 | 7 | Médio | Alta | 14.29 |
| B002 | GTK4 widgets legados | Alta | 8 | 9 | 5 | Baixo | Alta | 14.40 |
| B003 | Observabilidade de testes | Média | 6 | 8 | 2 | Baixo | Média | 24.00 |
| B004 | Segurança/segredos | Média | 7 | 4 | 3 | Baixo | Média | 9.33 |
| B005 | Desktop tablet real | Média | 6 | 6 | 6 | Médio | Média | 6.00 |

## Achados detalhados

### B001 — Migração GTK4 completa ainda não confirmada
- Categoria: Migração GTK4.
- Evidência:
  - Comando+saída: `sh tools/ci/gtk4-complete-migration-check.sh` => FAIL (`reports/EVIDENCE/21_baseline_gtk4_complete.log`, `reports/EVIDENCE/42_after_gtk4_complete.log`).
  - Contagens legadas ainda altas: L01=1827, L03=514, L04=891, L05=665.
- Impacto: bloqueia fechamento de migração completa para GTK4.
- Causa provável: superfície legada ampla (box/container/tree/file-chooser/clipboard/dnd/window APIs antigas).
- Recomendação: continuar ondas em `libgimpwidgets` e `app/widgets` com wrappers/port direto, priorizando arquivos com baixa cardinalidade de chamadas.
- Validação atual: FAIL.
- Risco + mitigação: risco de regressão visual/comportamental; mitigar com `ui-visual-diff-smoke`, `e2e-core-checklist`, build incremental por onda.
- Rollback: `git revert <commit_da_onda>`.

### B002 — Superfície legada GtkTreeView/selection ainda grande
- Categoria: UI/arquitetura de widgets.
- Evidência: `L05=665` e `L06=201` em `reports/EVIDENCE/42_after_gtk4_complete.log`.
- Impacto: porta para GTK4 exige retrabalho estrutural de modelos/seleção.
- Causa provável: dependência histórica de `GtkTreeView` em widgets centrais.
- Recomendação: criar plano por domínio (listas simples -> `GtkListView`, tabelas complexas -> `GtkColumnView`) com pilotos por componente.
- Validação atual: FAIL (gate completo).
- Risco + mitigação: regressão funcional em seleção/edição; mitigar com testes de roundtrip/reorder/selection por widget.
- Rollback: revert por commit de onda.

### B003 — Métricas de teste incompletas por ausência de JUnit local
- Categoria: Observabilidade/QA.
- Evidência: `No testlog.junit.xml found locally; skipping metrics extraction.` (`reports/EVIDENCE/17_baseline_ui_metrics.log`).
- Impacto: perda de visibilidade quantitativa de regressão de testes.
- Causa provável: execução local sem geração prévia de JUnit.
- Recomendação: gerar JUnit local antes de `ui-test-metrics.sh` ou ajustar script para fallback explícito com status distinto.
- Validação atual: PASS técnico do script, FAIL de cobertura de métrica.
- Risco + mitigação: risco baixo; mitigar com etapa fixa no pipeline local.
- Rollback: remoção de ajuste de script/etapa.

### B004 — Verificação de segredos com falso-positivo alto
- Categoria: Segurança.
- Evidência: scan regex capturou muitos “token” sem segredo real (`reports/EVIDENCE/23_security_desktop_scan.log`).
- Impacto: baixa precisão para detectar credenciais reais.
- Causa provável: regra genérica sem contexto semântico.
- Recomendação: adotar scanner especializado (ex.: gitleaks/trufflehog) com baseline e allowlist versionada.
- Validação atual: NÃO VERIFICADO para exposição real de segredo.
- Risco + mitigação: risco de falso negativo; mitigar com scanner semântico e CI blocking em padrões críticos.
- Rollback: remover job/scanner e restaurar baseline anterior.

### B005 — Matriz tablet validada apenas por checklist
- Categoria: Desktop/entrada de dispositivo.
- Evidência: `tablet-matrix-check` valida estrutura e IDs, não execução com hardware (`reports/EVIDENCE/20_baseline_tablet_matrix.log`).
- Impacto: cobertura parcial para regressões de stylus/tilt/eraser em runtime.
- Causa provável: indisponibilidade de hardware em ambiente local/CI.
- Recomendação: adicionar job opcional com evidência real (logs/screencast/artefatos) quando hardware estiver disponível.
- Validação atual: PASS de checklist, NÃO VERIFICADO de comportamento real.
- Risco + mitigação: regressões específicas de driver/plataforma; mitigar com janela de validação manual assistida por script.
- Rollback: desativar job de hardware opcional.
