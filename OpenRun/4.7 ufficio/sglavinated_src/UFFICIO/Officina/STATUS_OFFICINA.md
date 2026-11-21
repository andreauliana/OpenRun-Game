# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 20/11/2026 13:54 (@developer): V47 passo ∞ V9 (fluidita' a giro), venticinquesimo giro, lv_01_neon. M `motore_nuovo/src/render/renderer.cpp`: in `drawSegmentTrapezoid` la soglia invariante `giro_ - FASCIA_TERRENO` (fascia di cambio tema) veniva ricalcolata con una sottrazione a ogni riga dentro `misto`; precalcolata una volta per trapezio in `giroMenoFascia` fuori dal ciclo per riga (-1 sottrazione/riga in tappa 2+); stessi operandi/ordine -> misto/oltre/rumble identici, resa e replay IDENTICI, frameHash identici. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno61_post" lv_01_neon percorso` exit 0 — `GATE PERCORSO OK` (1989 frame, 33.2 s, 0% fuori). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno61_post_smoke" lv_01_neon smoke` exit 0 — `GATE OK`, medio 0.775 max 3.222 ms (frame 0), replay IDENTICO, frameHash 0 3594390208 (uguale al riferimento storico -> resa invariata). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito il 20/11/2026 (Turno 3) exit 0 — `GATE T1: TUTTO OK`, 6/6 `GATE PERCORSO OK`. Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 20/11/2026 12:34 (@developer-junior): V47 VJ5 ∞ review nuovo hunk `motore_nuovo/src/render/renderer.cpp` — `zebraFondo = bivioAperto ? lerpColore(road, nero, 0.18f) : 0u` calcolato una volta per trapezio (:387) e riusato :624, tolto calcolo per-riga :616. Nessun errore: guard `bivioAperto` (:604), `road`/`bivioAperto` const, `lerpColore` puro -> resa/replay identici; rilievo documentale :407-412 cita ancora "riga 408 (dichiarazione)" (ora :413). `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 232.77 s, `prova_gate` ricompilato); `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_atomico_zebra_2011" lv_02_atomico percorso` exit 0 — GATE PERCORSO OK, 5200 seg, 2476 frame/41.3 s @60 Hz, fuori strada 0, caricamento 5.3 ms, init 13.0 ms. `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
