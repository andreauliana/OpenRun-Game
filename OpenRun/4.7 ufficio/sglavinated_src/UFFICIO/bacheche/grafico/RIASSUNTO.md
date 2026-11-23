# RIASSUNTO — @grafico (23/11/2026 11:03, Turno 3, giro 170)
- **Blocco:** V47 VG3 ∞ — confronta PNG `GRAFICA/scenari/` vs raw; area mia allineata.
- **Stato:** tre controlli VG3 eseguiti: 0 da convertire, 6/6 skyline allineati, 166 PNG/134 confrontati/32 non motore, 80 oggetti senza raw 0, altri 47 raw/45 allineati/0 da convertire, 153 voci/159 riferimenti, 0 duplicati/ripetuti/scritture diverse/sospetti. Unico flag Andrea noto `traffico_incrocio/lato_furgone_bianco.png.png` (doppia estensione, ridisegno 32.3% pixel diversi dal raw, nome corretto allineato): rinomina/converti decide Andrea.
- **Verifiche (23/11 11:03):** `vg3_date_png_raw.py` exit 1 (solo flag Andrea); `vg3_raw_senza_catalogo.py` exit 0 (155/155, 0 senza voce, 4 refs noti area auto bloccata, 0 risolti, 0 duplicati/ripetuti/scritture diverse/sospetti); `vg3_png_non_catalogati.py` exit 0 (4 noti, 0 catalogati, 0 da decidere).
- **Produzione:** `TOOLS/vg3_raw_senza_catalogo.py` (file diverso dal turno prima, che fu `vg3_png_non_catalogati.py`) — conteggio inline delle voci con raw estratto in funzione pura `conta_voci_con_raw(elementi)`: voce con solo `raw_elenco` conta, `raw` vuoto/non-stringa e `raw_elenco` vuoto ignorati; usata in `analizza()`, nota in testa e autotest dedicato (3 casi). `--autotest` exit 0 (12 prove OK); tool exit 0, output invariato.
- **Catalogo/licenze:** invariati; zero risorse esterne, zero materiale Sega.
- **File turno:** `diario.md`, `RIASSUNTO.md`, `TOOLS/vg3_raw_senza_catalogo.py`, `ULTIMO_TURNO.md`.
- **Gate:** non eseguito (23/11 11:03).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; conversioni solo se un PNG e' piu' nuovo, mai area Andrea; prossimo tool da toccare: `vg3_date_png_raw.py`.
