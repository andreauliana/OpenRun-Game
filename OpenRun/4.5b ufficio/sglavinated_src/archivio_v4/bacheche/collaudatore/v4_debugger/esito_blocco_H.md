# Bacheca @debugger — esito Blocco H + CHECK 2 (16 ott 2026)

NOTA: SCAMBIO.md e PER_CLAUDE.md in root sono write-protette in questo ambiente (edit x2, write x2, permission.rejected). La voce 20 prevista per SCAMBIO.md è riportata qui integralmente; va pubblicata su SCAMBIO.md non appena il file sia sbloccato (regola 21: non insistere sul canale negato).

---

### 20. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026 (da pubblicare su SCAMBIO.md)
Blocco H + CHECK 2 (esito ispezione, nessun file di @developer modificato):
- Build cmake NON eseguibile qui (regola 21): nessun cmake, nessun g++/c++/cc, shell negata. Gate "compila con cmake" rinviato a ambiente con toolchain (cmake + SDL2 + nlohmann-json). Comando atteso: cd motore_nuovo && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j (dettagli motore_nuovo/docs/BUILD.md §4).
- Difetti di build trovati per ispezione (da passare a @developer/@developer-junior): rep('#include "game/game.h>', '#include "game/game.h"') in src/game/game.cpp:1 e src/runner/main.cpp:1; stessi include chiusi con '>' in src/game/game.h:2-12; CMakeLists.txt manca find_package(SDL2) prima di if(SDL2_FOUND): target runner mai costruito, nessun errore ma nessun eseguibile.
- Replay deterministico: CONFERMATO. frame_counter.h (FrameClock u64, tick, unico orologio), rng.h (xorshift32 seme esplicito), replay.h/cpp (record/playback input), game.cpp update() determinista (dt=1/50, clock_.tick() finale, replay_.record), runner: SDL_GetTicks solo pacing (core libero da SDL, nessun std::chrono). runner logga frameHash ogni 300 frame; spec prevede ogni 60: da allineare per il test "stesso input due volte -> stesso hash ogni 60".
- Rischio prestazioni: spawnTraffic() fa clear()+6 push_back per frame e replay_.record puo' riallocare oltre reserve(4096): viola "zero allocazioni ciclo di gioco" (ARCHITETTURA.md). Da verificare in misura.
- FPS: non misurabili qui (nessun eseguibile). Piano Pi 400 pronto: build Release, replay deterministico 2x stesso input (stesso hash ogni 60 frame), ms/frame (media/p99/max su 600+ frame), vcgencmd measure_temp + get_throttled prima/durante/dopo; target <=20 ms, obiettivo <=12 ms, 50 fps stabili, zero picchi ripetuti, zero allocazioni ciclo. Numeri da riportare in SCAMBIO alla prima build su Pi.
- Output blocco H: motore_nuovo/test/ARCHITETTURA_CHECK.md prodotto (check vincoli duri + difetti + piano fps).

---

File prodotti da @debugger in questo ciclo:
- motore_nuovo/test/ARCHITETTURA_CHECK.md (output Blocco H, check vincoli duri vs codice consegnato)
- bacheche/debugger/esito_blocco_H.md (questa voce, in attesa di pubblicazione su SCAMBIO.md)

Stato gate: "compila con cmake" RINVIATO a ambiente con toolchain (Pi 400 con build-essential cmake libsdl2-dev, o Mac con brew). Nessun materiale di @developer modificato (sola lettura).

---

## Aggiornamento 16 ott 2026 — verifica BLOCCO C dopo correzioni @developer (voce 22 -> voce 29)

Voce per SCAMBIO.md salvata anche in `motore_nuovo/test/VOCE_SCAMBIO_29.md` (SCAMBIO.md resta write-protetto per @debugger). Report completo: `motore_nuovo/test/ESITO_BLOCCO_H.md`.

### 29. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026
Esito BLOCCO H (verifica statica BLOCCO C, voce 22). AMBIENTE: nessun cmake (`cmake` -> command not found, exit 127), nessun compilatore C++ -> build NON eseguibile qui ("rimandata a Pi 400 con toolchain (Andrea)").
1) Build/fps/ms-frame: N/D (no toolchain). motore_nuovo/build/ vuota (0 file).
2) Determinismo (statico): OK. FrameClock contatore (frame_counter.h:10-14; game.cpp:65-71); RNG xorshift32 con seme (rng.h:9-23, road.cpp:17; runner Game(20261015u)); dt=1/50; nessun orologio di sistema nel core (SDL_GetTicks solo runner/main.cpp:100,142,146). frameHash ogni 60: OK (main.cpp:149).
3) Replay "stesso file 2x -> stesso hash /60": NON eseguibile qui (manca harness di playback nel runner). Misura mancante.
4) Licenze: SPDX MIT 25/25 file src (0/25 prima). quota_open.py legge la licenza solo a livello cartella (manifest/LICENSE) e NON i commenti inline -> classifica tutti i 25 file src come "senza licenza aperta (nessuna)". Fix proposto: motore_nuovo/LICENSE con prima riga `SPDX-License-Identifier: MIT`.
5) Niente Sega: src/ 3 match "nessun... Sega" OK; dati/ 10 match note/negazioni OK; citazioni Lou's Pseudo 3d + Jake Gordon solo come idee.
6) Correzioni reviewer voce 20: tutte SANATE su disco.
7) Strumenti: check_catalogo.py exit 0 "catalogo: 11 righe, 2 cartelle"; quota_open.py exit 0 "72.8% (107738 di 148033 byte, soglia 50.0%)" -> sopra soglia.
8) Proposte: (A) LICENSE in motore_nuovo/; (B) CMake `find_package(SDL2 QUIET)` (oggi REQUIRED -> ramo else morto); (C) harness replay; (D) warning road.cpp:37-38 midCurve/midHill, game.cpp:3 `<cstring>`, renderer.cpp:13 vector projs per-render.
9) Correzione nota @developer 28(c): NON e' vero che "meta' buffer resta silenzio": callback len = 882*2*2 = 3528 B = g_audioFrame (1764 i16). Copia piena.
9) BLOCCO H resta [~] finche' @capo non conferma; build/fps reali spettano ad Andrea sul Pi 400.

File prodotti in questo ciclo:
- motore_nuovo/test/ESITO_BLOCCO_H.md (report verifica)
- motore_nuovo/test/VOCE_SCAMBIO_29.md (voce pronta per incollo da @capo)
- motore_nuovo/test/ARCHITETTURA_CHECK.md (sistemato riferimento SGLAVINATED_LIBRETRO -> BUILD_LIBRETRO, richiesta @developer 28b)
- bacheche/debugger/esito_blocco_H.md (questa voce)
