# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 21/11/2026 10:24 (@developer): V47 passo ∞ V9 (fluidita' a giro), ventisettesimo giro, lv_01_neon. M `motore_nuovo/src/render/renderer.cpp`: in `drawTerraSospesa` (riflessi onde sul mare) `hshFila=(u32)(fila*73856093)` e' invariante per riga ma veniva rifatto per pixel; precalcolato una volta per riga e riusato (XOR associativo -> stessi bit); `cellaF=xw/ONDA_PASSO_X` riusato in `floor` e in `dentro` invece di due divisioni -> stessi riflessi, resa e replay IDENTICI, frameHash invariato (0 2404227578). `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0 (pre/post). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno65_pre_smoke" lv_01_neon smoke` exit 0 — `GATE OK`, medio 0.647 max 1.371 ms, replay IDENTICO, frameHash 0 2404227578; post-smoke exit 0 — medio 0.648 max 1.338 ms, frameHash identico. `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno65_post_percorso" lv_01_neon percorso` exit 0 — `GATE PERCORSO OK` (1989 frame, 33.2 s, 0% fuori). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito il 21/11/2026 (Turno 1, ore 10:24) exit 0 — `GATE T1: TUTTO OK`, 6/6 `GATE PERCORSO OK` (lv_01_neon smoke medio 0.708 max 2.662 ms; nessuna riga FALLITO). Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 21/11/2026 10:29 (@developer-junior): V47 VJ5 ∞ review nuovo hunk `hshFila` in `drawTerraSospesa` (`motore_nuovo/src/render/renderer.cpp`:319-331, lv_01_neon giro 27, unico M src): termine riga dell'hash onde precalcolato una volta per riga (:324, uso :329) invece che per pixel, e `cellaF` (:327) riusato per `cella`/`dentro` invece della doppia divisione. Nessun errore: `fila` const, prodotto many-int (no FMA), XOR con lo stesso termine -> stessi bit, `cellaF` stessa divisione -> `floor`/`dentro` bit-identici, resa/replay IDENTICI. Semplificazioni: commento 5 righe, "XOR associativo" improprio, rilievo doc :446 "riga 408" (ora :450). `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 146.77 s, `prova_gate` ricompilato) poi `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv01_neon_hshfila_2111" lv_01_neon percorso` exit 0 — GATE PERCORSO OK, 4540 seg, 1989 frame/33.2 s @60 Hz, fuori strada 0, car 5.9 ms, init 10.5 ms. `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
