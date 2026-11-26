#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
DEST = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
CONTENUTO = """\
# ULTIMO_TURNO — @grafico (26/11/2026 17:49, Turno 1, giro 221)
- **Blocco/passo:** V47 VG3 ∞ — controllo date PNG/raw + produzione su `vg3_png_non_catalogati.py` (diverso dal turno prima: `vg3_raw_senza_catalogo.py`).
- **File cambiati:** `UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py` (`classifica(fuori, noti_noti=None)`, nota in testa, autotest), `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py`, `UFFICIO/bacheche/grafico/diario.md` (1 riga), `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.
- **Comandi ed exit code (26/11 17:49):**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 noti non-asset, 0 da decidere; output identico al baseline).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0 (27 prove OK, caso `classifica_iniettabile` aggiunto).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0 (0 da convertire, 6/6 skyline, 165 PNG/133 confrontati).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` — exit 0 (30 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155, 0 senza voce, 4 refs noti bloccati).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` — exit 0 (30 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py"` — exit 0 (ULTIMO_TURNO.md riscritto).
- **Produzione:** l'elenco dei residui noti non e' piu' letto dal globale dentro `classifica`, ma passato come `noti_noti` (default `NOTI_NON_ASSET`); autotest con elenco custom, vuoto e default. Nessuna scrittura su GRAFICA.
- **Conversioni/catalogo:** nessuna; catalogo/licenze invariati; zero risorse esterne, zero Sega; auto del giocatore di Andrea non toccata.
- **Gate:** non eseguito in questo turno (26/11 17:49).
- **Prossimo:** VG3 ∞, prossimo tool da toccare `vg3_date_png_raw.py`.
"""

if __name__ == "__main__":
    DEST.write_text(CONTENUTO, encoding="utf-8")
    print(f"OK {DEST}")
