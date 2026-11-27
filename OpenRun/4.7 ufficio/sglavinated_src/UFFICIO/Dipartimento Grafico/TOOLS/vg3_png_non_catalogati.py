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

Aggiunta 26/11: le prove esito_noti/analizza_png dell'autotest usavano ancora il
vecchio 'furgone' (definito piu' sotto e ormai fuori da NOTI_NON_ASSET): il test
si fermava con NameError. Ora usano le tre pose custom effettivamente note, con
un solo esempio coerente di 'sparito'/'catalogato' come nelle prove vicine.

Aggiunta 26/11 (turno 2): la sequenza che porta dai PNG trovati alle righe del
report (fuori_catalogo -> analizza_png -> righe_report, prima inline in main) e'
ora la funzione pura report_da_png(), provata in autotest: fissa in un punto
solo l'ordine delle tre fasi e prova che il report dipenda solo dai PNG trovati
e dai percorsi citati dal catalogo.

Aggiunta 26/11 (turno 3): il caricamento del catalogo (prima json.load(open(...))
inline in main, con il file lasciato al garbage collector) e' ora la funzione
carica_catalogo() con lettura iniettabile, provata in autotest: read_text chiude
subito il file e il parsing sta in un punto solo. L'output non cambia.

Aggiunta 26/11 (turno 4): la sequenza file_usati(cat) -> report_da_png(trovati,
usati), prima inline in main, e' ora la funzione pura report_da_dati(cat, trovati),
provata in autotest: fissa in un punto solo il passaggio dal catalogo letto alle
righe del report, cosi' main() non calcola piu' 'usati' a mano. L'output non cambia.

Aggiunta 26/11 (turno 5): l'ultima logica rimasta inline in main() (controllo
'CATALOGO esiste?', messaggio di errore e scelta del codice 4/0) e' ora la funzione
pura esito_main(esiste, cat, trovati, percorso), provata in autotest: il messaggio
di catalogo mancante e la policy d'uscita stanno in un punto solo. L'output non cambia.

Aggiunta 26/11 (turno 6): classifica() riceve l'elenco dei noti iniettabile
(default NOTI_NON_ASSET) invece di leggere sempre il globale, come le funzioni di
stampa rese pure nei turni prima; nota in testa e autotest dedicato (noti custom,
vuoto, default). La chiamata in analizza_png() usa il default: l'output non cambia.

Aggiunta 26/11 (turno 7): l'ultima logica rimasta inline in main() (il ciclo
'for riga in righe: print(riga)') e' ora la funzione pura testo_report(), provata
in autotest: Una riga per elemento, nessuna riga vuota in coda e stampa solo se
il testo non e' vuoto. L'output non cambia.

Aggiunta 27/11: il passo di stampa condizionale rimasto in main() ('if testo:
print(testo)') e' ora la funzione stampa_se_testo() con 'stampa' iniettabile,
provata in autotest: nessuna chiamata se il testo e' vuoto, una stampa del testo
altrimenti. E' l'ultimo passo di I/O di main(), reso provabile senza stdout.
L'output non cambia.

Aggiunta 27/11 (turno 5): la composizione rimasta inline in main()
('stampa_se_testo(testo_report(righe))') e' ora emetti_report(righe, stampa),
con la funzione di stampa iniettabile e autotest dedicato: prova che righe vuote
non stampino nulla e che righe piene stampino un solo testo con le righe unite.
main() non compone piu' nulla a mano. L'output non cambia.

Aggiunta 27/11 (turno 6): i sei default risolti inline all'inizio di
report_da_disco() (lib, catalogo, pattern, esiste_cat, carica, png_trov) sono ora
la funzione pura risolvi_default_disco(), provata in autotest: i valori espliciti
restano invariati e i None prendono i default reali, con '.exists()' chiamato sul
catalogo gia' risolto. L'output di main() non cambia.

Aggiunta 27/11 (turno 7): il caricamento condizionato rimasto in report_da_disco()
('cat = carica(...) if esiste else None' e 'trovati = png_trov(...) if esiste else
[]') e' ora la funzione pura carica_se_esiste(), provata in autotest: con esiste
False i due callable non vengono chiamati e si ottengono (None, []), con esiste
True il catalogo e i PNG sono quelli restituiti dai callable iniettati. La regola
'catalogo mancante -> non leggere nulla' sta in un punto solo. L'output non cambia.

Aggiunta 27/11 (turno 8): noti_mancanti() e noti_catalogati(), che leggevano
sempre il globale NOTI_NON_ASSET, ricevono l'elenco dei noti iniettabile
(default NOTI_NON_ASSET), come classifica(): l'autotest prova noti custom e
vuoti senza toccare il globale. Le chiamate in noti_spariti()/esito_noti()
lasciano il default, quindi l'output di main() non cambia.

Aggiunta 27/11 (turno 9): noti_spariti(), che chiamava noti_mancanti(noti) col
default, riceve a sua volta l'elenco dei noti iniettabile (default
NOTI_NON_ASSET) e lo passa a noti_mancanti(): completa cosi' la catena resa pura
nel turno prima. L'autotest prova noti custom, vuoti e default senza toccare il
globale. La chiamata in esito_noti() lascia il default, output invariato.

Aggiunta 27/11 (turno 10): le 34 righe di esito dell'autotest (prima concatenate
inline con 'autotest <nome>: ' + ('OK'/'FALLITO')) sono ora la funzione pura
riga_autotest(nome, ok), provata in autotest: il formato minuscolo di questo tool
resta identico e sta in un punto solo. L'output di main() non cambia.

Aggiunta 27/11 (turno 11): il messaggio 'FALLITO: catalogo non trovato in ...'
(prima un f-string inline nel ramo 'catalogo mancante' di esito_main()) e' ora la
funzione pura riga_catalogo_mancante(percorso), provata in autotest: il formato
sta in un punto solo, come promesso nella docstring di esito_main(). L'output
di main() non cambia.

Aggiunta 27/11 (turno 12): l'ultima decisione rimasta inline in autotest()
('return 0 if ok else 1') e' ora la funzione pura esito_autotest(ok), provata in
autotest: la policy del codice di uscita (True -> 0, False -> 1) sta in un punto
solo, come nel tool gemello vg3_raw_senza_catalogo.py. L'output non cambia.

Aggiunta 27/11 (turno 13): l'elenco ordinato dei noti 'tranne uno' (prima tre
comprensioni inline diverse in autotest(), con due che riordinavano e una no) e'
ora la funzione pura noti_tranne(), provata in autotest: filtro e ordine stanno
in un punto solo. Le prove usano lo stesso elenco atteso, main() non cambia.
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
}


def classifica(fuori, noti_noti=None):
    """Divide i PNG fuori catalogo in noti (con motivo) e nuovi da decidere.

    L'elenco dei residui noti e' iniettabile (default NOTI_NON_ASSET), come per
    le funzioni di stampa rese pure nei turni prima: cosi' l'autotest prova i
    casi 'noti custom' e 'nessun noto' senza leggere il globale. La chiamata in
    analizza_png() lascia il default, quindi l'output di main() non cambia.
    """
    if noti_noti is None:
        noti_noti = NOTI_NON_ASSET
    noti = [f for f in fuori if f in noti_noti]
    da_decidere = [f for f in fuori if f not in noti_noti]
    return noti, da_decidere


def noti_mancanti(presenti, noti_noti=None):
    """Residui noti che non sono piu' sul disco (erano attesi, ora spariti).

    Come classifica(), l'elenco dei noti e' iniettabile (default NOTI_NON_ASSET):
    l'autotest prova noti custom e vuoti senza leggere il globale. La chiamata in
    noti_spariti() lascia il default, quindi l'output di main() non cambia.
    """
    if noti_noti is None:
        noti_noti = NOTI_NON_ASSET
    visti = set(presenti)
    return [f for f in sorted(noti_noti) if f not in visti]


def noti_catalogati(usati, noti_noti=None):
    """Noti non-asset ora presenti nel catalogo (non piu' fuori catalogo).

    Un noto che finisce in CATALOGO_GRAFICO.json non appare piu' tra i PNG fuori
    catalogo: senza questo controllo veniva segnalato come NOTO-SPARITO, che e'
    fuorviante (il file puo' essere ancora su disco). Diagnosi informativa:
    il noto va tolto da NOTI_NON_ASSET o rivalutato.

    Come classifica() e noti_mancanti(), l'elenco dei noti e' iniettabile
    (default NOTI_NON_ASSET): l'autotest prova noti custom e vuoti senza leggere
    il globale. La chiamata in esito_noti() lascia il default: output invariato.
    """
    if noti_noti is None:
        noti_noti = NOTI_NON_ASSET
    visti = set(usati)
    return [f for f in sorted(noti_noti) if f in visti]


def noti_spariti(noti, catalogati, noti_noti=None):
    """Noti fuori catalogo spariti dal disco e NON finiti nel catalogo (puro).

    Un noto che e' andato in CATALOGO_GRAFICO.json non appare piu' tra i PNG fuori
    catalogo, ma puo' essere ancora su disco: stampare insieme NOTO-CATALOGATO e
    NOTO-SPARITO era fuorviante. Estrarre la regola che era inline in main() la
    rende provabile e documenta l'interazione tra i due elenchi.

    Come noti_mancanti()/noti_catalogati(), l'elenco dei noti e' iniettabile
    (default NOTI_NON_ASSET) e viene passato a noti_mancanti(): l'autotest prova
    noti custom e vuoti senza leggere il globale. La chiamata in esito_noti()
    lascia il default, quindi l'output di main() non cambia.
    """
    cat = set(catalogati)
    return [f for f in noti_mancanti(noti, noti_noti) if f not in cat]


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


def carica_catalogo(path, leggi=None):
    """Carica CATALOGO_GRAFICO.json come dict, con lettura iniettabile (pura).

    main() faceva json.load(open(path, encoding=...)): il file restava affidato
    al garbage collector invece di essere chiuso subito. Qui la lettura e' un
    callable (default Path.read_text, che chiude da solo) e il parsing sta in un
    solo punto, provabile in autotest senza toccare il disco. L'esistenza del
    file resta di competenza di main(), cosi' il messaggio di errore non cambia.
    """
    if leggi is None:
        leggi = lambda p: pathlib.Path(p).read_text(encoding="utf-8")
    return json.loads(leggi(path))


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


def report_da_png(trovati, usati):
    """Dai PNG trovati alle righe del report (puro), senza leggere il disco.

    main() faceva in sequenza fuori_catalogo(), analizza_png() e righe_report():
    tenerli in un solo punto fissa l'ordine delle tre fasi e rende provabile che
    il report dipenda solo dall'elenco dei PNG e dai percorsi citati dal catalogo.
    'trovati' sono i percorsi relativi restituiti da png_trovati(), 'usati'
    l'insieme restituito da file_usati().
    """
    fuori = fuori_catalogo(trovati, usati)
    noti, da_decidere, catalogati, spariti = analizza_png(fuori, usati)
    return righe_report(noti, da_decidere, catalogati, spariti)


def report_da_dati(cat, trovati):
    """Dal catalogo caricato e dai PNG trovati alle righe del report (pura).

    main() faceva in sequenza file_usati(cat) e report_da_png(trovati, usati):
    tenerli in un solo punto fissa l'ordine delle due fasi e rende provabile che
    il report dipenda solo dal catalogo letto e dall'elenco dei PNG trovati,
    senza leggere il disco. L'output non cambia.
    """
    usati = file_usati(cat)
    return report_da_png(trovati, usati)


def riga_catalogo_mancante(percorso):
    """Riga di errore per catalogo mancante (funzione pura).

    esito_main() costruiva il messaggio inline con un f-string nel ramo
    'catalogo non trovato': tenerlo qui mette il formato in un punto solo,
    provabile in autotest senza leggere il disco. L'output di main() non cambia.
    """
    return f"FALLITO: catalogo non trovato in {percorso}"


def esito_main(esiste, cat, trovati, percorso):
    """Righe da stampare e codice d'uscita finali di main() (puri).

    main() ha il controllo 'CATALOGO esiste?' col messaggio di errore e il
    ritorno 4 inline, poi il percorso normale che stampa il report e torna 0.
    Tenerlo qui rende provabile senza disco sia il ramo 'catalogo mancante'
    (una riga e codice 4) sia quello normale (righe del report e codice 0),
    cosi' il formato del messaggio e la policy dei codici stanno in un punto
    solo. 'cat' e 'trovati' sono ignorati quando 'esiste' e' False.
    """
    if not esiste:
        return [riga_catalogo_mancante(percorso)], 4
    return report_da_dati(cat, trovati), 0


def risolvi_default_disco(lib, catalogo, pattern, esiste_cat, carica, png_trov):
    """Risolve in un punto solo i default di report_da_disco() (pura).

    Prima i sei argomenti venivano controllati uno per uno con 'if x is None'
    all'inizio di report_da_disco(); tenerli qui rende provabile in autotest
    che un valore esplicito (percorso, pattern, esiste_cat, callable) non venga
    mai sostituito e che '.exists()' sia chiamato sul catalogo risolto, non su
    quello passato (che puo' essere None). L'output di main() non cambia.
    """
    cat_path = CATALOGO if catalogo is None else catalogo
    return (
        LIBRERIA if lib is None else lib,
        cat_path,
        PATTERN if pattern is None else pattern,
        cat_path.exists() if esiste_cat is None else esiste_cat,
        carica_catalogo if carica is None else carica,
        png_trovati if png_trov is None else png_trov,
    )


def carica_se_esiste(esiste, catalogo, carica, lib, pattern, png_trov):
    """Carica catalogo e PNG solo se il catalogo esiste (pura, callable iniettati).

    report_da_disco() faceva inline 'cat = carica(catalogo) if esiste else None'
    e 'trovati = png_trov(lib, pattern) if esiste else []': tenerli qui, con
    carica e png_trov passabili come parametri, rende provabile in autotest che
    un catalogo mancante non legga nulla (i callable non vengono chiamati) e che
    con catalogo presente si ottengano esattamente i dati restituiti. L'output
    di main() non cambia.
    """
    if not esiste:
        return None, []
    return carica(catalogo), png_trov(lib, pattern)


def report_da_disco(lib=None, catalogo=None, pattern=None, esiste_cat=None, carica=None, png_trov=None):
    """Legge catalogo e PNG da disco e restituisce (righe, codice) come main().

    Come vg3_raw_senza_catalogo.py e vg3_date_png_raw.py: la lettura disco in
    main() e' estratta in una funzione pura con dipendenze iniettabili. Output
    invariato rispetto al flusso precedente.
    """
    lib, catalogo, pattern, esiste_cat, carica, png_trov = risolvi_default_disco(
        lib, catalogo, pattern, esiste_cat, carica, png_trov
    )
    esiste = esiste_cat
    cat, trovati = carica_se_esiste(esiste, catalogo, carica, lib, pattern, png_trov)
    return esito_main(esiste, cat, trovati, catalogo)


def testo_report(righe):
    """Serializza le righe del report in un testo (funzione pura).

    Era il ciclo 'for riga in righe: print(riga)' in main(): qui si prova in
    isolamento che ordine e separatori (una riga per elemento, nessuna riga
    vuota in coda) restino invariati. main() stampa il testo solo se non vuoto,
    come faceva il ciclo (che non stampava nulla per una lista vuota).
    """
    return "\n".join(righe)


def stampa_se_testo(testo, stampa=print):
    """Stampa il testo del report solo se non vuoto, con 'stampa' iniettabile.

    Era l'ultimo passo di I/O inline in main() ('if testo: print(testo)'): tenerlo
    qui, con la funzione di stampa passabile come parametro, rende provabile in
    autotest la regola 'testo vuoto -> nessuna stampa' senza catturare stdout.
    L'output di main() non cambia.
    """
    if testo:
        stampa(testo)


def emetti_report(righe, stampa=print):
    """Serializza e stampa le righe del report, con 'stampa' iniettabile (pura).

    Era l'ultima composizione inline in main(): 'stampa_se_testo(testo_report(righe))'.
    Tenerla qui, con la funzione di stampa passabile come parametro, rende provabile
    in autotest che un report vuoto non stampi nulla e che un report con righe
    produca una sola stampa col testo gia' unito, senza catturare stdout.
    L'output di main() non cambia.
    """
    stampa_se_testo(testo_report(righe), stampa)


def main() -> int:
    righe, codice = report_da_disco()
    emetti_report(righe)
    return codice


def riga_autotest(nome, ok):
    """Riga di esito di una prova di autotest (funzione pura).

    Erano 34 concatenazioni inline nel corpo di autotest(): qui si prova in
    isolamento il formato ('OK' / 'FALLITO'), identico a prima (minuscolo
    'autotest', come in questo tool). Nessun disco, nessuna stampa.
    """
    return f"autotest {nome}: {'OK' if ok else 'FALLITO'}"


def esito_autotest(ok):
    """Codice di uscita dell'autotest: 0 se la prova e' OK, 1 altrimenti (pura).

    Era l'espressione inline 'return 0 if ok else 1' in coda a autotest(): qui si
    prova in isolamento la policy (True -> 0, False -> 1), come nel tool gemello
    vg3_raw_senza_catalogo.py. Nessun disco.
    """
    return 0 if ok else 1


def noti_tranne(noti, escluso):
    """Elenco ordinato dei noti tranne 'escluso' (funzione pura).

    Nelle prove un noto e' presente/catalogato e quindi va tolto dall'elenco dei
    'spariti' attesi: prima la comprensione era ripetuta in tre punti di
    autotest(), due con sorted() e uno senza, con il rischio che l'ordine atteso
    divergesse. Tenerla qui fissa filtro e ordine in un punto solo; 'noti' puo'
    essere una lista o un dict (le chiavi), come NOTI_NON_ASSET. main() non cambia.
    """
    return sorted(f for f in noti if f != escluso)


def autotest():
    """I noti restano tali; un PNG nuovo nella stessa cartella NON deve essere coperto."""
    noti, da_decidere = classifica([
        "GRAFICA/sprite/giocatore_pose/car-custom.png",
        "GRAFICA/sprite/giocatore_pose/car-custom-sx.png",
        "GRAFICA/sprite/traffico/nuovo_ridisegno.png",
    ])
    ok = (
        len(noti) == 2
        and da_decidere == ["GRAFICA/sprite/traffico/nuovo_ridisegno.png"]
    )
    print(riga_autotest("classifica", ok))
    # classifica con elenco di noti iniettabile: un noto custom viene riconosciuto,
    # un PNG fuori dall'elenco resta da decidere; con elenco vuoto tutti sono da
    # decidere; il default (None) resta NOTI_NON_ASSET e non cambia l'output.
    noti_custom = {
        "GRAFICA/sprite/giocatore_pose/car-custom.png": "motivo custom",
    }
    ok_iniet = (
        classifica(
            ["GRAFICA/sprite/giocatore_pose/car-custom.png", "GRAFICA/sprite/traffico/nuovo.png"],
            noti_noti=noti_custom,
        ) == (
            ["GRAFICA/sprite/giocatore_pose/car-custom.png"],
            ["GRAFICA/sprite/traffico/nuovo.png"],
        )
        and classifica(
            ["GRAFICA/sprite/giocatore_pose/car-custom.png"], noti_noti={}
        ) == ([], ["GRAFICA/sprite/giocatore_pose/car-custom.png"])
        and classifica(["GRAFICA/sprite/giocatore_pose/car-custom.png"])
        == (["GRAFICA/sprite/giocatore_pose/car-custom.png"], [])
    )
    ok = ok and ok_iniet
    print(riga_autotest("classifica_iniettabile", ok_iniet))
    ok_spariti = (
        noti_mancanti([
            "GRAFICA/sprite/giocatore_pose/car-custom.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-dx.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-sx.png",
        ]) == []
        and noti_mancanti([
            "GRAFICA/sprite/giocatore_pose/car-custom.png",
            "GRAFICA/sprite/giocatore_pose/car-custom-dx.png",
        ]) == ["GRAFICA/sprite/giocatore_pose/car-custom-sx.png"]
    )
    ok = ok and ok_spariti
    print(riga_autotest("noti_mancanti", ok_spariti))
    ok_catalogati = (
        noti_catalogati({"GRAFICA/sprite/giocatore_pose/car-custom.png"})
        == ["GRAFICA/sprite/giocatore_pose/car-custom.png"]
        and noti_catalogati({"GRAFICA/sprite/traffico/altro.png"}) == []
    )
    ok = ok and ok_catalogati
    print(riga_autotest("noti_catalogati", ok_catalogati))
    # noti iniettabile: noti_mancanti()/noti_catalogati(), come classifica(), non
    # leggono sempre il globale. Con noti custom cambia quali noti contano, con
    # elenco vuoto non ne conta nessuno, con None resta NOTI_NON_ASSET (default,
    # che e' cio' che usano noti_spariti()/esito_noti() e quindi main()).
    noto_custom = "GRAFICA/sprite/giocatore_pose/car-custom.png"
    ok_noti_iniet = (
        noti_mancanti([], noti_custom) == [noto_custom]
        and noti_mancanti([noto_custom], noti_custom) == []
        and noti_catalogati([noto_custom], noti_custom) == [noto_custom]
        and noti_catalogati([], noti_custom) == []
        and noti_mancanti([], {}) == []
        and noti_catalogati([noto_custom], {}) == []
        and noti_mancanti([]) == sorted(NOTI_NON_ASSET)
        and noti_catalogati(sorted(NOTI_NON_ASSET)) == sorted(NOTI_NON_ASSET)
    )
    ok = ok and ok_noti_iniet
    print(riga_autotest("noti_iniettabile", ok_noti_iniet))
    # noti_spariti: un noto finito nel catalogo non e' sparito (non va stampato
    # due volte come NOTO-CATALOGATO e NOTO-SPARITO); uno assente da entrambi si.
    esempio = "GRAFICA/sprite/giocatore_pose/car-custom-sx.png"
    tutti = sorted(NOTI_NON_ASSET)
    senza_esempio = noti_tranne(tutti, esempio)
    ok_spariti = (
        noti_spariti(tutti, []) == []
        and noti_spariti(senza_esempio, []) == [esempio]
        and noti_spariti(senza_esempio, [esempio]) == []
    )
    ok = ok and ok_spariti
    print(riga_autotest("noti_spariti", ok_spariti))
    # noti_spariti iniettabile: come noti_mancanti/noti_catalogati, legge l'elenco
    # dei noti dal parametro (default NOTI_NON_ASSET). Con un noto custom cambia
    # quali noti possono risultare spariti, con elenco vuoto nessuno lo e', e il
    # default resta NOTI_NON_ASSET (cio' che usa esito_noti() e quindi main()).
    noto_custom = "GRAFICA/sprite/giocatore_pose/car-custom.png"
    noti_custom_mappa = {noto_custom: "motivo custom"}
    ok_spariti_iniet = (
        noti_spariti([], [], noti_custom_mappa) == [noto_custom]
        and noti_spariti([noto_custom], [], noti_custom_mappa) == []
        and noti_spariti([], [noto_custom], noti_custom_mappa) == []
        and noti_spariti([], [], {}) == []
        and noti_spariti(tutti, []) == []
    )
    ok = ok and ok_spariti_iniet
    print(riga_autotest("noti_spariti_iniettabile", ok_spariti_iniet))
    # esito_noti: unifica catalogati+spariti con la stessa regola di precedenza;
    # un noto finito nel catalogo non deve finire tra gli spariti. Usa lo stesso
    # esempio delle prove qui sopra: il vecchio furgone non e' piu' un noto.
    ok_esito = (
        esito_noti(tutti, []) == ([], [])
        and esito_noti(senza_esempio, []) == ([], [esempio])
        and esito_noti(senza_esempio, [esempio]) == ([esempio], [])
        and esito_noti(tutti, [esempio]) == ([esempio], [])
    )
    ok = ok and ok_esito
    print(riga_autotest("esito_noti", ok_esito))
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
    print(riga_autotest("sorgente", ok_sorgente))
    ok_bloccata = (
        area_bloccata("GRAFICA/sprite/giocatore_pose/auto_nuova.png")
        and area_bloccata("GRAFICA/sprite/giocatore/auto_nuova.png")
        and not area_bloccata("GRAFICA/sprite/traffico/nuovo.png")
        and not area_bloccata("GRAFICA/sprite/giocatore_pose_extra/nuovo.png")
    )
    ok = ok and ok_bloccata
    print(riga_autotest("area_bloccata", ok_bloccata))
    ok_conteggio = conteggio_per_sorgente([
        "GRAFICA/scenari/lv_01_neon/oggetti/a.png",
        "GRAFICA/scenari/lv_02_atomico/oggetti/b.png",
        "GRAFICA/scenari/lv_04_cromo/skyline.png",
        "GRAFICA/sprite/traffico/c.png",
    ]) == {"scenario-oggetti": 2, "scenario-skyline": 1, "sprite": 1}
    ok = ok and ok_conteggio
    print(riga_autotest("conteggio_per_sorgente", ok_conteggio))
    ok_vuoti = classifica([]) == ([], []) and conteggio_per_sorgente([]) == {}
    ok = ok and ok_vuoti
    print(riga_autotest("casi_vuoti", ok_vuoti))
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
    print(riga_autotest("file_usati_voce", ok_voce))
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
    print(riga_autotest("file_usati", ok_usati))
    ok_usati_vuoti = file_usati({"elementi": []}) == set()
    ok = ok and ok_usati_vuoti
    print(riga_autotest("file_usati_vuoti", ok_usati_vuoti))
    # fuori_catalogo: un PNG nuovo resta fuori, uno catalogato e' escluso,
    # l'ordine resta stabile e un doppio passaggio resta visibile (non si dedupa).
    ok_fuori = fuori_catalogo(
        ["GRAFICA/z.png", "GRAFICA/a.png", "GRAFICA/catalogato.png", "GRAFICA/a.png"],
        {"GRAFICA/catalogato.png"},
    ) == ["GRAFICA/a.png", "GRAFICA/a.png", "GRAFICA/z.png"]
    ok_fuori = ok_fuori and fuori_catalogo([], {"GRAFICA/a.png"}) == []
    ok = ok and ok_fuori
    print(riga_autotest("fuori_catalogo", ok_fuori))
    # riga_sintesi: formato esatto della riga finale (i quattro conteggi, nell'ordine
    # in cui main() li passa); un conteggio spostato cambierebbe il testo stampato.
    ok_riga = (
        riga_sintesi(0, 4, 0, 0)
        == "png non catalogati: 0 (noti non-asset 4, noti catalogati 0, noti spariti 0)"
        and riga_sintesi(3, 1, 2, 5)
        == "png non catalogati: 3 (noti non-asset 1, noti catalogati 2, noti spariti 5)"
    )
    ok = ok and ok_riga
    print(riga_autotest("riga_sintesi", ok_riga))
    # dettaglio_per_area: formato stabile, chiavi ordinate alfabeticamente.
    ok_dettaglio = (
        dettaglio_per_area({"sprite": 2, "scenario-oggetti": 1}) == "scenario-oggetti 1, sprite 2"
        and dettaglio_per_area({"sponsor": 1}) == "sponsor 1"
        and dettaglio_per_area({}) == ""
    )
    ok = ok and ok_dettaglio
    print(riga_autotest("dettaglio_per_area", ok_dettaglio))
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
    print(riga_autotest("riga_noto", ok_noto))
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
    print(riga_autotest("righe_noti", ok_noti_righe))
    # voci_noti: solo ordine e tag, senza motivo/formato (dati puri per righe_noti).
    ok_voci = voci_noti([], [], []) == []
    ok_voci = ok_voci and voci_noti([a], [b], [c]) == [
        ("noto non-asset", a), ("NOTO-CATALOGATO", b), ("NOTO-SPARITO", c),
    ]
    ok_voci = ok_voci and voci_noti([c, a], [], []) == [
        ("noto non-asset", c), ("noto non-asset", a),
    ]
    ok = ok and ok_voci
    print(riga_autotest("voci_noti", ok_voci))
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
    print(riga_autotest("righe_da_decidere", ok_righe))
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
    print(riga_autotest("png_trovati", ok_trovati))
    # analizza_png: raccoglie i 4 gruppi del report in un solo punto puro.
    # Caso misto: b e' noto fuori catalogo, un nuovo sprite resta da_decidere;
    # gli altri due noti non sono in fuori ne' in usati -> spariti.
    altri_noti = noti_tranne(NOTI_NON_ASSET, b)
    ok_analizza = analizza_png(
        [b, "GRAFICA/sprite/traffico/nuovo.png"],
        set(),
    ) == ([b], ["GRAFICA/sprite/traffico/nuovo.png"], [], altri_noti)
    # Caso catalogato: b nel catalogo -> compare in catalogati, non in spariti;
    # a,b,c presenti in fuori, nessun nuovo.
    ok_analizza = ok_analizza and analizza_png(
        [a, b, c],
        {b},
    ) == ([a, b, c], [], [b], [])
    # Caso sparito: a assente da fuori e da catalogo -> sparito.
    ok_analizza = ok_analizza and analizza_png(
        [b, c],
        set(),
    ) == ([b, c], [], [], [a])
    ok_analizza = ok_analizza and analizza_png([], set()) == ([], [], [], sorted(NOTI_NON_ASSET))
    ok = ok and ok_analizza
    print(riga_autotest("analizza_png", ok_analizza))
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
    print(riga_autotest("righe_report", ok_report))
    # report_da_png: la sequenza completa e' la composizione delle tre fasi; un
    # PNG nuovo resta 'da decidere', un noto catalogato non finisce tra gli
    # spariti e i noti assenti da tutto finiscono tra gli spariti.
    x = "GRAFICA/sprite/traffico/x.png"
    ok_pipeline = report_da_png([], set()) == righe_report([], [], [], sorted(NOTI_NON_ASSET))
    ok_pipeline = ok_pipeline and report_da_png([x], set()) == righe_report(
        [], [x], [], sorted(NOTI_NON_ASSET),
    )
    ok_pipeline = ok_pipeline and report_da_png([b, x], {b}) == righe_report(
        [], [x], [b], noti_tranne(NOTI_NON_ASSET, b),
    )
    ok = ok and ok_pipeline
    print(riga_autotest("report_da_png", ok_pipeline))
    # carica_catalogo: parsing con lettura iniettabile, nessun file reale aperto.
    ok_carica = (
        carica_catalogo("finto.json", leggi=lambda p: '{"elementi": [{"id": "x"}]}')
        == {"elementi": [{"id": "x"}]}
        and carica_catalogo("finto.json", leggi=lambda p: '{"elementi": []}')
        == {"elementi": []}
    )
    ok = ok and ok_carica
    print(riga_autotest("carica_catalogo", ok_carica))
    # report_da_dati: catalogo + PNG trovati -> righe, senza disco. Un catalogo
    # vuoto lascia il PNG tra i 'da decidere'; un PNG citato dal catalogo (campo
    # 'file') non finisce tra i da_decidere. Deve essere la composizione esatta
    # di file_usati() e report_da_png().
    ok_dati = (
        report_da_dati({"elementi": []}, [x])
        == report_da_png([x], set())
        and report_da_dati({"elementi": [{"file": x}]}, [x])
        == report_da_png([x], {x})
        and report_da_dati({"elementi": []}, []) == righe_report([], [], [], sorted(NOTI_NON_ASSET))
    )
    ok = ok and ok_dati
    print(riga_autotest("report_da_dati", ok_dati))
    # riga_catalogo_mancante: formato del messaggio di catalogo assente, identico
    # al vecchio f-string inline in esito_main(), provato senza disco.
    ok_mancante = (
        riga_catalogo_mancante("CATALOGO.json")
        == "FALLITO: catalogo non trovato in CATALOGO.json"
        and riga_catalogo_mancante(CATALOGO)
        == f"FALLITO: catalogo non trovato in {CATALOGO}"
    )
    ok = ok and ok_mancante
    print(riga_autotest("riga_catalogo_mancante", ok_mancante))
    # esito_main: ramo catalogo mancante (una riga + codice 4, cat/trovati
    # ignorati) e ramo normale (righe del report + codice 0), senza disco.
    mancante = [riga_catalogo_mancante(CATALOGO)]
    ok_main = esito_main(False, None, [], CATALOGO) == (mancante, 4)
    ok_main = ok_main and esito_main(
        False, {"elementi": [{"file": x}]}, [x], CATALOGO,
    ) == (mancante, 4)
    ok_main = ok_main and esito_main(
        True, {"elementi": []}, [x], CATALOGO,
    ) == (report_da_dati({"elementi": []}, [x]), 0)
    ok = ok and ok_main
    print(riga_autotest("esito_main", ok_main))
    # testo_report: serializzazione provata in isolamento, senza disco.
    ok_testo = (
        testo_report(["riga 1", "riga 2"]) == "riga 1\nriga 2"
        and testo_report(["unica"]) == "unica"
        and testo_report([]) == ""
    )
    ok = ok and ok_testo
    print(riga_autotest("testo_report", ok_testo))
    # stampa_se_testo: testo vuoto -> nessuna stampa, testo pieno -> una stampa
    # col testo esatto; 'stampa' iniettato registra le chiamate senza stdout.
    chiamate = []
    registratore = lambda t: chiamate.append(t)
    stampa_se_testo("", registratore)
    ok_stampa = chiamate == []
    stampa_se_testo("riga 1\nriga 2", registratore)
    ok_stampa = ok_stampa and chiamate == ["riga 1\nriga 2"]
    ok = ok and ok_stampa
    print(riga_autotest("stampa_se_testo", ok_stampa))
    # emetti_report: composizione testo_report + stampa_se_testo provata in
    # isolamento; righe vuote -> nessuna stampa, righe piene -> una stampa col
    # testo unito; 'stampa' iniettato registra le chiamate senza stdout.
    chiamate = []
    registratore = lambda t: chiamate.append(t)
    emetti_report([], registratore)
    ok_emetti = chiamate == []
    emetti_report(["riga 1", "riga 2"], registratore)
    ok_emetti = ok_emetti and chiamate == ["riga 1\nriga 2"]
    ok = ok and ok_emetti
    print(riga_autotest("emetti_report", ok_emetti))
    # risolvi_default_disco: valori espliciti preservati (percorso, pattern,
    # esiste_cat e callable) e None -> default reali, senza leggere il disco
    # perche' esiste_cat viene sempre iniettato. Il catalogo finto non esiste:
    # con esiste=False report_da_disco restituisce il ramo 'catalogo mancante'.
    finto_png = lambda lib, pattern: ["GRAFICA/x.png"]
    finto_carica = lambda path: {"elementi": []}
    percorso_finto = pathlib.Path("CATALOGO_FINTO.json")
    lib_finta = pathlib.Path("radice_finta")
    pattern_finto = ["GRAFICA/finta/*.png"]
    ok_default = risolvi_default_disco(
        lib_finta, percorso_finto, pattern_finto, False, finto_carica, finto_png,
    ) == (
        lib_finta, percorso_finto, pattern_finto, False, finto_carica, finto_png,
    )
    ok_default = ok_default and risolvi_default_disco(
        None, None, None, False, None, None,
    ) == (LIBRERIA, CATALOGO, PATTERN, False, carica_catalogo, png_trovati)
    ok_default = ok_default and risolvi_default_disco(
        None, CATALOGO, None, True, None, None,
    )[3] is True
    ok_default = ok_default and report_da_disco(
        lib_finta, percorso_finto, pattern_finto, False, finto_carica, finto_png,
    ) == esito_main(False, None, [], percorso_finto)
    ok = ok and ok_default
    print(riga_autotest("risolvi_default_disco", ok_default))
    # carica_se_esiste: con esiste False i due callable non vanno chiamati e il
    # risultato e' (None, []); con esiste True tornano catalogo e PNG iniettati.
    chiamate_carica = []
    chiamate_png = []
    finto_carica2 = lambda path: (chiamate_carica.append(path) or {"elementi": []})
    finto_png2 = lambda lib, pattern: (chiamate_png.append((lib, pattern)) or [x])
    ok_carica_se = carica_se_esiste(
        False, CATALOGO, finto_carica2, LIBRERIA, PATTERN, finto_png2,
    ) == (None, [])
    ok_carica_se = ok_carica_se and chiamate_carica == [] and chiamate_png == []
    ok_carica_se = ok_carica_se and carica_se_esiste(
        True, CATALOGO, finto_carica2, LIBRERIA, PATTERN, finto_png2,
    ) == ({"elementi": []}, [x])
    ok_carica_se = ok_carica_se and chiamate_carica == [CATALOGO]
    ok_carica_se = ok_carica_se and chiamate_png == [(LIBRERIA, PATTERN)]
    ok = ok and ok_carica_se
    print(riga_autotest("carica_se_esiste", ok_carica_se))
    # riga_autotest: formato 'autotest <nome>: OK/FALLITO' identico a prima delle
    # 34 righe inline; provato in isolamento (funzione pura, nessun disco).
    ok_riga_autotest = (
        riga_autotest("x", True) == "autotest x: OK"
        and riga_autotest("x", False) == "autotest x: FALLITO"
    )
    ok = ok and ok_riga_autotest
    print(riga_autotest("riga_autotest", ok_riga_autotest))
    # esito_autotest: policy del codice di uscita, provata in isolamento (pura,
    # nessun disco): ok True -> 0, ok False -> 1, come nel tool gemello.
    ok_esito_autotest = esito_autotest(True) == 0 and esito_autotest(False) == 1
    ok = ok and ok_esito_autotest
    print(riga_autotest("esito_autotest", ok_esito_autotest))
    # noti_tranne: elenco ordinato dei noti senza l'escluso, provato in isolamento
    # (puro). Con lista non ordinata l'ordine atteso resta alfabetico; con un
    # escluso assente l'elenco e' comunque ordinato; elemento assente -> vuoto.
    ok_noti_tranne = (
        noti_tranne([], "x") == []
        and noti_tranne(["b", "a", "c"], "b") == ["a", "c"]
        and noti_tranne(["b", "a", "c"], "z") == ["a", "b", "c"]
        and noti_tranne({"b": "m", "a": "m"}, "b") == ["a"]
    )
    ok = ok and ok_noti_tranne
    print(riga_autotest("noti_tranne", ok_noti_tranne))
    return esito_autotest(ok)


if __name__ == "__main__":
    if "--autotest" in sys.argv:
        sys.exit(autotest())
    sys.exit(main())
