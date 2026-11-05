# Esito Blocco H — @debugger — 17 ott 2026 13:00

## Stato
Blocco H [~] — build/replay/fps/temperatura NON eseguibili qui (regola 21: shell negata, nessun compilatore/cmake). Da eseguire su Pi 400 con toolchain Andrea (SCAMBIO voce 1138, risolta da 1139).

## File verificato
- `motore_nuovo/test/prova_gate.cpp` (139 righe, SPDX MIT riga 1)
- Target CMake: righe 111-114 di `motore_nuovo/CMakeLists.txt` (OK, si attiva se prova_gate.cpp esiste)

## Problemi residui (diff proposti in SCAMBIO 1139, non applicati)
1. Exit code sempre 0 (`return 0;` riga 138): nessun controllo `playerSpeed > 0` al frame 60.
2. `frameHash()` ogni 60 frame non loggato (regola 5b determinismo): `sgl::Game::frameHash()` esiste (game.h:72) ma non viene mai chiamato.

## Sintassi nucleo
34/34 file OK, `clang++ -std=c++17 -fsyntax-only -Isrc`, exit 0, 0 errori (voci 76/78/80).

## Gate da eseguire su Pi 400 (Andrea)
1. `cmake --build build --target prova_gate` (D9 DEFAULT: solo cmake --build)
2. Replay 2x stesso input: frameHash/60 identico
3. ms/frame <= 20ms (obiettivo <= 12ms), fps 50 stabili, temperatura
4. prova_gate 10s headless + 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione

## Report precedente
out/debugger/ESITO_VERIFICA_PROVA_GATE_17OTT_1258.md (12:58) — 3 bug statici trovati, 2 gia' sanati su disco (include path, sc_frames[2]).
