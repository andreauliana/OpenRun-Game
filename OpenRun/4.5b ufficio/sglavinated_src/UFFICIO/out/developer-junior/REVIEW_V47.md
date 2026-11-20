# REVIEW V47 — VJ5 review nuovi hunk src + VJ4 lv_05 soak (20/11/2026 11:10)

- **Ruolo**: @developer-junior (VJ5 review del diff src del @developer + VJ4 soak 5/6, nessuna modifica a src).

## VJ5 — review diff src (nuovo rispetto alle 10:50)
`git diff -- motore_nuovo/src/render/renderer.cpp motore_nuovo/src/render/renderer.h`: oltre ai due hunk gia' rivisti (drawRoad `segs`, `calcolaIncrocio`), sono presenti:
- `renderer.cpp:353-362` + `:471-490` (drawSegmentTrapezoid): `centroNear[2]`/`centroFar[2]` precalcolati per trapezio (prima ricalcolati a ogni riga y). **Nessun errore**: `pxNear`/`pxFar` (`:348-349`), `rami` (`:352`), `sospeso`/`galleria` (`:335-338`) sono const prima del ciclo righe (`:387`); gli array sono letti solo per `r < rami` e nel blocco `if (bivioAperto)` dove `bivioAperto == (rami==2)` (`:377`). Stesse espressioni float, stesse troncature `lx/rx`, resa e replay identici (ctest 27/27, soak replay 2x IDENTICO).
- `renderer.cpp:380-385` + `:399` (drawSegmentTrapezoid): `calcolaIncrocio` fuori dal ciclo righe. **Nessun errore** (gia' rivisto alle 10:50, riconfermato): tutti i termini invarianti per trapezio.
- `renderer.cpp:1797-1813` + `renderer.h:503-511` (drawSprite): LUT `colLut_[256]` ricostruita solo se cambiano `spPal`/`palBase`/`nPalette`. **Nessun errore** (gia' rivista alle 10:05): ogni voce = vecchio `coloreIdx(idx)`.
- `renderer.cpp:~1871-1905` (drawSprite): lettura `tx0`/`tx1` spostata nel solo ramo di ricerca del texel pieno, con `largh`/`fineRiga` precalcolati per colonna e `fineRiga += sw`. **Nessun errore**: `fineRiga_r = rigaCercaBase + tx0 + (tx1-tx0) + r*sw = rigaCercaBase + tx1 + r*sw`, identico al vecchio `pCerca + (tx1-tx0)` ricalcolato per riga; stessi texel, stessa immagine.
- **Semplificazione proposta** (`renderer.cpp:~1898-1901`): eliminare il locale `largh` e scrivere direttamente `const u8* fineRiga = rigaCercaBase + pCol->tx1;` (algebraicamente identico a `pCerca + largh`): risparmia una sottrazione e un add per colonna e un locale.
- Restano aperte: chiave LUT senza contenuti palette (se `pal_`/`spPal` cambiano in place con stessa chiave la LUT e' stale; oggi non attivo), init morta `rigaTex` (`:1799`), `screenYRow` solo in `inTubo` (`:1884`).

## VJ4 Soak (20/11 11:10) — lv_05_spray (5/6)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 226.78 s; `prova_gate` ricompilato prima della prova).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv05_soak_2011" lv_05_spray soak` **exit 0** (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash; ms/frame medio 1.486, max 14.066 al frame 25351; caricamento 3.7/3.1 ms, init 11.0/8.8 ms; offroad frame 156; 3441 oggetti, 603 cartelloni).
- Rilievo: max 14.066 ms > soglia V9 (8 ms) ma il gate limita solo la media (20 ms) -> da segnalare per V9 lv_05. Picco piu' basso dei soak lv_03 (39.373) e lv_04 (22.087).
- `gate_t1.sh` non eseguito.

## Prossimo
- VJ4 soak lv_06_desktop (6/6), ultimo della serie. VJ5: review del prossimo diff src del @developer.
