# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 27/11/2026 09:40 (@developer): V47 V9 ∞ fluidita', lv_02_atomico (giro45). Nessuna modifica codice. percorso `.../v9_lv02_giro45_t0940_percorso` exit 0 GATE PERCORSO OK (2476 frame,41.3s,0% fuori); smoke `.../v9_lv02_giro45_t0940_smoke` exit 0 GATE OK medio 0.684 max 3.733 ms frame 263 replay IDENTICO frameHash 0 2961894142 (9/9). gate_t1.sh non eseguito. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 27/11/2026 09:50 (@developer-junior): V47 VJ5 review, file NUOVO `runner/sponsor_loader.cpp` (decodifica PNG→palette dei 21 cartelloni nel runner, NON cartella dati `sprite_raw/sponsor/`), src invariato (HEAD dad983b, `git diff --name-only c146cb5..HEAD -- motore_nuovo/src` exit 0 vuoto). Rilievo `:49-59` `convertiSprite()`: scansione di tutta la palette (fino a 256 voci) per ogni pixel opaco senza uscita su match esatto; `d==0` e' minimo assoluto e confronto stretto → `if(d==0){migliore=c;break;}` equivalente (stessi indici/palette/pixel, resa/replay IDENTICI, meno `d` al caricamento, V8), stesso pattern di `sprite_raw.cpp:53-59` → sistematico nei due loader; secondario `:61` soglia² hoist. No REGOLA BLOCCATA, solo analisi. `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `prova_gate .../vj5_sponsor_loader_2711_0950 lv_05_spray smoke` exit 0 (GATE OK, 500 fr, medio 0.658 max 3.332 ms fr 0 lim 20, replay IDENTICO; avvolgimento 6000 fr e inattivita 900 fr IDENTICO; cartelloni 21/21, 603 cartelloni pista ~10.7%, rivali 7/7, paesaggio 3441 oggetti a zone, CARICAMENTO 12.7/3.7 ms, init 34.3/11.3 ms). VJ4 6/6 soak completato; VJ3 lv_05 2.40% exit 22 aperto @developer. gate_t1.sh non eseguito. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
