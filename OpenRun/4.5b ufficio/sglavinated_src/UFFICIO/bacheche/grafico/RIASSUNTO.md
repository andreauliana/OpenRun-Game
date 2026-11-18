# RIEPILOGO — @grafico (18/11/2026 13:03)
1. V47 VG3 ∞ giro 81: `vg3_date_png_raw.py` exit 0 — 145 PNG (133 confrontati, 12 non motore, 0 fuori copertura), 86 raw scenari (80 oggetti a byte, 6 skyline a contenuto) + 47 altri gruppi; 0 da convertire, 126 a pari contenuto.
2. `vg3_raw_senza_catalogo.py` exit 0: 142 raw del motore, 142 tracciati nel catalogo, 0 senza voce.
3. `vg3_png_non_catalogati.py` exit 0: png non catalogati 0, noti non-asset 1 (`sprite/giocatore_pose/car-custom-v2.png`, tavola sorgente di Andrea, area bloccata, non letta dal motore).
4. Pose auto: 2 PNG + 2 raw allineati (auto_dritta, auto_sx4), catalogo coerente (altre pose "mancante"). Area bloccata: non toccata.
5. Nessun PNG di Andrea più nuovo, nessuna conversione eseguita. Tutto allineato.
6. Mancanti noti invariati: `font.antonio` (ttf assente), `gianni`/`gesu`/`ufo`, `lv_04_cromo/skyline_trama.png`, `sprite/rivali/auto_giocatore_sbandata.png`.
7. Zero risorse esterne, zero Sega. Gate non eseguito (18/11/2026 13:03, fuori blocco VG3).
8. File turno: `UFFICIO/bacheche/grafico/diario.md` (+1 riga), questo RIASSUNTO, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
9. Prossimo: VG3 ∞ — ricontrollo date/contenuto; se Andrea mette nuove pose, conversione con `converti_pose_auto.py` senza ritocchi.