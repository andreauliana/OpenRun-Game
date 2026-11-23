#!/usr/bin/env python3
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
contenuto = """# ULTIMO TURNO — @grafico (23/11/2026 07:42, giro 159)
- **Blocco/passo:** V47 VG3 ∞ — confronto PNG `GRAFICA/scenari/` vs `.sglraw`; area mia allineata, nessuna conversione.
- **File cambiati:**
  - `UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py` (autotest `classifica_percorsi` esteso: stringa vuota, path assoluto, dict vuoto)
  - `UFFICIO/bacheche/grafico/diario.md` (riga)
  - `UFFICIO/bacheche/grafico/RIASSUNTO.md` (riscritto)
  - `UFFICIO/out/grafico/ULTIMO_TURNO.md` (riscritto)
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` → exit 1 (solo flag Andrea noto `traffico_incrocio/lato_furgone_bianco.png.png`; 0 da convertire, 6/6 skyline, 166 PNG/134 confrontati/32 non motore)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` → exit 0 (155/155, 0 senza voce, 4 refs noti area auto bloccata, 0 risolti, 0 sospetti)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` → exit 0 (4 noti, 0 catalogati, 0 da decidere)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` → exit 0 (8 prove OK)
- **Gate:** non eseguito (23/11 07:42). Zero risorse esterne, zero Sega.
"""
p = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
p.write_text(contenuto, encoding="utf-8")
print(f"scritto {p}")
