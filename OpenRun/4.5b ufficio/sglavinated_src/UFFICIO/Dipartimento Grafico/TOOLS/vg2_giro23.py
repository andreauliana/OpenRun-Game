#!/usr/bin/env python3
"""VG2 giro 23: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    2:  ("QUICK DEAL:", "SIGN AND COME"),
    9:  ("SOUL FEELS HEAVY", "WARM UP WITH ME"),
    14: ("SIGN THE DEAL,", "NO TURNING BACK"),
    22: ("THE UFO SEES YOU", "FLYING TOO HIGH"),
    27: ("HEY YOU, WAKE UP", "OPEN YOUR EYES"),
    28: ("GET UP AND DRIVE", "KEEP IT CLEAN"),
    34: ("ONE SYRINGE", "CAN KILL YOU"),
    45: ("WANNA ROLL ONE?", "JUST SAY YES"),
    46: ("JUST ONE PUFF,", "COME ON, BRO"),
    50: ("GOT STRONGER", "STUFF FOR YOU"),
    59: ("TIME TO KILL?", "STAY WITH GIANNI"),
    66: ("CUT IT OUT NOW,", "DROP THAT STUFF"),
    68: ("FOR GOODNESS'", "SAKE, GET BACK"),
    74: ("FROM WEED TO", "NEEDLES IS FAST"),
    78: ("YOUR MOM WAITS", "FOR YOU AT HOME"),
    92: ("ONE PURPLE SIP", "SLOWS ALL DOWN"),
    106: ("NO SLEEP AT ALL?", "IT CONTROLS YOU"),
    109: ("RAPPERS ON LEAN", "ENDED UP DEAD"),
    119: ("STEP BY STEP,", "YOU GO DOWN"),
    137: ("NO ONE DIES", "FROM ONE JOINT"),
}

def check(s):
    assert len(s) <= 16, f"too long: {s!r} ({len(s)})"
    assert s == s.upper(), f"not upper: {s!r}"
    assert all(ord(c) < 128 for c in s), f"non-ascii: {s!r}"

with open(PATH, "r", encoding="utf-8") as f:
    data = json.load(f)

frasi = {x["id"]: x for x in data["frasi"]}
for fid, (l1, l2) in UPDATES.items():
    check(l1); check(l2)
    frasi[fid]["linea1"] = l1
    frasi[fid]["linea2"] = l2

with open(PATH, "w", encoding="utf-8") as f:
    json.dump(data, f, ensure_ascii=False, indent=2)
    f.write("\n")

print(f"updated {len(UPDATES)} frasi")
