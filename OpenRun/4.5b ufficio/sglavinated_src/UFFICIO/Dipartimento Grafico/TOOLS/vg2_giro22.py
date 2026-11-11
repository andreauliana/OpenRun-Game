#!/usr/bin/env python3
"""VG2 giro 22: migliora 20 frasi EN (linea1/linea2) in frasi.json."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PATH = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    5:  ("MAKE THE DEAL,", "FEEL SO HIGH"),
    15: ("YOUR SOUL NOW", "BELONGS TO ME"),
    17: ("COME DOWN, FOOL", "THE DEAL IS DONE"),
    29: ("OPEN YOUR EYES,", "DROP THAT SMOKE"),
    32: ("WAKE UP, BRO:", "I STAY CLEAN"),
    36: ("HEY, WANT A", "SMOKE BREAK?"),
    49: ("BORED OF WEED?", "THERE IS MORE"),
    51: ("ECSTASY MAKES", "NIGHTS SHINE"),
    53: ("ONE TAB OF LSD", "AND YOU WILL FLY"),
    55: ("HIGH AT SPEED,", "FEEL UNSTOPPABLE"),
    63: ("TRY IT ONCE AND", "YOU WANT MORE"),
    65: ("STUCK LIKE ME?", "THEN COME ALONG"),
    79: ("ONE JOINT, THEN", "TROUBLE AWAITS"),
    88: ("TAKE ONE PILL,", "IT EASES IT ALL"),
    102: ("FIRST A PILL,", "THEN HARD DRUGS"),
    108: ("SYRUP AND BOOZE", "CAN STOP BREATH"),
    118: ("COSTS SO LITTLE", "TAKES EVERYTHING"),
    126: ("I TRIED IT TOO,", "IT FELT SO WRONG"),
    148: ("CALL IT WEED OR", "STILL JUST SMOKE"),
    155: ("ONE JOINT ALONE", "CAN RUIN YOU"),
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
