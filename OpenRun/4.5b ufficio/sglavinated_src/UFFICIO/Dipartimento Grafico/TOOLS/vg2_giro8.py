#!/usr/bin/env python3
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# Giro 8 VG2: 20 frasi migliorate, solo EN, it1/it2 intatti.
MODIFICHE = {
    8:   ("YOU ARE ALONE,", "NO ONE AT ALL"),
    9:   ("HEAVY SOUL,",    "BURN WITH ME"),
    19:  ("OFF THE EDGE",   "BACK ON TRACK"),
    21:  ("FEELING LOW,",   "COME ON DOWN"),
    27:  ("OH, YOU FOOL,",  "OPEN YOUR EYES"),
    37:  ("ROLL ONE, HUH",  "ON YOUR OWN?"),
    49:  ("BORED OF GRASS?", "THERE'S MORE"),
    57:  ("FIRST ONE FREE,", "THEN YOU'LL SEE"),
    61:  ("TRY JUST GRASS,", "THEN DECIDE"),
    65:  ("STUCK LIKE ME?", "COME ALONG, BRO"),
    74:  ("GRASS TO NEEDLE", "IS A SHORT STEP"),
    79:  ("ONE JOINT, THEN", "A CORPSE? YOU?"),
    86:  ("COME ON OVER,",  "CREW IS HERE"),
    92:  ("ONE PURPLE SIP", "TIME GOES SLOW"),
    93:  ("SIP OF LEAN",    "FEELS SO GOOD"),
    95:  ("JUST TONIGHT,",  "STOP TOMORROW"),
    96:  ("YOU ARE NOT",    "HOOKED, STOP"),
    98:  ("RENT TO PAY?",   "GRAB A DIERA"),
    101: ("STAIR C, KNOCK,", "RING NO MORE"),
    103: ("WORK TOMORROW?", "ONE DIERA, GO"),
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
PY
