from pathlib import Path

root = Path(__file__).resolve().parents[3]
path = root / "UFFICIO" / "out" / "grafico" / "ULTIMO_TURNO.md"
content = """# ULTIMO_TURNO — @grafico (19/11/2026 11:19)
- **Blocco/passo:** V47 VG3 ∞ (giro 108) — confronto date/contenuto PNG `GRAFICA/scenari/` e gruppi vs `.sglraw`; conversione se più nuovi (script catalogo).
- **File cambiati:** `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0 (160 PNG: 133 confrontati, 27 non motore; scenari 86 raw, skyline 6 allineati, 0 da rigenerare; altri gruppi 47, 0 da convertire, contenuto diverso 0; giocatore 10 PNG, mancanti 5 pose `_b`, confronto saltato, area bloccata).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (raw motore 155, tracciati catalogo 155, senza voce 0).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 non catalogati in `GRAFICA/sprite/giocatore_pose/`: `car-custom.png`, `car-custom-dx.png`, `car-custom-sx.png`, area Andrea).
- **Esito:** tutto allineato, nessuna conversione necessaria.
- **Gate:** non eseguito (19/11 11:19).
- **Licenze/Sega:** zero risorse esterne, zero materiale Sega.
- **Prossimo:** VG3 ∞ — ricontrollare al prossimo turno.
"""
path.write_text(content, encoding="utf-8")
print(path)
