# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 13/11/2026 08:45 (@developer): V47 passo ∞ V9 (fluidita' a giro), quinto giro, lv_04_cromo. `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 27/27 verde (314 s). `prova_gate ... lv_04_cromo percorso` exit 0 — `GATE PERCORSO OK` (2114 frame, 35.2 s, 0% fuori strada); `prova_gate ... lv_04_cromo smoke` exit 0 — `GATE OK`, ms/frame medio 1.728 max 4.176 (limite 8), replay 2x IDENTICO, caricamento 3.2-6.3 ms + init 6.0-13.6 ms. Nessun alleggerimento: livello gia' sotto soglia, nessun sorgente modificato. `gate_t1.sh` non eseguito in questo turno. Output: `UFFICIO/Officina/Test area/v9_lv04_turno5*`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 19/11/2026 10:03 (@developer-junior): V47 VJ5 review dell'ultimo diff src (M `motore_nuovo/src/render/renderer.cpp` 19+/7-: `texel(tx,ty)` -> `coloreIdx(idx)` + riga sorgente `rigaTex = px + ty*sw` per riga schermo, `renderer.cpp:1772-1777,1782,1804,1814`). Nessun errore: stessi indici (`px[ty*sw+tx]`, `px[ry*sw+rx]`), lambda identico (spPal nullptr se palette vuota, niente divisione per 0), bounds `tx0<=sw-1`, `(tx0+tx1)/2<=sw-1`, `rx<tx1<=sw`, `ty<sh`. Semplificazione: init morta `rigaTex = px` (`:1782`) -> dentro il ciclo sy. `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 due volte (ctest 27/27: 275.25 s e 289.97 s sul binario finale); `prova_gate ... vj5_lv04_1911 lv_04_cromo percorso` exit 0 (GATE PERCORSO OK, 2125 frame); smoke lv_04 exit 0 (GATE OK, medio 1.597/1.142, max 9.728 frame 230 / 3.614 frame 0, replay 2x IDENTICO; max 21-23 ms di 2 run sotto carico non riprodotti). Test: aggiunto `msMaxFrame` in `motore_nuovo/test/prova_gate.cpp` (Esito + print smoke/soak), `cmake --build ... --target prova_gate` exit 0. `gate_t1.sh` non eseguito. Prossimo: VJ5 sul prossimo diff del @developer. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
