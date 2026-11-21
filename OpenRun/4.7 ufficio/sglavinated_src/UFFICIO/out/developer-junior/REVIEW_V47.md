# REVIEW V47 — VJ5 ∞ hoist riga Bayer spiaggia sfumata (21/11/2026 12:38)

- **Ruolo**: @developer-junior (review codice @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞. `git status --short`: unico M src `motore_nuovo/src/render/renderer.cpp` (altri M: coordinamento; D `GRAFICA/scenari/lv_04_cromo/skyline.png` preesistente). `git diff -- motore_nuovo/src/render/renderer.cpp` exit 0: 3 hunk. Nuovo rispetto al turno 12:25: primo hunk `bayerRigaSpiaggia` in `drawTerraSospesa` ramo spiaggia sfumata :303-310, lv_04_cromo giro 28 (livello diverso da lv_03_foglie/schiuma del turno prima). Secondo hunk schiuma :316-325 e terzo parete :876-912 gia' rivisti (12:25, 12:09), invariati.

## VJ5 — review `renderer.cpp:303-310` (spiaggia sfumata, lv_04_cromo giro 28)
- Cambio: nel ramo `spiaggia && !schiuma` con `sfuma > 0.0f` hoista `bayerRigaSpiaggia = BAYER4[y & 3]` (:308) e lo riusa a :310 al posto di `BAYER4[y & 3][x & 3]` riletto a ogni pixel.
- **Nessun errore trovato**: `y` e' la variabile del ciclo esterno `for (i32 y = a0; y <= a1; ++y)` (:298), costante nel ciclo x interno; `BAYER4` e' `constexpr u8[4][4]` (renderer.cpp:20), non mutata. Stessa espressione `(f32)bayerRigaSpiaggia[x & 3] + 0.5f < sogliaBayerSfuma ? prato : sabbia`, stessi operandi/ordine, indicizzazione intera e cast f32 identici -> resa/replay IDENTICI. Coerente con gli hoist Bayer gia' presenti in battigia schiuma (:322) e con lo schema delle righe precalcolate in `drawTerraSospesa`.
- **Semplificazione (riga)**: `renderer.cpp:303-307` commento 5 righe per 1 riga di hoist; accorciabile. Nome `bayerRigaSpiaggia` distinto da `bayerRiga` del ramo schiuma e' chiaro (rami separati da `continue`), nessun problema.
- **Nota**: rilievo doc persistente `renderer.cpp:477` cita "riga 408" (fuori hunk).

## Prove (21/11 12:38)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 8** (ctest 26/27, 147.65 s; `prova_gate` ricompilato). UNICO FAIL: `gate_decorazioni` — `lv_04_cromo: id 'skyline.futurista' non risolto (file inesistente GRAFICA/scenari/lv_04_cromo/skyline.png)`, coerente con `D GRAFICA/scenari/lv_04_cromo/skyline.png` preesistente; non causato da questo hunk renderer.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv04_cromo_spiaggia_bayer_2111" lv_04_cromo percorso` **exit 0** (GATE PERCORSO OK, 4840 seg, 2125 frame/35.4 s @60 Hz, fuori strada 0, car 3.7 ms, init 11.0 ms).
- `gate_t1.sh` non eseguito.

## Stato
- VJ4 chiusa 6/6. VJ5 ∞ prosegue: prossimo turno rileggere ultimo cambio `src/` se cambia (livello/file diverso da `lv_04_cromo`/`drawTerraSospesa` spiaggia sfumata). Fail gate_decorazioni da skyline lv_04 mancante preesistente. Flakiness smoke/spike storici sopra target Pi 400 12 ms.