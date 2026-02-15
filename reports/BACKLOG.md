# BACKLOG

## Priorização consolidada

Score = (Impacto × Probabilidade) ÷ Esforço

| ID | Categoria | Severidade | Impacto | Prob. | Esforço | Risco Regressão | Urgência | Score |
|---|---|---|---:|---:|---:|---|---|---:|
| B001 | GTK4 migração completa | Alta | 10 | 10 | 7 | Médio | Alta | 14.29 |
| B002 | Migração TreeView/Selection | Alta | 8 | 9 | 5 | Médio | Alta | 14.40 |
| B003 | Observabilidade de testes | Média | 6 | 7 | 2 | Baixo | Média | 21.00 |
| B004 | Segurança/segredos | Média | 7 | 4 | 3 | Baixo | Média | 9.33 |
| B005 | Desktop tablet real | Média | 6 | 6 | 6 | Médio | Média | 6.00 |
| B006 | Toolchain local para runtime tests | Média | 7 | 8 | 3 | Baixo | Alta | 18.67 |

## Achados detalhados

### B001 — Migração GTK4 completa ainda não confirmada
- Evidência: `reports/EVIDENCE/90_gtk4_functional_suite_after_color_widgets.log` (`gtk4-complete-migration-check` FAIL).
- Contagens legadas atuais relevantes: `L01=1801`, `L02=102`, `L03=510`, `L04=887`, `L05=665`.
- Delta da última rodada: `reports/EVIDENCE/91_legacy_api_delta_after_color_widgets.tsv`.
- Impacto: bloqueia fechamento formal da migração completa GTK4.
- Recomendação: continuar ondas incrementais por arquivo com foco em APIs de maior volume.
- Validação: FAIL.
- Rollback: `git revert <commit_da_onda>`.

### B002 — Superfície legada GtkTreeView/selection ainda alta
- Evidência: `L05=665`, `L06=201` em `reports/EVIDENCE/89_gtk4_complete_migration_after_color_widgets.log`.
- Impacto: requer transformação estrutural além de wrappers simples.
- Recomendação: plano por domínio para `GtkListView`/`GtkColumnView`.
- Validação: FAIL (gate completo).

### B003 — JUnit local ainda não disponível por padrão
- Evidência: `reports/EVIDENCE/82_ui_metrics_autogen_after_hardening.log`.
- Situação atual: script melhorado tenta gerar JUnit automaticamente, mas pode cair em SKIP local.
- Impacto: métricas quantitativas podem continuar ausentes em ambiente incompleto.
- Recomendação: provisionar ambiente de teste completo e/ou definir `UI_METRICS_REQUIRED=1` em pipelines dedicados.
- Validação: PASS técnico do script, cobertura parcial.

### B004 — Verificação de segredos com falso-positivo alto
- Evidência: `reports/EVIDENCE/23_security_desktop_scan.log`.
- Impacto: baixa precisão para detectar segredo real.
- Recomendação: scanner dedicado com baseline/allowlist.
- Validação: NÃO VERIFICADO para exposição real.

### B005 — Matriz tablet validada por checklist, não hardware real
- Evidência: `reports/EVIDENCE/20_baseline_tablet_matrix.log`.
- Impacto: cobertura parcial de comportamento real.
- Recomendação: job opcional com evidência real de hardware.
- Validação: PASS checklist; runtime real NÃO VERIFICADO.

### B006 — Runtime tests dependem de toolchain/deps locais completos
- Evidência: `fatal error: gegl.h: No such file or directory` durante compilação de testes em `reports/EVIDENCE/90_gtk4_functional_suite_after_color_widgets.log`.
- Impacto: execução runtime local pode virar SKIP e reduzir profundidade de validação.
- Recomendação: padronizar ambiente local/CI com dependências necessárias para compilar todos os testes.
- Validação: mitigado por fallback SKIP controlado; pendência de ambiente.
