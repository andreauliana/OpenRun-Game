# RIEPILOGO P1b — Fix 3 rilievi reviewer (@developer, 17/10/2026 07:40)

## Blocco attivo: VOCE 71 (CAPO -> CODER, [APERTA]) — P1b [~] + 3 rilievi reviewer

### Modifiche (solo motore_nuovo/src/, ogni file con SPDX MIT riga 1)

**1) motore_nuovo/src/player/player.cpp — offroad prima del clamp (rilievo 3)**
- Prima: `offroad = (x < -OFFROAD_LIMIT) || (x > OFFROAD_LIMIT);` era DOPO il clamp a `X_LIMIT=900`. Poiché `OFFROAD_LIMIT=1200 > X_LIMIT=900`, `offroad` era sempre falso (codice morto `pal_[12]` in renderer.cpp).
- Dopo: calcolo di `offroad` spostato PRIMA del clamp; il clamp resta a `X_LIMIT=900` (auto sempre visibile su 320 px).

**2) motore_nuovo/src/game/game.cpp:160-163 — guardia sponsorSpriteIdx_.empty() (rilievo 1)**
- Prima: `spriteIdx = sponsorSpriteIdx_[rndCampi.next((u32)sponsorSpriteIdx_.size())];` senza controllo; se `sponsorSpriteIdx_` vuoto -> modulo per zero / indice fuori range (UB, crash).
- Dopo: `if (sponsorSpriteIdx_.empty()) return false;` prima dell'accesso; `init()` usa il posizionamento cartelloni di fallback (Rng con seed_, deterministico).

**3) motore_nuovo/src/game/game.cpp:77-84 — rimosso `radioM.path = "shots/pcmr_M.bin"` (rilievo 2)**
- Prima: path hardcodato su `shots/` (cartella del filone Hack Mega Drive v87, possibile materiale Sega, regola 1; fuori dal flusso audio guidato da dati `motore_nuovo/dati/audio/playlist.json`, regola 8).
- Dopo: `RadioDef radioM` senza path; `mixer_.loadRadio(radioM)` restituisce 0 (silenzio, nessun crash). Il runner carica il brano da `playlist.json` e lo passa al mixer.

### P1b punto 1 (già su disco)
- `trafficoAttivo_ = false` in game.h; `spawnTraffic()` e collisioni `|dx|<700` eseguiti solo dentro `if (trafficoAttivo_)` (game.cpp:192-207). Solo strada + auto, nessun crash infinito sterzando.

### Build
- `cmake` / `make` NON eseguibili: shell e compilatore negati (regola 21), nessun `cmake`/`g++`/`clang++` in questo ambiente. Stesso vincolo dei turni precedenti.
- Build, replay deterministico (frameHash ogni 60 frame), fps 50 su Pi 400 rimandati a @debugger (blocco H) e al Pi 400 con toolchain di Andrea (regola 9/21).

### Licenza / quota open
- Tutti i file toccati: `// SPDX-License-Identifier: MIT` riga 1.
- `quota_open.py` / `check_catalogo.py` non rieseguibili qui (shell negata); ultimo valore noto 91.8% > soglia 50%.

### Stato RICHIESTA.md
- P1b resta [~] fino a build reale su Pi 400 (@debugger blocco H).
- Nessun file cancellato, nessun tocco di `../`, nessun asset Sega, nessun codice terzi senza licenza libera.
