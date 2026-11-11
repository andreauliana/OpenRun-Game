#!/usr/bin/env python3
"""VG2 giro 26: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    33: ("LSD CAN SHATTER", "YOUR WHOLE MIND"),
    54: ("WEED FEELS SAFE", "JUST KEEP TOKING"),
    73: ("ONE LINE CAN", "STOP YOUR HEART"),
    74: ("WEED TO NEEDLES", "HAPPENS SO FAST"),
    104: ("LEAN AND WEED", "FEEL SO GOOD"),
    106: ("CAN'T SLEEP NOW?", "IT OWNS YOU NOW"),
    107: ("LEAN SLOWS DOWN", "YOUR BREATHING"),
    108: ("SYRUP AND BOOZE", "CAN KILL YOU"),
    109: ("LEAN TOOK SOME", "RAPPERS' LIVES"),
    111: ("I CAN'T SAVE YOU", "ONLY YOU CAN"),
    112: ("DRUGS USE YOU,", "YOU DON'T USE IT"),
    120: ("I'LL WIPE AWAY", "ALL YOUR TEARS"),
    121: ("GOT GOOD WEED,", "WANT A TRY?"),
    125: ("DON'T TRUST HIM", "HE WILL TRICK U"),
    127: ("BETTER TO READ", "THE GOOD BOOK"),
    128: ("DON'T SHOOT UP", "IN YOUR VEINS!"),
    129: ("DON'T FALL FOR", "HEROIN, PLEASE"),
    130: ("CALL DON LURIDO", "HE CAN HELP YOU"),
    131: ("LOVE WHO YOU", "WANT, NOT DRUGS"),
    132: ("REAL PRIDE IS", "STAYING SOBER"),
}


def check(s):
    assert len(s) <= 16, f"too long: {s!r} ({len(s)})"
    assert s == s.upper(), f"not upper: {s!r}"
    assert all(ord(c) < 128 for c in s), f"non-ascii: {s!r}"


with open(PATH, "r", encoding="utf-8") as f:
    data = json.load(f)

frasi = {x["id"]: x for x in data["frasi"]}
for fid, (l1, l2) in UPDATES.items():
    check(l1)
    check(l2)
    frasi[fid]["linea1"] = l1
    frasi[fid]["linea2"] = l2

with open(PATH, "w", encoding="utf-8") as f:
    json.dump(data, f, ensure_ascii=False, indent=2)
    f.write("\n")

print(f"updated {len(UPDATES)} frasi")
