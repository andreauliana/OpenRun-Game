#!/usr/bin/env python3
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
FRASI = ROOT / "motore_nuovo" / "dati" / "frasi.json"

UPDATES = {
    13: ("EVERYONE DOES", "WHY NOT YOU?"),
    24: ("DRUGS AND DRIVE", "CAN KILL YOU"),
    33: ("LSD CAN BLOW", "YOUR MIND APART"),
    34: ("A SYRINGE IS", "A DEATH TRAP"),
    56: ("NO ONE WILL", "EVER FIND OUT"),
    62: ("WHY STOP AT", "ONE JOINT?"),
    81: ("THIRSTY? I GIVE", "LIVING WATER"),
    91: ("PURPLE DRANK", "SLOWS YOU DOWN"),
    94: ("RAPPERS DO IT", "SO WHY NOT YOU?"),
    99: ("GO TO FLOOR SIX", "STUFF IS THERE"),
    104: ("PURPLE AND WEED", "FEEL SO GOOD"),
    110: ("ENOUGH ALREADY", "CALL THE CLINIC"),
    118: ("COSTS SO LITTLE", "TAKES IT ALL"),
    125: ("DON'T TRUST HIM", "HE'LL TRICK YOU"),
    128: ("DON'T SHOOT UP", "IN YOUR VEIN!"),
    131: ("LOVE WHO YOU", "WANT, NOT DRUGS"),
    137: ("NO ONE DIES", "FROM A JOINT"),
    140: ("IT'S JUST", "A SMALL JOINT"),
    146: ("GRASS UNDER", "FEELS LIKE HOME"),
    155: ("NO ONE DIES", "FROM ONE JOINT"),
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
        f["linea1"] = l1
        f["linea2"] = l2
    FRASI.write_text(json.dumps(data, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"updated {len(UPDATES)} frasi")

if __name__ == "__main__":
    main()
