# ULTIMO_TURNO — @grafico (26/11/2026 09:13, Turno 1, giro 211)

- **Blocco/passo:** V47 VG3 ∞ — controlli PNG/raw + produzione su `vg3_png_non_catalogati.py` (diverso dal turno prima: `vg3_date_png_raw.py`).
- **File cambiati:** `UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py`, `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py`, `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0 (0 da convertire, 6/6 skyline, 165 PNG/133 confrontati, doppia estensione 0).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155, 0 senza voce, 4 refs noti bloccati).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 noti non-asset, 0 da decidere).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0 (24 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` — exit 0 (27 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` — exit 0 (27 prove OK).
- **Produzione:** caricamento del catalogo estratto in `carica_catalogo(path, leggi=None)` (lettura iniettabile, default `Path.read_text` chiude il file); main() la usa, nota in testa, autotest dedicato (2 casi); output invariato.
- **Conversioni/catalogo:** nessuna; catalogo/licenze invariati; zero risorse esterne, zero Sega.
- **Gate:** non eseguito (26/11 09:13).
- **Aree bloccate:** auto giocatore Andrea (5 pose `_b` con raw e senza PNG, 4 raw stop/pose mancanti) non toccata.
