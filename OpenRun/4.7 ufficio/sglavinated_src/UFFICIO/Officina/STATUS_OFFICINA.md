# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 22/11/2026 11:21 (@developer): V47 ∞ V9 fluidita', trentesimo giro, lv_04_cromo. M `motore_nuovo/src/render/renderer.cpp` (galleria parete): hoistato `dvNeon = tratto ? std::abs(v-0.16f) : 0.0f` per-pixel nel tratto attivo (prima abs ricalcolato nel ramo alone); stessi valori, resa e replay IDENTICI, frameHash 0 4118572995 invariato. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. `prova_gate .../v9_lv04_turno84_post_smoke lv_04_cromo smoke` exit 0 — GATE OK, medio 0.791 max 4.294 ms (frame 0), replay IDENTICO, frameHash 4118572995. `prova_gate .../v9_lv04_turno84_post_percorso lv_04_cromo percorso` exit 0 — GATE PERCORSO OK (2125 frame, 35.4 s, 0% fuori). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 22/11/2026 11:21 exit 0 — GATE T1: TUTTO OK, 6/6 GATE PERCORSO OK (lv_04_cromo smoke medio 0.708 max 2.976 ms frame 0; nessuna riga FALLITO). Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 22/11/2026 11:52 (@developer-junior): V47 VJ5 ∞ review ultimo cambio src M `motore_nuovo/src/render/renderer.cpp` (diff V9 galleria): `luceEsterna` :848-850 usata solo da parete :967, soffittoRiga :985-988 inlinea logica identica con `sogliaLuceG` -> duplicazione (semplificazione, nessun errore), resa/replay IDENTICI. Rilievo doc persistente :477 "riga 408". `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 8** (ctest 26/27, 151.11 s, ricompilato) FAIL unico preesistente `gate_decorazioni` lv_04_cromo `skyline.futurista` mancante. `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_atomico_luceesterna_sogliag_2211_t2" lv_02_atomico percorso` **exit 0** (GATE PERCORSO OK, 2476 frame/41.3 s @60 Hz, fuori strada 0). gate_t1.sh non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
