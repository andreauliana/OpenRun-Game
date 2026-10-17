# Riepilogo @debugger — verifica sintassi nucleo, Turno 2 (16 ott 2026)

Progetto: Outrun Sglavinated Edition v3 (open), motore nuovo (pista C).

## Eseguito
- Verifica statica (read/grep, nessun file modificato): 35/35 file .h/.cpp di `motore_nuovo/src/` con `// SPDX-License-Identifier: MIT` in riga 1; 0 include malformati (`#include "..." >` = 0); `core/` privo di `SDL`/`chrono`/`gettimeofday`; `frameHash` ogni 60 frame (`runner/main.cpp:173`); `dt = 1/50` (`game/game.cpp:178`); `game.cpp` legge `cartelloni_sponsor` da `livello.json` (voce 56 integrata, zero alloc nel ciclo); 0 riferimenti a Sega in `src/`.
- Perimetro per `clang++ -fsyntax-only`: 31 file (core 7 incl. json_mini.h, road 3, render 7, input 1, audio 2, player 2, game 2, gameplay 7); esclusi 4 file SDL (`runner/main.cpp`, `runner/sponsor_loader.{h,cpp}`, `libretro/sglavinated_libretro.cpp`).

## Non eseguibile in questo ambiente (regola 21: shell respinta)
- `clang++ -std=c++17 -fsyntax-only` sul nucleo: misura PASS/FAIL mancante (3 tentativi negati, stop regola 21).
- Build cmake, runner, replay deterministico (hash identico su 2 esecuzioni stesso input, ogni 60 frame), ms/frame (<= 20 ms, obiettivo <= 12 ms), fps 50, temperatura.

## Stato blocchi
- Blocco C [~] e Blocco H [~]: restano tali finché `clang++ -fsyntax-only` non passa sul nucleo (CHECKPOINT Claude CLI 10:14). Build/misure reali rimandate a Pi 400 con toolchain (Andrea); non è un rifiuto del codice.

## File
- `motore_nuovo/test/ESITO_VERIFICA_SINTASSI.md` — esito completo (comando, esito, elenco file).
- `motore_nuovo/test/VOCE_SCAMBIO_64.md` — voce SCAMBIO da incollare da @capo (risponde a voce 59).
- `bacheche/debugger/diario.md` — aggiornato.
- `BAR.md` — riga di pausa aggiunta (regola 17).
