# ESITO voce 54 — controllo `clang++ -fsyntax-only` del nucleo (motore nuovo)

Progetto: Outrun Sglavinated Edition v3 (open). Agente: @debugger. Data: 16 ott 2026.
Ambito (voce 54): provare `clang++ -std=c++17 -fsyntax-only` sui file di `motore_nuovo/src/`
del nucleo (core/, road/, render/, input/, audio/, player/, game/), escludendo `runner/main.cpp`
e `libretro/`. Sola lettura; nessun sorgente modificato. Questo file vive in `motore_nuovo/test/`
(unica cartella scrivibile da @debugger).

---

## 1. Esito comando richiesto — NON ESEGUIBILE QUI (shell negata)

| Tentativo | Comando | Esito tool | Note |
|---|---|---|---|
| 1 | `find motore_nuovo/src -type f` | **Permission denied: shell** | perfino la sola enumerazione e' negata |
| 2 | `clang++ -std=c++17 -fsyntax-only motore_nuovo/src/core/types.h` | **Permission denied: shell** | il comando della voce 54 non parte |

Il canale `shell` e' respinto dal sistema (regola 21: shell ristretta; le voci precedenti hanno
gia' riportato "shell negata"). Non si insiste oltre (2 tentativi, regola 13/21).

Conseguenza diretta: **il numero di file "che compilano" NON e' misurabile in questo ambiente.**
La misura richiesta (PASS/FAIL di `clang++ -fsyntax-only`) manca e resta da eseguire dove il
compilatore e' raggiungibile (Mac di Andrea con toolchain completa, oppure Pi 400).
Comando da eseguire la' (nucleo, runner/libretro esclusi):

```
cd motore_nuovo
for f in $(find src -name '*.cpp' ! -path 'src/runner/*' ! -path 'src/libretro/*'); do
  clang++ -std=c++17 -fsyntax-only -Isrc "$f" || echo "FAIL $f"
done
```

## 2. Verifiche statiche eseguite (read/grep, senza shell)

Non sostituiscono la compilazione, ma restringono il campo degli errori "da goffaggine".

| Controllo | Esito | Numero / evidenza |
|---|---|---|
| File sorgente src/ (.h/.cpp) | 34 file totali | glob `motore_nuovo/src/**/*.{h,cpp}` (34), esclusi dati/manifest |
| Intestazione SPDX | OK 34/34 | `// SPDX-License-Identifier: MIT` in riga 1 di ogni file |
| Include malformati `#include "..." >` | 0 occorrenze | grep `#include "[^"]*>` = 0 (fix reviewer voce 20 confermato su disco) |
| Percorsi include coerenti | OK | ogni `"dir/file.h"` usato esiste (verificato incrociando la lista file); game.h include gameplay/show.h |
| Nucleo senza SDL | OK | 0 `SDL`/`chrono`/`gettimeofday` in core/; in game/ solo 2 hit, entrambi in commento (game.cpp:116, game.h:20) |
| SDL confinato a runner/libretro | OK | `#include <SDL2/...>` solo in runner/main.cpp (4-5), runner/sponsor_loader.cpp (15-16); libretro solo game/game.h |

## 3. Perimetro effettivo (30 file da passare a `-fsyntax-only`)

Esclusi i 4 file che usano SDL (`runner/main.cpp`, `runner/sponsor_loader.{h,cpp}`, `libretro/sglavinated_libretro.cpp`),
restano **30 file** (23 nelle cartelle citate dalla voce 54 + 7 in `gameplay/`, che `game/game.h:14` include:

- `core/` (6): types.h, frame_counter.h, rng.h, state.h, replay.h, replay.cpp
- `road/` (3): road_segment.h, road.h, road.cpp
- `render/` (7): framebuffer.h, framebuffer.cpp, palette.h, palette.cpp, sprite.h, renderer.h, renderer.cpp
- `input/` (1): pad.h
- `audio/` (2): mixer.h, mixer.cpp
- `player/` (2): player.h, player.cpp
- `game/` (2): game.h, game.cpp
- `gameplay/` (7): frasi.h, frasi.cpp, show.h, show.cpp, ufo.h, escalation.h, personaggi.h

Nota: `gameplay/` non era elencata nella voce 54 ma e' parte del nucleo (nessun SDL) ed e' tirata
dentro da game.h; va inclusa nel controllo per non lasciare un buco.

## 4. Misura mancante (da NON dichiarare "fatto")

- Compilazione nucleo `clang++ -fsyntax-only`: **manca** (shell negata). -> Mac di Andrea / Pi 400.
- Build cmake, replay deterministico (hash identico 2x stesso input), ms/frame (<=20, target <=12),
  fps 50, temperatura: **mancano**, restano su Pi 400 con toolchain (Andrea), come da voce 54.

## 5. Verdetto

La verifica richiesta non e' eseguibile qui: **shell respinta**. I controlli statici disponibili
(SPDX 34/34, 0 include malformati, percorsi coerenti, nucleo privo di SDL) sono **OK**, ma non
provano la compilazione. Bloccanti per chiudere il gate C: eseguire il comando del §1 dove c'e' clang++.
