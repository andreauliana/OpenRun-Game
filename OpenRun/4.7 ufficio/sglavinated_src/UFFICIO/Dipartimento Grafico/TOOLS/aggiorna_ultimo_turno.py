#!/usr/bin/env python3
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
contenuto = """# ULTIMO TURNO — @grafico (23/11/2026 07:10, giro 157)
- **Blocco/passo:** V47 VG3 ∞ — confronto PNG `GRAFICA/scenari/` vs raw, nessuna conversione.
- **File cambiati:**
  - `UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py` (aggiunto contatore `altri_doppia` nel riepilogo orfani)
  - `UFFICIO/bacheche/grafico/diario.md` (riga)
  - `UFFICIO/bacheche/grafico/RIASSUNTO.md` (riscritto)
  - `UFFICIO/out/grafico/ULTIMO_TURNO.md` (riscritto)
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` → exit 1 (solo flag Andrea noto `traffico_incrocio/lato_furgone_bianco.png.png`, doppia estensione; 0 da convertire, 6/6 skyline)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` → exit 0 (155/155, 0 senza voce, 4 refs noti area auto bloccata)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` → exit 0 (4 noti, 0 da decidere)
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` → exit 0 (9 prove OK)
- **Gate:** non eseguito (23/11 07:10). Zero risorse esterne, zero Sega.
"""
p = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
p.write_text(contenuto, encoding="utf-8")
print(f"scritto {p}")
