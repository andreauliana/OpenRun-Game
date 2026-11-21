# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 21/11/2026 11:22 (@developer): V47 ∞ V9 fluidita', ventisettesimo giro, lv_05_spray. M `motore_nuovo/src/render/renderer.cpp` (drawSegmentTrapezoid, strisce pedonali dell'incrocio): base della riga framebuffer precalcolata e scrittura diretta invece di `fb_.put` per pixel con bounds ridondanti, guard di y fuori dal ciclo; stessi pixel/resa/replay, frameHash invariato 0 4250821778. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. `prova_gate .../v9_lv05_turno67_post_smoke lv_05_spray smoke` exit 0 — GATE OK, medio 0.605 max 2.794 ms, replay IDENTICO, frameHash 4250821778. `prova_gate .../v9_lv05_turno67_post_percorso lv_05_spray percorso` exit 0 — GATE PERCORSO OK (2920 frame, 48.7 s, 0% fuori). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 21/11/2026 11:22 exit 0 — GATE T1: TUTTO OK, 6/6 GATE PERCORSO OK (lv_05 smoke medio 0.606 max 2.730 ms; nessuna riga FALLITO). Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 21/11/2026 11:50 (@developer-junior): V47 VJ5 ∞ review del NUOVO hunk src M `motore_nuovo/src/render/renderer.cpp` (unico M in src; `git status --short`/`git diff` exit 0; 5 hunk, il nuovo = "V9 ∞ 21/11 lv_01_neon giro 28" in `drawTerraSospesa` :331-348, livello/file diverso da lv_06_desktop/drawSegmentTrapezoid del turno prima): riflessi delle onde da `fb_.put(x,y,riflesso)` per pixel a guard y fuori dal ciclo (:337), base riga una volta (:338), `rigaOnde[x]` (:346). Nessun errore: x in [0,WIDTH) dal loop -> controlli x di put sempre falsi, guard y = controllo y di put (framebuffer.cpp:18), `data()` = stessa buf_ (framebuffer.h:26), indice max 71679 < 71680, riga dipinta da hline :314, calcoli :340-345 puri -> resa/replay IDENTICI, coerente con hshFila/sogliaBayerSfuma/shiftSchiuma. Semplificazioni: commento :331-336 (6 righe) troppo lungo, guard y ridondante (y in [~112,223]) ma innocuo, rilievo doc :460 "riga 408" (dichiarazione :464). `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 142.73 s, `prova_gate` ricompilato) poi `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv01_neon_onde_riga_2111" lv_01_neon percorso` exit 0 — GATE PERCORSO OK, 4540 seg, 1989 frame/33.2 s @60 Hz, fuori strada 0, caricamento 6.8 ms, init 14.8 ms. `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
