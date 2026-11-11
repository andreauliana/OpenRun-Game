import json
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

# giro 10: 20 frasi migliorate (solo linea1/linea2 EN)
UPDATES = {
    3:  ("YOUR SOUL IS", "FREE TO TRY"),
    11: ("BIG NEEDLE?", "JUST ONCE, C'MON"),
    19: ("OFF THE ROAD", "BACK AGAIN, BRO"),
    26: ("DON'T GET HIGH", "WHEN YOU DRIVE"),
    64: ("STUCK AND LOST", "SMOKE AND HOPE"),
    65: ("STUCK LIKE ME?", "COME WITH ME"),
    88: ("TAKE A PILL", "IT FIXES YOU"),
    89: ("ONE PILL AND GO", "IT ALL PASSES"),
    90: ("NO PILL MEANS", "NO SLEEP, RIGHT?"),
    98: ("RENT TO PAY?", "TAKE A PILL"),
    103:("WORK TOMORROW?", "ONE PILL, GO ON"),
    121:("GOT SOME GRASS?", "MINE IS BETTER"),
    124:("LOST THE ROAD?", "I'LL FIX IT"),
    134:("OFF ROAD? ONE", "HIT, BACK ON"),
    135:("STUCK IN SAND?", "YOU NEED GIANNI"),
    136:("ROAD'S EDGE NOW", "LIKE YOUR LIFE"),
    139:("WANT A PILL", "RIGHT NOW?"),
    142:("EDGE? RELAX", "TAKE A PILL"),
    152:("GRAVEL AND SMOKE", "GO WELL TOGETHER"),
    153:("BRAKES? NO NEED", "JUST LIGHT UP"),
}

def main():
    data = json.loads(PERCORSO.read_text(encoding="utf-8"))
    for frase in data["frasi"]:
        fid = frase["id"]
        if fid in UPDATES:
            l1, l2 = UPDATES[fid]
            assert len(l1) <= 16 and len(l2) <= 16, (fid, l1, l2)
            frase["linea1"] = l1
            frase["linea2"] = l2
    PERCORSO.write_text(json.dumps(data, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"updated {len(UPDATES)} frasi")

if __name__ == "__main__":
    main()
