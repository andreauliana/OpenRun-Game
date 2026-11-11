#!/usr/bin/env python3
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# Giro 7 VG2: 20 frasi + 6 fix lunghezza (18,55,64,134,136,143), solo EN, it1/it2 intatti.
MODIFICHE = {
    18:  ("COME HERE, STAY", "FORGET THE SUN"),
    24:  ("DRUGS AND DRIVE", "MEAN DEATH, BRO"),
    28:  ("GET UP AND DRIVE", "STAY CLEAN, BRO"),
    41:  ("GET UP AND GO",   "LIGHT IT UP NOW"),
    52:  ("ONE PILL AND",    "DANCE ALL NIGHT"),
    55:  ("HIGH AT FULL",    "SPEED, LIKE GOD"),
    64:  ("STUCK, SMOKING",  "NAILS AND HOPES"),
    66:  ("DAMN, ENOUGH",    "OF THAT STUFF"),
    68:  ("DAMN IT, GET",    "BACK ON THE ROAD"),
    69:  ("SHUT YOUR MOUTH", "OPEN YOUR EYES"),
    72:  ("YOU ARE RUINING", "YOUR LIFE, BRO"),
    73:  ("ONE LINE AND",    "YOUR HEART STOPS"),
    75:  ("I AM HERE, GET",  "BACK ON THE ROAD"),
    87:  ("TELL NO ONE,",    "JUST DO IT"),
    96:  ("YOU ARE NOT",     "HOOKED, GO ON"),
    102: ("FIRST THE PILL",  "THEN HARD STUFF"),
    103: ("WORK TOMORROW?",  "ONE PILL, GO"),
    111: ("I CAN'T SAVE YOU", "SAVE YOURSELF"),
    112: ("STUFF USES YOU,", "YOU DON'T USE IT"),
    116: ("ON THE SIXTH",    "NO ONE SAVES YOU"),
    117: ("WHO SELLS TO YOU", "WON'T HELP YOU"),
    126: ("I TRIED IT TOO",  "IT DID NO GOOD"),
    134: ("OFF ROAD? A HIT", "AND YOU'RE BACK"),
    136: ("ROAD EDGE LIKE",  "YOUR OWN LIFE"),
    138: ("HEY, LET'S GO",   "GET ONE NOW"),
    143: ("IN THE FIELDS",   "I GROW IT ALL"),
}

def main():
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    by_id = {f["id"]: f for f in dati["frasi"]}
    for fid, (l1, l2) in MODIFICHE.items():
        assert len(l1) <= 16 and len(l2) <= 16, (fid, l1, l2)
        by_id[fid]["linea1"] = l1
        by_id[fid]["linea2"] = l2
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"OK: aggiornate {len(MODIFICHE)} frasi: {sorted(MODIFICHE)}")

if __name__ == "__main__":
    main()
