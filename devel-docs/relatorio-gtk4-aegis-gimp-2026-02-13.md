# Relatorio Unificado GTK4 + AEGIS UI (GIMP)

Data: 2026-02-13
Repositorio: `master...origin/master` (working tree com alteracoes locais desta rodada)
Escopo desta auditoria: arvore local de codigo/configuracao; sem execucao de benchmark E2E nesta rodada.

## A) Resumo executivo
- O build atual esta ancorado em GTK3 (`gtk+-3.0 >= 3.24.0`) e nao ha dependencia Meson para GTK4 no estado atual (`meson.build:484`, `meson.build:485`).
- O risco de migracao e alto nos pontos obrigatorios do GIMP: docking, canvas/renderizacao, input/tablet, file dialogs, atalhos e color management (`app/display/gimpdisplayshell.c:566`, `app/widgets/gimpfiledialog.h:36`, `app/dialogs/welcome-dialog.c:326`, `app/main.c:503`, `app/core/gimpimage-color-profile.c:182`).
- Ha sinais de preparacao para GTK4, mas ainda pontuais (comentarios TODO e notas de release), nao estruturais (`NEWS:419`, `app/widgets/gimptextstyleeditor.c:223`, `libgimpwidgets/gimpfilechooser.c:364`).
- Superficie legada de API GTK3 e extensa, com uso massivo de `gtk_box_pack_start`, `gtk_container_add` e `GtkTreeView` (contagens desta auditoria).
- Fluxos criticos de produto (abrir/salvar/exportar, undo/redo, janelas/docks, filtros) estao explicitamente modelados em actions e testes de UI (`app/actions/file-actions.c:73`, `app/actions/edit-actions.c:65`, `app/actions/windows-actions.c:120`, `app/actions/filters-actions.c:66`, `app/tests/test-ui.c:602`).
- Regressao de layout de docks e persistencia de sessao tem cobertura de testes, o que reduz risco de migracao incremental de janela/dock (`app/tests/test-ui.c:227`, `app/tests/gimp-test-session-utils.c:172`).
- Extensibilidade (PDB, plug-ins, Script-Fu/Python) e eixo critico e deve ser congelada em compatibilidade durante a portabilidade (`app/pdb/gimppdb.h:58`, `app/plug-in/gimppluginmanager-file.c:155`, `plug-ins/script-fu/libscriptfu/README:34`).
- Distribuicao atual e multi-canal (Flatpak/AppImage/Snap/Windows installer/MS Store) e exige estrategia de runtime GTK4 por canal antes de mudar toolkit (`build/linux/flatpak/org.gimp.GIMP-nightly.json:4`, `.gitlab-ci.yml:511`, `build/windows/installer/base_gimp3264.iss:226`).
- Politica de update ja existe e pode ser usada como alavanca de rollout/rollback por canal (`meson_options.txt:18`, `app/config/gimpcoreconfig.c:661`, `app/gimp-update.c:564`).
- Status global desta auditoria para migracao GTK4: **FAIL** (baseline e riscos mapeados; migracao ainda nao iniciada por ondas).

## B) Escopo analisado + baseline

### Confirmado (com evidencia)
- Aplicacao desktop nativa GLib/GObject: confirmado por estrutura e dependencias C/GObject (`meson.build:1`, `meson.build:481`).
- Base GTK3: `gtk+-3.0 >= 3.24.0` (`meson.build:484`, `meson.build:485`).
- Sem dependencia GTK4 no Meson: nenhum match para `gtk4`/`gtk+-4.0` em `meson.build` e `meson_options.txt`.
- Dominio de imagem (babl/GEGL/lcms): (`meson.build:437`, `meson.build:453`, `meson.build:498`).
- Plataformas Linux/Windows no build: (`meson.build:178`, `meson.build:182`).
- X11 no Linux (dependencias e headers): (`meson.build:738`, `meson.build:746`, `meson.build:749`).
- Build/CI com Meson/Ninja: (`.gitlab-ci.yml:282`, `.gitlab-ci.yml:290`).
- CI Windows nativa MSYS2 ativa: (`.gitlab-ci.yml:511`, `.gitlab-ci.yml:593`).
- Suite de testes de UI da app: (`app/tests/meson.build:65`, `app/tests/meson.build:67`, `app/tests/test-ui.c:602`).
- Runtime/distribuicao Linux Flatpak atual em runtime GNOME com tag GTK+3: (`build/linux/flatpak/org.gimp.GIMP-nightly.json:4`, `build/linux/flatpak/org.gimp.GIMP-nightly.json:39`).
- Windows minimo suportado: (`devel-docs/os-support.txt:39`, `build/windows/store/AppxManifest.xml:22`, `build/windows/installer/base_gimp3264.iss:226`).

### Baseline tecnico (estado atual)
- Versao do projeto no Meson: `3.2.0-RC1+git` (`meson.build:3`).
- Estado de migração GTK4: pre-migracao (sem dependencia GTK4, com TODOs locais).
- Contagens de superficie legada (auditoria local):
  - `gtk_box_pack_start`: 1863 refs.
  - `gtk_container_add`: 535 refs.
  - `GtkTreeView|gtk_tree_view_`: 722 refs.
  - `GtkContainer|gtk_container_`: 915 refs.
  - `gtk_drag_dest_set`: 19 refs.
  - `gtk_drag_source_set`: 7 refs.
  - `gtk_selection_data_`: 80 refs.
  - `gtk_clipboard_`: 66 refs.
  - `GtkDropTarget`: 0 refs.
  - `GtkDragSource` (GTK4): 0 refs.
  - `GdkClipboard`: 0 refs.

### Nao verificado (com como verificar)
- Baseline de latencia p95 por fluxo (open/save/export/render/zoom): **NAO VERIFICADO**.
  - Como verificar:
    - Instrumentar tempo por acao em `app/actions/*` e pipeline de draw em `app/display/*`.
    - Rodar carga controlada com imagens grandes e coletar percentis.
- Crash rate por plataforma em producao: **NAO VERIFICADO**.
  - Como verificar:
    - Consolidar artefatos de `CrashLog` (`app/signals.c:89`) e logs de erros (`app/errors.c:125`) por build/versionamento.
- Regressao visual automatizada por screenshot diff: **NAO VERIFICADO**.
  - Como verificar:
    - Gerar baseline de screenshots por janela e comparar em CI (job dedicado).
- Cobertura de acessibilidade por leitor de tela/teclado em Linux e Windows: **NAO VERIFICADO**.
  - Como verificar:
    - Definir suite A11y com navegacao teclado/foco/ordem/tab e leitores de tela.
- Paridade de dialogs via portal para abrir/salvar/exportar: **NAO VERIFICADO**.
  - Como verificar:
    - Rodar em Flatpak sandbox com xdg-desktop-portal e validar fluxos de file chooser.
- Requisitos de firewall no Windows para este app desktop (sem servidor local): **NAO VERIFICADO** no repositorio.
  - Como verificar:
    - Validar comportamento real do instalador/execucao em VM limpa Windows com logging de prompts.

## C) Mapa do sistema e fluxos criticos (3-5 E2E)

### Mapa de componentes (alto nivel)
- Nucleo e dominio de imagem: `app/core/*`, GEGL/babl (`meson.build:437`, `meson.build:453`).
- UI principal e windows/docks: `app/widgets/*`, `app/display/*`, `app/actions/*`.
- Canvas/renderizacao: `app/display/gimpdisplayshell*.c`.
- Extensibilidade/PDB/plug-ins: `app/pdb/*`, `app/plug-in/*`, `plug-ins/*`.
- Config/update/sessao: `app/config/*`, `app/gimp-update.c`, `app/tests/gimp-test-session-utils.c`.
- Distribuicao: `.gitlab-ci.yml`, `build/linux/*`, `build/windows/*`.

### Fluxo E2E 1: Abrir -> editar -> salvar/exportar
- Entrada de acao: `file-open` (`app/actions/file-actions.c:73`).
- Persistencia: `file-save`/`file-save-as` (`app/actions/file-actions.c:136`).
- Exportacao: `file-export`/`file-export-as` (`app/actions/file-actions.c:162`, `app/actions/file-actions.c:174`).
- Gate de sensibilidade/estado (ex.: overwrite/export): (`app/actions/file-actions.c:287`).

### Fluxo E2E 2: Desfazer/refazer na edicao
- Acoes definidas: `edit-undo`/`edit-redo` (`app/actions/edit-actions.c:65`, `app/actions/edit-actions.c:71`).
- Atualizacao dinamica de label/sensibilidade: (`app/actions/edit-actions.c:342`, `app/actions/edit-actions.c:345`).

### Fluxo E2E 3: Layout de janelas, docks e modo single-window
- Acoes de janela: `windows-hide-docks`, `windows-use-single-window-mode` (`app/actions/windows-actions.c:120`, `app/actions/windows-actions.c:134`).
- Teste de restaurar dock fechado: (`app/tests/test-ui.c:227`).
- Teste de manter geometria ao hide/show docks: (`app/tests/test-ui.c:280`).
- Persistencia de `sessionrc`/`dockrc`: (`app/tests/gimp-test-session-utils.c:172`, `app/tests/gimp-test-session-utils.c:230`).

### Fluxo E2E 4: Ferramentas de pintura/texto e filtros
- Ferramenta de texto (eventos, desenho, popup): (`app/tools/gimptexttool.c:91`, `app/tools/gimptexttool.c:135`).
- Ferramenta de pintura (press/motion/release): (`app/tools/gimppainttool.c:72`, `app/tools/gimppainttool.c:84`, `app/tools/gimppainttool.c:401`).
- Filtros GEGL (interactive/non-interactive): (`app/actions/filters-actions.c:66`, `app/actions/filters-actions.c:134`).

### Fluxo E2E 5: Plug-ins/scripts e PDB
- Registro e lookup de procedimentos: (`app/pdb/gimppdb.h:58`, `app/pdb/gimppdb.h:63`).
- Resolucao de file procedures para open/save/export: (`app/plug-in/gimppluginmanager-file.c:141`, `app/plug-in/gimppluginmanager-file.c:170`).
- Script-Fu como plug-in/PDB em processo dedicado: (`plug-ins/script-fu/libscriptfu/README:34`).

## D) Achados detalhados

### D-01 Build travado em GTK3
- Categoria: Arquitetura/Toolkit
- Severidade: Alta
- Evidencia: `meson.build:484`, `meson.build:485`.
- Impacto: bloqueia compilacao dual GTK3/GTK4 e impede onda 2.
- Causa provavel: inexistencia de camada de compatibilidade no build.
- Recomendacao (o que e onde): adicionar opcoes de build por toolkit em `meson_options.txt` e ramificacao controlada em `meson.build`.
- Validacao PASS/FAIL: **FAIL** (GTK4 nao compilavel).
- Risco e mitigacao: risco alto de explosao de `#ifdef`; mitigar com wrappers por subsistema (atalho, file chooser, DnD, tree/list).

### D-02 Superficie legada de containers/layout muito alta
- Categoria: GTK4 Porting
- Severidade: Alta
- Evidencia: contagens desta auditoria (`gtk_box_pack_start=1863`, `gtk_container_add=535`, `GtkContainer*=915`).
- Impacto: custo alto e risco de regressao visual/funcional em larga escala.
- Causa provavel: UI historica GTK3 com hierarquias profundas.
- Recomendacao: priorizar migracao por vertical funcional (janela/dialogo), nao por busca/replace global.
- Validacao PASS/FAIL: **FAIL** (API removida em GTK4 ainda predominante).
- Risco e mitigacao: regressao de layout; mitigar com baseline visual por janela antes/depois.

### D-03 Dependencia extensa de GtkTreeView
- Categoria: GTK4 Porting / UI
- Severidade: Alta
- Evidencia: 722 refs; exemplo em `app/widgets/gimpcontainertreeview.c:254`, `libgimpwidgets/gimppatheditor.c:269`.
- Impacto: widgets centrais de listas/arvores sem equivalente drop-in no GTK4.
- Causa provavel: arquitetura de modelos/colunas fortemente acoplada a TreeView.
- Recomendacao: criar adaptador de modelo e migrar pilotos para `GtkListView`/`GtkColumnView` em ondas.
- Validacao PASS/FAIL: **FAIL**.
- Risco e mitigacao: perda de UX (selecao/edicao/drag). Mitigar com testes de comportamento por widget migrado.

### D-04 File dialogs acoplados a GtkFileChooserDialog
- Categoria: Integracao SO / Portabilidade
- Severidade: Alta
- Evidencia: `app/widgets/gimpfiledialog.h:36`, `app/widgets/gimpfiledialog.h:69`, `app/widgets/gimpfiledialog.c:347`; comentario de migracao em `libgimpwidgets/gimpfilechooser.c:364`.
- Impacto: alto risco em sandbox/portal e em UX nativa por plataforma.
- Causa provavel: design herdado com heranca direta de `GtkFileChooserDialog`.
- Recomendacao: introduzir backend abstrato de file dialog e manter paridade de filtros/preview/metadados.
- Validacao PASS/FAIL: **FAIL** (sem caminho GTK4/portal consolidado para open/save/export).
- Risco e mitigacao: quebra de fluxos core; mitigar com suite E2E de file open/save/export em Linux/Windows.

### D-05 Clipboard e Drag&Drop legados
- Categoria: GTK4 Porting
- Severidade: Alta
- Evidencia: `app/widgets/gimpclipboard.c:593`, `app/widgets/gimpclipboard.c:643`, `app/widgets/gimpclipboard.c:690`; `libgimpwidgets/gimpcolorarea.c:243`, `libgimpwidgets/gimpcolorarea.c:329`, `libgimpwidgets/gimpcolorarea.c:935`; `libgimp/gimpimagecombobox.c:102`, `libgimp/gimpimagecombobox.c:232`.
- Impacto: risco de regressao de interoperabilidade com outros apps e perda de formatos.
- Causa provavel: dependencia de `GtkClipboard`, `GtkSelectionData` e API DnD GTK3.
- Recomendacao: camada `clipboard_dnd_compat` para transicao controlada para `GdkClipboard`/`GtkDropTarget`/`GtkDragSource`.
- Validacao PASS/FAIL: **FAIL**.
- Risco e mitigacao: regressao silenciosa de formatos; mitigar com matriz de testes de texto/imagem/arquivo.

### D-06 Canvas/renderizacao com caminho critico sensivel
- Categoria: Performance/Confiabilidade
- Severidade: Alta
- Evidencia: `app/display/gimpdisplayshell.c:566`, `app/display/gimpdisplayshell.c:635`, `app/display/gimpdisplayshell-draw.c:142`, `app/display/gimpdisplayshell-render.c:144`, `app/display/gimpcanvas.c:211`.
- Impacto: risco de jank/redraw/correcao visual em imagens grandes durante port.
- Causa provavel: pipeline customizado com cache cairo e chunking acoplado a sinais GTK3.
- Recomendacao: migrar canvas somente apos baseline de fps/redraw/latencia com corpus de imagens.
- Validacao PASS/FAIL: **FAIL** (baseline de performance ainda NAO VERIFICADO).
- Risco e mitigacao: degradacao perceptivel. Mitigar com gates de performance por fluxo.

### D-07 Atalhos no Welcome Dialog com API marcada para migracao
- Categoria: UI/Atalhos/A11y
- Severidade: Media
- Evidencia: `app/dialogs/welcome-dialog.c:326`, `app/dialogs/welcome-dialog.c:328`.
- Impacto: risco de perda de atalhos ao portar janelas/dialogos.
- Causa provavel: uso direto de `GtkAccelGroup` local.
- Recomendacao: encapsular registro de atalhos em helper com backends GTK3/GTK4.
- Validacao PASS/FAIL: **FAIL** para readiness GTK4.
- Risco e mitigacao: regressao de teclado; mitigar com testes automatizados de shortcut por acao.

### D-08 Input avancado (tablet/caneta) com pontos especificos por plataforma
- Categoria: Integracao SO
- Severidade: Alta
- Evidencia: Linux tilt em `modules/controller-linux-input.c:97`; Windows tilt em `modules/controller-dx-dinput.c:464`; variavel Win32 em `app/main.c:503`, `app/main.c:519`; picker Win32 TODO GTK4 em `libgimpwidgets/gimppickbutton-win32.c:54`.
- Impacto: alto risco funcional para artistas digitais (pressao/inclinacao).
- Causa provavel: dependencia de APIs de backend GTK3/Win32.
- Recomendacao: congelar contrato de input e criar suite de validacao de caneta em Linux+Windows antes da onda 3.
- Validacao PASS/FAIL: **FAIL** (paridade GTK4 NAO VERIFICADA).
- Risco e mitigacao: regressao de precision e atalhos de caneta; mitigar com smoke manual orientado por dispositivo.

### D-09 Color management e funcionalidade critica
- Categoria: Dominio
- Severidade: Alta
- Evidencia: `app/core/gimpimage-color-profile.c:182`, `app/display/gimpdisplayshell-profile.c:104`, `app/display/gimpdisplayshell-profile.c:160`, `app/pdb/image-color-profile-cmds.c:610`, `app/pdb/image-color-profile-cmds.c:873`.
- Impacto: regressao de fidelidade de cor e fluxo profissional.
- Causa provavel: pipeline multi-perfil com transformacoes em tempo de display e PDB.
- Recomendacao: gate de release com corpus ICC + comparacao de histograma/DeltaE por cenarios chave.
- Validacao PASS/FAIL: **FAIL** (gate ainda inexistente nesta auditoria).
- Risco e mitigacao: regressao dificil de detectar; mitigar com testes automatizados + aprovacao humana.

### D-10 UI AEGIS: cobertura funcional existe, cobertura visual/a11y automatizada e insuficiente
- Categoria: UI/UX/A11y/QA
- Severidade: Media
- Evidencia: testes de janela/dock em `app/tests/test-ui.c:227`, `app/tests/test-ui.c:280`, `app/tests/test-ui.c:606`; sem evidencias de pipeline de diff visual dedicado nesta auditoria.
- Impacto: risco de regressao pixel/layout em migracao de containers e lists.
- Causa provavel: foco atual em testes funcionais e nao em baseline visual.
- Recomendacao: adicionar baseline screenshot por janela critica e checks de foco/teclado por viewport.
- Validacao PASS/FAIL: **FAIL** para criterio AEGIS completo.
- Risco e mitigacao: regressao visual tardia; mitigar com job de QA visual em CI.

### D-11 Distribuicao e update multi-canal exigem estrategia de rollout
- Categoria: Release/Operacao
- Severidade: Media
- Evidencia: Flatpak runtime atual (`build/linux/flatpak/org.gimp.GIMP-nightly.json:4`), canais Windows (`.gitlab-ci.yml:511`, `.gitlab-ci.yml:593`), opcao de update (`meson_options.txt:18`, `app/gimp-update.c:564`).
- Impacto: risco operacional alto se toolkit mudar sem rollout por canal.
- Causa provavel: ecossistema de distribuicao heterogeneo.
- Recomendacao: rollout faseado por canal e kill-switch de update por pacote quando necessario.
- Validacao PASS/FAIL: **FAIL** (plano de rollout GTK4 ainda nao formalizado).
- Risco e mitigacao: suporte fragmentado; mitigar com ondas e matriz por canal.

### D-12 Modo Desktop "Web UI local" e firewall
- Categoria: Compliance do prompt
- Severidade: Baixa
- Evidencia: sem servidor local identificado; busca textual sem ocorrencias de firewall no repositorio nesta auditoria.
- Impacto: baixo para GIMP nativo atual.
- Causa provavel: nao aplicavel ao produto atual.
- Recomendacao: marcar como N/A para GIMP neste escopo; validar apenas em caso de novos componentes de rede local.
- Validacao PASS/FAIL: **PASS (N/A)**.
- Risco e mitigacao: se surgir feature de servidor local, reavaliar bind `127.0.0.1` e prompts de firewall.

## E) Patches propostos (nao aplicados nesta rodada)

### P-01 Shortcut bridge GTK3/GTK4
- Objetivo: eliminar acoplamento direto de `GtkAccelGroup` em dialogs.
- Arquivos alvo:
  - `app/dialogs/welcome-dialog.c`
  - `app/widgets/gimpshortcutbridge.h` (novo)
  - `app/widgets/gimpshortcutbridge.c` (novo)
- Mudanca proposta:
  - extrair registro de atalhos para helper unico com backend GTK3 hoje e GTK4 depois.
- Rollback: remover helper e voltar chamadas diretas no dialog.
- Validacao: shortcuts `app.image-new` e `app.file-open` funcionais antes/depois.

### P-02 Abstracao de file dialogs
- Objetivo: desacoplar `GimpFileDialog` de heranca direta em `GtkFileChooserDialog`.
- Arquivos alvo:
  - `app/widgets/gimpfiledialog.h`
  - `app/widgets/gimpfiledialog.c`
  - `app/widgets/gimpfiledialog-backend.h` (novo)
  - `app/widgets/gimpfiledialog-backend-gtk3.c` (novo)
- Mudanca proposta:
  - interface interna com operacoes open/save/export + filtros/preview.
- Rollback: backend unico GTK3 sem mudar call sites externos.
- Validacao: checklist E2E open/save/export em Linux/Windows.

### P-03 Job CI de prontidao GTK4
- Objetivo: impedir aumento de superficie legada durante transicao.
- Arquivos alvo:
  - `.gitlab-ci.yml`
  - `tools/ci/gtk4-readiness.sh` (novo)
- Mudanca proposta:
  - job que conta APIs legadas e falha se ultrapassar threshold acordado.
- Rollback: desabilitar job por regra CI.
- Validacao: pipeline falha ao introduzir novo uso de API banida.

### P-04 Camada de compatibilidade DnD/Clipboard
- Objetivo: migrar por etapas sem quebrar formatos.
- Arquivos alvo:
  - `app/widgets/gimpclipboard.c`
  - `libgimpwidgets/gimpcolorarea.c`
  - `libgimp/gimpimagecombobox.c`
  - `libgimpwidgets/gimpdndclipboard-compat.[ch]` (novo)
- Mudanca proposta:
  - wrappers para set/get de texto/imagem/arquivo e drag source/target.
- Rollback: manter implementacao GTK3 por macro de build.
- Validacao: interop com apps externos em matriz de formatos.

### P-05 Piloto de migracao TreeView -> List/ColumnView
- Objetivo: reduzir risco antes de migrar areas criticas.
- Arquivos alvo:
  - `libgimpwidgets/gimppatheditor.c` (piloto)
- Mudanca proposta:
  - substituir widget de lista de pastas por API moderna em piloto isolado.
- Rollback: flag de build para fallback TreeView.
- Validacao: adicionar/remover/selecionar pasta e persistencia de estado.

### P-06 QA visual automatizado
- Objetivo: garantir criterio AEGIS de regressao visual.
- Arquivos alvo:
  - `app/tests/` (novos testes)
  - `.gitlab-ci.yml` (job dedicado)
- Mudanca proposta:
  - baseline screenshot por janela critica com diff por tolerancia.
- Rollback: manter job apenas em nightly inicialmente.
- Validacao: PASS quando diff dentro da tolerancia aprovada.

## F) Roadmap incremental (Ondas 1-4) + dependencias + riscos + rollback

### Onda 1 - Preparacao e baseline
- Entregas:
  - inventario congelado de APIs legadas por subsistema.
  - job `gtk4-readiness` em CI.
  - baseline funcional E2E (open/save/export, undo/redo, docks, filtros, pintura/texto).
- Dependencias:
  - corpus de testes e imagens.
  - criterios de PASS/FAIL por fluxo.
- Riscos:
  - baseline incompleto gera falso positivo de sucesso.
- Rollback:
  - thresholds em modo informativo por 1 sprint, depois bloquear.

### Onda 2 - Port de base (infra)
- Entregas:
  - suporte de build dual (GTK3/GTK4) com wrappers centrais.
  - migracao de atalhos/acoes e menus sem alterar comportamento.
- Dependencias:
  - P-01 e P-03.
- Riscos:
  - divergencia de comportamento entre toolkits.
- Rollback:
  - manter GTK3 default no release channel.

### Onda 3 - Casos dificeis
- Entregas:
  - file dialogs backend.
  - DnD/clipboard compat.
  - pilotos de TreeView migrados.
  - hardening de input/tablet.
- Dependencias:
  - Onda 2 estavel + matriz de hardware.
- Riscos:
  - regressao de fluxo profissional (tablet/cor).
- Rollback:
  - feature flags por subsistema, fallback GTK3.

### Onda 4 - Endurecimento e release
- Entregas:
  - regressao visual automatizada.
  - gates de performance e confiabilidade.
  - documentacao de suporte/rollback por canal.
- Dependencias:
  - cobertura E2E e observabilidade.
- Riscos:
  - custos de CI e tempo de validacao.
- Rollback:
  - rollout canario por canal (nightly -> beta -> estavel).

## G) Backlog executavel (priorizado)

Formula de score usada: `(Impacto x Probabilidade) / Esforco`, com escala numerica Alta=3, Media=2, Baixa=1; Esforco Pequeno=1, Medio=2, Grande=3.

| ID | Item | Severidade | Impacto | Prob. | Esforco | Risco Regressao | Urgencia | Score |
|---|---|---|---|---|---|---|---|---|
| B01 | Criar `gtk4-readiness` no CI | Alta | Operacao | Alta | Pequeno | Baixo | imediata | 9.0 |
| B02 | Definir baseline E2E obrigatorio (5 fluxos) | Alta | Usuario/Negocio | Alta | Medio | Medio | imediata | 4.5 |
| B03 | Implementar bridge de atalhos (P-01) | Media | Usuario | Media | Pequeno | Baixo | proxima sprint | 4.0 |
| B04 | Abstrair backend de file dialog (P-02) | Alta | Usuario/Operacao | Alta | Grande | Alto | proxima sprint | 3.0 |
| B05 | Criar camada DnD/clipboard compat (P-04) | Alta | Usuario | Alta | Grande | Alto | proxima sprint | 3.0 |
| B06 | Piloto de migracao TreeView em widget de baixo risco (P-05) | Media | Manutenibilidade | Media | Medio | Medio | proxima sprint | 2.0 |
| B07 | Instrumentar latencia p95 em fluxos core | Alta | Operacao/Custo | Media | Medio | Baixo | proxima sprint | 3.0 |
| B08 | Gate de regressao visual em CI (P-06) | Media | Usuario | Media | Medio | Medio | planejada | 2.0 |
| B09 | Matriz de teste tablet/caneta Linux+Windows | Alta | Usuario | Media | Grande | Alto | proxima sprint | 2.0 |
| B10 | Gate de color management (ICC corpus) | Alta | Usuario/Qualidade | Media | Medio | Medio | proxima sprint | 3.0 |
| B11 | Plano de rollout por canal (Flatpak/Win installer/MS Store) | Media | Operacao | Media | Medio | Baixo | planejada | 2.0 |
| B12 | Documento de rollback por onda | Media | Operacao | Baixa | Pequeno | Baixo | planejada | 2.0 |

## H) Instrumentacao e validacao continua

### Metricas minimas
- Funcional:
  - taxa de sucesso dos 5 fluxos E2E (%).
  - regressao por acao (open/save/export/undo/redo).
- Performance:
  - p95 de abertura de imagem (tamanhos S/M/L).
  - p95 de exportacao (PNG/JPEG/XCF).
  - tempo de redraw no canvas (zoom/pan em imagem grande).
- Confiabilidade:
  - crash rate por 1000 sessoes.
  - taxa de falha de plug-in por tipo.
- Qualidade UI:
  - diffs visuais por janela critica.
  - falhas de navegacao por teclado/foco.

### Fontes e hooks existentes reutilizaveis
- CrashLog e sinais: `app/signals.c:89`, `app/signals.c:97`.
- Logs de erro e backup recovery: `app/errors.c:110`, `app/errors.c:169`.
- Dashboard de recursos: `app/widgets/gimpdashboard.c:87`, `app/widgets/gimpdashboard.c:113`.
- Update/release metadata: `app/gimp-update.c:474`, `app/gimp-update.c:564`.

### Checks CI recomendados
- `check-gtk4-readiness`: contagem de APIs legadas.
- `check-e2e-core`: execucao da suite `app/tests/test-ui.c` e testes de sessao.
- `check-visual-baseline`: screenshot diff por janela.
- `check-color-icc`: conversoes de perfil e comparacao de saida.
- `check-input-tablet`: smoke por backend Linux/Windows (manual assistido + automatizado onde possivel).

## I) Checklist final QA/Release (PASS/FAIL)

| Item | Criterio | Status |
|---|---|---|
| Paridade funcional 100% (5 fluxos E2E) | mesmo resultado antes/depois | FAIL |
| Docking/persistencia (sessionrc/dockrc) | restauracao e geometria preservadas | PASS (GTK3 baseline) |
| Canvas performance gate | p95 dentro de SLO definido | FAIL (SLO NAO VERIFICADO) |
| Input tablet/caneta | pressao/inclinacao paritarios Linux+Windows | FAIL |
| File dialogs + portal | paridade open/save/export em sandbox | FAIL |
| Clipboard/DnD interop | texto/imagem/arquivo com apps externos | FAIL |
| Color management | ICC assign/convert/simulation sem regressao | FAIL |
| Regressao visual AEGIS | 0 clipping critico, sem overflow-x, overlays ok | FAIL |
| Acessibilidade minima | teclado/foco/labels/leitor de tela | FAIL |
| Rollback operacional | plano por onda e canal documentado/testado | FAIL |

## Anexo UI (AEGIS) - checklist final visual

- 0 clipping em janelas criticas (canvas/toolbox/docks/dialogs): **FAIL (NAO VERIFICADO sistematicamente)**.
- `overflow-x = 0` em containers criticos: **FAIL (NAO VERIFICADO por automacao)**.
- Overlays/dialogos sem vazamento visual/foco: **FAIL (NAO VERIFICADO por baseline automatizado)**.
- Navegacao por teclado e foco visivel em fluxos core: **FAIL (NAO VERIFICADO completo)**.
- Equivalencia funcional 100% antes/depois migracao: **FAIL (migracao ainda nao executada)**.

## Evidencias adicionais relevantes
- Preparacao explicita para GTK4 em release notes: `NEWS:419`, `NEWS:420`.
- Comentarios de migracao em widgets/dialogos:
  - `app/dialogs/welcome-dialog.c:326`
  - `app/widgets/gimptextstyleeditor.c:223`
  - `libgimpwidgets/gimpfilechooser.c:364`
  - `libgimpwidgets/gimppickbutton-win32.c:54`
- GResource + GtkBuilder em menus de plug-ins:
  - `plug-ins/gfig/gfig-menus.gresource.xml:3`
  - `plug-ins/help-browser/help-menus.gresource.xml:3`
  - `plug-ins/imagemap/imagemap-menus.gresource.xml:3`
  - `plug-ins/ifs-compose/ifs-menus.gresource.xml:3`
  - `app/widgets/gimpuimanager.c:1028`


## J) Atualizacao de execucao automatica (2026-02-13)

### Itens executados nesta rodada
- B01 concluido: gate de prontidao GTK4 adicionado e ativo em CI.
  - Evidencia: `tools/ci/gtk4-readiness.sh`, `.gitlab-ci.yml` job `gtk4-readiness`.
- B02 parcialmente concluido: baseline E2E com guard estatico e validacao de casos obrigatorios no parser de JUnit.
  - Evidencia: `tools/ci/e2e-core-checklist.sh`, `tools/ci/ui-test-metrics.sh`.
- B03 concluido: bridge de atalhos aplicado no welcome dialog.
  - Evidencia: `app/widgets/gimpshortcutbridge.[ch]`, `app/dialogs/welcome-dialog.c`.
- B04 parcialmente concluido: backend de file dialog desacoplado em camadas (backend generico + backend GTK3).
  - Evidencia: `app/widgets/gimpfiledialog-backend.[ch]`, `app/widgets/gimpfiledialog-backend-gtk3.[ch]`, `app/widgets/gimpfiledialog.c`.
- B05 parcialmente concluido: camada de compatibilidade de clipboard introduzida e aplicada em `app/widgets/gimpclipboard.c`.
  - Evidencia: `app/widgets/gimpclipboardcompat.[ch]`, `app/widgets/gimpclipboard.c`.
- B06 parcialmente concluido: piloto de migracao de view em `GimpPathEditor`, trocando `GtkTreeView`/`GtkTreeSelection` por `GtkListBox` com preservacao da API publica.
  - Evidencia: `libgimpwidgets/gimppatheditor.c`, `tools/ci/b06-path-editor-modernization.sh`, `.gitlab-ci.yml` job `b06-path-editor-modernization`.
- B07 parcialmente concluido: metrica p95 baseada em JUnit e top slow tests em CI.
  - Evidencia: `tools/ci/ui-test-metrics.sh`, job `ui-test-metrics`.
- B08 parcialmente concluido: check de baseline visual documental/screenshot manifest em CI.
  - Evidencia: `tools/ci/ui-visual-baseline-check.sh`, job `ui-visual-baseline-check`.
- B10 parcialmente concluido: check estatico de trilha Color/ICC em CI.
  - Evidencia: `tools/ci/color-icc-checklist.sh`, job `color-icc-checklist`.
- B11/B12 concluido em documentacao operacional.
  - Evidencia: `devel-docs/gtk4-rollout-rollback-plan.md`, `tools/ci/gtk4-rollout-doc-check.sh`, job `gtk4-rollout-doc-check`.

### Itens ainda pendentes (alto risco / depende de ambiente)
- B06 pendente (fase seguinte): evoluir piloto de `GtkListBox` para `GtkListView/GtkColumnView` quando o branch GTK4 estiver ativo no build.
- B09 pendente: matriz de teste tablet/caneta Linux+Windows com hardware real.
- B10 pendente (parte dinamica): gate com corpus ICC e comparacao de saida de imagem.
- B08 pendente (parte dinamica): diff visual automatizado de janelas da aplicacao (captura runtime).

### Estado consolidado do backlog
| ID | Estado |
|---|---|
| B01 | CONCLUIDO |
| B02 | PARCIAL |
| B03 | CONCLUIDO |
| B04 | PARCIAL |
| B05 | PARCIAL |
| B06 | PARCIAL |
| B07 | PARCIAL |
| B08 | PARCIAL |
| B09 | PENDENTE |
| B10 | PARCIAL |
| B11 | CONCLUIDO |
| B12 | CONCLUIDO |
