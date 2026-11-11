#!/usr/bin/env python3
"""VG2 giro 5: migliora 20 frasi EN (solo linea1/linea2)."""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

MODIFICHE = {
    10: ("TREMBLING, HUH?", "COME ON DOWN"),
    13: ("EVERYONE DOES", "A LINE, WHY NOT?"),
    21: ("FEEL LIKE A", "LOSER? COME ON"),
    33: ("LSD WILL BLOW", "YOUR MIND UP"),
    39: ("NOW YOU WANT", "TWO, DON'T YOU?"),
    50: ("I HAVE STRONGER", "STUFF FOR YOU"),
    52: ("ONE PILL AND", "YOU DANCE ALL"),
    56: ("NO ONE WILL", "EVER KNOW IT"),
    58: ("JUST ONE JOINT,", "NO BIG DEAL"),
    59: ("KILL SOME TIME", "WITH GIANNI"),
    62: ("WHY STOP AT", "JUST A JOINT?"),
    63: ("TRY ONCE AND", "YOU WON'T STOP"),
    67: ("YOU GET HIGH", "AND CRASH, NO?"),
    70: ("QUIET, BROTHER,", "WORLD'S A MESS"),
    79: ("ONE JOINT, THEN", "A CORPSE: YOU?"),
    90: ("NO DIERA MEANS", "NO SLEEP, HUH?"),
    127: ("BETTER READ", "THE WORD, BRO"),
    132: ("REAL PRIDE IS", "STAYING CLEAR"),
    148: ("ANY WEED NAME:", "IT'S JUST SMOKE"),
    151: ("ALL IS VANITY,", "DEAR FRIEND"),
}

def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    aggiornate = 0
    for frase in dati["frasi"]:
        fid = frase["id"]
        if fid in MODIFICHE:
            l1, l2 = MODIFICHE[fid]
            if frase["linea1"] != l1 or frase["linea2"] != l2:
                frase["linea1"] = l1
                frase["linea2"] = l2
                aggiornate += 1
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"aggiornate: {aggiornate}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
