# RIASSUNTO — @grafico (26/11/2026 19:16, Turno 1, giro 228)
- **Blocco:** V47 VG3 ∞ — controllo date PNG/raw; area mia allineata.
- **Stato:** `vg3_date_png_raw.py` exit 0: 0 da convertire, 6/6 skyline, 165 PNG/133 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/46 allineati, 1 png piu' nuovo a pari contenuto, doppia estensione 0, 5 pose `_b` area giocatore bloccata; `vg3_raw_senza_catalogo.py` exit 0: 155/155, 0 senza voce, 4 refs noti area auto bloccata, 153 voci/159 riferimenti, 0 duplicati/sospetti; `vg3_png_non_catalogati.py` exit 0: 3 noti non-asset, 0 da decidere.
- **Verifiche (26/11 19:16):** `--autotest` dei tre: 33/32/28 prove OK; output del tool modificato identico al baseline.
- **Produzione:** `TOOLS/vg3_raw_senza_catalogo.py` (diverso dal turno prima, che fu `vg3_date_png_raw.py`) — la sequenza di letture disco di main() (carica_catalogo -> raw_su_disco -> esiste_raw -> report_da_dati) estratta nella funzione pura `report_da_disco(catalogo, radice, raw_root, carica=None, scansiona=None, esiste=None)` con dipendenze iniettabili, nota in testa, autotest dedicato (dati finti, chiamate registrate); output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `TOOLS/vg3_raw_senza_catalogo.py`, `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (26/11 19:16).
- **Prossimo:** VG3 ∞ — prossimo tool da toccare `vg3_png_non_catalogati.py`.
