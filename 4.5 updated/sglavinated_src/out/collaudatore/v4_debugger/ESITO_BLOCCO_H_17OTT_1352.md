# Esito blocco H — @debugger — 17/10/2026 13:52

Blocco H [~]: build/replay/fps/temperatura NON eseguibili in questo ambiente (regola 21: shell negata, nessun cmake/compilatore qui). Da eseguire su Pi 400 con toolchain Andrea (voce SCAMBIO 1138, D9 DEFAULT: cmake --build build --target prova_gate).

Verifica statica su disco (lettura file, nessuna modifica a codice di prodotto):
1. prova_gate.cpp riga 1: // SPDX-License-Identifier: MIT [OK]
2. prova_gate.cpp righe 13-14: #include "game/game.h" + #include "render/framebuffer.h" — allineati a include_dir=src (CMakeLists:113) [OK]
3. road.cpp riga 108: if (segs_.empty() || totalLen_ <= 0) return; [OK]
4. CMakeLists.txt righe 109-113: target prova_gate attivo quando test/prova_gate.cpp esiste [OK]
5. prova_gate.cpp: smoke test 500 frame (10s @50Hz), soak 30000 frame, determinismo runA==runB, frameHash ogni 60 frame, exit 0 se speed>0 frame 60, ms/frame medio/max, 3 screenshot PPM in out/debugger/ [OK statico]

Tre rilievi del reviewer CHECKPOINT 1 (13:26, voci SCAMBIO 1135/1136/1139) gia' sanati su disco da @coder: include path errato, guard strada vuota, intestazione SPDX mancante. Confermati su disco (lettura diretta).

BLOCCATORE AMBIENTALE: build cmake, replay deterministico 2x stesso input (frameHash/60 identico), ms/frame (<=20ms, obiettivo <=12), fps 50 stabili, temperatura, 3 screenshot out/debugger/ (partenza/curva/fuori strada) + log velocita/posizione non eseguibili qui. Rimandato a Pi 400 con toolchain Andrea. H resta [~].

File di prova: motore_nuovo/test/prova_gate.cpp (core-only, nessun SDL, output PPM, SPDX MIT).
Livello default: lv_01_neon (motore_nuovo/dati/livelli/lv_01_neon/livello.json presente; soak anche su lv_02_atomico, lv_03_foglie).
Test deterministico 2x: gia' implementato in prova_gate.cpp (esegui(outDir,livello,true,.) poi esegui(outDir,livello,false,.) con confronto a.hash==b.hash).
