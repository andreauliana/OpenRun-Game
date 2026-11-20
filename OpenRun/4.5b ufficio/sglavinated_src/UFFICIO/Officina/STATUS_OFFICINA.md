# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 20/11/2026 11:22 (@developer): V47 passo ∞ V9 (fluidita' a giro), ventiquattresimo giro, lv_05_spray. M `motore_nuovo/src/render/renderer.cpp`: in `drawSprite` il testo per pixel `inTubo(scrX, screenYRow)` sostituito con `clipTubo && tuboRiga[scrX]` (base riga di `tubo_` precalcolata una volta per riga schermo, coordinate gia' ritagliate al framebuffer, `clipTubo = clipAttivo_ && dentroGalleria_` costante); stessi pixel, frameHash identici. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0 (pre/post). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno50_pre" lv_05_spray percorso` exit 0 — `GATE PERCORSO OK` (2920 frame, 48.7 s, 0% fuori); smoke exit 0 — `GATE OK`, medio 0.809 max 3.390 ms, replay IDENTICO, frameHash 0 4250821778. Post: percorso exit 0 `GATE PERCORSO OK`; smoke exit 0 medio 0.796 max 3.184 ms, replay IDENTICO, frameHash identici. `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito il 20/11/2026 11:22 exit 0 — `GATE T1: TUTTO OK`, 6/6 `GATE PERCORSO OK`. Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 20/11/2026 10:50 (@developer-junior): V47 VJ4 soak 4/6 su lv_04_cromo + VJ5 (rivisti i due hunk src nuovi: `drawRoad` riusa `segs` `renderer.cpp:244-251`, `calcolaIncrocio` fuori dal ciclo righe `:370-375`/`:389` -> nessun errore; proposta: `!segs.empty()` ridondante a `:250`, `nS` riusabile). `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 300.61 s, `prova_gate` ricompilato); `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv04_soak_2011" lv_04_cromo soak` exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash, medio 1.737 max 22.087 ms (frame 5554), caricamento 5.8/8.5 ms, init 19.3/18.8 ms, offroad frame 156. Rilievo: max 22.087 ms sopra la soglia V9 (8 ms), il gate limita solo la media (20 ms) -> segnalato per V9 lv_04. `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
