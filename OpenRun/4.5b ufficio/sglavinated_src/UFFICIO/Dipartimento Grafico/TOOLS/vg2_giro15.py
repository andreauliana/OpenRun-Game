#!/usr/bin/env python3
"""VG2 giro 15 (11/11/2026): 20 frasi EN più naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    21: ("FEELING LOW?", "COME DOWN, BRO"),
    29: ("OPEN YOUR EYES,", "DROP THAT JOINT"),
    32: ("WAKE UP, BRO:", "I DON'T SMOKE"),
    49: ("BORED OF GRASS?", "THERE IS MORE"),
    57: ("FIRST IS FREE,", "THEN YOU'LL SEE"),
    61: ("IT'S JUST GRASS,", "THEN DECIDE"),
    68: ("FOR GOD'S SAKE,", "BACK ON THE ROAD"),
    74: ("GRASS TO NEEDLE", "ONE SHORT STEP"),
    79: ("ONE JOINT, THEN", "YOU'RE A CORPSE"),
    84: ("LOOK, I MAKE", "ALL THINGS NEW"),
    86: ("COME ON OVER,", "CREW'S ALL HERE"),
    97: ("ONE MORE JOINT,", "I PROMISE"),
    106: ("NO SLEEP?", "SHE'S IN CHARGE"),
    109: ("RAPPERS ON LEAN", "END UP DEAD"),
    120: ("I WILL DRY", "ALL YOUR TEARS"),
    123: ("ON THE GRASS", "YOU RIDE LIGHT"),
    126: ("I TRIED IT TOO", "NO GOOD AT ALL"),
    138: ("HEY, LET'S GO", "ROLL ONE NOW"),
    141: ("I SEE YOU", "FROM ABOVE"),
    147: ("MARY WON'T", "SAVE YOU, BRO"),
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
