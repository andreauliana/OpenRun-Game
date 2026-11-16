#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico), copertura inversa: ogni .sglraw che il motore legge deve
# comparire nel catalogo (campo "raw" di un elemento), cosi' licenza e provenienza
# restano tracciate. I due controlli esistenti guardano l'altro verso:
#   check_catalogo_grafico.py -> PNG di GRAFICA devono essere catalogati
#   vg3_date_png_raw.py       -> PNG di GRAFICA vs raw del motore
# Qui si parte dai raw e si cerca l'elemento di catalogo.
# Non scrive nulla: solo diagnosi. Exit 0 se ogni raw e' tracciato, 1 altrimenti.
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
CATALOGO = RADICE / "GRAFICA/CATALOGO_GRAFICO.json"
RAW_ROOT = RADICE / "motore_nuovo/dati/sprite_raw"

cat = json.loads(CATALOGO.read_text(encoding="utf-8"))
tracciati = {}
for e in cat["elementi"]:
    r = e.get("raw")
    if isinstance(r, str):
        tracciati[r] = e.get("id", "?")
    for r2 in e.get("raw_elenco") or []:
        tracciati[r2] = e.get("id", "?")

raws = sorted(
    p.relative_to(RADICE).as_posix() for p in RAW_ROOT.rglob("*.sglraw")
)
senza = [r for r in raws if r not in tracciati]

for r in senza:
    print("RAW NON CATALOGATO", r)
print(f"raw del motore: {len(raws)}; tracciati nel catalogo: {len(raws) - len(senza)}; senza voce: {len(senza)}")
print(f"voci di catalogo con 'raw': {len(tracciati)}")
raise SystemExit(1 if senza else 0)
