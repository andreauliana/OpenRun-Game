# Esito verifica statica prova_gate.cpp — @developer/@developer-junior — 17 ott 2026 13:20

File: motore_nuovo/test/prova_gate.cpp (152 righe, // SPDX-License-Identifier: MIT riga 1)
Target CMake: prova_gate (CMakeLists.txt:111-114), include_dir = src, linka solo sglavinated_core (core-only, nessun SDL, output PPM).

## Esito
| Check | Esito | Evidenza |
|---|---|---|
| Include path | KO | :9 `#include "motore_nuovo/src/game/game.h"` ; :10 `#include "motore_nuovo/src/render/framebuffer.h"`. Con include_dir=src servono "core/game.h" e "render/framebuffer.h". Blocca build target prova_gate. |
| Exit code speed>0 frame 60 | OK | :108-110 cattura playerSpeed al frame 60; :150 `return (speed_at_frame_60 > 0.0f) ? 0 : 1;`. |
| frameHash ogni 60 frame | OK | :114 `u32 fhash = game.frameHash();`; :124-127 `if (frame % 60 == 0) printf("frameHash %u\n", fhash);`; log stdout + file. |
| frameHash copre stato | OK | motore_nuovo/src/game/game.cpp:372-389: framebuffer, frame, playerSpeed, playerX, playerZ, playerOffroad, rng_.state(). |
| Determinismo (regola 5b) | Parziale | frameHash esiste, ma replay da file (--record/--replay) assente: test "stesso file input 2x -> stesso hash/60" non eseguibile senza attrezzo @developer (voci 77/88/90). |
| Path log/screenshots | Da verificare a runtime | "out/debugger/prova_gate.log" e prova_gate_{partenza,curva,fuoristrada}.ppm sono relativi al cwd dell'eseguibile (eseguire da sglavinated_src/ su Pi 400). |

## Correzione proposta (diff, NON applicata — regola debugger, applica @developer)
rep('#include "motore_nuovo/src/game/game.h"', '#include "core/game.h"')  prova_gate.cpp:9
rep('#include "motore_nuovo/src/render/framebuffer.h"', '#include "render/framebuffer.h"')  prova_gate.cpp:10

## Blocco H
Stato [~] invariato. Build cmake, replay 2x stesso input (frameHash/60 identico), ms/frame <=20ms (obiettivo <=12ms), fps 50 stabili, temperatura, prova_gate 10s headless + 3 screenshot out/debugger/ + log velocita/posizione: NON eseguibili qui (regola 21, shell/compilatore negati). Da eseguire su Pi 400 con toolchain Andrea (voce 1138, D9 DEFAULT: cmake --build build --target prova_gate).

## ComandI statici eseguibili (quando sbloccato)
cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target prova_gate
./build/prova_gate (da sglavinated_src/)
clang++ -std=c++17 -fsyntax-only motore_nuovo/test/prova_gate.cpp
