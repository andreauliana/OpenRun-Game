#!/usr/bin/env python3
"""VG2 giro 14 (11/11/2026): 20 frasi EN più naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    0: ("HEY BROTHER,", "COME WITH ME"),
    1: ("TINY FIRE,", "WARM YOURSELF"),
    4: ("FEED THE FIRE,", "LOOKS SO GOOD"),
    5: ("SIGN THE DEAL,", "FEEL LIKE A GOD"),
    10: ("SHAKING, HUH?", "COME ON DOWN"),
    15: ("YOUR SOUL IS", "MINE NOW"),
    17: ("COME DOWN, FOOL", "DEAL IS SEALED"),
    25: ("WATCH THE ROAD", "STAY ALERT"),
    27: ("HEY YOU FOOL,", "OPEN YOUR EYES"),
    28: ("GET UP AND DRIVE", "STAY CLEAN"),
    30: ("LIGHT IS HERE,", "YOU LOST THE WAY"),
    31: ("TAKE IT EASY,", "LIFE IS SHORT"),
    41: ("GET GOING NOW", "LIGHT IT UP"),
    43: ("I'LL HELP YOU", "JUST TRUST ME"),
    52: ("TAKE ONE PILL", "DANCE ALL NIGHT"),
    69: ("CLOSE YOUR MOUTH", "OPEN YOUR EYES"),
    70: ("BE QUIET, BRO", "WORLD'S A MESS"),
    72: ("YOU'RE WASTING", "YOUR LIFE, BRO"),
    73: ("ONE LINE CAN", "STOP YOUR HEART"),
    75: ("I AM HERE, GO", "BACK TO THE ROAD"),
}


def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    trovati = 0
    for frase in dati["frasi"]:
        fid = frase["id"]
        if fid in MODIFICHE:
            l1, l2 = MODIFICHE[fid]
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
