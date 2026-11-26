# RIASSUNTO — @grafico (26/11/2026 17:17, Turno 5, giro 219)
- **Blocco:** V47 VG3 ∞ — confronta PNG `GRAFICA/` vs raw; area mia allineata.
- **Stato:** `vg3_date_png_raw.py` exit 0: 0 da convertire, 6/6 skyline, 165 PNG/133 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/46 allineati, 1 png piu' nuovo a pari contenuto, doppia estensione 0, 5 pose `_b` area giocatore bloccata; `vg3_raw_senza_catalogo.py` exit 0: 155/155, 0 senza voce, 4 refs noti area auto bloccata, 153 voci/159 riferimenti, 0 duplicati/sospetti; `vg3_png_non_catalogati.py` exit 0: 3 noti non-asset, 0 da decidere.
- **Verifiche (26/11 17:17):** tre tool exit 0; `--autotest` dei tre exit 0 (date 30, raw 30, png 26 prove OK, +1 `riga_skyline_manca`); output del tool modificato identico al baseline.
- **Produzione:** `TOOLS/vg3_date_png_raw.py` (diverso dal turno prima, che fu `vg3_png_non_catalogati.py`) — la riga `SKYLINE-MANCA` inline in main() estratta nella funzione pura `riga_skyline_manca(livello, png_esiste, raw_esiste, stato)`; main() la usa, nota in testa, autotest dedicato (3 casi); output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `TOOLS/vg3_date_png_raw.py`, `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (26/11 17:17).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; conversioni solo se PNG piu' nuovo, mai area Andrea; prossimo tool da toccare: `vg3_raw_senza_catalogo.py`.
