# RIEPILOGO — @grafico (18/11/2026 12:08)
1. V47 VG3 ∞ giro 80: `vg3_date_png_raw.py` exit 0 — 145 PNG (133 confrontati, 12 non motore, 0 fuori copertura), 86 raw scenari (80 oggetti a byte, 6 skyline a contenuto) + 47 altri gruppi; 0 da convertire, 126 a pari contenuto.
2. Due controlli di catalogo eseguiti: `vg3_raw_senza_catalogo.py` exit 0 (142 raw del motore, 142 tracciati, 0 senza voce); `vg3_png_non_catalogati.py` exit 0.
3. Tool corretto (`vg3_png_non_catalogati.py`): `sprite/giocatore_pose/car-custom-v2.png` ora e' un "noto non-asset" con motivo (tavola sorgente delle pose di Andrea, area bloccata, il motore non la legge). Non era un asset da catalogare: era una decisione non scritta.
4. Prima del fix il tool stampava "png non catalogati: 1"; ora "0 (noti non-asset 1)" — le decisioni sono distinte dai casi aperti.
5. Pose auto: 2 PNG + 2 raw allineati (auto_dritta, auto_sx4), catalogo coerente (altre pose "mancante"). Area bloccata: non toccata.
6. Mancanti noti invariati: `font.antonio` (ttf assente), `gianni`/`gesu`/`ufo`, `lv_04_cromo/skyline_trama.png`, `sprite/rivali/auto_giocatore_sbandata.png`.
7. Zero risorse esterne, zero Sega. Gate non eseguito (fuori dal blocco VG3).
8. File turno: `TOOLS/vg3_png_non_catalogati.py`, `UFFICIO/bacheche/grafico/diario.md` (+1 riga), questo RIASSUNTO, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
9. Prossimo: VG3 ∞ — ricontrollo date/contenuto; se Andrea mette nuove pose, conversione con `converti_pose_auto.py` senza ritocchi.
