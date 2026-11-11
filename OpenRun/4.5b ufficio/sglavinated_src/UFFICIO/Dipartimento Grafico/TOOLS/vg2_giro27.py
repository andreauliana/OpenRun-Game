#!/usr/bin/env python3
"""VG2 giro 27: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    0: ("HEY KID, COME ON", "RIDE WITH ME"),
    1: ("A WARM FIRE HERE", "JUST FOR YOU"),
    3: ("SELL YOUR SOUL,", "GIVE IT A TRY"),
    4: ("FEED THE FIRE,", "FEELS SO GOOD"),
    6: ("SIGN THE DEAL,", "AND FLY AWAY"),
    7: ("COME WITH ME,", "DON'T BE SILLY"),
    8: ("NOW YOU'RE ALONE", "NO WAY BACK"),
    10: ("SHAKING, HUH?", "COME ON DOWN"),
    12: ("DO YOU MISS ME?", "FEEL THE URGE?"),
    14: ("SIGN THE DEAL,", "NO WAY BACK NOW"),
    15: ("YOUR SOUL IS", "MINE RIGHT NOW"),
    16: ("I'LL HANDLE IT,", "JUST KEEP GOING"),
    17: ("COME DOWN, FOOL", "DEAL IS SEALED"),
    20: ("FIRE WAITS HERE", "STAY WITH ME"),
    25: ("WATCH THE ROAD,", "STAY SHARP NOW"),
    34: ("ONE SYRINGE", "CAN END IT ALL"),
    35: ("HEY, WHAT'S UP?", "STUCK AGAIN?"),
    36: ("HEY, WANT A", "SMOKE BREAK NOW?"),
    40: ("TELL NO ONE,", "JUST LIGHT UP"),
    42: ("BAD LUCK, HUH?", "PUFF AND PASS"),
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
