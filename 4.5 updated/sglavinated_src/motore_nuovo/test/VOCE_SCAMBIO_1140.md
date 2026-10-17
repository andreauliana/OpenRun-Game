### 1140. DEBUGGER -> CAPO | stato [APERTA] | 17 ott 2026 13:20
Richiesta/Esito: Blocco H [~]. Verifica statica prova_gate.cpp (motore_nuovo/test/, 152 righe, SPDX MIT riga 1).
Causa probabile: include path errato linee 9-10 ("motore_nuovo/src/game/game.h", "motore_nuovo/src/render/framebuffer.h"); target prova_gate ha include_dir=src (CMakeLists:113) -> servono "core/game.h" e "render/framebuffer.h", altrimenti build fallisce e H non parte.
Prova: read prova_gate.cpp:9-10 (include errati) + grep CMakeLists:113 (include_dir=src). Uscite già OK su disco: exit code speed>0 frame 60 (linee 108-110, 150); frameHash ogni 60 frame (linee 114, 124-127); game.cpp:372-389 include playerX/Z/offroad/rng (regola 5b).
Diff proposta (NON applicata, regola debugger, applica @coder):
  rep('#include "motore_nuovo/src/game/game.h"', '#include "core/game.h"')  prova_gate.cpp:9
  rep('#include "motore_nuovo/src/render/framebuffer.h"', '#include "render/framebuffer.h"')  prova_gate.cpp:10
Rilievo residuo non bloccante: replay da file (--record/--replay) assente -> test deterministico stesso-input-2x non eseguibile senza attrezzo @coder (voci 77/88/90).
Build/replay/fps/temperatura NON eseguibili qui (regola 21, shell negata): da eseguire su Pi 400 con toolchain Andrea (voce 1138, D9 DEFAULT cmake --build build --target prova_gate). H resta [~]. Esito in out/debugger/ESITO_VERIFICA_STATICA_PROVA_GATE_17OTT_1320.md. Voce da pubblicare su SCAMBIO.md quando sbloccata (SCAMBIO.md write-protetta per debugger, regola 21).
