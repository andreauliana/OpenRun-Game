#!/usr/bin/env python3
"""VG2 giro 6: sistema 20 frasi EN in frasi.json (solo linea1/linea2)."""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

CAMBI = {
    4: ("BLOW ON THE FIRE", "LOOKS SO NICE"),
    12: ("DO YOU MISS ME?", "CAN YOU FEEL IT?"),
    19: ("OFF THE EDGE NOW", "COMING BACK, BRO"),
    37: ("ROLLING ONE, HUH", "ALL ALONE?"),
    43: ("I WILL HELP YOU", "JUST TRUST ME"),
    44: ("NICE CURVE HERE", "SMOOTH LIKE YOU"),
    54: ("GRASS BELOW YOU", "DEFEND YOURSELF"),
    55: ("HIGH AT TOP SPEED", "FEELS LIKE GOD"),
    64: ("STUCK AND SMOKING", "NAILS AND HOPES"),
    65: ("STUCK LIKE ME?", "JOIN IN, BRO"),
    85: ("A BEER AND THEN", "A SPLIFF?"),
    92: ("ONE PURPLE SIP", "ALL GOES SLOW"),
    104: ("PURPLE AND GRASS", "SO GOOD, BRO"),
    134: ("OFF ROAD? ONE HIT", "AND YOU'RE BACK"),
    136: ("ROAD EDGE IS LIKE", "YOUR OWN LIFE"),
    143: ("IN THE FIELDS, BRO", "I GROW IT ALL"),
    144: ("SWERVING? RELAX", "YOU'RE FLYING"),
    152: ("GRAVEL AND SMOKE", "PERFECT MATCH"),
    153: ("BRAKES? NO NEED", "LIGHT UP AND GO"),
    18: ("COME HERE WITH ME", "FORGET THE SUN"),
}

def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    trovate = 0
    for frase in dati["frasi"]:
        fid = frase["id"]
        if fid in CAMBI:
            l1, l2 = CAMBI[fid]
            frase["linea1"] = l1
            frase["linea2"] = l2
            trovate += 1
    if trovate != len(CAMBI):
        print(f"ERRORE: aggiornate {trovate} su {len(CAMBI)}")
        return 1
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"OK: aggiornate {trovate} frasi")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())