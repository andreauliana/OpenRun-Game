# Riassunto developer — 07/11/2026 ~12:09 (turno 3)
- Gate T1 verde: sh UFFICIO/Officina/TOOL/gate_t1.sh exit 0 alle ~11:24; ctest 27/27 (gate_urto OK, gate_gara OK, 6/6 percorsi GATE PERCORSO OK, replay IDENTICO, frequenze 50/60 Hz OK, ms/frame medio 1.401 max 4.801); screenshot P1 OK (personaggio+fumetto frame 446).
- ENG1: testi a schermo in inglese (fatto). ARC1: nome salvato + classifica Top-10 (fatto).
- KO1: implementato (rivale_urti_ko=4, contatore urti, danni/tremolio da 2a botta, segnalino 2.5 s, KO uscita animata e OUT).
- AUTO1: chiuso tecnico (A4 rotazione, scala 1.0, componenti ruote/luci_stop/fumo in renderer.cpp/game.cpp); review @grafico positiva; J-A1 gate OK.
- RIV punto 1 e 2 fatti; punto 3 sprite rivali fatti (7 retro, game.cpp:1373); lato per le curve bloccato (vincolo Non toccare src/runner/).
- PIST1 P1/P2 fatti; P3 parapetti oggetto sull'acqua contromandato (Andrea 06/11: ponte aperto, parapetto basso + piloni nel renderer).
- Nessun blocco di codice eseguibile questo turno: attendere decisione Andrea o catalogo @grafico.
