#!/usr/bin/env python3
"""VG2 giro 20 (11/11/2026): 20 frasi EN piu' naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    3: ("SELL YOUR SOUL,", "JUST TRY IT"),
    8: ("ALL ALONE NOW,", "NO TURNING BACK"),
    11: ("A BIG NEEDLE?", "JUST ONCE, OK?"),
    19: ("OFF THE ROAD,", "COME BACK, BRO"),
    48: ("ONE JOINT IS", "ONLY THE START"),
    58: ("JUST ONE JOINT,", "NO BIG DEAL"),
    63: ("TRY IT ONCE AND", "YOU'LL WANT MORE"),
    84: ("LOOK, I MAKE", "ALL THINGS NEW"),
    86: ("COME ON OVER,", "ALL FRIENDS HERE"),
    89: ("TAKE ONE PILL,", "LET IT ALL FADE"),
    91: ("PURPLE DRANK", "MAKES YOU SLOW"),
    103: ("WORK TOMORROW?", "ONE PILL WILL DO"),
    105: ("OFF TRACK, BRO?", "ASK FOR HELP"),
    115: ("CLINIC, DOCTOR,", "A FRIEND: TALK"),
    122: ("IN THE MEADOW,", "JUST LIKE ME"),
    134: ("OFF THE ROAD?", "GET BACK ON"),
    136: ("AT ROAD'S EDGE,", "LIKE YOUR LIFE"),
    142: ("ON THE EDGE?", "PUFF AND RELAX"),
    143: ("IN THE FIELDS,", "I GROW IT ALL"),
    144: ("SWERVING? RELAX", "YOU'RE FLYING"),
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
