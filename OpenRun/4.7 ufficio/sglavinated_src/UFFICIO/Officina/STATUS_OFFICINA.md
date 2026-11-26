# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 26/11/2026 10:09 (@developer): V47 V9 ∞ fluidita', lv_01_neon (giro40). M `motore_nuovo/src/render/renderer.cpp` (drawEffettiAuto segni gomme/polvere fuoristrada/scintille atterraggio: scrittura diretta fb_ con bounds check al posto di fb_.put; stessi pixel, resa/replay IDENTICI, frameHash 0 3594390208 9/9). Pre percorso `.../v9_lv01_giro40_pre_percorso` exit 0 GATE PERCORSO OK (1989 frame,33.2s,0% fuori); pre smoke `.../v9_lv01_giro40_pre_smoke` exit 0 GATE OK medio 0.594 max 2.934 ms frame 0 replay IDENTICO. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. Post smoke `.../v9_lv01_giro40_post_smoke` exit 0 GATE OK medio 0.599 max 4.292 ms frame 0 replay IDENTICO 9/9; post percorso `.../v9_lv01_giro40_post_percorso` exit 0 GATE PERCORSO OK; regressione lv_04_cromo `.../v9_lv01_giro40_post_smoke_lv04` exit 0 GATE OK frameHash 0 1415607011 invariante. gate_t1.sh non eseguito (riferimento 25/11/2026 13:04 exit 0). Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 26/11/2026 08:01 (@developer-junior): V47 VJ5 ∞ review del cambio nuovo `renderer.cpp` (mtime 07:58:56) = hoist `absGiro` in drawOggettoTerra faro :1320-1333 (bit-identico, `giro` const, stessi operandi/ordine; nit: con -O2 era già CSE). `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (prova_gate ricompilato, ctest 27/27, 147.69 s). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_smoke_lv03_2611_0805" lv_03_foglie smoke` exit 0 (GATE OK, replay 2x IDENTICO, medio 0.668 max 3.289 frame 0, limite 20). RILIEVO BLOCCANTE ancora aperto: hunk drawPlayerCar :2627-2628/:2714-2715 toccano area bloccata (Andrea 06/11) -> autorizzazione o revert. Restano VJ3 lv_03-lv_06. gate_t1.sh non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
