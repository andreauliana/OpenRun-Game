#!/usr/bin/env python3
"""VG2 giro 18: migliora 20 frasi EN in frasi.json (solo linea1/linea2)."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PERCORSO = ROOT / "motore_nuovo" / "dati" / "frasi.json"

MODIFICHE = {
    13: ("ALL DO IT NOW,", "WHY NOT YOU?"),
    24: ("DRIVE ON DRUGS?", "YOU COULD DIE"),
    34: ("ONE SYRINGE IS", "A DEATH TRAP"),
    40: ("TELL NO ONE,", "JUST LIGHT UP"),
    47: ("WEED NEVER", "HURT ANYONE"),
    56: ("NO ONE WILL", "EVER KNOW"),
    60: ("GO OUT TONIGHT,", "GET HIGH, BRO"),
    62: ("WHY STOP AT", "JUST ONE?"),
    76: ("I AM THE FIRST", "AND THE LAST"),
    77: ("DON'T LET THEM", "TRICK YOU, BRO"),
    80: ("DRUGS TAKE", "ALL YOU HAVE"),
    81: ("ARE YOU THIRSTY?", "I GIVE WATER"),
    82: ("DO NOT FEAR,", "I AM ALIVE"),
    83: ("GET AWAY NOW,", "TEMPTER!"),
    84: ("LOOK, I MAKE", "ALL NEW AGAIN"),
    87: ("DON'T TELL", "ANYONE, OK?"),
    91: ("PURPLE DRANK", "SLOWS YOU DOWN"),
    94: ("RAPPERS DO IT,", "WHY NOT YOU?"),
    95: ("JUST TONIGHT,", "QUIT TOMORROW"),
    102: ("FIRST A PILL,", "THEN HARD DRUGS"),
}


def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    trovate = 0
    for frase in dati["frasi"]:
        fid = frase["id"]
        if fid in MODIFICHE:
            frase["linea1"], frase["linea2"] = MODIFICHE[fid]
            trovate += 1
    if trovate != len(MODIFICHE):
        print(f"ERRORE: modificate {trovate}, attese {len(MODIFICHE)}")
        return 1
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"OK: {trovate} frasi aggiornate (giro 18)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
