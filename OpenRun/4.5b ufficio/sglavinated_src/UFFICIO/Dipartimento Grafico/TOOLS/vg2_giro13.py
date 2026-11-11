#!/usr/bin/env python3
"""VG2 giro 13 (11/11/2026): 20 frasi EN più naturali in frasi.json.
Solo linea1/linea2; it1/it2 invariati. Max 16 caratteri per riga.
"""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# id: (linea1, linea2)
MODIFICHE = {
    6: ("SIGN THE DEAL,", "THEN FLY HIGH"),
    7: ("COME WITH ME,", "DON'T BE A FOOL"),
    12: ("DO YOU MISS ME?", "FEEL THE PULL?"),
    14: ("SIGN AND COME", "NO TURNING BACK"),
    20: ("FIRE WAITS HERE", "STAY WITH ME"),
    33: ("LSD CAN TEAR", "YOUR MIND APART"),
    39: ("NOW YOU WANT", "TWO OF THEM?"),
    48: ("ONE JOINT IS", "JUST THE START"),
    50: ("I HAVE STRONGER", "STUFF IF YOU DO"),
    58: ("JUST ONE JOINT,", "IT'S NO BIG DEAL"),
    59: ("KILL TIME NOW", "HANG WITH GIANNI"),
    63: ("TRY IT ONCE AND", "YOU WANT MORE"),
    100: ("IN THE BIG BLOCK", "IT'S SO CHEAP"),
    113: ("TALK TO SOMEONE", "YOU'RE NOT ALONE"),
    122: ("IN THE MEADOW", "JUST LIKE ME"),
    129: ("DON'T FALL FOR", "HEROIN, PLEASE"),
    132: ("REAL PRIDE IS", "STAYING SOBER"),
    145: ("NICE SWERVE!", "ANOTHER ROUND?"),
    148: ("CALL IT WHAT", "YOU WANT, SMOKE"),
    151: ("ALL IS VANITY,", "MY FRIEND"),
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
