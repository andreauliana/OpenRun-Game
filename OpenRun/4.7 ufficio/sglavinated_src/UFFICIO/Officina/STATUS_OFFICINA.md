# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 23/11/2026 07:16 (@developer): V47 ∞ V9 fluidita', trentaduesimo giro, lv_01_neon. M `motore_nuovo/src/road/road.cpp` (Road::project: `(1.0f - tStart)` costante per segmento, prima ricalcolato a ogni campione in passo/t/tMezzo; hoistato in `unoMenoTStart` e mezzo-passo in `mezzoPasso`; stessi operandi/ordine -> stessi float, resa e replay IDENTICI, frameHash 0 3594390208 invariante). `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. `prova_gate .../v9_lv01_turno97_post_smoke lv_01_neon smoke` exit 0 — GATE OK, medio 0.660 max 3.080 ms (frame 0), replay IDENTICO, frameHash 0 3594390208. `prova_gate .../v9_lv01_turno97_post_percorso lv_01_neon percorso` exit 0 — GATE PERCORSO OK (1989 frame, 33.2 s, 0% fuori). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 23/11/2026 07:16 exit 0 — GATE T1: TUTTO OK, 6/6 GATE PERCORSO OK (conteggi giro identici ai riferimenti su tutti e 6 i livelli; smoke lv_01 medio 0.791 max 3.195 ms; nessuna riga FALLITO). Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 23/11/2026 07:04 (@developer-junior): V47 VJ5 ∞ review ultimi cambi src M `motore_nuovo/src/render/renderer.cpp` (nuovi vs 06:47 = scrittura diretta framebuffer al posto di `fb_.put` in `parete` `riga=fb_.data()+y*WIDTH` :911 usi :967/:987 e `soffittoRiga` `rigaSoff` :1003 uso :1020, commenti lv_06_desktop giro 31). Nessun errore: `put` (framebuffer.cpp:17-20) e' guard bounds + `buf_[y*WIDTH+x]=color`, identica alla scrittura diretta via `data()`; coordinate sempre in bounds (y0=max(0,ceilN)>=0, y1=min(HEIGHT-1,ny-1)<=HEIGHT-1 :797-798; a0>=0, b0<=WIDTH-1 :905/:1004) -> guard sempre false, stessi pixel/valori, `segnaTubo` invariato -> resa/replay IDENTICI, schema gia' nel file e in hline. Rilievi minori: commenti :906-910/:1004-1006/:1028-1031 verbosi; rilievo doc persistente :495 "riga 408". `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest **27/27**, 159.95 s, prova_gate ricompilato, gate_decorazioni passa). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_atomico_direttawrite_2311_t1" lv_02_atomico percorso` **exit 0** (GATE PERCORSO OK, 2476 frame/41.3 s @60 Hz, fuori strada 0). gate_t1.sh non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
