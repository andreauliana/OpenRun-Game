### 36. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026
Richiesta/Esito: BLOCCO H verifica statica BLOCCO C (motore_nuovo/src/ + CMakeLists.txt), sola lettura, nessun file di @coder modificato.
- `python3 motore_nuovo/strumenti_py/check_catalogo.py` -> exit 0, "catalogo: 11 righe, 2 cartelle".
- `python3 motore_nuovo/strumenti_py/quota_open.py` -> exit 0, "quota open: 91.8% (247656 di 269832 byte), soglia 50.0%" -> sopra soglia (unica eccezione assets_originali/genera_bloccoF.py di @asset, non mia consegna).
- Build cmake / runner: N/D. Nessun cmake, nessun g++/c++/cc in questo ambiente (regola 21: non insistere). motore_nuovo/build/ vuota. Build e misura fps/temperatura rimandate a Pi 400 con toolchain (Andrea): `cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`.
- Verifica statica: OK su tutti i requisiti voce 22:
  * SPDX MIT 25/25 file src/ + motore_nuovo/src/manifest.json ("licenza": "MIT").
  * 0 include chiusi con `>` (grep `#include "[^"]*">` = 0).
  * 0 riferimenti SDL in core/ (core indipendente dalla piattaforma; SDL solo runner).
  * frameHash ogni 60 frame (runner/main.cpp:149 `frames % 60 == 0`).
  * dt = 1.0f/50.0f (game.cpp:54); FrameClock contatore (frame_counter.h:10-13); RNG xorshift32 seme esplicito (rng.h:11-12; runner Game(20261015u)).
  * 0 riferimenti sega|outrun|genesis|megadrive|z80|68000 in src/.
- Replay deterministico "stesso file di input due volte -> stesso hash ogni 60 frame": NON eseguibile qui (manca harness playback nel runner + toolchain). Misura ms/frame/fps/temperatura su Pi 400 (piano motore_nuovo/test/ARCHITETTURA_CHECK.md §5: target <=20 ms, obiettivo <=12 ms, 50 fps stabili, zero alloc ciclo).
Risultati condivisi: motore_nuovo/test/ESITO_BLOCCO_H.md, bacheche/debugger/diario.md, out/debugger/RIEPILOGO_CONSEGNA_H_TURNO1.md. Blocco H resta [~] (build/fps reali spettano a Pi 400). SCAMBIO.md write-protetto per @debugger: voce 22 non chiudibile qui, questa voce 36 va incollata da @capo in fondo a SCAMBIO.md.
