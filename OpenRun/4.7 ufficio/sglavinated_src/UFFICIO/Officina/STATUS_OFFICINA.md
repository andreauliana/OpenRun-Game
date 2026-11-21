# STATUS OFFICINA — OpenRun v4.7

Aggiornato: 2026-11-18 (Claude, pulizia del flusso). Unico handoff tecnico dell'Officina per il @capo.
Regola: @developer e @developer-junior RISCRIVONO la propria riga della tabella (max 30 righe in tutto il file). Niente sezioni "Report turno", niente storico: lo storico e' in git e nei diari.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | turno 21/11/2026 11:22 (@developer): V47 ∞ V9 fluidita', ventisettesimo giro, lv_05_spray. M `motore_nuovo/src/render/renderer.cpp` (drawSegmentTrapezoid, strisce pedonali dell'incrocio): base della riga framebuffer precalcolata e scrittura diretta invece di `fb_.put` per pixel con bounds ridondanti, guard di y fuori dal ciclo; stessi pixel/resa/replay, frameHash invariato 0 4250821778. `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0. `prova_gate .../v9_lv05_turno67_post_smoke lv_05_spray smoke` exit 0 — GATE OK, medio 0.605 max 2.794 ms, replay IDENTICO, frameHash 4250821778. `prova_gate .../v9_lv05_turno67_post_percorso lv_05_spray percorso` exit 0 — GATE PERCORSO OK (2920 frame, 48.7 s, 0% fuori). `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 21/11/2026 11:22 exit 0 — GATE T1: TUTTO OK, 6/6 GATE PERCORSO OK (lv_05 smoke medio 0.606 max 2.730 ms; nessuna riga FALLITO). Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettagli: `UFFICIO/out/developer/ULTIMO_TURNO.md`. |
| Ultimo report developer-junior | turno 21/11/2026 12:38 (@developer-junior): V47 VJ5 ∞ review nuovo hunk src M `motore_nuovo/src/render/renderer.cpp` (3 hunk; `git status --short`/`git diff` exit 0; nuovo = 1' `bayerRigaSpiaggia=BAYER4[y&3]` :308 in `drawTerraSospesa` spiaggia sfumata :303-310, lv_04_cromo giro 28, livello diverso da lv_03_foglie/schiuma turno prima; 2' schiuma :316-325 e 3' parete :876-912 gia' rivisti): hoist riga Bayer per pixel in sfuma spiaggia. Nessun errore: y costante nel ciclo x, BAYER4 constexpr :20, stessa espressione/cast -> resa/replay IDENTICI. Semplificazione: commento :303-307 lungo. `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 8** (ctest 26/27, 147.65 s, ricompilato): FAIL unico `gate_decorazioni` lv_04_cromo `skyline.futurista` non risolto (D `GRAFICA/scenari/lv_04_cromo/skyline.png` preesistente, non da hunk). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv04_cromo_spiaggia_bayer_2111" lv_04_cromo percorso` **exit 0** — GATE PERCORSO OK, 4840 seg, 2125 frame/35.4 s @60 Hz, fuori strada 0, car 3.7 ms, init 11.0 ms. `gate_t1.sh` non eseguito. Dettagli: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md`. |
| Gate | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (build + tutte le prove, log in `UFFICIO/Officina/Test area/gate_t1.log`). Riportare solo se eseguito nel turno, con data, ora ed exit code. |
| Ultimo exit code dichiarato (agenti) | vedi le due righe sopra (prova_gate exit 0, ctest 27/27 il 13/11). |
| Performance | Mac: limite 8 ms/frame per livello (V9). Pi 400: limite 12 ms, non ancora misurato (blocco H sospeso). |
| Test | `sh UFFICIO/Officina/TOOL/run_tests.sh` = build + ctest (27 test). Eseguire ctest in serie: i test smoke scrivono nella stessa cartella. |

## Strumenti dell'Officina (`TOOL/`)

- `run_tests.sh` build Release + ctest; `gate_t1.sh` (anche `gate_t1.sh soak`) prove integrate; `check_sdl_init.sh` controllo del runner SDL; `build_libretro.sh` core libretro (V6); `prepara_build.sh` usato dagli altri script.
- Build in `UFFICIO/Officina/build/` (fuori da git); log e screenshot in `UFFICIO/Officina/Test area/` (fuori da git).
- Lo status lungo fino al 18/11 resta nella storia di git di questo file.
