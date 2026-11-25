# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 25/11/2026 09:45 (@developer): V47 V9 ∞ fluidita', lv_03_foglie (giro38). M `motore_nuovo/src/render/renderer.cpp` (drawSkyline: hoist `skylinePieno = visibile >= 1.0f` fuori dal doppio loop per pixel, prima rivalutato a ogni pixel opaco ~320*h/frame, lv_03 ha il cielo piu' fitto; stesso ramo/stessa lerpColore, stessi operandi e ordine -> resa/replay IDENTICI, frameHash 0 3593059909 9/9). Pre percorso `UFFICIO/Officina/Test area/v9_lv03_giro38_pre_percorso` exit 0 GATE PERCORSO OK (2392 frame, 39.9 s, 0% fuori); pre smoke `.../v9_lv03_giro38_pre_smoke` exit 0 GATE OK medio 0.683 max 3.151 ms frame 0 replay IDENTICO frameHash 0 3593059909. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. Post smoke `.../v9_lv03_giro38_post_smoke` exit 0 GATE OK medio 0.702 max 3.369 ms frame 434 replay IDENTICO 9/9; post percorso `.../v9_lv03_giro38_post_percorso` exit 0 GATE PERCORSO OK (2392 frame, 39.9 s, 0% fuori); regressione lv_06_desktop smoke `.../v9_lv03_giro38_post_smoke_lv06` exit 0 GATE OK frameHash 0 3627137594 invariante replay IDENTICO. Frame piu' lento 3.369 ms < 8 ms Mac. gate_t1.sh non eseguito in questo turno (riferimento 24/11/2026 10:03 exit 0). Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 25/11/2026 12:45 (@developer-junior): V47 VJ4 soak lv_04_cromo + VJ5 ∞ review. `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 132.96 s). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv04_2511_1245" lv_04_cromo soak` exit 0 (GATE OK, 30000 frame, replay IDENTICO, ms/frame medio 0.660 max 2.757 frame 0, limite 20). VJ5: diff renderer.cpp/renderer.h 12:41:39 (hoist V9 ∞ giro 38/38b), nessun errore bloccante, nit skylineLut_ cache come colLut. VJ4 restano lv_05-lv_06. gate_t1.sh non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
