#!/usr/bin/env python3
"""VG2 giro 21 (11/11/2026): 20 frasi EN piu' naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    4: ("STOKE THE FIRE,", "FEELS SO GOOD"),
    6: ("SIGN THE DEAL,", "THEN FLY HIGH"),
    7: ("COME WITH ME,", "DON'T BE A FOOL"),
    10: ("SHAKING, HUH?", "COME ON DOWN"),
    12: ("DO YOU MISS ME?", "FEEL THE PULL?"),
    16: ("I'LL HANDLE IT,", "JUST DRIVE ON"),
    18: ("COME HERE NOW,", "FORGET IT ALL"),
    20: ("FIRE AWAITS YOU", "STAY WITH ME"),
    25: ("WATCH THE ROAD,", "STAY ALERT NOW"),
    30: ("LIGHT IS HERE,", "LOST YOUR WAY"),
    31: ("TAKE IT EASY,", "LIFE IS SHORT"),
    33: ("LSD CAN TEAR", "YOUR MIND APART"),
    39: ("WANT TWO NOW?", "WHY STOP AT ONE?"),
    41: ("GET GOING NOW,", "LIGHT IT UP"),
    43: ("I'LL HELP YOU,", "JUST TRUST ME"),
    57: ("FIRST ONE'S FREE", "THEN YOU'LL SEE"),
    61: ("IT'S JUST GRASS,", "THEN DECIDE"),
    70: ("BE QUIET, BRO,", "WORLD'S A MESS"),
    72: ("YOU'RE WASTING", "YOUR LIFE, BRO"),
    75: ("I AM HERE NOW,", "BACK TO THE ROAD"),
}


def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    trovati = 0
    for frase in dati["frasi"]:
        fid = frase["id"]
        if fid in MODIFICHE:
            l1, l2 = MODIFICHE[fid]
            assert len(l1) <= 16 and len(l2) <= 16, (fid, l1, len(l1), l2, len(l2))
            frase["linea1"] = l1
            frase["linea2"] = l2
            trovati += 1
    if trovati != len(MODIFICHE):
        print(f"ERRORE: attesi {len(MODIFICHE)} id, trovati {trovati}")
        return 1
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"OK: aggiornate {trovati} frasi")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
