# Blocco H — esito turno 17/10/2026 12:05 (@debugger)
Stato: [~] — build cmake, replay deterministico, ms/frame, fps e temperatura NON eseguibili in questo ambiente (shell negata, regola 21); rimandati a Pi 400 con toolchain Andrea (D9: solo `cmake --build`).

Verifica statica (lettura, nessun comando eseguito):
- motore_nuovo/test/prova_gate.cpp presente (145 righe): core-only, nessun SDL, output PPM puro, guida deterministica 500 frame = 10s @ 50 Hz, 3 screenshot (frame 1/100/300) in out/debugger/, log CSV frame/velocita/playerX/playerZ/offroad/frameHash, seed fisso 20261015u, nessun orologio di sistema.
- Target CMake `prova_gate` righe 110-114, attivo quando prova_gate.cpp esiste.
- Sintassi nucleo ultima verifica: OK 34/34 file (voce 76/78); voce 87 (core/rng.h getter Rng::state(), game.cpp frameHash con playerX/playerZ/offroad/rng_.state()) letta su disco, non rivista con -fsyntax-only (nessun compilatore qui).

3 correzioni proposte a @coder in SCAMBIO voce 1135 (diff rep(old,new), io non applico, regola 15):
1. CMakeLists.txt riga 1: aggiungi `## SPDX-License-Identifier: MIT` (regola 10).
2. road.cpp righe 87-88: sostituisci il wrap playerX a +/-4000 con un wrap a +/-ROAD_HALF_WORLD (900), coerente con il clamp di game.cpp (+/-900) e road.h:25.
3. renderer.cpp righe 114-118: sostituisci la scansione lineare O(sprite x DRAW_DIST) su projs_/worldZ con std::lower_bound (O(log DRAW_DIST)); verificare che projs_ resti ordinato per worldZ crescente dopo il wrap di road::project.

Prossimo passo: su Pi 400 con toolchain Andrea — `cmake --build build --target prova_gate`, replay 2x stesso input (frameHash/60 identico), ms/frame <= 20 ms (obiettivo <= 12 ms), fps 50 stabili, temperatura, prova_gate 10s headless + 3 screenshot + log. H passa a [x] solo dopo queste misure reali.
