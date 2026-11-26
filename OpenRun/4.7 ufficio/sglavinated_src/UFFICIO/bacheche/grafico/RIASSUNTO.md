# RIASSUNTO — @grafico (26/11/2026 10:31, Turno 2, giro 217)
- **Blocco:** V47 VG3 ∞ — confronta PNG `GRAFICA/` vs raw; area mia allineata.
- **Stato:** `vg3_date_png_raw.py` exit 0: 0 da convertire, 6/6 skyline, 165 PNG/133 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/46 allineati, 1 png piu' nuovo a pari contenuto, doppia estensione 0, 5 pose `_b` area giocatore bloccata; `vg3_raw_senza_catalogo.py` exit 0: 155/155, 0 senza voce, 4 refs noti area auto bloccata, 153 voci/159 riferimenti, 0 duplicati/sospetti; `vg3_png_non_catalogati.py` exit 0: 3 noti non-asset, 0 da decidere.
- **Verifiche (26/11 10:31):** tre tool exit 0; `--autotest` dei tre exit 0 (date 29, raw 30 con +1 `raw_su_disco`, png 24 prove OK); output del tool modificato identico al baseline.
- **Produzione:** `TOOLS/vg3_raw_senza_catalogo.py` (diverso dal turno prima, che fu `vg3_date_png_raw.py`) — la scansione inline in main() (`RAW_ROOT.rglob("*.sglraw")` + `path_raw_relativi`) estratta nella funzione pura `raw_su_disco(radice, raw_root, rglob=None)` con rglob iniettabile (default `raw_root.rglob`), usata in main(), nota in testa, autotest dedicato (3 casi: ordinamento, underfolder, vuoto); primo autotest Exit 1 per aspettativa errata sul prefisso, corretto il test non la logica; output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `TOOLS/vg3_raw_senza_catalogo.py`, `TOOLS/aggiorna_ultimo_turno.py`, `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (26/11 10:31).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; conversioni solo se PNG piu' nuovo, mai area Andrea; prossimo tool da toccare: `vg3_png_non_catalogati.py`.
