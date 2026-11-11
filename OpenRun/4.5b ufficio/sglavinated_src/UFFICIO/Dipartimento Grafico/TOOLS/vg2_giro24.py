#!/usr/bin/env python3
"""VG2 giro 24: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    0:  ("HEY LITTLE BRO,", "RIDE WITH ME"),
    1:  ("A LITTLE FIRE", "TO KEEP YOU WARM"),
    11: ("ONE BIG SHOT?", "JUST ONCE, OK?"),
    13: ("EVERYONE DOES IT", "WHY NOT YOU?"),
    21: ("FEELING SO LOW?", "COME DOWN, BRO"),
    23: ("WATCH THE UFOS,", "BROTHER!"),
    24: ("DRIVE HIGH NOW?", "YOU COULD DIE"),
    26: ("NEVER DRIVE", "WHEN HIGH, BRO"),
    31: ("TAKE IT EASY,", "LIFE IS SO SHORT"),
    33: ("LSD CAN BREAK", "YOUR MIND APART"),
    35: ("HEY, WHAT'S UP?", "STUCK AGAIN, BRO"),
    38: ("STANDING STILL?", "LET'S ROLL ONE"),
    40: ("TELL NO ONE,", "JUST LIGHT IT UP"),
    47: ("WEED HURTS NONE", "TRY IT, BRO"),
    52: ("ONE PILL NOW,", "DANCE TILL DAWN"),
    56: ("NO ONE WILL", "EVER FIND OUT"),
    60: ("GO OUT TONIGHT,", "GET HIGH, BRO"),
    62: ("WHY STOP AT", "ONLY ONE?"),
    64: ("LOST AND STUCK?", "SMOKE FOR HOPE"),
    67: ("GET HIGH, CRASH", "COINCIDENCE?"),
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
