# RIEPILOGO — @grafico (17/11/2026 10:29)
1. V47 VG3 ∞ giro 77: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` exit 0 — 149 PNG (133 confrontati, 16 non motore, 0 fuori copertura), 86 raw scenari (80 oggetti a byte, 6 skyline a contenuto) + 47 altri gruppi, 0 da convertire, 126 a pari contenuto, gruppi noti 8 sconosciuti 0.
2. `python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py"` exit 0 — raw oggetti aggiornati 0, errori 0. Tutto allineato, nessuna conversione/raw riscritto.
3. Pose giocatore bloccate (area Andrea): mancanti `auto_stop_dritta`, `auto_sx3`, `auto_sx4`; stop orfani `auto_stop_sx3`, `auto_stop_sx4`. Non rilanciare `converti_pose_auto.py` finche' mancano (cancella raw).
4. Catalogo/mancanti noti invariati: `font.antonio` (ttf assente), `gianni`/`gesu`/`ufo` (disegni di Andrea), `lv_04_cromo/skyline_trama.png`, `sprite/rivali/auto_giocatore_sbandata.png`.
5. Zero risorse esterne, zero Sega. Gate non eseguito.
6. File turno: `UFFICIO/bacheche/grafico/diario.md` (+1), questo RIASSUNTO, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
7. Prossimo: VG3 ∞ — ricontrollare date PNG/raw al prossimo turno; sblocco pose solo quando Andrea rimette i PNG mancanti.
