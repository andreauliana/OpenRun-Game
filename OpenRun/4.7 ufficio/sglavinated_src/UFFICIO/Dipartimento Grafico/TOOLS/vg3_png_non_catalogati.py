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


def main() -> int:
    if not CATALOGO.exists():
        print(f"FALLITO: catalogo non trovato in {CATALOGO}")
        return 4
    cat = json.load(open(CATALOGO, encoding="utf-8"))
    usati = set()
    for e in cat["elementi"]:
        files = e.get("file") or []
        files = [files] if isinstance(files, str) else files
        usati.update(files)
        if e.get("raw"):
            usati.add(e["raw"])

    fuori = []
    for pat in PATTERN:
        for g in glob.glob(os.path.join(glob.escape(str(LIBRERIA)), pat), recursive=True):
            rel = os.path.relpath(g, str(LIBRERIA))
            if rel not in usati:
                fuori.append(rel)
    fuori.sort()
    noti, da_decidere = classifica(fuori)
    for f in noti:
        print(f"  noto non-asset: {f} ({NOTI_NON_ASSET[f]})")
    spariti = noti_mancanti(noti)
    for f in spariti:
        print(f"  NOTO-SPARITO: {f} ({NOTI_NON_ASSET[f]})")
    print(
        f"png non catalogati: {len(da_decidere)} (noti non-asset {len(noti)},"
        f" noti spariti {len(spariti)})"
    )
    for f in da_decidere:
        print(f"  {sorgente(f)}: {f}")
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
    return 0 if ok else 1


if __name__ == "__main__":
    if "--autotest" in sys.argv:
        sys.exit(autotest())
    sys.exit(main())
