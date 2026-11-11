#!/usr/bin/env python3
"""VG2 giro 25: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    57: ("FIRST ONE'S FREE", "THEN YOU'LL PAY"),
    61: ("IT'S JUST GRASS,", "WHAT COULD HURT?"),
    69: ("SHUT YOUR MOUTH,", "OPEN YOUR EYES"),
    71: ("YOU'RE DONE FOR,", "YOU KNOW IT NOW"),
    77: ("DON'T LET THEM", "FOOL YOU, BRO"),
    81: ("FEELING THIRSTY?", "DRINK FROM ME"),
    86: ("COME ON OVER,", "FRIENDS ARE HERE"),
    90: ("NO PILL MEANS", "NO SLEEP, HUH?"),
    93: ("ONE SIP OF LEAN", "FEELS SO RIGHT"),
    95: ("JUST TONIGHT,", "QUIT TOMORROW"),
    98: ("RENT IS DUE?", "TAKE A PILL"),
    99: ("GO TO FLOOR SIX,", "STUFF WAITS YOU"),
    100: ("IN THAT BLOCK", "IT'S REAL CHEAP"),
    105: ("LOST YOUR WAY?", "ASK FOR HELP"),
    114: ("ASKING FOR HELP", "TAKES COURAGE"),
    116: ("ON FLOOR SIX", "NO ONE SAVES YOU"),
    117: ("WHO SELLS TO YOU", "WON'T SAVE YOU"),
    119: ("STEP BY STEP,", "YOU FALL DOWN"),
    123: ("ON THE GRASS", "YOU FEEL LIGHT"),
    136: ("AT ROAD'S EDGE,", "LIKE YOUR LIFE"),
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
