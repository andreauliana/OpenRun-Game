# RIASSUNTO — @grafico (25/11/2026 12:56, Turno 3, giro 203)
- **Blocco:** V47 VG3 ∞ — confronta PNG `GRAFICA/scenari/` vs raw; area mia allineata.
- **Stato:** 3 controlli VG3: 0 da convertire, 6/6 skyline allineati, 166 PNG/134 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/45 allineati/0 da convertire, 155 raw/155 in catalogo/0 senza voce, 153 voci/159 riferimenti, 0 duplicati/ripetuti/scritture diverse/sospetti, 4 noti non-asset/0 catalogati/0 da decidere. Unico flag Andrea noto `traffico_incrocio/lato_furgone_bianco.png.png` (doppia estensione, 32.3% pixel diversi dal raw, nome corretto allineato): rinomina/converti decide Andrea. Area bloccata giocatore: 5 pose `_b` con raw e senza PNG (noto, nessuna conversione da qui).
- **Verifiche (25/11 12:56):** `vg3_date_png_raw.py` exit 1 (solo flag Andrea); `vg3_raw_senza_catalogo.py` exit 0; `vg3_png_non_catalogati.py` exit 0; `vg3_png_non_catalogati.py --autotest` exit 0 (22 prove OK).
- **Produzione:** `TOOLS/vg3_png_non_catalogati.py` (file diverso dal turno prima, che fu `vg3_raw_senza_catalogo.py`) — stato residui noti estratto in `esito_noti()` pura con autotest; `analizza_png()` la usa; output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`, `TOOLS/vg3_png_non_catalogati.py`.
- **Gate:** non eseguito (25/11 12:56).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; conversioni solo se PNG piu' nuovo, mai area Andrea; prossimo tool da toccare: `vg3_date_png_raw.py`.
