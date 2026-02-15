# AGENTS.md — Regras do repositório (Codex)

## Regra-mãe: zero intervenção
- NÃO faça perguntas ao usuário. Se houver ambiguidade, escolha a opção mais conservadora e registre como ASSUMIDO (com justificativa e risco).
- Se faltar evidência, tente obtê-la automaticamente (comandos, logs, diffs). Se ainda faltar: marque como NÃO VERIFICADO e descreva como o Codex tentaria verificar em execução.

## Evidência obrigatória
- Para cada afirmação técnica relevante, inclua evidência: arquivo+linha, comando+saída, log, endpoint, screenshot/diff.
- Todo patch deve indicar caminho do arquivo e contexto ANTES/DEPOIS.

## Segurança e reversibilidade
- Proibido remover/ocultar funcionalidades. Paridade funcional antes/depois = 100%.
- Mudanças pequenas, incrementais e reversíveis. Sempre inclua rollback.
- Evite comandos destrutivos (rebase forçado, limpeza agressiva) salvo absoluta necessidade e com mitigação explícita.

## Saídas padrão (sempre gerar)
- Criar reports/ contendo:
  - EXEC_SUMMARY.md
  - BACKLOG.md
  - BASELINE.md
  - VALIDATION.md
  - CHANGES.md (lista de commits/diffs)
  - reports/EVIDENCE/ (logs e artefatos)

## Skill padrão
- Para tarefas de orquestração completa (desktop + AEGIS UI + GTK4), use a skill:
  $orquestrador-unificado
