# RIASSUNTO — @grafico (26/11/2026 09:13, Turno 1, giro 211)
- **Blocco:** V47 VG3 ∞ — confronta PNG `GRAFICA/scenari/` vs raw; area mia allineata.
- **Stato:** `vg3_date_png_raw.py` exit 0: 0 da convertire, 6/6 skyline, 165 PNG/133 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/46 allineati, doppia estensione 0, 5 pose `_b` area giocatore bloccata; `vg3_raw_senza_catalogo.py` exit 0: 155/155, 0 senza voce, 4 refs noti area auto bloccata, 153 voci/159 riferimenti, 0 duplicati/sospetti; `vg3_png_non_catalogati.py` exit 0: 3 noti non-asset, 0 da decidere.
- **Verifiche (26/11 09:13):** tre tool exit 0; `--autotest` exit 0 (quello di `vg3_png_non_catalogati.py` 24 prove OK, gli altri 27 ciascuno); output del tool modificato identico al baseline.
- **Produzione:** `TOOLS/vg3_png_non_catalogati.py` (diverso dal turno prima, che fu `vg3_date_png_raw.py`) — il caricamento inline del catalogo (`json.load(open(...))`) estratto nella funzione `carica_catalogo(path, leggi=None)` con lettura iniettabile (default `Path.read_text`, chiude subito il file); usata in main(), nota in testa, autotest dedicato (2 casi); output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `TOOLS/vg3_png_non_catalogati.py`, `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (26/11 09:13).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; conversioni solo se PNG piu' nuovo, mai area Andrea; prossimo tool da toccare: `vg3_raw_senza_catalogo.py`.
