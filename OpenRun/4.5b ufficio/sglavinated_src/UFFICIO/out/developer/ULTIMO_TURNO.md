# Ultimo turno @developer — 09/11/2026 08:51 (turno 1)
- Blocco/passo: MIG1 D1 [x] — paletti abbattuti in coordinate mondo (Andrea 08/11).
- File cambiati: `motore_nuovo/src/game/game.h`, `motore_nuovo/src/game/game.cpp`, `motore_nuovo/src/render/renderer.h`, `motore_nuovo/src/render/renderer.cpp`.
- Implementato: `Detrito` con z del paletto (nOra*PALO_PASSO) e laterale mondo x0 (bordo strada) -> x1 (erba a fianco, ~+480); volo 0.7 s di lato, poi resta appoggiato finché non esce dietro l'auto (12 slot in luogo dei 4); `Renderer::drawDetriti` proietta con la stessa proiezione della strada (projs_, lower_bound), salta galleria/ponte/carr. doppia, arco in volo, angolo da dritto a quasi sdraiato, base clampata sopra la riga delle ruote (~208 px). Gameplay `abbattiPali` invariato.
- Comando 1: `sh UFFICIO/Officina/TOOL/run_tests.sh` — exit 0, ctest 27/27 OK (gate_tratti verde this run).
- Comando 2: `sh UFFICIO/Officina/TOOL/gate_t1.sh` — exit 0 (09/11/2026 08:51).
- Esito gate_t1: GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK, replay IDENTICI, ms/frame medio 1.07-1.66 (max 10.47 spike unico, limite 20), screenshot P1 OK.
- Next: MIG1 D2 sportellata efficace (recupero rivale ~5 s; solo costanti `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`).
- J2 (junior): dopo questo turno si può provare D1 con screenshot.
