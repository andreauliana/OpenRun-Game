#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Elenca i PNG di GRAFICA che non compaiono in CATALOGO_GRAFICO.json.

Stessa regola di motore_nuovo/strumenti_py/check_catalogo_grafico.py (che li conta
e avvisa senza fallire): cartelloni, sprite, oggetti e skyline. Serve a decidere,
uno per uno, se sono varianti/doppioni da scartare o elementi da catalogare.

Uso, dalla radice sglavinated_src:
    python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"
    python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest
Sola lettura: non scrive ne' modifica file. main() esce 0 sempre; --autotest esce
0 se la classificazione dei noti funziona, un noto sparito viene segnalato e un
PNG nuovo resta "da decidere". un noto che sparisce puo' voler dire che Andrea lo
ha rinominato (es. il .png.png del furgone): segnalarlo evita che la conversione
resti invisibile solo perche' il motivo non viene piu' stampato.

Aggiunta 22/11: un PNG nuovo nell'area auto del giocatore (giocatore_pose/ o
giocatore/) si etichetta 'giocatore-area-bloccata' e non 'sprite': e' roba di
Andrea (regola bloccata 06/11), non un asset che VG3 puo' convertire/catalogare.

Aggiunta 23/11: la regola 'spariti vs catalogati' (prima inline in main) e' ora
la funzione pura noti_spariti(), provata in autotest: un noto finito nel catalogo
non va stampato due volte come NOTO-CATALOGATO e NOTO-SPARITO.

Aggiunta 23/11 (turno 2): il raccolto dei percorsi citati dal catalogo (campi
'file' e 'raw', con 'file' che puo' essere stringa o lista) e' ora la funzione
pura file_usati(), provata in autotest: e' il passo che decide quali PNG sono
"fuori catalogo" e prima era inline in main().

Aggiunta 23/11 (turno 3): lo scarto dei PNG gia' catalogati, che produce l'elenco
'fuori catalogo', e' ora la funzione pura fuori_catalogo(), provata in autotest:
prima era inline in main() e non si poteva provare che un PNG nuovo resta fuori
mentre uno catalogato viene escluso e l'ordine resta stabile.
"""
import glob
import json
import os
import pathlib
import sys

RADICE = pathlib.Path(__file__).resolve().parents[3]
LIBRERIA = RADICE  # percorsi del catalogo: GRAFICA/...
CATALOGO = LIBRERIA / "GRAFICA/CATALOGO_GRAFICO.json"

PATTERN = (
    "GRAFICA/sponsor/*/*.png",
    "GRAFICA/sprite/**/*.png",
    "GRAFICA/scenari/*/oggetti/*.png",
    "GRAFICA/scenari/*/skyline.png",
)

# PNG fuori dal catalogo per decisione, non per dimenticanza: un PNG nuovo qui
# deve avere un motivo scritto (e un PNG noto che sparisce non va piu' stampato).
# Elenco ESATTO (non pattern): cosi' un PNG nuovo nella stessa cartella resta
# "da decidere" e non viene coperto dai motivi noti. I quattro qui sotto sono i
# residui gia' seguiti a mano: tre pose custom dell'auto di Andrea (area bloccata,
# non caricata dal motore) e il ridisegno col nome sbagliato del furgone incrocio.
# Il motivo e' stampato accanto al file, così il residuo non sembra un problema
# nuovo; il controllo autorevole resta vg3_date_png_raw.py (ALTRI-PNG-DOPPIA-...).
NOTI_NON_ASSET = {
    "GRAFICA/sprite/giocatore_pose/car-custom.png": (
        "auto del giocatore, area bloccata di Andrea, non letta dal motore"
    ),
    "GRAFICA/sprite/giocatore_pose/car-custom-dx.png": (
        "auto del giocatore, area bloccata di Andrea, non letta dal motore"
    ),
    "GRAFICA/sprite/giocatore_pose/car-custom-sx.png": (
        "auto del giocatore, area bloccata di Andrea, non letta dal motore"
    ),
    "GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png": (
        "ridisegno di Andrea con doppia estensione; vero asset lato_furgone_bianco.png"
        " allineato al raw, rinominare/convertire decide Andrea (vedi vg3_date_png_raw.py)"
    ),
}


def classifica(fuori):
    """Divide i PNG fuori catalogo in noti (con motivo) e nuovi da decidere."""
    noti = [f for f in fuori if f in NOTI_NON_ASSET]
    da_decidere = [f for f in fuori if f not in NOTI_NON_ASSET]
    return noti, da_decidere


def noti_mancanti(presenti):
    """Residui noti che non sono piu' sul disco (erano attesi, ora spariti)."""
    visti = set(presenti)
    return [f for f in sorted(NOTI_NON_ASSET) if f not in visti]


def noti_catalogati(usati):
    """Noti non-asset ora presenti nel catalogo (non piu' fuori catalogo).

    Un noto che finisce in CATALOGO_GRAFICO.json non appare piu' tra i PNG fuori
    catalogo: senza questo controllo veniva segnalato come NOTO-SPARITO, che e'
    fuorviante (il file puo' essere ancora su disco). Diagnosi informativa:
    il noto va tolto da NOTI_NON_ASSET o rivalutato.
    """
    visti = set(usati)
    return [f for f in sorted(NOTI_NON_ASSET) if f in visti]


def noti_spariti(noti, catalogati):
    """Noti fuori catalogo spariti dal disco e NON finiti nel catalogo (puro).

    Un noto che e' andato in CATALOGO_GRAFICO.json non appare piu' tra i PNG fuori
    catalogo, ma puo' essere ancora su disco: stampare insieme NOTO-CATALOGATO e
    NOTO-SPARITO era fuorviante. Estrarre la regola che era inline in main() la
    rende provabile e documenta l'interazione tra i due elenchi.
    """
    cat = set(catalogati)
    return [f for f in noti_mancanti(noti) if f not in cat]


# Area auto del giocatore: solo Andrea la disegna (regola bloccata 06/11), VG3 non
# converte ne' cataloga nulla li'. Le pose note sono gia' in NOTI_NON_ASSET, ma un
# file NUOVO in queste cartelle arrivava a sorgente() etichettato come 'sprite'
# normale, insieme a traffico/rivali che invece si possono convertire.
AREA_GIOCATORE = ("/sprite/giocatore/", "/sprite/giocatore_pose/")


def area_bloccata(png):
    """True se il PNG sta nell'area auto del giocatore (solo Andrea, bloccata).

    Un PNG nuovo li' va solo segnalato: non e' un asset da convertire/catalogare
    da VG3. Senza questa funzione sembrava uno sprite convertibile qualsiasi e
    portava a cercare un raw che non si deve toccare.
    """
    return any(seg in png for seg in AREA_GIOCATORE)


def sorgente(png):
    """Area di un PNG fuori catalogo: dice dove guardare se e' un ridisegno nuovo.

    VG3 converte i PNG di scenario piu' nuovi del raw; sapere subito se un PNG
    nuovo e' un oggetto, una skyline, uno sprite o uno sponsor evita di confonderlo
    con le pose dell'auto di Andrea (area bloccata, gia' segnalate tra i noti).
    Dal 22/11 l'area auto ha un'etichetta propria: le pose note non arrivano qui,
    ma un file nuovo in giocatore_pose/ verrebbe altrimenti stampato come 'sprite'
    e sembrerebbe convertibile.
    """
    if area_bloccata(png):
        return "giocatore-area-bloccata"
    if "/scenari/" in png and "/oggetti/" in png:
        return "scenario-oggetti"
    if "/scenari/" in png and png.endswith("/skyline.png"):
        return "scenario-skyline"
    if "/sprite/" in png:
        return "sprite"
    if "/sponsor/" in png:
        return "sponsor"
    return "altro"


def conteggio_per_sorgente(pngs):
    """Conta i PNG per area (sorgente): utile quando arrivano piu' ridisegni nuovi."""
    conteggi = {}
    for p in pngs:
        s = sorgente(p)
        conteggi[s] = conteggi.get(s, 0) + 1
    return conteggi


def file_usati(cat):
    """Insieme dei percorsi citati dal catalogo (campi 'file' e 'raw' di ogni voce).

    main() deve sapere quali PNG sono gia' catalogati per decidere quali restano
    "fuori catalogo". Il campo 'file' puo' essere una stringa o una lista e il
    campo 'raw' e' un riferimento aggiuntivo: raccoglierli qui, come funzione pura,
    permette di provare entrambi i casi senza leggere il catalogo dal disco.
    Un 'file' vuoto o assente e un 'raw' vuoto non citano nulla.
    """
    usati = set()
    for e in cat["elementi"]:
        files = e.get("file") or []
        files = [files] if isinstance(files, str) else files
        usati.update(files)
        if e.get("raw"):
            usati.add(e["raw"])
    return usati


def fuori_catalogo(trovati, usati):
    """PNG relativi a GRAFICA/ non presenti tra i percorsi citati dal catalogo.

    main() raccoglie i PNG con glob e poi scarta quelli gia' catalogati: tenere
    lo scarto in una funzione pura, che lavora sulla lista di percorsi relativi
    gia' ricavata, permette di provare che un PNG nuovo resta fuori, uno
    catalogato viene escluso e l'ordine (e l'eventuale duplicato) e' stabile,
    senza leggere il disco. 'usati' e' l'insieme restituito da file_usati().
    """
    return sorted(rel for rel in trovati if rel not in usati)


def main() -> int:
    if not CATALOGO.exists():
        print(f"FALLITO: catalogo non trovato in {CATALOGO}")
        return 4
    cat = json.load(open(CATALOGO, encoding="utf-8"))
    usati = file_usati(cat)

    trovati = []
    for pat in PATTERN:
        for g in glob.glob(os.path.join(glob.escape(str(LIBRERIA)), pat), recursive=True):
            trovati.append(os.path.relpath(g, str(LIBRERIA)))
    fuori = fuori_catalogo(trovati, usati)
    noti, da_decidere = classifica(fuori)
    for f in noti:
        print(f"  noto non-asset: {f} ({NOTI_NON_ASSET[f]})")
    catalogati = noti_catalogati(usati)
    for f in catalogati:
        print(f"  NOTO-CATALOGATO: {f} ({NOTI_NON_ASSET[f]})")
    spariti = noti_spariti(noti, catalogati)
    for f in spariti:
        print(f"  NOTO-SPARITO: {f} ({NOTI_NON_ASSET[f]})")
    print(
        f"png non catalogati: {len(da_decidere)} (noti non-asset {len(noti)},"
        f" noti catalogati {len(catalogati)}, noti spariti {len(spariti)})"
    )
    per_area = conteggio_per_sorgente(da_decidere)
    for f in da_decidere:
        print(f"  {sorgente(f)}: {f}")
    if per_area:
        dettaglio = ", ".join(f"{k} {v}" for k, v in sorted(per_area.items()))
        print(f"  da decidere per area: {dettaglio}")
    return 0


def autotest():
    """I noti restano tali; un PNG nuovo nella stessa cartella NON deve essere coperto."""
    noti, da_decidere = classifica([
        "GRAFICA/sprite/giocatore_pose/car-custom.png",
        "GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png",
        "GRAFICA/sprite/traffico/nuovo_ridisegno.png",
    ])
    ok = (
        len(noti) == 2
        and da_decidere == ["GRAFICA/sprite/traffico/nuovo_ridisegno.png"]
    )
    print("autotest classifica: " + ("OK" if ok else "FALLITO"))
    ok_spariti = (
        noti_mancanti([
            "GRAFICA/sprite/giocatore_pose/car-custom.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-dx.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-sx.png",
            "GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png",
        ]) == []
        and noti_mancanti([
            "GRAFICA/sprite/giocatore_pose/car-custom.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-dx.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-sx.png",
        ]) == ["GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png"]
    )
    ok = ok and ok_spariti
    print("autotest noti_mancanti: " + ("OK" if ok_spariti else "FALLITO"))
    ok_catalogati = (
        noti_catalogati({"GRAFICA/sprite/giocatore_pose/car-custom.png"})
        == ["GRAFICA/sprite/giocatore_pose/car-custom.png"]
        and noti_catalogati({"GRAFICA/sprite/traffico/altro.png"}) == []
    )
    ok = ok and ok_catalogati
    print("autotest noti_catalogati: " + ("OK" if ok_catalogati else "FALLITO"))
    # noti_spariti: un noto finito nel catalogo non e' sparito (non va stampato
    # due volte come NOTO-CATALOGATO e NOTO-SPARITO); uno assente da entrambi si.
    furgone = "GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png"
    tutti = sorted(NOTI_NON_ASSET)
    senza_furgone = [f for f in tutti if f != furgone]
    ok_spariti = (
        noti_spariti(tutti, []) == []
        and noti_spariti(senza_furgone, []) == [furgone]
        and noti_spariti(senza_furgone, [furgone]) == []
    )
    ok = ok and ok_spariti
    print("autotest noti_spariti: " + ("OK" if ok_spariti else "FALLITO"))
    ok_sorgente = (
        sorgente("GRAFICA/scenari/lv_03_foglie/oggetti/nuovo.png") == "scenario-oggetti"
        and sorgente("GRAFICA/scenari/lv_04_cromo/skyline.png") == "scenario-skyline"
        and sorgente("GRAFICA/sprite/traffico/nuovo.png") == "sprite"
        and sorgente("GRAFICA/sponsor/neon/nuovo.png") == "sponsor"
        and sorgente("ALTRO/nuovo.png") == "altro"
        and sorgente("GRAFICA/sprite/giocatore_pose/auto_nuova.png")
        == "giocatore-area-bloccata"
        and sorgente("GRAFICA/sprite/giocatore/auto_nuova.png")
        == "giocatore-area-bloccata"
    )
    ok = ok and ok_sorgente
    print("autotest sorgente: " + ("OK" if ok_sorgente else "FALLITO"))
    ok_bloccata = (
        area_bloccata("GRAFICA/sprite/giocatore_pose/auto_nuova.png")
        and area_bloccata("GRAFICA/sprite/giocatore/auto_nuova.png")
        and not area_bloccata("GRAFICA/sprite/traffico/nuovo.png")
        and not area_bloccata("GRAFICA/sprite/giocatore_pose_extra/nuovo.png")
    )
    ok = ok and ok_bloccata
    print("autotest area_bloccata: " + ("OK" if ok_bloccata else "FALLITO"))
    ok_conteggio = conteggio_per_sorgente([
        "GRAFICA/scenari/lv_01_neon/oggetti/a.png",
        "GRAFICA/scenari/lv_02_atomico/oggetti/b.png",
        "GRAFICA/scenari/lv_04_cromo/skyline.png",
        "GRAFICA/sprite/traffico/c.png",
    ]) == {"scenario-oggetti": 2, "scenario-skyline": 1, "sprite": 1}
    ok = ok and ok_conteggio
    print("autotest conteggio_per_sorgente: " + ("OK" if ok_conteggio else "FALLITO"))
    ok_vuoti = classifica([]) == ([], []) and conteggio_per_sorgente([]) == {}
    ok = ok and ok_vuoti
    print("autotest casi_vuoti: " + ("OK" if ok_vuoti else "FALLITO"))
    # file_usati: 'file' stringa e lista, piu' 'raw'; vuoti/assenti non citano nulla.
    ok_usati = file_usati({"elementi": [
        {"file": "GRAFICA/a.png", "raw": "raw/a.sglraw"},
        {"file": ["GRAFICA/b.png", "GRAFICA/c.png"]},
        {"file": [], "raw": ""},
        {},
    ]}) == {
        "GRAFICA/a.png", "raw/a.sglraw", "GRAFICA/b.png", "GRAFICA/c.png",
    }
    ok = ok and ok_usati
    print("autotest file_usati: " + ("OK" if ok_usati else "FALLITO"))
    ok_usati_vuoti = file_usati({"elementi": []}) == set()
    ok = ok and ok_usati_vuoti
    print("autotest file_usati_vuoti: " + ("OK" if ok_usati_vuoti else "FALLITO"))
    # fuori_catalogo: un PNG nuovo resta fuori, uno catalogato e' escluso,
    # l'ordine resta stabile e un doppio passaggio resta visibile (non si dedupa).
    ok_fuori = fuori_catalogo(
        ["GRAFICA/z.png", "GRAFICA/a.png", "GRAFICA/catalogato.png", "GRAFICA/a.png"],
        {"GRAFICA/catalogato.png"},
    ) == ["GRAFICA/a.png", "GRAFICA/a.png", "GRAFICA/z.png"]
    ok_fuori = ok_fuori and fuori_catalogo([], {"GRAFICA/a.png"}) == []
    ok = ok and ok_fuori
    print("autotest fuori_catalogo: " + ("OK" if ok_fuori else "FALLITO"))
    return 0 if ok else 1


if __name__ == "__main__":
    if "--autotest" in sys.argv:
        sys.exit(autotest())
    sys.exit(main())
