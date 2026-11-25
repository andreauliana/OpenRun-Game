#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
DEST = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
CONTENUTO = """\
# ULTIMO_TURNO — @grafico (25/11/2026 11:53, Turno 2, giro 200)

- **Blocco/passo:** V47 VG3 ∞ — controlli PNG/raw + produzione su `vg3_png_non_catalogati.py` (diverso dal turno prima: `vg3_raw_senza_catalogo.py`).
- **File cambiati:** `UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py`, `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
- **Comandi ed exit code:**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 1 (solo flag Andrea noto `lato_furgone_bianco.png.png`, 0 da convertire, 6/6 skyline, 80 oggetti senza raw 0).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155 in catalogo, 0 senza voce, 4 refs noti area auto bloccata).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0 (21 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (4 noti, 0 catalogati, 0 da decidere), output invariato.
- **Produzione:** estratta `file_usati_voce()` pura con autotest; `file_usati()` la usa.
- **Conversioni/catalogo:** nessuna; catalogo/licenze invariati; zero risorse esterne, zero Sega.
- **Gate:** non eseguito (25/11 11:53).
- **Aree bloccate:** auto giocatore Andrea (5 pose `_b` con raw e senza PNG) non toccata; flag `lato_furgone_bianco.png.png` resta per decisione Andrea.
"""

if __name__ == "__main__":
    DEST.write_text(CONTENUTO, encoding="utf-8")
    print(f"OK {DEST}")
