# ULTIMO_TURNO — @grafico (26/11/2026 09:01, Turno 1, giro 210)

- **Blocco/passo:** V47 VG3 ∞ — controlli PNG/raw + produzione su `vg3_date_png_raw.py` (diverso dal turno prima: `vg3_png_non_catalogati.py`).
- **File cambiati:** `UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py`, `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py`, `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0 (0 da convertire, 6/6 skyline, 165 PNG/133 confrontati, doppia estensione 0).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155, 0 senza voce, 4 refs noti bloccati).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 noti non-asset, 0 da decidere).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` — exit 0 (27 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` — exit 0 (27 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0 (23 prove OK).
- **Produzione:** le quattro diramazioni del confronto oggetti di `scenari/` estratte in `esito_oggetti(stato, motivo, livello, nome, data_png, data_raw)` pura con autotest (4 casi); main() la usa; output invariato.
- **Conversioni/catalogo:** nessuna; catalogo/licenze invariati; zero risorse esterne, zero Sega.
- **Gate:** non eseguito (26/11 09:01).
- **Aree bloccate:** auto giocatore Andrea (5 pose `_b` con raw e senza PNG) non toccata.
