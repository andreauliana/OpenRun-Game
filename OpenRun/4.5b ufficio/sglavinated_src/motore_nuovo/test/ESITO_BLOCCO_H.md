# ESITO_BLOCCO_H — verifica statica BLOCCO C (motore nuovo)

Progetto: Outrun Sglavinated Edition v3 (open). Agente: @debugger. Data: 16 ott 2026.
Ambito: verifica del BLOCCO C (motore_nuovo/src/ + CMakeLists.txt) dopo le correzioni di @developer/@developer-junior (voce 21). Sola lettura del codice; nessun file di @developer/@developer-junior modificato. Questo file vive in motore_nuovo/test/ (cartella scrivibile da @developer/@developer-junior).

Ambiente: Pi/Mac del turno, shell ristretta (regola 21). **Nessun cmake** (`cmake` -> command not found, exit 127), nessun compilatore C++ raggiungibile. Build reale NON eseguibile qui.

---

## 1. Build (gate cmake) — NON ESEGUIBILE QUI

| Comando | Esito | Numero |
|---|---|---|
| `cd motore_nuovo && cmake -S . -B build` | command not found | exit 127 |
| `cmake --build build` | impossibile (cmake assente) | n/d |
| g++/c++/cc | non raggiungibili (shell negata) | n/d |
| motore_nuovo/build/ | cartella vuota (nessun artefatto) | 0 file |

Verdetto: **"build non eseguibile qui, rimandata a Pi 400 con toolchain (Andrea)"**. Non è un rifiuto del codice. Da riverificare dipendenze @capo riporta ad Andrea.

## 2. Replay deterministico (ispezione statica)

| Requisito | File:riga | Esito | Evidenza |
|---|---|---|---|
| Tempo = contatore di frame | core/frame_counter.h:10-14 | OK | `FrameClock { u64 frame=0; void tick(){++frame;} }` |
| Frame aggiornato una volta per ciclo | game/game.cpp:65-71 | OK | `state_.frame = clock_.frame;` poi `clock_.tick()` a fine update |
| dt fisso 50 Hz | game/game.cpp:45 | OK | `f32 dt = 1.0f/50.0f;` |
| RNG con seme (xorshift32) | core/rng.h:9-23 | OK | `Rng(u32 seed)`, `seed()= (s==0)?0x9E3779B9u:s`; usato in road.cpp:17 |
| Seme fisso nel runner | runner/main.cpp:93 | OK | `sgl::Game game(20261015u);` |
| Nessun orologio di sistema nel core | grep su src/ | OK | 0 hit `time/clock/chrono/gettimeofday/rand/srand` nel core; `SDL_GetTicks` solo in runner/main.cpp:100,142,146 (pacing, fuori core) |
| Registrazione/Riproduzione input | core/replay.h, core/replay.cpp | OK (codice) | `Mode{Off,Record,Playback}`, `record()`, `next()`, `startRecord()` con `reserve(4096)` |
| Hash ogni 60 frame | runner/main.cpp:149 | OK | `if (frames % 60 == 0)` logga `frameHash()` |

**Misura MANCANTE (richiede toolchain + harness):** il runner NON carica un file di input né confronta gli hash tra due esecuzioni. Il test "stesso file di input due volte -> stesso hash ogni 60 frame" non è eseguibile qui: manca l'harness di playback. Da fare su Pi 400 (vedi §5).

## 3. Licenze SPDX

`grep "// SPDX-License-Identifier" motore_nuovo/src/**/*.{h,cpp}` -> **25/25 file, tutti MIT** (prima della correzione: 0/25). Nessun file di src/ senza SPDX inline.

ATTENZIONE — discrepanza con lo strumento: `quota_open.py` legge la licenza SOLO a livello di cartella (primo `manifest.json` con campo `"licenza"`, oppure `LICENSE` con prima riga `SPDX-License-Identifier:`). Non legge i commenti `// SPDX` inline. Non esistono `motore_nuovo/LICENSE` né `motore_nuovo/manifest.json` -> il tool classifica **tutti e 25 i file src/ come "senza licenza aperta (nessuna)"** (stampa limitata a 20 righe). La licenza c'è (inline), ma non è conteggiata. Correzione proposta in §6-A.

## 4. Niente Sega

| Base | Match | Esito |
|---|---|---|
| src/ `-i sega|outrun|genesis|megadrive` | 3 match, tutti commenti negativi ("nessun audio esterno, nessun file Sega"; "nessun asset Sega"; "Nessun tracciato Sega") | OK |
| dati/ `-i sega|outrun|genesis|megadrive|z80|68000` | 10 match, tutti note/negazioni ("nessun materiale Sega", "budget Mega Drive NON si portano", "nessun elemento Sega") | OK |
| Citazioni tecniche | types.h:6,9; road_segment.h:9,10 | OK | Lou's Pseudo 3d Page e Jake Gordon (JavaScript Racer) citati solo come idee, nessun codice/asset riusato |

Nessun asset, tracciato, musica o codice Sega presente.

## 5. Correzioni @reviewer voce 20 — verifica su disco

| # | Richiesta | Esito | Evidenza |
|---|---|---|---|
| 1 | include chiusi con `>` | SANATO | 61 `#include`: tutti `"..."` o `<...>` corretti; 0 chiusi con `>` |
| 2 | game.h membri fb_,pal_ prima di ren_ | SANATO | game.h:56 fb_, 57 pal_, 58 ren_ (ordine di init corretto, nessun UB) |
| 3 | CMake find_package(SDL2) | PRESENTE | CMakeLists.txt:43 `find_package(SDL2 REQUIRED)` prima di `if(SDL2_FOUND)` (44) |
| 4 | opzione SGLAVINATED_LIBRETRO -> BUILD_LIBRETRO | SANATO | CMakeLists.txt:59 `option(BUILD_LIBRETRO ... OFF)` |
| 5 | audioCallback bound copia frame | SANATO | runner/main.cpp:56-58 `n = min(avail,len)`, memcpy di n, silenzio se non pronto |
| 6 | frameHash ogni 60 (non 300) | SANATO | runner/main.cpp:149 |

## 6. Strumenti Python (eseguiti)

| Strumento | Esito | Numeri |
|---|---|---|
| `python3 motore_nuovo/strumenti_py/check_catalogo.py` | exit 0 | `catalogo: 11 righe, 2 cartelle` |
| `python3 motore_nuovo/strumenti_py/quota_open.py` | exit 0 | `quota open: 72.8% (107738 di 148033 byte), soglia 50.0%` -> sopra soglia |

---

## Proposte di correzione (NON applicate — @developer/@developer-junior/@grafico)

### A. Le licenze inline SPDX non sono conteggiate da quota_open.py
Aggiungere un `LICENSE` a livello `motore_nuovo/` (copre tutto `src/`, che non ha manifest proprio). Prima riga esatta `SPDX-License-Identifier: MIT`.
```diff
--- /dev/null
+++ motore_nuovo/LICENSE
@@ -0,0 +1,1 @@
+SPDX-License-Identifier: MIT
```
(Alternativa: `motore_nuovo/manifest.json` con `{"licenza":"MIT"}`.)

### B. find_package(SDL2 REQUIRED) rende morto il ramo else e fa fallire l'intero configure senza SDL2
Su un host senza SDL2 `cmake -S . -B build` si interrompe e non costruisce nemmeno il core. Preferibile QUIET per mantenere il fallback documentato.
```diff
--- a/motore_nuovo/CMakeLists.txt
+++ b/motore_nuovo/CMakeLists.txt
@@ -40,7 +40,7 @@
 # Richiede SDL2 (video + audio). Non e' parte del core.
 # ---------------------------------------------------------------------------
-find_package(SDL2 REQUIRED)
+find_package(SDL2 QUIET)
 if(SDL2_FOUND)
```

### C. Harness replay deterministico assente nel runner
Il percorso Playback esiste nel core ma nulla lo attiva da file. Serve un ramo nel runner (o un target di test headless) che legga un file di input e stampi `frameHash` ogni 60 frame, senza orologi nel core. Da progettare con @architetto; misura rimandata al Pi 400.

### D. Note minori (warning, non bloccanti)
- road.cpp:37-38 `midCurve`/`midHill` calcolati e non usati -> warning `-Wunused-variable`.
- game.cpp:3 `#include <cstring>` non usato.
- libretro/sglavinated_libretro.cpp (BUILD_LIBRETRO=ON) usa `RETRO_API_VERSION`/`struct retro_system_info` non definiti: non compila finché i header libretro non sono nel progetto. Default OFF, coerente con BUILD.md.
- renderer.cpp:13 `std::vector<ProjectedSeg> projs;` allocato a ogni `render()` (fuori dal ciclo update, ma incide sul tempo frame): da valutare per il target "zero alloc".

---

Fatto da @developer/@developer-junior, sola ispezione + strumenti Python. Nessun file di @developer/@developer-junior modificato.
