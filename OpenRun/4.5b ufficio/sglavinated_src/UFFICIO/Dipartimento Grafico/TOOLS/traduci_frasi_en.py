# SPDX-License-Identifier: MIT
"""BLOCCO V47 @grafico V5 frasi: traduce motore_nuovo/dati/frasi.json in inglese
semplice (regole testi: maiuscole, max 2 righe, riga <= 16 caratteri, no accenti).

- Il testo italiano originale viene conservato nei campi "it1"/"it2" di ogni frase
  (nessuna perdita, reversibile).
- Validazione PRIMA della scrittura: 156 frasi, id 0..155, lunghezza riga,
  maiuscole ASCII, coerenza personaggio (Gianni 97 / Gesu 59).
- Se una validazione fallisce il file NON viene toccato e lo script esce con 1.

Uso: python3 "UFFICIO/Dipartimento Grafico/TOOLS/traduci_frasi_en.py"
Radice: parents[3] = sglavinated_src
"""
import json
import sys
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
FILE = RADICE / "motore_nuovo" / "dati" / "frasi.json"
MAX_RIGA = 16

# id -> (linea1, linea2) in inglese semplice
EN = {
    0: ("HEY, BROTHER,", "COME WITH ME"),
    1: ("LITTLE FIRE,", "WARM IT UP"),
    2: ("FAST DEAL, SIGN", "AND COME DOWN"),
    3: ("YOUR SOUL, FREE", "TRY IT NOW"),
    4: ("BLOW THE FIRE,", "LOOK HOW NICE"),
    5: ("SIGN, COME DOWN,", "YOU ARE GOD, BRO"),
    6: ("SIGN THE DEAL,", "THEN FLY AWAY"),
    7: ("COME DOWN HERE,", "YOU ARE CRAZY"),
    8: ("YOU ARE ALONE,", "NO SON EITHER"),
    9: ("HEAVY SOUL,", "WARM IT WITH ME"),
    10: ("TREMBLING, HUH?", "COME DOWN, FOOL"),
    11: ("BIG SYRINGE?", "ONE ONLY, C'MON"),
    12: ("DO YOU MISS ME?", "DO YOU FEEL IT?"),
    13: ("ANYONE CAN DO", "A LINE"),
    14: ("SIGN, COME DOWN,", "NEVER GO UP"),
    15: ("SOUL IS MINE,", "I TOOK IT"),
    16: ("I'LL HANDLE IT,", "YOU JUST DRIVE"),
    17: ("COME DOWN, FOOL,", "THE DEAL IS DONE"),
    18: ("COME WITH ME", "FORGET THE SUN"),
    19: ("OFF THE EDGE,", "COMING BACK, BRO"),
    20: ("FIRE AWAITS YOU", "STAY HERE, GO ON"),
    21: ("FEEL A LOSER?", "COME DOWN THEN"),
    22: ("THE UFO SEES YOU", "ARE YOU HIGH?"),
    23: ("WATCH OUT FOR", "THE UFOS, BRO"),
    24: ("DRUGS? DRIVING?", "CERTAIN DEATH"),
    25: ("WATCH THE ROAD,", "COME ON"),
    26: ("NO TRIP WHEN", "YOU ARE DRIVING"),
    27: ("OH, YOU FOOL,", "LOOK, MAN"),
    28: ("GET UP, DRIVE,", "DON'T DO IT, BRO"),
    29: ("OPEN YOUR EYES,", "DROP THE JOINT"),
    30: ("LIGHT IS HERE,", "YOU ARE ON TRACK"),
    31: ("EASY, YOU FOOL,", "LIFE IS ONE"),
    32: ("WAKE UP, BRO:", "I DO NOT SMOKE"),
    33: ("LSD WILL BLOW", "YOUR MIND AWAY"),
    34: ("A SYRINGE IS", "AN OPEN COFFIN"),
    35: ("HEY, WHAT'S UP?", "STUCK HERE?"),
    36: ("HEY, A BREAK?", "LIGHT ONE UP?"),
    37: ("MAKING ONE, HUH?", "YOU ALONE?"),
    38: ("ALL STOPPED...", "FLIP THE MAP?"),
    39: ("IN FACT, NOW YOU", "WANT TWO"),
    40: ("CALL NOBODY,", "JUST SMOKE"),
    41: ("START AGAIN AND", "LIGHT IT, GO ON"),
    42: ("WHAT BAD LUCK!", "PULL AND PASS"),
    43: ("I WILL HELP YOU", "TRUST ME"),
    44: ("NICE CURVE HERE,", "LIKE A JOINT"),
    45: ("HEY, A SPLIFF?", "WHY NOT?"),
    46: ("JUST ONE LIGHT", "PULL, GO ON"),
    47: ("GRASS NEVER HURT", "ANYBODY"),
    48: ("A JOINT IS ONLY", "THE START, GO ON"),
    49: ("BORED OF GRASS?", "THERE IS MORE"),
    50: ("I HAVE STRONGER", "STUFF, WANT IT?"),
    51: ("ECSTASY OPENS UP", "YOUR NIGHT"),
    52: ("ONE PILL AND YOU", "DANCE FOR HOURS"),
    53: ("LSD: ONE SHEET", "AND YOU FLY AWAY"),
    54: ("GRASS BELOW,", "PLAY DEFENSE"),
    55: ("AT 250 SMOKED", "YOU FEEL A GOD"),
    56: ("NOBODY WILL", "EVER KNOW"),
    57: ("FIRST ONE FREE,", "THEN YOU SEE"),
    58: ("JUST ONE SPLIFF,", "NOTHING AT ALL"),
    59: ("KILL TIME WITH", "GIANNI"),
    60: ("GO OUT TONIGHT", "AND GET HIGH"),
    61: ("ONLY GRASS, THEN", "YOU DECIDE"),
    62: ("WHY STOP AT", "THE JOINT?"),
    63: ("WHOEVER TRIES", "NEVER STOPS"),
    64: ("LYING, SMOKING,", "NAILS, HOPE"),
    65: ("OUT TOO? THEN", "YOU ARE IN, BRO"),
    66: ("DAMN, ENOUGH", "OF THIS STUFF"),
    67: ("YOU GET HIGH,", "COINCIDENCE?"),
    68: ("DAMN IT, BACK", "ON THE ROAD"),
    69: ("CLOSE YOUR MOUTH", "OPEN YOUR EYES"),
    70: ("BE QUIET, BRO,", "WORLD IS SICK"),
    71: ("YOU ARE ENDING,", "YOU KNOW, HUH?"),
    72: ("YOU ARE WRECKING", "YOUR LIFE, BRO"),
    73: ("ONE LINE AND THE", "HEART STOPS"),
    74: ("FROM GRASS TO A", "NEEDLE IS SHORT"),
    75: ("I AM HERE: GET", "BACK ON THE ROAD"),
    76: ("I AM THE ALPHA", "AND THE OMEGA"),
    77: ("DON'T LET THEM", "FOOL YOU"),
    78: ("MOM IS WAITING", "AT HOME FOR YOU"),
    79: ("ONE JOINT, THEN", "A DEATH: YOU?"),
    80: ("DRUGS TAKE AWAY", "EVERYTHING"),
    81: ("TO THE THIRSTY I", "WILL GIVE WATER"),
    82: ("FEAR NOT, I AM", "THE LIVING ONE"),
    83: ("GET BEHIND ME,", "TEMPTER!"),
    84: ("LOOK, I MAKE ALL", "THINGS NEW"),
    85: ("A BEER AND A", "SPLIFF?"),
    86: ("COME, ALL YOUR", "PEOPLE ARE HERE"),
    87: ("TELL NOBODY,", "GO ON"),
    88: ("HAVE A DIERA, IT", "WILL FIX YOU UP"),
    89: ("A DIERA AND GO,", "ALL PASSES"),
    90: ("WITHOUT DIERA", "NO SLEEP, HUH?"),
    91: ("PURPLE DRANK:", "SLIDES SLOW"),
    92: ("ONE PURPLE SIP", "AND ALL SLOWS"),
    93: ("A GLASS OF DRANK", "LOOKS COOL"),
    94: ("THE RAPPERS DO", "IT, GO ON"),
    95: ("ONLY TONIGHT,", "STOP TOMORROW"),
    96: ("YOU ARE NOT", "ADDICTED, GO ON"),
    97: ("JUST ONE MORE,", "PROMISED"),
    98: ("RENT, WORK? ONE", "DIERA AND GO"),
    99: ("UP TO FLOOR SIX", "STUFF IS ALWAYS"),
    100: ("IN THE BIG BLOCK", "IT COSTS LITTLE"),
    101: ("STAIR C, KNOCK,", "DON'T RING"),
    102: ("FIRST THE DIERA,", "THEN HEAVY STUFF"),
    103: ("WORK TOMORROW?", "ONE DIERA AND GO"),
    104: ("PURPLE, GRASS:", "GREAT, BRO"),
    105: ("YOU ARE ON TRACK", "ASK FOR HELP"),
    106: ("NO SLEEP? SHE", "IS IN CHARGE"),
    107: ("THE PURPLE STOPS", "YOUR BREATHING"),
    108: ("SYRUP, ALCOHOL,", "THEY STOP YOU"),
    109: ("DRANK RAPPERS:", "SOME ARE DEAD"),
    110: ("ENOUGH, DAMN IT.", "CALL THE CLINIC"),
    111: ("I CAN'T SAVE YOU", "ONLY YOU CAN"),
    112: ("STUFF USES YOU", "YOU DON'T USE IT"),
    113: ("TALK ABOUT IT,", "YOU'RE NOT ALONE"),
    114: ("ASKING FOR HELP", "IS NOT WEAK"),
    115: ("CLINIC, DOCTOR,", "FRIEND: TALK"),
    116: ("ON FLOOR SIX", "NOBODY SAVES YOU"),
    117: ("WHO SELLS TO YOU", "DOES NOT HELP"),
    118: ("IT COSTS LITTLE,", "THEN TAKES ALL"),
    119: ("STEP AFTER STEP,", "GOING DOWN"),
    120: ("I WILL DRY ALL", "OF YOUR TEARS"),
    121: ("GRASS ON BOARD?", "MINE IS BETTER"),
    122: ("YOU ARE IN THE", "MEADOW, LIKE ME"),
    123: ("ON GRASS YOU", "GO LIGHTER"),
    124: ("LOST THE ROAD?", "I'LL ROLL IT"),
    125: ("DO NOT TRUST IT", "IT WILL FOOL YOU"),
    126: ("I TRIED TOO:", "IT WAS NO GOOD"),
    127: ("BETTER READ THE", "TEXT, BRO"),
    128: ("DON'T SHOOT INTO", "YOUR VEIN!"),
    129: ("TRY NOT TO GET", "ON HEROIN"),
    130: ("CALL DON LURIDO:", "HE HELPS YOU"),
    131: ("LOVE WHO YOU", "WANT, NOT STUFF"),
    132: ("TRUE PRIDE: LIVE", "CLEAR-HEADED"),
    133: ("RAINBOW YES,", "NEEDLES NO"),
    134: ("OFF ROAD? A PULL", "AND YOU'RE BACK"),
    135: ("ON THE SAND?", "YOU NEED GIANNI"),
    136: ("ROAD EDGE: LIKE", "YOUR LIFE"),
    137: ("NOBODY DIES OF", "SPLIFFS"),
    138: ("HEY, LET'S GO", "HAVE ONE"),
    139: ("SHALL WE HAVE A", "DIERA?"),
    140: ("IT IS JUST A", "LITTLE SPLIFF"),
    141: ("I SEE YOU FROM", "ABOVE"),
    142: ("EDGE? RELAX AND", "HAVE A DIERA"),
    143: ("IN THE FIELDS", "I GROW IT, BRO"),
    144: ("SWERVING? RELAX,", "YOU ALREADY FLY"),
    145: ("NICE SWERVE! ONE", "ANOTHER GO?"),
    146: ("WHEELS ON GRASS", "MY HOME, BRO"),
    147: ("MARY WON'T SAVE", "YOU, BRO"),
    148: ("ARTICHOKE OR K:", "IT IS JUST SMOKE"),
    149: ("APOCALYPSE:", "THE DRAGON HERE"),
    150: ("WHO DRINKS OF ME", "WILL NEVER DIE"),
    151: ("ALL IS VANITY,", "MY FRIEND"),
    152: ("GRAVEL, SMOKE:", "PERFECT COUPLE"),
    153: ("BRAKES? NO WAY,", "LIGHT IT AND GO"),
    154: ("WHO SOWS WIND", "REAPS A STORM"),
    155: ("NOBODY DIES FROM", "ONE SPLIFF"),
}

# id -> (it1, it2) testo italiano originale, copia verbatim dal file letto
# all'avvio del turno (10/11/2026 07:43, stato pre-traduzione): serve per
# conservare l'originale CC-BY-4.0 di Andrea dopo la traduzione V5.
IT = {
    0: ("HEY, FRATELLINO,", "SCENDI CON ME"),
    1: ("FUOCINO, DAI,", "SCALDA QUA"),
    2: ("PATTO VELOCE,", "FIRMI E SCENDI"),
    3: ("TI PORTO L'ANIMA", "GRATIS, PROVA"),
    4: ("SOFFIA IL FUEGO,", "VEDI CHE BELLO"),
    5: ("PATTO E SCENDI", "E SEI DIO, FRA"),
    6: ("FIRMA IL PATTO,", "POI VOLI VIA"),
    7: ("SCENDI CON ME,", "IL PAZZO SEI TU"),
    8: ("SEI SOLO SENZA", "UN FIGLIO A META"),
    9: ("L'ANIMA PESANTE,", "SCALDALA CON ME"),
    10: ("TREMENI, VERO?", "SCENDI, SCEMO"),
    11: ("SIRINGONE?", "UNA SOLA, DAI"),
    12: ("MANCHI DI ME?", "TE LO SENTI?"),
    13: ("UNA RIGA LA", "FANNO TUTTI"),
    14: ("FIRMA, SCENDI,", "NON RISALI"),
    15: ("L'ANIMA E' MIA", "TE L'HO PRESA"),
    16: ("CI PENSO IO,", "TU GUIDA E TIRA"),
    17: ("SCENDI, SCEMO,", "IL PATTO E FATTO"),
    18: ("VIENI QUA CON ME", "SCORDA IL SOLE"),
    19: ("FUORI DAL BORDO", "TORNANDO, FRA"),
    20: ("FUOCO TI ASPETTA", "STAI QUI, DAI"),
    21: ("TI SENTI UNO", "SFIGATO? SCENDI"),
    22: ("L'UFO TI VEDE!", "SEI SBALLATO?"),
    23: ("ATTENTO AGLI", "UFO, FRATE!"),
    24: ("DROGA E VOLANTE:", "MORTE CERTA"),
    25: ("ATTENTO ALLA", "STRADA, DAI"),
    26: ("NIENTE SBALLO", "SE GUIDI"),
    27: ("OH, COGLIONE,", "GUARDA, FRA"),
    28: ("ALZATI E GUIDA,", "NON FARTI, FRA"),
    29: ("APRI GLI OCCHI,", "MOLLA LA CANNA"),
    30: ("LA LUCE E' QUI,", "TU SEI IN ROTA"),
    31: ("PIANO, CAZZONE,", "LA VITA E' UNA"),
    32: ("SVEGLIA, FRA:", "IO NON FUMO"),
    33: ("L'LSD TI SPACCA", "LA TESTA"),
    34: ("UNA SIRINGA E'", "UNA BARA APERTA"),
    35: ("HEY, COSA FAI?", "TI SEI PIANTATO?"),
    36: ("HEY, PAUSETTA?", "ACCENDIAMO?"),
    37: ("NE STAI FACENDO", "SU UNO, VERO?"),
    38: ("TUTTO FERMO...", "GIRA LA CARTINA?"),
    39: ("ANZI, ORA CE", "NE VUOI DUE"),
    40: ("NON CHIAMARE", "NESSUNO, FUMA"),
    41: ("RIPARTI E", "ACCENDI, DAI"),
    42: ("CHE SFIGA!", "TIRA E PASSA"),
    43: ("TI AIUTO IO,", "FIDATI DI ME"),
    44: ("CHE BELLA CURVA,", "COME LA CANNA"),
    45: ("HEY, FAMMO NO", "SPINO?"),
    46: ("DAI, SOLO UN", "TIRO LEGGERO"),
    47: ("L'ERBA NON FA", "MALE A NESSUNO"),
    48: ("LA CANNA E' SOLO", "L'INIZIO, DAI"),
    49: ("TI ANNOI CON LA", "ERBA? C'E' ALTRO"),
    50: ("HO ROBA PIU", "FORTE, VUOI?"),
    51: ("L'ECSTASY TI", "SBLOCCA LA SERA"),
    52: ("UNA PASTICCA E", "BALLI PER ORE"),
    53: ("LSD: UN CARTONE", "E VOLI VIA"),
    54: ("ERBA SOTTO,", "FAI LA DIFESA"),
    55: ("A 250 FUMATO", "TI SENTI UN DIO"),
    56: ("NESSUNO LO", "SAPRA MAI"),
    57: ("LA PRIMA E'", "GRATIS, POI VEDI"),
    58: ("SOLO UNO SPINO,", "NON E' NIENTE"),
    59: ("PASSA IL TEMPO", "CON GIANNI"),
    60: ("STASERA ESCI", "E TI SBALLI"),
    61: ("E' SOLO ERBA,", "POI VEDI TU"),
    62: ("PERCHE' FERMARTI", "ALLA CANNA?"),
    63: ("CHI PROVA NON", "SI FERMA PIU"),
    64: ("STRAI E FUMI,", "CHIODI, SPERANZE"),
    65: ("FUORI ANCHE TU?", "POI CI SEI, BRO"),
    66: ("MINCHIA, BASTA", "CON STA ROBA"),
    67: ("TI FAI E SBANDI,", "COINCIDENZA?"),
    68: ("PORCA MISERIA,", "TORNA IN STRADA"),
    69: ("CHIUDI LA BOCCA", "E APRI GLI OCCHI"),
    70: ("STAI ZITTO, FRA", "IL MONDO NON VA"),
    71: ("STAI PER FINIRE,", "LO SAI, VERO?"),
    72: ("TI STAI FOTTENDO", "LA VITA, FRA"),
    73: ("UNA RIGA, E IL", "CUORE SI FERMA"),
    74: ("DALL'ERBA A AGO", "IL PASSO E BREVE"),
    75: ("SONO QUI: TORNA", "SULLA STRADA"),
    76: ("IO SONO L'ALFA", "E L'OMEGA"),
    77: ("NON LASCIARTI", "INGANNARE"),
    78: ("TUA MADRE TI", "ASPETTA A CASA"),
    79: ("UNA CANNA, POI", "UN MORTO: TU?"),
    80: ("LA DROGA TI", "TOGLIE TUTTO"),
    81: ("A CHI HA SETE", "DARO ACQUA VIVA"),
    82: ("NON TEMERE, IO", "SONO IL VIVENTE"),
    83: ("VADE RETRO,", "TENTATORE!"),
    84: ("ECCO, FACCIO", "NUOVE LE COSE"),
    85: ("UNA BIRRA E", "UNO SPINO?"),
    86: ("VIENI, CI SONO", "TUTTI I TUOI"),
    87: ("NON DIRLO A", "NESSUNO, DAI"),
    88: ("FATTI NA DIERA,", "CHE TI SISTEMA"),
    89: ("DIERA E VIA,", "PASSA TUTTO"),
    90: ("SENZA DIERA", "NON DORMI, EH?"),
    91: ("PURPLE DRANK:", "SCIVOLA LENTO"),
    92: ("UN SORSO VIOLA", "E RALLENTA TUTTO"),
    93: ("UN BICCHIERE DI", "DRANK FA STILE"),
    94: ("LO FANNO I", "RAPPER, DAI"),
    95: ("SOLO STASERA,", "DOMANI SMETTI"),
    96: ("NON SEI", "DIPENDENTE, DAI"),
    97: ("UN'ALTRA E", "BASTA, PROMESSO"),
    98: ("AFFITTO, LAVORO?", "DIERA E PASSA"),
    99: ("SALI AL SESTO:", "C'E' SEMPRE ROBA"),
    100: ("NEL PALAZZONE", "COSTA POCO"),
    101: ("SCALA C, NON", "SUONARE, BUSSA"),
    102: ("PRIMA LA DIERA,", "POI ROBA PESA"),
    103: ("LAVORI DOMANI?", "UNA DIERA E VAI"),
    104: ("VIOLA E ERBA:", "TOP, FRA"),
    105: ("SEI IN ROTA:", "CHIEDI AIUTO"),
    106: ("NON DORMI PIU?", "COMANDA LEI"),
    107: ("IL VIOLA FERMA", "IL RESPIRO"),
    108: ("SCIROPPO E ALCOL", "TI FERMANO"),
    109: ("ALCUNI RAPPER", "DEL DRANK: MORTI"),
    110: ("BASTA, CAZZO.", "CHIAMA IL SERT"),
    111: ("NON TI SALVO IO,", "TI SALVI TU"),
    112: ("LA ROBA TI USA,", "NON IL CONTRARIO"),
    113: ("PARLANE, FRA:", "NON SEI SOLO"),
    114: ("CHIEDERE AIUTO", "NON E' DEBOLEZZA"),
    115: ("SERT, MEDICO,", "AMICO: PARLANE"),
    116: ("AL SESTO PIANO", "NESSUNO TI SALVA"),
    117: ("CHI TI VENDE", "NON TI AIUTA"),
    118: ("COSTA POCO, POI", "TI PRENDE TUTTO"),
    119: ("SCALINO DOPO", "SCALINO, GIU"),
    120: ("ASCIUGHERO", "OGNI TUA LACRIMA"),
    121: ("ERBA A BORDO?", "LA MIA E' MEGLIO"),
    122: ("SEI NEL PRATO,", "COME ME, FRA"),
    123: ("SULL'ERBA VAI", "PIU LEGGERO"),
    124: ("PERSO LA STRADA?", "TE LA RULLO IO"),
    125: ("NON TI FIDARE!", "TI VUOLE FREGARE"),
    126: ("ANCHE IO AVEVO", "PROVATO: NO."),
    127: ("MEGLIO LEGGERE", "IL TESTO, FRA"),
    128: ("NON INIETTARTI", "IN VENA!"),
    129: ("CERCA DI NON", "FARTI DI EROINA"),
    130: ("CHIAMA DON", "LURIDO: TI AIUTA"),
    131: ("AMA CHI VUOI,", "NON LA ROBA"),
    132: ("ORGOGLIO VERO:", "VIVERE LUCIDI"),
    133: ("ARCOBALENO SI,", "SIRINGHE NO"),
    134: ("FUORI STRADA?", "UN TIRO, RIENTRI"),
    135: ("SULLA SABBIA?", "TI SERVE GIANNI"),
    136: ("BORDO STRADA:", "COME LA TUA VITA"),
    137: ("DI SPINELLI", "NESSUNO MUORE"),
    138: ("HEY ANDIAMO A", "FARCI UNO"),
    139: ("CI FACCIAMO UNA", "DIERA?"),
    140: ("E' SOLO UNO", "SPINELLINO"),
    141: ("TI VEDO", "DALL'ALTO"),
    142: ("BORDO? RILASSATI", "FATTI UNA DIERA"),
    143: ("NEI CAMPI, FRA", "CRESCO IO, FRA"),
    144: ("SBANDI? TRANQUI,", "SEI GIA IN VOLO"),
    145: ("SBANDATA BELLA!", "UN ALTRO GIRO?"),
    146: ("RUOTE NEL VERDE:", "CASA MIA, BRO"),
    147: ("LA MARIA NON", "TI SALVA, FRA"),
    148: ("CARCIOFO O K:", "E' SOLO FUMO"),
    149: ("APOCALIPSE:", "IL DRAGO E' QUI"),
    150: ("CHI BEVE DA ME", "NON MORRA MAI"),
    151: ("VANITAS", "VANITATUM, FRA"),
    152: ("GHIAIA E FUMO,", "COPPIA PERFETTA"),
    153: ("FRENI? MA NO,", "ACCENDI E VAI"),
    154: ("CHI SEMINA VENTO", "MIETE TEMPESTA"),
    155: ("NESSUNO MUORE", "PER UNO SPINO"),
}


def riga_ok(r: str) -> bool:
    if len(r) > MAX_RIGA:
        return False
    if not r.isascii():
        return False
    if r != r.upper():
        return False
    return True


def main() -> int:
    if not FILE.is_file():
        print(f"ERRORE: manca {FILE}")
        return 1
    dati = json.loads(FILE.read_text(encoding="utf-8"))
    frasi = dati.get("frasi")
    if not isinstance(frasi, list):
        print("ERRORE: campo 'frasi' non e' una lista")
        return 1

    errori = []
    if len(frasi) != 156:
        errori.append(f"attese 156 id, trovati {len(frasi)}")
    ids = sorted(f.get("id") for f in frasi)
    if ids != list(range(156)):
        errori.append("id non continui 0..155")

    n_gianni = n_gesu = 0
    for f in frasi:
        fid = f.get("id")
        if fid not in EN:
            errori.append(f"id {fid}: manca traduzione")
            continue
        if fid not in IT:
            errori.append(f"id {fid}: manca l'originale italiano IT")
            continue
        it1, it2 = IT[fid]
        for nome, riga in (("it1", it1), ("it2", it2)):
            if not riga or not riga.isascii() or riga != riga.upper():
                errori.append(f"id {fid} {nome} italiano non valido: {riga!r}")
        f["it1"], f["it2"] = it1, it2
        l1, l2 = EN[fid]
        f["linea1"], f["linea2"] = l1, l2
        for nome, riga in (("linea1", l1), ("linea2", l2)):
            if not riga_ok(riga):
                errori.append(f"id {fid} {nome} '{riga}' non valida "
                              f"(len {len(riga)})")
        p = f.get("personaggio")
        if p == "gianni":
            n_gianni += 1
        elif p == "gesu":
            n_gesu += 1
        else:
            errori.append(f"id {fid}: personaggio '{p}' sconosciuto")

    if n_gianni != 97 or n_gesu != 59:
        errori.append(f"conteggi personaggio errati: gianni {n_gianni}, "
                      f"gesu {n_gesu}")
    if len(EN) != 156:
        errori.append(f"dizionario con {len(EN)} voci, attese 156")
    if len(IT) != 156:
        errori.append(f"originale italiano con {len(IT)} voci, attese 156")

    if errori:
        print("VALIDAZIONE FALLITA — file NON modificato:")
        for e in errori:
            print(f"  - {e}")
        return 1

    dati["versione"] = 3
    dati["lingua"] = "en"
    dati["note"] = (
        "Testi in inglese semplice (BLOCCO V47 V5, @grafico 10/11/2026): "
        "156 frasi tradotte, stesso id/personaggio, max 16 caratteri per riga, "
        "maiuscole, no accenti. Testo italiano originale conservato nei campi "
        "it1/it2 di ogni frase (reversibile). Fonte dei testi originali: hack "
        "v87 (sola lettura, CC-BY-4.0 Andrea); nel nuovo motore sono dati "
        "sostituibili."
    )
    FILE.write_text(json.dumps(dati, ensure_ascii=False, indent=2) + "\n",
                    encoding="utf-8")
    print(f"OK: {len(frasi)} frasi in inglese scritte in "
          f"{FILE.relative_to(RADICE)}")
    print(f"    gianni {n_gianni}, gesu {n_gesu}, versione 3, "
          f"italiano salvato in it1/it2")
    return 0


if __name__ == "__main__":
    sys.exit(main())
