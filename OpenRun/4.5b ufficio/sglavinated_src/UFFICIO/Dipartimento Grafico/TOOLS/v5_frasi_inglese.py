#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""V47 @grafico V5 frasi: traduce le 156 frasi di motore_nuovo/dati/frasi.json
in inglese semplice (BLOCCO V47, passo "@grafico V5 frasi").

Regole applicate (frasi.json regole_testo_nuovo + ANDREA_REVIEW 5):
  - maiuscole, max 2 righe, riga <= 16 caratteri, solo ASCII (no accenti);
  - id, personaggio e tabelle invariati (invariante personaggio<->id della
    verifica del motore: TestiFrasi::verifica);
  - nessun contenuto sessuale, nessun insulto a gruppi.

Uso: python3 "UFFICIO/Dipartimento Grafico/TOOLS/v5_frasi_inglese.py"
Output atteso: exit 0, riepilogo a schermo. Il file dati viene riscritto in
modo idempotente (rieseguendo, i testi restano quelli inglesi).
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]  # sglavinated_src
SRC = ROOT / "motore_nuovo" / "dati" / "frasi.json"

MAX_RIGA = 16

# id -> (linea1, linea2) in inglese semplice, toni originali invariati:
# gianni = tentatore scorbutico, gesu = predicatore duro ma premuroso.
TRAD = {
    0: ("HEY LITTLE BRO,", "COME ON DOWN"),
    1: ("FIRE, C'MON HUH,", "WARM IT UP"),
    2: ("QUICK DEAL, SIGN", "AND COME DOWN"),
    3: ("YOUR SOUL, FREE", "JUST TRY IT"),
    4: ("BLOW THE FIRE,", "SEE HOW NICE"),
    5: ("SIGN AND DESCEND", "AND YOU ARE GOD"),
    6: ("SIGN THE DEAL,", "THEN FLY AWAY"),
    7: ("COME DOWN WITH", "ME, YOU ARE MAD"),
    8: ("YOU ARE SO ALONE", "NO KID, NO ONE"),
    9: ("HEAVY SOUL, HUH,", "WARM IT WITH ME"),
    10: ("YOU SHIVER, HUH,", "COME DOWN, FOOL"),
    11: ("BIG NEEDLE, HMM?", "JUST ONE, C'MON"),
    12: ("DO YOU MISS ME?", "CAN YOU FEEL IT"),
    13: ("EVERYONE DOES", "ONE LINE"),
    14: ("SIGN, COME DOWN", "NO GOING BACK"),
    15: ("SOUL IS MINE NOW", "I TOOK IT"),
    16: ("I'LL HANDLE IT", "YOU DRIVE, PUFF"),
    17: ("COME DOWN, FOOL,", "THE DEAL IS DONE"),
    18: ("COME WITH ME NOW", "FORGET THE SUN"),
    19: ("OFF THE EDGE,", "COMING BACK, BRO"),
    20: ("FIRE AWAITS YOU", "STAY HERE, C'MON"),
    21: ("FEEL A LOSER?", "COME ON DOWN"),
    22: ("THE UFO SEES YOU", "ARE YOU HIGH?"),
    23: ("WATCH OUT FOR", "THE UFO, BRO"),
    24: ("DRUGS + WHEEL:", "CERTAIN DEATH"),
    25: ("WATCH THE ROAD,", "C'MON"),
    26: ("NO TRIP WHILE", "YOU DRIVE"),
    27: ("OH, YOU IDIOT,", "LOOK, MAN"),
    28: ("GET UP, DRIVE,", "DON'T DO IT, BRO"),
    29: ("OPEN YOUR EYES,", "DROP THE JOINT"),
    30: ("THE LIGHT: HERE", "YOU ARE ON TRACK"),
    31: ("SLOW DOWN, IDIOT", "LIFE IS ONLY ONE"),
    32: ("WAKE UP, MAN:", "I DO NOT SMOKE"),
    33: ("LSD WILL BREAK", "YOUR HEAD APART"),
    34: ("ONE NEEDLE IS", "AN OPEN COFFIN"),
    35: ("HEY, WHAT'S UP?", "ARE YOU STUCK?"),
    36: ("HEY, A BREAK?", "LIGHT ONE UP?"),
    37: ("YOU ARE DOING", "ONE, RIGHT?"),
    38: ("ALL STOP...", "ROLL THE PAPER?"),
    39: ("ACTUALLY NOW", "YOU WANT TWO"),
    40: ("DON'T CALL", "ANYONE, SMOKE"),
    41: ("START AGAIN", "AND LIGHT UP"),
    42: ("WHAT BAD LUCK!", "PUFF AND PASS"),
    43: ("I'LL HELP YOU", "TRUST ME"),
    44: ("WHAT A NICE TURN", "LIKE THE JOINT"),
    45: ("HEY, HOW ABOUT", "A SPLIFF?"),
    46: ("C'MON, JUST", "A LIGHT PUFF"),
    47: ("GRASS NEVER", "HURTS ANYONE"),
    48: ("THE JOINT IS", "JUST THE START"),
    49: ("BORED OF POT?", "THERE IS MORE"),
    50: ("I HAVE STRONG", "STUFF, WANT IT"),
    51: ("ECSTASY UNLOCKS", "YOUR NIGHT"),
    52: ("ONE PILL AND", "YOU DANCE HOURS"),
    53: ("LSD: ONE SHEET", "AND YOU FLY OFF"),
    54: ("POT UNDERNEATH,", "PLAY DEFENSE"),
    55: ("AT 250, SMOKED,", "YOU FEEL A GOD"),
    56: ("NOBODY WILL", "EVER KNOW"),
    57: ("THE FIRST ONE", "IS FREE, YOU SEE"),
    58: ("JUST ONE SPLIFF", "NOTHING AT ALL"),
    59: ("PASS THE TIME", "WITH GIANNI"),
    60: ("GO OUT TONIGHT", "AND GET HIGH"),
    61: ("IT IS JUST POT", "YOU WILL SEE"),
    62: ("WHY STOP AT", "ONE JOINT?"),
    63: ("WHO TRIES ONCE", "NEVER STOPS"),
    64: ("LIE AND SMOKE,", "NAILS AND HOPES"),
    65: ("OUTSIDE TOO?", "THEN YOU ARE IN"),
    66: ("DAMN, ENOUGH", "WITH THIS STUFF"),
    67: ("YOU GET HIGH,", "SWERVE: LUCK?"),
    68: ("DAMN IT ALL,", "GET BACK ON ROAD"),
    69: ("SHUT YOUR MOUTH", "AND OPEN EYES"),
    70: ("BE QUIET, MAN", "WORLD IS WRONG"),
    71: ("YOU ARE FINISHED", "YOU KNOW, HUH?"),
    72: ("YOU ARE WASTING", "YOUR LIFE, MAN"),
    73: ("ONE LINE AND THE", "HEART STOPS"),
    74: ("POT TO NEEDLE", "IS A SHORT STEP"),
    75: ("I AM HERE: GET", "BACK ON THE ROAD"),
    76: ("I AM THE ALPHA", "AND THE OMEGA"),
    77: ("DO NOT LET", "HIM FOOL YOU"),
    78: ("YOUR MOTHER IS", "WAITING AT HOME"),
    79: ("ONE JOINT, THEN", "ONE DEAD: YOU?"),
    80: ("DRUGS TAKE", "EVERYTHING"),
    81: ("TO THE THIRSTY", "I GIVE WATER"),
    82: ("DO NOT FEAR,", "I AM THE LIVING"),
    83: ("GET BEHIND ME,", "TEMPTER!"),
    84: ("SEE, I MAKE", "ALL THINGS NEW"),
    85: ("A BEER AND", "A SPLIFF?"),
    86: ("COME, ALL YOUR", "FRIENDS ARE HERE"),
    87: ("DON'T TELL", "ANYONE, C'MON"),
    88: ("HAVE A SPLIFF,", "IT WILL FIX YOU"),
    89: ("ONE SPLIFF AND", "ALL PASSES"),
    90: ("NO SPLIFF AND", "NO SLEEP, HUH?"),
    91: ("PURPLE DRANK:", "SLIDES DOWN SLOW"),
    92: ("A PURPLE SIP", "SLOWS ALL DOWN"),
    93: ("A GLASS OF", "DRANK IS COOL"),
    94: ("THE RAPPERS", "DO IT, C'MON"),
    95: ("ONLY TONIGHT,", "QUIT TOMORROW"),
    96: ("YOU ARE NOT", "ADDICTED, C'MON"),
    97: ("ONE MORE AND", "DONE, PROMISED"),
    98: ("RENT AND WORK?", "A SPLIFF: BYE"),
    99: ("GO UP FLOOR SIX", "THERE IS STUFF"),
    100: ("IN THE BIG BLOCK", "IT COSTS LITTLE"),
    101: ("STAIR C, DON'T", "RING, KNOCK"),
    102: ("FIRST THE SPLIFF", "THEN HEAVY STUFF"),
    103: ("WORK TOMORROW?", "ONE SPLIFF, GO"),
    104: ("PURPLE AND POT:", "GREAT, BRO"),
    105: ("YOU ARE ON TRACK", "ASK FOR HELP"),
    106: ("NO MORE SLEEP?", "SHE IS IN CHARGE"),
    107: ("THE PURPLE STOPS", "YOUR BREATHING"),
    108: ("SYRUP, ALCOHOL,", "THEY STOP YOU"),
    109: ("SOME DRANK", "RAPPERS: DEAD"),
    110: ("ENOUGH, DAMN IT", "CALL THE CLINIC"),
    111: ("I WON'T SAVE YOU", "SAVE YOURSELF"),
    112: ("DRUG USES YOU,", "NOT THE REVERSE"),
    113: ("TALK ABOUT IT,", "YOU'RE NOT ALONE"),
    114: ("ASKING FOR HELP", "IS NOT WEAKNESS"),
    115: ("CLINIC, DOCTOR,", "FRIEND: TALK"),
    116: ("SIXTH FLOOR:", "NO ONE SAVES YOU"),
    117: ("WHO SELLS TO YOU", "DOES NOT HELP"),
    118: ("COSTS A LITTLE,", "THEN TAKES ALL"),
    119: ("ONE STEP AFTER", "ANOTHER, DOWN"),
    120: ("I WILL DRY", "ALL YOUR TEARS"),
    121: ("POT ON BOARD?", "MINE IS BETTER"),
    122: ("YOU ARE IN THE", "GRASS, LIKE ME"),
    123: ("ON THE GRASS", "YOU FEEL LIGHTER"),
    124: ("LOST THE ROAD?", "I WILL ROLL IT"),
    125: ("DO NOT TRUST HIM", "HE PLANS A TRICK"),
    126: ("I TRIED TOO:", "IT IS NO GOOD"),
    127: ("BETTER READ", "THE LABEL, MAN"),
    128: ("DO NOT SHOOT", "INTO A VEIN"),
    129: ("TRY NOT TO", "USE HEROIN"),
    130: ("CALL DON", "LURIDO: HE HELPS"),
    131: ("LOVE ANYONE YOU", "WANT, NOT DRUGS"),
    132: ("TRUE PRIDE:", "TO LIVE LUCID"),
    133: ("RAINBOWS YES,", "NEEDLES NO"),
    134: ("OFF ROAD?", "A PUFF, BACK IN"),
    135: ("ON THE SAND?", "YOU NEED GIANNI"),
    136: ("ROAD EDGE:", "LIKE YOUR LIFE"),
    137: ("FROM A SPLIFF,", "NOBODY DIES"),
    138: ("HEY LET'S GO", "HAVE ONE"),
    139: ("SHALL WE HAVE", "A SPLIFF?"),
    140: ("IT IS JUST", "A LITTLE SPLIFF"),
    141: ("I SEE YOU", "FROM ABOVE"),
    142: ("EDGE? RELAX,", "HAVE A SPLIFF"),
    143: ("IN THE FIELDS,", "BRO, I GROW IT"),
    144: ("SWERVE? EASY,", "YOU ARE FLYING"),
    145: ("NICE DRIFT!", "ANOTHER LAP?"),
    146: ("WHEELS IN GREEN", "HOME, BRO"),
    147: ("POT DOES NOT", "SAVE YOU, MAN"),
    148: ("ARTICHOKE OR K:", "IT IS JUST SMOKE"),
    149: ("APOCALYPSE:", "THE DRAGON: HERE"),
    150: ("WHO DRINKS OF ME", "NEVER DIES"),
    151: ("VANITAS", "VANITATUM, MAN"),
    152: ("GRAVEL AND SMOKE", "PERFECT COUPLE"),
    153: ("BRAKES? NAH,", "LIGHT UP AND GO"),
    154: ("WHO SOWS WIND", "REAPS A STORM"),
    155: ("NOBODY DIES", "FROM A SPLIFF"),
}

NOTA_V5 = (
    " V5 (10/11/2026, @grafico): tutte le 156 frasi tradotte in inglese "
    "semplice (BLOCCO V47, passo @grafico V5 frasi); stesse regole "
    "(maiuscole, 2 righe, riga <= 16 caratteri, no accenti), id, "
    "personaggio e tabelle invariati."
)


def errore(msg: str) -> int:
    print(f"ERRORE: {msg}")
    return 1


def main() -> int:
    if not SRC.is_file():
        return errore(f"file dati non trovato: {SRC}")
    dati = json.loads(SRC.read_text(encoding="utf-8"))
    frasi = dati.get("frasi")
    if not isinstance(frasi, list):
        return errore("campo 'frasi' assente")

    ids = [f.get("id") for f in frasi]
    if len(frasi) != 156:
        return errore(f"attese 156 id, trovati {len(frasi)}")
    if len(set(ids)) != len(ids):
        return errore("id duplicati")
    if set(ids) != set(TRAD):
        mancanti = sorted(set(ids) - set(TRAD))
        extra = sorted(set(TRAD) - set(ids))
        return errore(f"copertura TRAD incompleta: mancanti={mancanti} extra={extra}")

    # Invariante personaggio<->id (stesso controllo di TestiFrasi::verifica).
    range_p = dati.get("range_personaggio_v87", {})
    for p, lista in range_p.items():
        if not isinstance(lista, list):
            return errore(f"range_personaggio_v87.{p} non e' una lista")
    n_p = {p: 0 for p in range_p}

    modificati = 0
    for f in frasi:
        fid = f["id"]
        p = f.get("personaggio")
        if p not in range_p:
            return errore(f"id {fid}: personaggio sconosciuto {p!r}")
        if fid not in range_p[p]:
            return errore(f"id {fid} fuori dal range di {p}")
        l1, l2 = TRAD[fid]
        for n, riga in (("linea1", l1), ("linea2", l2)):
            if not riga:
                return errore(f"id {fid} {n} vuota")
            if len(riga) > MAX_RIGA:
                return errore(f"id {fid} {n} lunga {len(riga)}: {riga!r}")
            if riga != riga.upper():
                return errore(f"id {fid} {n} non maiuscola: {riga!r}")
            if not riga.isascii():
                return errore(f"id {fid} {n} non ASCII: {riga!r}")
        if (f.get("linea1"), f.get("linea2")) != (l1, l2):
            modificati += 1
        f["linea1"] = l1
        f["linea2"] = l2
        n_p[p] += 1

    if n_p.get("gianni") != 97 or n_p.get("gesu") != 59:
        return errore(f"conteggi personaggio inattesi: {n_p}")

    # Metadati: versione e nota di revisione (coerenza con il catalogo dati).
    dati["versione"] = 3
    nota = dati.get("note", "")
    if "V5 (10/11/2026" not in nota:
        dati["note"] = nota + NOTA_V5

    SRC.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n",
                   encoding="utf-8")

    print(f"OK {len(frasi)} frasi tradotte in inglese "
          f"(gianni {n_p['gianni']}, gesu {n_p['gesu']})")
    print(f"righe max {MAX_RIGA}: tutte nel limite; solo ASCII; id e "
          f"personaggio invariati")
    print(f"frasi aggiornate in questo giro: {modificati}; "
          f"versione -> {dati['versione']}")
    print(f"scritto: {SRC.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
