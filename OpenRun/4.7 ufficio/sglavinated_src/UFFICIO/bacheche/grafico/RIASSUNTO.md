# RIASSUNTO — @grafico (26/11/2026 17:49, Turno 1, giro 221)
- **Blocco:** V47 VG3 ∞ — controllo date PNG/raw; area mia allineata.
- **Stato:** `vg3_date_png_raw.py` exit 0: 0 da convertire, 6/6 skyline, 165 PNG/133 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/46 allineati, 1 png piu' nuovo a pari contenuto, doppia estensione 0, 5 pose `_b` area giocatore bloccata; `vg3_raw_senza_catalogo.py` exit 0: 155/155, 0 senza voce, 4 refs noti area auto bloccata, 153 voci/159 riferimenti, 0 duplicati/sospetti; `vg3_png_non_catalogati.py` exit 0: 3 noti non-asset, 0 da decidere.
- **Verifiche (26/11 17:49):** tool e `--autotest` dei tre exit 0 (27/30/30 prove OK); output del tool modificato identico al baseline.
- **Produzione:** `TOOLS/vg3_png_non_catalogati.py` (diverso dal turno prima, che fu `vg3_raw_senza_catalogo.py`) — `classifica(fuori, noti_noti=None)` riceve il dizionario dei noti iniettabile (default NOTI_NON_ASSET) invece di leggere il globale, come le funzioni di stampa rese pure nei turni prima; nota in testa, autotest dedicato (noti custom, vuoto, default).
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `TOOLS/vg3_png_non_catalogati.py`, `diario.md`, `RIASSUNTO.md`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (26/11 17:49).
- **Prossimo:** VG3 ∞ — prossimo tool da toccare `vg3_date_png_raw.py`.
