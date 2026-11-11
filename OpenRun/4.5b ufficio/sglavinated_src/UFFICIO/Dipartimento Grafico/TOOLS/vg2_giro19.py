#!/usr/bin/env python3
"""VG2 giro 19 (11/11/2026): 20 frasi EN piu' naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    37: ("ROLL ONE UP,", "WANT A TRY?"),
    38: ("STANDING STILL?", "LET'S ROLL ONE"),
    42: ("BAD LUCK, BRO?", "PUFF AND PASS"),
    44: ("NICE CURVE, HUH?", "RIDE WITH ME"),
    45: ("WANNA ROLL ONE?", "SAY YES, BRO"),
    51: ("ECSTASY WILL", "LIGHT YOUR NIGHT"),
    52: ("ONE PILL NOW,", "DANCE ALL NIGHT"),
    54: ("GRASS IS SAFE,", "JUST PUFF AWAY"),
    55: ("HIGH AT SPEED,", "FEEL LIKE A GOD"),
    64: ("LOST AND STUCK?", "SMOKE FOR HOPE"),
    85: ("BEER AND GRASS", "GO HAND IN HAND"),
    88: ("TAKE ONE PILL,", "IT HELPS A LOT"),
    96: ("NOT HOOKED YET,", "JUST THIS ONCE"),
    97: ("ONE MORE JOINT,", "I PROMISE, BRO"),
    99: ("GO TO FLOOR SIX,", "STUFF IS THERE"),
    100: ("IN THAT BIG", "BLOCK IT'S CHEAP"),
    101: ("TAKE STAIR C,", "KNOCK, NO BELL"),
    104: ("PURPLE AND WEED", "FEEL SO GOOD"),
    121: ("GOT GOOD GRASS,", "TRY IT OUT"),
    125: ("DON'T TRUST HIM,", "HE'LL TRICK YOU"),
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
