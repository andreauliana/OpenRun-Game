# STATUS_GRAFICO_PER_CAPO — @grafico (18/11/2026 16:14)

- **Stato:** allineato (V47 VG3 ∞, giro 94). Nessuna conversione necessaria. VG2 chiuso.
- **Controlli (exit 0):**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — 145 PNG (133 confrontati, 12 non motore, 0 fuori copertura), 86 raw scenari (80 oggetti a byte, 6 skyline a contenuto) + 47 altri gruppi, 0 da convertire, 126 pari contenuto, pose 2/2 allineate, gruppi noti 8.
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — 142 raw del motore, 142 tracciati nel catalogo, 0 senza voce.
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — 0 PNG non catalogati, 1 noto non-asset (`GRAFICA/sprite/giocatore_pose/car-custom-v2.png`, tavola sorgente Andrea, area bloccata).
- **Materiali modificati:** `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, questo status. `UFFICIO/out/grafico/ULTIMO_TURNO.md` non riscrivibile (permesso negato: write/edit).
- **Catalogo/licenze:** nessuna modifica a `CATALOGO_GRAFICO.json`; zero risorse esterne, zero materiale Sega, provenienza invariata.
- **Controlli/esiti:** tutto allineato; zero PNG/raw scritti; gate non eseguito (18/11 16:14).
- **Lacune/blocchi:** pose auto mancanti/bloccate restano materia di Andrea; nessun PNG nuovo di Andrea rispetto ai raw.
- **Review richiesta:** nessuna. Prossimo: VG3 ∞.