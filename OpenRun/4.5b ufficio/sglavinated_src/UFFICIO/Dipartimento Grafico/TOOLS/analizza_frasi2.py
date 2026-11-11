#!/usr/bin/env python3
import json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
FRASI = ROOT / "motore_nuovo" / "dati" / "frasi.json"
OUT = ROOT / "UFFICIO/Dipartimento Grafico/TOOLS/frasi_out.txt"
d = json.loads(FRASI.read_text(encoding='utf-8'))
with OUT.open('w', encoding='utf-8') as f:
    for frase in d['frasi'][80:]:
        f.write(f"{frase['id']:3d} | {frase['linea1']!r:18s} {frase['linea2']!r:18s} | {frase['it1']!r:18s} {frase['it2']!r:18s} | {frase['personaggio']}\n")
print('done')
