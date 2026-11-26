# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 26/11/2026 17:20 (@developer): V47 V9 ∞ fluidita', lv_06_desktop (giro40). Nessuna modifica codice (max 2.909 ms < 8 ms). Pre percorso `.../v9_lv06_giro40_t6_pre_percorso` exit 0 GATE PERCORSO OK (2125 frame,35.4s,0% fuori); pre smoke `.../v9_lv06_giro40_t6_pre_smoke` exit 0 GATE OK medio 0.675 max 2.909 ms frame 0 replay IDENTICO frameHash 0 3627137594 (9/9). gate_t1.sh non eseguito (riferimento 25/11/2026 13:04 exit 0). Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 26/11/2026 17:59-18:15 (@developer-junior): V47 VJ5 ∞. Nessun cambio nuovo in `src/` (HEAD `c146cb5`, 11 hunk renderer.cpp gia' review). Angolo VJ5 = diagnosi/fix driver gate `pali` (prova_gate.cpp:2604-2613): tentato driver adattivo (soglia 880, recupero speed<150) -> REGRESSIONE lv_01 (x 864-888, hit 0, ratio non rilevato) e lv_06 ancora FAIL (vel 72-92<150, soglia spPrima>150:2624 mai vera); driver ripristinato all'originale, ricompilato. Conferma: `prova_gate .../vj5_pali_lv01_revert_2611_1812 lv_01_neon pali` exit 0 (GATE PALI OK, hit 10). `prova_gate .../vj5_pali_lv06_fix_2611_1803 lv_06_desktop pali` exit 27 (hit 0). `ctest --test-dir UFFICIO/Officina/build --output-on-failure` exit 0 (27/27, 163.99 s). Rilievo: gate pali lv_01-tuned (drift naturale porta a x~1532); su lv_06 serve driver che raggiunga |xRel|>=860 con speed>150 o dichiarare lv_01-only. RILIEVO BLOCCANTE invariato: 5 hunk drawPlayerCar (Andrea 06/11). VJ3 lv_05 2.40% aperto, VJ4 chiuso 6/6. gate_t1.sh non eseguito. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
