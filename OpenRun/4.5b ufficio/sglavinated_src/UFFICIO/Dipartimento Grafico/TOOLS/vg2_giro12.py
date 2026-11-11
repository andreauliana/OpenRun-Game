#!/usr/bin/env python3
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
FRASI = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    2: ("FAST DEAL NOW", "SIGN AND COME"),
    3: ("SOUL FOR SALE", "TRY IT NOW"),
    8: ("ALL ALONE NOW", "NO WAY BACK"),
    16: ("I'LL GET YOU IN", "JUST DRIVE ON"),
    18: ("COME HERE NOW", "FORGET IT ALL"),
    19: ("OFF THE ROAD", "COME BACK BRO"),
    37: ("ROLL ONE NOW", "WANNA TRY IT"),
    44: ("NICE CURVE NOW", "RIDE WITH ME"),
    45: ("WANNA ROLL ONE", "SAY YES BRO"),
    54: ("GRASS IS SAFE", "SO PUFF AWAY"),
    55: ("HIGH AT SPEED", "FEEL LIKE GOD"),
    64: ("LOST AND STUCK", "SMOKE TO HOPE"),
    85: ("BEER AND GRASS", "GO HAND IN HAND"),
    88: ("TAKE ONE PILL", "IT HELPS NOW"),
    96: ("NOT HOOKED YET", "JUST ONE TIME"),
    99: ("GO TO FLOOR SIX", "ROBA IS THERE"),
    101: ("STAIR C NOW", "KNOCK ONCE"),
    121: ("GOT GOOD GRASS", "TRY IT NOW"),
    134: ("OFF ROAD NOW", "BACK ON TRACK"),
    142: ("ON THE EDGE", "PUFF AND GO"),
}

def main():
    data = json.loads(FRASI.read_text(encoding="utf-8"))
    by_id = {f["id"]: f for f in data["frasi"]}
    for fid, (l1, l2) in UPDATES.items():
        if fid not in by_id:
            raise SystemExit(f"missing id {fid}")
        f = by_id[fid]
        if len(l1) > 16 or len(l2) > 16:
            raise SystemExit(f"id {fid} too long: {l1!r} {l2!r}")
        if l1 != l1.upper() or l2 != l2.upper():
            raise SystemExit(f"id {fid} must be upper")
        if any(ch in l1+l2 for ch in "àèéìòùÀÈÉÌÒÙ"):
            raise SystemExit(f"id {fid} accents")
        f["linea1"] = l1
        f["linea2"] = l2
    FRASI.write_text(json.dumps(data, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"updated {len(UPDATES)} frasi")

if __name__ == "__main__":
    main()
