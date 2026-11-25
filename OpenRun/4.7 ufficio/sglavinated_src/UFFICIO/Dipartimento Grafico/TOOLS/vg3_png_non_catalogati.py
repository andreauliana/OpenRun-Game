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

Aggiunta 24/11: la riga di riepilogo finale (prima costruita inline in main con
un f-string spezzato) e' ora la funzione pura riga_sintesi(), provata in autotest:
e' l'unica riga che riporta i quattro conteggi, quindi deve restare un solo
posto in cui il formato puo' cambiare.

Aggiunta 24/11 (turno 2): la formattazione del riepilogo per area (prima inline in
main) e' ora la funzione pura dettaglio_per_area(), provata in autotest: l'ordine
alfabetico delle chiavi deve restare stabile anche con piu' aree.

Aggiunta 24/11 (turno 3): la riga di un residuo noto (prima tre f-string identiche
inline in main per noto / NOTO-CATALOGATO / NOTO-SPARITO) e' ora la funzione pura
riga_noto(), provata in autotest: un solo posto in cui il formato puo' cambiare,
cosi' i tre casi restano allineati.

Aggiunta 24/11 (turno 4): le righe di stampa dei PNG 'da decidere' (una per PNG
col nome dell'area, piu' il riepilogo per area) erano un ciclo print inline in
main: ora sono la funzione pura righe_da_decidere(), provata in autotest, cosi'
l'ordine dei PNG e la presenza del riepilogo solo quando c'e' almeno un'area
restano verificabili senza leggere il disco.

Aggiunta 24/11 (turno 5): le tre righe dei residui noti (prima tre cicli print
inline in main, uno per stato) sono ora la funzione pura righe_noti(), provata in
autotest: l'ordine noto / NOTO-CATALOGATO / NOTO-SPARITO e il tag di ciascuno
restano verificabili senza leggere il catalogo.

Aggiunta 24/11 (turno 6): il raccolto dei PNG con glob e la conversione in
percorsi relativi a GRAFICA/ (prima un doppio ciclo inline in main) e' ora la
funzione png_trovati(), con glob e relpath iniettabili: l'autotest verifica
l'ordine e il formato relativo senza leggere il disco. E' l'ultimo blocco di
raccolta dati che restava inline in main().

Aggiunta 25/11: la composizione completa del report (noti, riga di sintesi,
righe da decidere) e' ora la funzione pura righe_report(), provata in autotest:
main() stampa solo le righe restituite, cosi' l'ordine delle tre sezioni resta
fissato in un punto solo.

Aggiunta 25/11 (turno 2): la classificazione completa in 4 gruppi (noti,
da_decidere, catalogati, spariti) e' ora la funzione pura analizza_png(),
provata in autotest: main() la usa al posto delle quattro chiamate inline.

Aggiunta 25/11 (turno 3): l'ordine/tag dei residui noti e' separato dalla
formattazione in voci_noti() pura con autotest; righe_noti() la usa per
costruire le righe, cosi' si prova lo stato senza dipendere dal testo.

Aggiunta 25/11 (turno 4): i percorsi citati da una singola voce di catalogo
sono estratti in file_usati_voce() pura con autotest; file_usati() la usa,
cosi' il caso stringa/lista/raw/vuoti si prova per voce senza catalogo intero.

Aggiunta 25/11 (turno 5): lo stato dei residui noti rispetto al catalogo
(catalogati, spariti) e' estratto in esito_noti() pura con autotest;
analizza_png() la usa, cosi' la precedenza catalogato-vince-su-sparito resta
provata in un punto solo.
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


def esito_noti(noti, usati):
    """Stato dei residui noti rispetto al catalogo: (catalogati, spariti) (puro).

    Unifica i due controlli che analizza_png faceva in sequenza: prima i noti
    finiti nel catalogo, poi quelli spariti dal disco ma non catalogati. Tenerli
    insieme rende provabile la regola di precedenza (catalogato vince su sparito).
    """
    catalogati = noti_catalogati(usati)
    spariti = noti_spariti(noti, catalogati)
    return catalogati, spariti


def analizza_png(fuori, usati):
    """Classifica i PNG fuori catalogo nei 4 gruppi del report (pura).

    Raccoglie la logica che main() faceva inline: noti/da_decidere dalla
    classificazione, catalogati dall'elenco usati, spariti dai noti non piu'
    sul disco e non finiti in catalogo. Un solo punto rende provabile l'intero
    stato senza leggere disco o catalogo.
    """
    noti, da_decidere = classifica(fuori)
    catalogati, spariti = esito_noti(noti, usati)
    return noti, da_decidere, catalogati, spariti


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


def dettaglio_per_area(per_area):
    """Formatta il riepilogo 'da decidere per area' (pura, ordine alfabetico).

    main() costruiva la stringa inline: estrarla rende provabile il formato e
    garantisce che le chiavi siano sempre ordinate, cosi' l'output non dipende
    dall'ordine di inserimento del dizionario.
    """
    return ", ".join(f"{k} {v}" for k, v in sorted(per_area.items()))


def file_usati_voce(e):
    """Percorsi citati da una singola voce di catalogo (campi 'file' e 'raw').

    'file' puo' essere stringa o lista; vuoti/assenti non citano nulla. Pura,
    cosi' l'autotest prova i casi per voce senza costruire un catalogo intero.
    """
    files = e.get("file") or []
    files = [files] if isinstance(files, str) else list(files)
    citati = [f for f in files if f]
    raw = e.get("raw")
    if raw:
        citati.append(raw)
    return citati


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
        usati.update(file_usati_voce(e))
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


def png_trovati(lib, pattern, globber=glob.glob, relativizza=os.path.relpath):
    """Percorsi relativi a GRAFICA/ di tutti i PNG trovati dai pattern (puro).

    main() aveva un doppio ciclo inline: per ogni pattern, glob ricorsivo e
    os.path.relpath di ogni risultato. Tenerlo qui, con glob e relpath passabili
    come parametri (default quelli reali), permette all'autotest di verificare
    ordine e formato relativo senza toccare il disco, e lascia un solo punto in
    cui cambia la raccolta dei PNG da esaminare. L'ordine e' quello di pattern e
    poi di glob: non viene riordinato, come prima.
    """
    trovati = []
    for pat in pattern:
        for g in globber(os.path.join(glob.escape(str(lib)), pat), recursive=True):
            trovati.append(relativizza(g, str(lib)))
    return trovati


def riga_sintesi(quanti, noti, catalogati, spariti):
    """Riga di riepilogo finale: un solo posto in cui si formatta (pura).

    main() stampa quattro conteggi in un f-string spezzato: tenerlo qui lo rende
    provabile senza leggere il disco e evita che la riga si rompa se un conteggio
    viene passato nell'ordine sbagliato (i nomi dei parametri sono il contratto).
    """
    return (
        f"png non catalogati: {quanti} (noti non-asset {noti},"
        f" noti catalogati {catalogati}, noti spariti {spariti})"
    )


def righe_da_decidere(pngs):
    """Righe di stampa dei PNG 'da decidere' (pura): una per PNG + riepilogo aree.

    main() stampava un ciclo 'per area: file' e poi la riga di riepilogo: tenere
    qui la costruzione delle righe rende provabili, senza disco, l'ordine dei PNG
    (quello ricevuto, non riordinato) e il fatto che il riepilogo compaia solo se
    c'e' almeno un'area, cosi' i due formati non divergono.
    """
    righe = [f"  {sorgente(f)}: {f}" for f in pngs]
    per_area = conteggio_per_sorgente(pngs)
    if per_area:
        righe.append(f"  da decidere per area: {dettaglio_per_area(per_area)}")
    return righe


def righe_report(noti, da_decidere, catalogati, spariti):
    """Report completo in ordine: noti, sintesi, da decidere (puro).

    main() stampava tre blocchi in sequenza: tenerli qui rende provabile che
    l'ordine sia noti -> riga_sintesi -> righe_da_decidere e che un solo
    punto controlli la composizione.
    """
    righe = righe_noti(noti, catalogati, spariti)
    righe.append(riga_sintesi(len(da_decidere), len(noti), len(catalogati), len(spariti)))
    righe.extend(righe_da_decidere(da_decidere))
    return righe


def riga_noto(tag, percorso, motivo):
    """Riga di un residuo noto, uguale per noto / NOTO-CATALOGATO / NOTO-SPARITO.

    main() stampava i tre casi con tre f-string identiche: tenerle in un solo
    posto rende provabile il formato (tag, percorso, motivo) e impedisce che una
    riga diverghi dalle altre quando si aggiunge un conteggio.
    """
    return f"  {tag}: {percorso} ({motivo})"


def voci_noti(noti, catalogati, spariti):
    """Terne (tag, percorso) dei residui noti, nell'ordine di stampa (pura).

    Separa l'ordine/tag dalla formattazione: righe_noti() aggiunge il motivo e
    il formato, qui si prova solo che lo stato giusto finisca nel gruppo giusto.
    """
    voci = [("noto non-asset", f) for f in noti]
    voci.extend(("NOTO-CATALOGATO", f) for f in catalogati)
    voci.extend(("NOTO-SPARITO", f) for f in spariti)
    return voci


def righe_noti(noti, catalogati, spariti):
    """Righe dei residui noti nei tre stati (pura), nell'ordine di stampa.

    main() aveva tre cicli print identici (uno per noto / NOTO-CATALOGATO /
    NOTO-SPARITO): tenerli in una sola funzione rende provabile, senza disco,
    l'ordine dei tre gruppi e il tag che spetta a ciascuno, cosi' una riga non
    puo' cambiare formato rispetto alle altre.
    """
    return [riga_noto(tag, f, NOTI_NON_ASSET[f]) for tag, f in voci_noti(noti, catalogati, spariti)]


def main() -> int:
    if not CATALOGO.exists():
        print(f"FALLITO: catalogo non trovato in {CATALOGO}")
        return 4
    cat = json.load(open(CATALOGO, encoding="utf-8"))
    usati = file_usati(cat)

    trovati = png_trovati(LIBRERIA, PATTERN)
    fuori = fuori_catalogo(trovati, usati)
    noti, da_decidere, catalogati, spariti = analizza_png(fuori, usati)
    for riga in righe_report(noti, da_decidere, catalogati, spariti):
        print(riga)
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
    # esito_noti: unifica catalogati+spariti con la stessa regola di precedenza;
    # il furgone catalogato non deve finire tra gli spariti.
    ok_esito = (
        esito_noti(tutti, []) == ([], [])
        and esito_noti(senza_furgone, []) == ([], [furgone])
        and esito_noti(senza_furgone, [furgone]) == ([furgone], [])
        and esito_noti(tutti, [furgone]) == ([furgone], [])
    )
    ok = ok and ok_esito
    print("autotest esito_noti: " + ("OK" if ok_esito else "FALLITO"))
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
    # file_usati_voce: una voce alla volta, 'file' stringa/lista, raw, vuoti ignorati.
    ok_voce = (
        file_usati_voce({"file": "GRAFICA/a.png", "raw": "raw/a.sglraw"})
        == ["GRAFICA/a.png", "raw/a.sglraw"]
        and file_usati_voce({"file": ["GRAFICA/b.png", "GRAFICA/c.png"]})
        == ["GRAFICA/b.png", "GRAFICA/c.png"]
        and file_usati_voce({"file": [], "raw": ""}) == []
        and file_usati_voce({}) == []
        and file_usati_voce({"file": ["", "GRAFICA/d.png"], "raw": ""})
        == ["GRAFICA/d.png"]
    )
    ok = ok and ok_voce
    print("autotest file_usati_voce: " + ("OK" if ok_voce else "FALLITO"))
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
    # riga_sintesi: formato esatto della riga finale (i quattro conteggi, nell'ordine
    # in cui main() li passa); un conteggio spostato cambierebbe il testo stampato.
    ok_riga = (
        riga_sintesi(0, 4, 0, 0)
        == "png non catalogati: 0 (noti non-asset 4, noti catalogati 0, noti spariti 0)"
        and riga_sintesi(3, 1, 2, 5)
        == "png non catalogati: 3 (noti non-asset 1, noti catalogati 2, noti spariti 5)"
    )
    ok = ok and ok_riga
    print("autotest riga_sintesi: " + ("OK" if ok_riga else "FALLITO"))
    # dettaglio_per_area: formato stabile, chiavi ordinate alfabeticamente.
    ok_dettaglio = (
        dettaglio_per_area({"sprite": 2, "scenario-oggetti": 1}) == "scenario-oggetti 1, sprite 2"
        and dettaglio_per_area({"sponsor": 1}) == "sponsor 1"
        and dettaglio_per_area({}) == ""
    )
    ok = ok and ok_dettaglio
    print("autotest dettaglio_per_area: " + ("OK" if ok_dettaglio else "FALLITO"))
    # riga_noto: stesso formato per i tre tag, motivo tra parentesi, due spazi iniziali.
    ok_noto = (
        riga_noto("noto non-asset", "GRAFICA/a.png", "motivo")
        == "  noto non-asset: GRAFICA/a.png (motivo)"
        and riga_noto("NOTO-CATALOGATO", "GRAFICA/b.png", "m2")
        == "  NOTO-CATALOGATO: GRAFICA/b.png (m2)"
        and riga_noto("NOTO-SPARITO", "GRAFICA/c.png", "m3")
        == "  NOTO-SPARITO: GRAFICA/c.png (m3)"
    )
    ok = ok and ok_noto
    print("autotest riga_noto: " + ("OK" if ok_noto else "FALLITO"))
    # righe_noti: ordine noto / NOTO-CATALOGATO / NOTO-SPARITO, tag corretto e
    # formato identico a riga_noto; elenco vuoto = nessuna riga.
    a = "GRAFICA/sprite/giocatore_pose/car-custom.png"
    b = "GRAFICA/sprite/giocatore_pose/car-custom-dx.png"
    c = "GRAFICA/sprite/giocatore_pose/car-custom-sx.png"
    motivo = NOTI_NON_ASSET[a]
    ok_noti_righe = righe_noti([], [], []) == []
    ok_noti_righe = ok_noti_righe and righe_noti([a], [b], [c]) == [
        f"  noto non-asset: {a} ({motivo})",
        f"  NOTO-CATALOGATO: {b} ({motivo})",
        f"  NOTO-SPARITO: {c} ({motivo})",
    ]
    ok_noti_righe = ok_noti_righe and righe_noti([c, a], [], []) == [
        f"  noto non-asset: {c} ({motivo})",
        f"  noto non-asset: {a} ({motivo})",
    ]
    ok = ok and ok_noti_righe
    print("autotest righe_noti: " + ("OK" if ok_noti_righe else "FALLITO"))
    # voci_noti: solo ordine e tag, senza motivo/formato (dati puri per righe_noti).
    ok_voci = voci_noti([], [], []) == []
    ok_voci = ok_voci and voci_noti([a], [b], [c]) == [
        ("noto non-asset", a), ("NOTO-CATALOGATO", b), ("NOTO-SPARITO", c),
    ]
    ok_voci = ok_voci and voci_noti([c, a], [], []) == [
        ("noto non-asset", c), ("noto non-asset", a),
    ]
    ok = ok and ok_voci
    print("autotest voci_noti: " + ("OK" if ok_voci else "FALLITO"))
    # righe_da_decidere: una riga per PNG nell'ordine ricevuto, riepilogo per area
    # solo se c'e' almeno un'area (elenco vuoto = nessuna riga).
    ok_righe = righe_da_decidere([]) == []
    ok_righe = ok_righe and righe_da_decidere([
        "GRAFICA/scenari/lv_01_neon/oggetti/a.png",
        "GRAFICA/sprite/traffico/c.png",
    ]) == [
        "  scenario-oggetti: GRAFICA/scenari/lv_01_neon/oggetti/a.png",
        "  sprite: GRAFICA/sprite/traffico/c.png",
        "  da decidere per area: scenario-oggetti 1, sprite 1",
    ]
    ok_righe = ok_righe and righe_da_decidere([
        "GRAFICA/sprite/giocatore_pose/nuova.png",
    ]) == [
        "  giocatore-area-bloccata: GRAFICA/sprite/giocatore_pose/nuova.png",
        "  da decidere per area: giocatore-area-bloccata 1",
    ]
    ok = ok and ok_righe
    print("autotest righe_da_decidere: " + ("OK" if ok_righe else "FALLITO"))
    # png_trovati: glob e relpath iniettati, nessun accesso al disco. L'ordine
    # segue pattern poi glob (non riordinato) e i percorsi sono relativi a lib.
    radice_finta = "/radice"
    glob_finto = lambda pat, recursive: {
        "/radice/GRAFICA/a/*.png": [
            "/radice/GRAFICA/a/z.png", "/radice/GRAFICA/a/a.png",
        ],
        "/radice/GRAFICA/b/*.png": ["/radice/GRAFICA/b/m.png"],
    }.get(pat, [])
    ok_trovati = png_trovati(
        radice_finta,
        ["GRAFICA/a/*.png", "GRAFICA/b/*.png", "GRAFICA/c/*.png"],
        globber=glob_finto,
        relativizza=lambda g, lib: os.path.relpath(g, lib),
    ) == [
        "GRAFICA/a/z.png", "GRAFICA/a/a.png", "GRAFICA/b/m.png",
    ]
    ok_trovati = ok_trovati and png_trovati(
        radice_finta, [], globber=glob_finto
    ) == []
    ok = ok and ok_trovati
    print("autotest png_trovati: " + ("OK" if ok_trovati else "FALLITO"))
    # analizza_png: raccoglie i 4 gruppi del report in un solo punto puro.
    # Caso misto: b e' noto fuori catalogo, un nuovo sprite resta da_decidere;
    # gli altri 3 noti non sono in fuori ne' in usati -> spariti.
    furgone = "GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png"
    altri_noti = sorted(f for f in NOTI_NON_ASSET if f != b)
    ok_analizza = analizza_png(
        [b, "GRAFICA/sprite/traffico/nuovo.png"],
        set(),
    ) == ([b], ["GRAFICA/sprite/traffico/nuovo.png"], [], altri_noti)
    # Caso catalogato: furgone nel catalogo -> compare in catalogati, non in spariti;
    # a,b,c presenti in fuori, nessun nuovo.
    ok_analizza = ok_analizza and analizza_png(
        [a, b, c],
        {furgone},
    ) == ([a, b, c], [], [furgone], [])
    # Caso sparito: furgone assente da fuori e da catalogo -> sparito.
    ok_analizza = ok_analizza and analizza_png(
        [a, b, c],
        set(),
    ) == ([a, b, c], [], [], [furgone])
    ok_analizza = ok_analizza and analizza_png([], set()) == ([], [], [], sorted(NOTI_NON_ASSET))
    ok = ok and ok_analizza
    print("autotest analizza_png: " + ("OK" if ok_analizza else "FALLITO"))
    # righe_report: composizione completa noti -> sintesi -> da_decidere, ordine fisso.
    ok_report = righe_report([], [], [], []) == [
        "png non catalogati: 0 (noti non-asset 0, noti catalogati 0, noti spariti 0)",
    ]
    ok_report = ok_report and righe_report([a], ["GRAFICA/sprite/traffico/x.png"], [b], [c]) == [
        f"  noto non-asset: {a} ({motivo})",
        f"  NOTO-CATALOGATO: {b} ({motivo})",
        f"  NOTO-SPARITO: {c} ({motivo})",
        "png non catalogati: 1 (noti non-asset 1, noti catalogati 1, noti spariti 1)",
        "  sprite: GRAFICA/sprite/traffico/x.png",
        "  da decidere per area: sprite 1",
    ]
    ok = ok and ok_report
    print("autotest righe_report: " + ("OK" if ok_report else "FALLITO"))
    return 0 if ok else 1


if __name__ == "__main__":
    if "--autotest" in sys.argv:
        sys.exit(autotest())
    sys.exit(main())
