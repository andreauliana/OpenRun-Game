#!/usr/bin/env python3
"""VG2 giro 17 (11/11/2026): 20 frasi EN piu' naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
Primo passaggio sulle frasi mai toccate nei giri 11-16:
11,23,26,35,53,65,66,89,90,93,98,103,107,119,124,127,130,139,149,152.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    11: ("A FAT NEEDLE?", "JUST ONE, OK?"),
    23: ("MIND THE UFOS,", "BROTHER"),
    26: ("NEVER DRIVE", "WHILE HIGH, BRO"),
    35: ("HEY, WHAT'S UP?", "STUCK AGAIN?"),
    53: ("ONE TAB OF LSD", "AND OFF YOU FLY"),
    65: ("STUCK LIKE ME?", "THEN FOLLOW ME"),
    66: ("CUT IT OUT,", "DROP THAT STUFF"),
    89: ("TAKE ONE PILL,", "IT ALL FADES"),
    90: ("NO PILL, NO", "SLEEP, RIGHT?"),
    93: ("ONE SIP OF LEAN", "FEELS SO GOOD"),
    98: ("RENT DUE, BRO?", "TAKE A PILL"),
    103: ("WORK TOMORROW?", "TAKE ONE PILL"),
    107: ("LEAN SLOWS DOWN", "YOUR BREATHING"),
    119: ("STEP BY STEP,", "GOING DOWN"),
    124: ("LOST THE ROAD?", "I'LL ROLL IT"),
    127: ("BETTER READ", "THE GOOD BOOK"),
    130: ("CALL DON LURIDO,", "HE CAN HELP"),
    139: ("WANT A PILL", "RIGHT NOW, BRO?"),
    149: ("APOCALYPSE:", "DRAGON'S HERE"),
    152: ("GRAVEL AND SMOKE", "A PERFECT PAIR"),
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
