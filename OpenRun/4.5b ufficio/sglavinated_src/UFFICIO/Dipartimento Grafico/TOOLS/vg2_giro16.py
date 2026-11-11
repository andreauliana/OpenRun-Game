#!/usr/bin/env python3
"""VG2 giro 16 (11/11/2026): 20 frasi EN piu' naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
Primo passaggio: 9,22,36,38,46,51,67,71,78,92,115.
Secondo passaggio: 1,2,14,50,59,99,101,108,146.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    1: ("A LITTLE FIRE", "TO WARM YOU"),
    2: ("QUICK DEAL: SIGN", "AND COME DOWN"),
    9: ("HEAVY SOUL?", "WARM IT WITH ME"),
    14: ("SIGN AND COME", "DOWN, NO RETURN"),
    22: ("THE UFO SEES YOU", "YOU'RE TOO HIGH"),
    36: ("HEY, A LITTLE", "SMOKE BREAK?"),
    38: ("STANDING STILL?", "WANNA ROLL ONE?"),
    46: ("JUST ONE LIGHT", "PUFF, COME ON"),
    50: ("GOT STRONGER", "STUFF, WANT IT?"),
    51: ("ECSTASY WILL", "LIGHT YOUR NIGHT"),
    59: ("TIME TO KILL?", "HANG WITH GIANNI"),
    67: ("HIGH THEN CRASH,", "COINCIDENCE?"),
    71: ("YOU'RE DONE FOR,", "YOU KNOW IT"),
    78: ("MOM'S WAITING", "FOR YOU AT HOME"),
    92: ("ONE PURPLE SIP", "AND TIME SLOWS"),
    99: ("GO TO FLOOR SIX", "STUFF'S UP THERE"),
    101: ("TAKE STAIR C", "KNOCK, NO BELL"),
    108: ("SYRUP AND BOOZE", "WILL STOP YOU"),
    115: ("CLINIC, DOCTOR", "OR FRIEND: TALK"),
    146: ("WHEELS ON GRASS", "FEELS LIKE HOME"),
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
