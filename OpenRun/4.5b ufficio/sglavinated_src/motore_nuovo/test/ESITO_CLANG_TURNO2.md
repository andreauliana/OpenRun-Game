# ESITO Turno 2 — controllo `clang++ -fsyntax-only` del nucleo (motore nuovo, Blocco H / C)

Progetto: Outrun Sglavinated Edition v3 (open). Agente: @debugger. Data: 16 ott 2026.
Ambito (voce 54 SCAMBIO.md): provare `clang++ -std=c++17 -fsyntax-only` sui file di `motore_nuovo/src/` del nucleo (core/, road/, render/, input/, audio/, player/, game/), escludendo `runner/main.cpp` e `libretro/`. Sola lettura; nessun sorgente modificato. Questo file vive in `motore_nuovo/test/` (unica cartella scrivibile da @debugger).

---

## 1. Esito comando richiesto — NON ESEGUIBILE QUI (shell negata)

| Tentativo | Comando | Esito tool | Note |
|---|---|---|---|
| 1 | `find motore_nuovo/src -type f` | Permission denied: shell | enumerazione negata (già in voce 54) |
| 2 | `clang++ -std=c++17 -fsyntax-only motore_nuovo/src/core/types.h` | Permission denied: shell | comando della voce 54 non parte |

Il canale `shell` e' respinto dal sistema (regola 21: shell ristretta). Non si insiste oltre (2 tentativi, regola 13/21).

Conseguenza: il numero di file "che compilano" NON e' misurabile in questo ambiente. La misura (PASS/FAIL di `clang++ -fsyntax-only`) manca e resta da eseguire dove il compilatore e' raggiungibile (Mac di Andrea con toolchain completa, oppure Pi 400).

Comando da eseguire là (nucleo, runner/libretro esclusi), un file per volta:
```
clang++ -std=c++17 -fsyntax-only -Imotore_nuovo/src motore_nuovo/src/core/types.h
```
(stesso per ogni .h/.cpp del nucleo sotto `motore_nuovo/src/`)

## 2. Verifiche statiche eseguite (read/grep, senza shell)

Non sostituiscono la compilazione, ma restringono il campo degli errori "da goffaggine".

| Controllo | Esito | Numero / evidenza |
|---|---|---|
| File sorgente src/ (.h/.cpp) | 35 file totali | glob `motore_nuovo/src/**/*.{h,cpp}` |
| Intestazione SPDX | OK 35/35 | `// SPDX-License-Identifier: MIT` in riga 1 di ogni file |
| Include malformati `#include "..." >` | 0 occorrenze | grep `#include "[^"]*>` = 0 |
| Percorsi include coerenti | OK | ogni `"dir/file.h"` usato esiste |
| Nucleo senza SDL | OK | 0 `SDL`/`chrono`/`gettimeofday` in `core/` |
| SDL confinato a runner/libretro | OK | `#include <SDL2/...>` solo in `runner/main.cpp`, `runner/sponsor_loader.{h,cpp}`, `libretro/` |
| frameHash ogni 60 frame | OK | `runner/main.cpp:173` `if (frames % 60 == 0)` |
| dt fisso 50 Hz | OK | `game/game.cpp:178` `f32 dt = 1.0f / 50.0f;` |
| Lettura cartelloni_sponsor da livello.json | OK | `game/game.cpp:128` `jsonGet(...,"cartelloni_sponsor",...)` (voce 56 integrata, nessun alloc nel ciclo) |
| Riferimenti Sega in src/ | 0 | grep `sega|outrun|genesis|megadrive` solo commenti negativi |

## 3. Perimetro effettivo per `-fsyntax-only` (30 file)

Esclusi i 4 file che usano SDL (`runner/main.cpp`, `runner/sponsor_loader.{h,cpp}`, `libretro/sglavinated_libretro.cpp`), restano 30 file:

- `core/` (7): types.h, frame_counter.h, rng.h, state.h, replay.h, replay.cpp, json_mini.h
- `road/` (3): road_segment.h, road.h, road.cpp
- `render/` (7): framebuffer.h/cpp, palette.h/cpp, sprite.h, renderer.h/cpp
- `input/` (1): pad.h
- `audio/` (2): mixer.h/cpp
- `player/` (2): player.h/cpp
- `game/` (2): game.h/cpp
- `gameplay/` (7): frasi.h/cpp, show.h/cpp, ufo.h, escalation.h, personaggi.h

`gameplay/` e' inclusa da `game/game.h:14`; fa parte del nucleo (nessun SDL).

## 4. Misure mancanti (da NON dichiarare "fatto")

- Compilazione nucleo `clang++ -fsyntax-only`: manca (shell negata). -> Mac di Andrea / Pi 400.
- Build cmake, replay deterministico (hash identico 2 esecuzioni, stesso input), ms/frame (<= 20 ms, obiettivo <= 12 ms), fps 50, temperatura: mancano, restano su Pi 400 con toolchain (Andrea), come da voce 54.

## 5. Verdetto

La verifica richiesta non e' eseguibile qui: shell respinta. I controlli statici disponibili (SPDX 35/35, 0 include malformati, percorsi coerenti, nucleo privo di SDL, frameHash %60, dt 1/50, cartelloni_sponsor integrato) sono OK, ma non provano la compilazione.

Blocco C [~] e Blocco H [~] restano tali finché `clang++ -fsyntax-only` non passa sul nucleo (CHECKPOINT Claude CLI 10:14). Build/replay/fps/temperatura reali sono rimandati a Pi 400 con toolchain (Andrea): non è un rifiuto del codice.

Fatto da @debugger, sola ispezione + read/grep. Nessun file di @developer/@developer-junior modificato.
