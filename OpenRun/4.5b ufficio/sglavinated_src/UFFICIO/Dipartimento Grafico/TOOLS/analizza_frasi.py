#!/usr/bin/env python3
import json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
FRASI = ROOT / "motore_nuovo" / "dati" / "frasi.json"
d = json.loads(FRASI.read_text(encoding='utf-8'))
for f in d['frasi']:
    print(f"{f['id']:3d} | {f['linea1']!r:18s} {f['linea2']!r:18s} | {f['it1']!r:18s} {f['it2']!r:18s} | {f['personaggio']}")
