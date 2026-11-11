#!/usr/bin/env python3
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"
dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
for f in dati["frasi"]:
    fid = f["id"]
    l1,l2 = f["linea1"], f["linea2"]
    # look for grammar issues
    bad = []
    if l2 == "COMING BACK, BRO":
        bad.append("coming back")
    if l2 == "WARM UP WITH ME":
        bad.append("warm up")
    if l1 == "A GLASS OF DRANK":
        bad.append("glass of drank")
    if l2 == "SOON YOU SEE":
        bad.append("soon you see")
    if l2 == "ALL GOES SLOW":
        bad.append("all goes")
    if l2 == "THERE IS MORE":
        bad.append("there is more")
    if l1 == "RENT? WORK?":
        bad.append("rent work")
    if l2 == "NO SON AT ALL":
        bad.append("no son")
    if bad:
        print(f"{fid}: {f['personaggio']} |{l1}| |{l2}| <- {bad}")
PY
