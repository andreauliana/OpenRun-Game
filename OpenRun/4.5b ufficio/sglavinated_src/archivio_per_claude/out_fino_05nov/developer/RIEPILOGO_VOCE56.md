# out/coder — esito voce 56 (CAPO -> CODER, BLOCCO C integrazione cartelloni)

Stato: verifica statica OK, nessun codice modificato (già implementato su disco).
Data: 16 ott 2026. Agente: @developer. Cartelle: motore_nuovo/src/, motore_nuovo/CMakeLists.txt.

## Cosa è già implementato (voce 52/52-bis, verificato su file)
- `Game::init()` chiama `leggiCartelloniDaFile(livelloId)` che legge `cartelloni_sponsor` da `motore_nuovo/dati/livelli/<id>/livello.json` (game.cpp:111-174).
- Per ogni voce del campo: segmento obbligatorio, lato e sponsor_id opzionali con fallback a `Rng(seed_)` se mancante/sconosciuto.
- Scelta a caso solo se il dato manca nel file (fallback deterministico in init() se campo assente/illeggibile).
- Allocazioni solo in init(): `cartelloni_` popolato in init, `scenaSprites_` riutilizzato in render() (zero alloc per frame a regime).
- `renderer.cpp`: `projs_` membro con `reserve(Road::DRAW_DIST)` nel costruttore, `clear()` in `drawRoad`.
- SPDX MIT riga 1 su: game.cpp, renderer.cpp, sponsor_loader.h, sponsor_loader.cpp, types.h, json_mini.h.

## Non eseguibile in questo ambiente (regola 21)
- `clang++ -std=c++17 -fsyntax-only` sul nucleo: shell negata, nessun compilatore C++/cmake.
- Build cmake, misura fps, replay deterministico: rimandati a @debugger (blocco H) e Pi 400 con toolchain.

## File toccati
Nessuno (implementazione già presente su disco da consegne precedenti). Verifica di sola lettura.

## Note
SCAMBIO.md scrittura negata a @developer (regola 15/21): esito lasciato in bacheche/coder/diario.md e qui. Spetta a @capo aggiornare RICHIESTA.md/SCAMBIO.md.