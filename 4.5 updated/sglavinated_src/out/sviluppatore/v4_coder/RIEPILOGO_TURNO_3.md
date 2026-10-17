# RIEPILOGO TURNO 3 — @coder (Turno 1, 16 ott 2026)

Blocco: BLOCCO C — stato in RICHIESTA.md: [~] (invariato).
Cartelle toccate: nessuna (motore_nuovo/src/, motore_nuovo/CMakeLists.txt invariati; nessun file sorgente modificato).

Esito:
- `clang++ -std=c++17 -fsyntax-only` sul nucleo (core/, road/, render/, input/, audio/, player/, game/, gameplay/; esclusi runner/main.cpp e libretro/ che dipendono da SDL): NON eseguibile. Shell comandi negati (`Permission denied: shell`, regola 21), nessun compilatore C++/cmake accessibile in questo ambiente. Non insisto (regola 21).
- Build cmake, replay deterministico (frameHash ogni 60 frame), ms/frame, fps 50, temperatura: rimandati a @debugger (blocco H) e a Pi 400 con toolchain (Andrea, regola 9).
- Verifica statica su file gi\`a effettuata (voci SCAMBIO 60/61): 30/30 file del nucleo con SPDX MIT riga 1; include `#include "..."` corretti; core privo di SDL e di orologio di sistema; `Game::init()` legge `cartelloni_sponsor` da `motore_nuovo/dati/livelli/<id>/livello.json` con fallback `Rng(seed_)` solo se il dato manca; zero alloc per frame (tutte le alloc in `init()`); `projs_` del renderer preallocato (reserve nel costruttore); membri `game.h` in ordine di dichiarazione (fb_, pal_ prima di ren_).
- SCAMBIO.md: edit negato al coder (regola 15/21, competenza @capo); esito lasciato in questo file e in bacheche/coder/diario.md.

Conseguenza: BLOCCO C e BLOCCO H restano [~] in RICHIESTA.md fino a build reale su Pi 400. Nessun blocco [ ]/[~] attuabile per @coder in questo turno.
Nessuno SPUNTO per BAR.md.
