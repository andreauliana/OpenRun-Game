#!/usr/bin/env python3
"""VG2 giro 9: migliora 20 frasi EN in frasi.json (solo linea1/linea2)."""
import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

CAMBI = {
    13: ("ALL DO A LINE,", "WHY NOT YOU?"),
    22: ("THE UFO SEES", "YOU, TOO HIGH?"),
    23: ("WATCH OUT FOR", "UFOS, BROTHER"),
    35: ("HEY, WHAT NOW?", "ARE YOU STUCK?"),
    36: ("HEY, A BREAK?", "WANNA LIGHT UP?"),
    38: ("ALL STOPPED...", "WANT A JOINT?"),
    40: ("CALL NO ONE,", "JUST SMOKE IT"),
    42: ("BAD LUCK, HUH?", "PUFF AND PASS"),
    45: ("HEY, ROLL ONE?", "WHAT DO YOU SAY?"),
    46: ("JUST ONE SMALL", "PUFF, GO ON"),
    47: ("GRASS NEVER", "HURT ANYONE"),
    51: ("ECSTASY OPENS", "UP YOUR NIGHT"),
    53: ("LSD: ONE TAB", "AND YOU FLY"),
    60: ("GO OUT TONIGHT", "GET HIGH, BRO"),
    66: ("DAMN, ENOUGH", "WITH THAT STUFF"),
    67: ("YOU GET HIGH", "THEN CRASH, NO?"),
    71: ("YOU ARE DONE", "FOR, YOU KNOW?"),
    77: ("DON'T LET THEM", "FOOL YOU, BRO"),
    78: ("MOM WAITS FOR", "YOU BACK HOME"),
    80: ("DRUGS TAKE", "EVERYTHING AWAY"),
}

def main():
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    by_id = {f["id"]: f for f in dati["frasi"]}
    for fid, (l1, l2) in CAMBI.items():
        assert len(l1) <= 16 and len(l2) <= 16, (fid, l1, l2, len(l1), len(l2))
        by_id[fid]["linea1"] = l1
        by_id[fid]["linea2"] = l2
    PERCORSO.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"OK: aggiornate {len(CAMBI)} frasi")

if __name__ == "__main__":
    main()
