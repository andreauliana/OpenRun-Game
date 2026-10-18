# Voce pronta per SCAMBIO.md (bloccata: SCAMBIO.md write-protetto per @debugger, regola 21)

@capo: incollare la voce seguente in fondo a SCAMBIO.md (nessuna modifica necessaria).

---
### 29. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026
Esito BLOCCO H (verifica statica BLOCCO C, voce 22). Report completo: motore_nuovo/test/ESITO_BLOCCO_H.md. AMBIENTE: nessun cmake (`cmake` -> command not found, exit 127), nessun compilatore C++ -> build NON eseguibile qui ("rimandata a Pi 400 con toolchain (Andrea)").
1) Build/fps/ms-frame: N/D (no toolchain). motore_nuovo/build/ vuota (0 file).
2) Determinismo (statico): OK. Tempo = FrameClock contatore (frame_counter.h:10-14; game.cpp:65-71); RNG xorshift32 con seme (rng.h:9-23, road.cpp:17; runner Game(20261015u)); dt=1/50 (game.cpp:45); nessun orologio di sistema nel core (0 hit time/clock/chrono/gettimeofday/rand in src/; SDL_GetTicks solo runner/main.cpp:100,142,146). frameHash ogni 60 frame: OK (main.cpp:149, sanato).
3) Replay "stesso file 2x -> stesso hash /60": NON eseguibile qui (manca harness di playback nel runner). Misura mancante, da fare su Pi 400.
4) Licenze: SPDX MIT 25/25 file src (0/25 prima). ATTENZIONE: quota_open.py legge la licenza solo a livello cartella (manifest.json "licenza" o LICENSE con prima riga SPDX) e NON i commenti inline -> classifica tutti i 25 file src come "senza licenza aperta (nessuna)". Fix proposto: creare motore_nuovo/LICENSE con prima riga `SPDX-License-Identifier: MIT` (@architetto/@coder).
5) Niente Sega: src/ 3 match "nessun... Sega" (commenti negativi) OK; dati/ 10 match note/negazioni OK; citazioni Lou's Pseudo 3d + Jake Gordon solo come idee. Nessun asset/tracciato/musica/codice Sega.
6) Correzioni reviewer voce 20: tutte SANATE su disco (include 61 tutti con `"`/`<`; game.h fb_/pal_ prima di ren_; CMake `find_package(SDL2 REQUIRED)` riga 43 + option BUILD_LIBRETRO riga 59; audioCallback n=min(avail,len) main.cpp:56-58; frameHash %60).
7) Strumenti: check_catalogo.py exit 0 "catalogo: 11 righe, 2 cartelle". quota_open.py exit 0 "72.8% (107738 di 148033 byte, soglia 50.0%)" -> sopra soglia.
8) Proposte minori: (A) LICENSE in motore_nuovo/ per il conteggio quota; (B) CMakeLists `find_package(SDL2 QUIET)` invece di REQUIRED (oggi il ramo else e' morto: senza SDL2 il configure fallisce e non costruisce nemmeno il core); (C) harness replay nel runner; (D) warning: road.cpp:37-38 midCurve/midHill inutilizzati, game.cpp:3 `<cstring>` inutilizzato; renderer.cpp:13 vector projs allocato a ogni render (tocca il target "zero alloc").
9) Correzione nota @coder 28(c): NON e' vero che "meta' buffer resta silenzio". want.samples=882 frame -> callback len = 882*2ch*2B = 3528 B; g_audioFrame = audioBuf_ = 882 frame *2 = 1764 i16 = 3528 B; n=min(3528,3528)=3528 -> copia piena. Allineato, nessun silenzio.
BLOCCO H resta [~] finche' @capo non conferma; build/fps reali sul Pi 400 spettano ad Andrea. @debugger non ha modificato file di @coder (solo motore_nuovo/test/ + questa voce). SCAMBIO.md write-protetto per @debugger: la voce va incollata da @capo.
---
