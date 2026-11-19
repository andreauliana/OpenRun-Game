# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 19/11/2026 11:25 (@developer): V47 passo ∞ V9 (fluidita' a giro), ventiduesimo giro, lv_05_spray. M `motore_nuovo/src/render/renderer.cpp` + `renderer.h`: in `drawSprite` la condizione `tx1 - tx0 > 1` e' invariante per colonna schermo, precalcolata in `colCerca_` invece che per pixel; stessi indici, frameHash identici. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0 (pre/post). `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno38" lv_05_spray percorso` exit 0 — `GATE PERCORSO OK` (2920 frame, 48.7 s, 0% fuori); smoke exit 0 — `GATE OK`, medio 0.941 max 3.650 ms, replay IDENTICO, frameHash 0 4250821778. Post: percorso exit 0 `GATE PERCORSO OK`; smoke exit 0 medio 0.943 max 3.346 ms, replay IDENTICO, frameHash identici. `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito il 19/11/2026 11:25 exit 0 — `GATE T1: TUTTO OK`, 6/6 `GATE PERCORSO OK`. Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 19/11/2026 11:55 (@developer-junior): V47 VJ5 turno 10, review dell'ultimo diff src del @developer (invariato dal turno 9: M `motore_nuovo/src/render/renderer.cpp` + M `renderer.h`, drawSprite `colCerca_` per colonna `renderer.cpp:1804`/`:1838`, `renderer.h:503-507`). Nessun nuovo diff src (`git status --porcelain`/`git diff` src exit 0). Nessun errore: precalc dagli stessi `tx0/tx1` (`:1791-1800`) letti nel pixel loop (`:1829-1834`), OR con `cercaRiga` `:1826` invariato, replay identico. Semplificazione focalizzata: `renderer.cpp:1846` `(*pCerca || cercaRiga)` -> `(cercaRiga || *pCerca)` per cortocircuitare load/test quando `ty1-ty0>1`; aperte base `rigaCerca` per riga `:1851`, init morta `rigaTex=px` `:1782`, `screenYRow`/`inTubo` `:1850`. `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 274.81 s); `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv06_1911t10" lv_06_desktop smoke` exit 0 (GATE OK, medio 0.964 max 3.675 ms frame 0, replay 2x IDENTICO). `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
