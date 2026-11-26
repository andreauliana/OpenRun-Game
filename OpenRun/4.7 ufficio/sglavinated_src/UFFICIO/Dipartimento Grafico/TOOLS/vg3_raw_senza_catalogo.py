#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico), copertura inversa: ogni .sglraw che il motore legge deve
# comparire nel catalogo (campo "raw" di un elemento), cosi' licenza e provenienza
# restano tracciate. I due controlli esistenti guardano l'altro verso:
#   check_catalogo_grafico.py -> PNG di GRAFICA devono essere catalogati
#   vg3_date_png_raw.py       -> PNG di GRAFICA vs raw del motore
# Qui si parte dai raw e si cerca l'elemento di catalogo.
# Aggiunta 22/11: si segnalano anche i riferimenti "raw" del catalogo che non
# puntano a un file esistente (spiegano perche' le voci con 'raw' possono essere
# piu' dei raw presenti su disco) e i raw citati da piu' di una voce.
# I riferimenti mancanti NOTI (pose auto di Andrea, area bloccata) sono
# distinti da quelli nuovi/da decidere: i primi hanno motivo, i secondi no.
# Aggiunta 22/11 (turno 11:01): si segnalano anche i riferimenti 'raw' il cui
# percorso non sta sotto sprite_raw/. Un riferimento alla vecchia sede (o con
# '..', percorso assoluto o estensione sbagliata) non risulta "mancante" se il
# file esiste altrove e resterebbe invisibile all'audit; e' informativo, non
# cambia l'exit.
# Aggiunta 22/11 (turno 11:58): il riepilogo distingue le voci di catalogo con un
# raw dai riferimenti raw distinti (due voci hanno raw_elenco da 4: prima il
# conteggio diceva "voci con raw: 159" quando le voci sono meno); inoltre si
# segnala un raw "noto mancante" ricomparso su disco, perche' l'elenco NOTI vada
# aggiornato. Anche queste sono diagnosi informative, non cambiano l'exit.
# Aggiunta 23/11: si distingue un raw citato da due voci distinte (duplicato) da
# uno che una sola voce elenca due volte ('raw' e 'raw_elenco', o due voci in
# raw_elenco): prima entrambi finivano in 'CITATO DA PIU' VOCI', quando il secondo
# e' un errore di una singola voce, non un conflitto fra voci. Informativo, exit
# invariato.
# Aggiunta 23/11 (turno 10:01): lo stesso file citato con due scritture diverse
# ('./a/x.sglraw', 'a//x.sglraw', 'a/b/../x.sglraw') sfuggiva ai duplicati e
# dava un falso 'RAW NON CATALOGATO' contro il percorso canonico su disco. Le
# scritture vengono normalizzate per il confronto e i gruppi sospetti si
# segnalano. Informativo, exit invariato.
# Aggiunta 23/11 (turno 3, 11:03): il conteggio delle voci di catalogo con un raw
# (prima inline in analizza()) e' estratto in conta_voci_con_raw(), funzione pura
# con autotest dedicato: il caso di una voce con solo raw_elenco, raw vuoto o
# non-stringa e' coperto in isolamento. Logica e output invariati.
# Aggiunta 24/11: la raccolta dei riferimenti raw del catalogo (prima inline in
# analizza()) e' estratta in raccogli_riferimenti(), funzione pura con autotest;
# raw vuoto/non-stringa in 'raw' o 'raw_elenco' ignorati. Output invariato.
# Aggiunta 24/11 (turno 2, 11:13): le righe di sintesi finale (prima f-string
# sparse in main()) sono estratte in righe_riepilogo(), funzione pura con
# autotest su ordine e formato; main() si limita a stamparle. Output invariato.
# Aggiunta 24/11 (turno 1, 12:06): le righe di stampa dei riferimenti mancanti
# (prima due cicli print in main()) sono estratte in righe_inesistenti(),
# funzione pura con autotest su formato e ordine noti/nuovi. Output invariato.
# Aggiunta 24/11 (turno 1, 13:03): la riga dei noti raw-mancanti ricomparsi
# (prima f-string nel ciclo di main()) e' estratta in righe_risolti(), funzione
# pura con autotest su formato e lista vuota. Output invariato.
# Aggiunta 24/11 (turno 2, 13:53): le tre righe dei rilievi su raw citati piu'
# volte / con piu' scritture (prima tre cicli print in main()) sono estratte in
# righe_classificazione(), funzione pura con autotest su formato, ordine delle
# chiavi e caso vuoto. Output invariato.
# Aggiunta 24/11 (turno 2, 14:43): le due righe di stampa rimaste inline in main()
# (raw su disco senza voce e riferimenti 'raw' con percorso sospetto) sono
# estratte in righe_senza_voce() e righe_sospetti(), funzioni pure con autotest
# su formato; main() si limita a stamparle. Output invariato.
# Aggiunta 25/11: la composizione completa del report (ordine sezioni + calcolo
# noti/nuovi/validi/sospetti/risolti) e' estratta in righe_report(), funzione
# pura con autotest; main() stampa solo le righe. Output invariato.
# Aggiunta 25/11 (turno 09:14): il confronto canonico che trova i raw su disco
# senza voce (prima inline in analizza()) e' estratto in raw_senza_voce(),
# funzione pura con autotest; analizza() la chiama. Output invariato.
# Aggiunta 25/11 (turno 10:37): il confronto che trova i riferimenti del catalogo
# senza file su disco (prima inline in analizza()) e' estratto in
# riferimenti_inesistenti(), funzione pura con autotest; analizza() la chiama.
# Output invariato.
# Aggiunta 25/11 (turno 11:37): le classificazioni intermedie del report (noti/
# nuovi, validi/sospetti, risolti) sono estratte in classifica_report(), funzione
# pura con autotest; righe_report() la usa. Output invariato.
# Aggiunta 25/11 (turno 12:31): la policy di exit code e' estratta in
# codice_uscita(), funzione pura con autotest; main() la usa. Output invariato.
# Aggiunta 26/11 (turno 07:47): la trasformazione dei Path dei raw su disco in
# path relativi posix ordinati (prima generatore inline in main()) e' estratta
# in path_raw_relativi(), funzione pura con autotest; main() la usa. Il disco e'
# acceso dal chiamante (rglob), la funzione trasforma solo i Path ricevuti.
# Output invariato.
# Aggiunta 26/11 (turno 08:36): il callable inline in main() che verifica
# l'esistenza di un raw su disco (lambda con RADICE / r, passata come 'esiste'
# ad analizza()) e' estratto in esiste_raw(), funzione pura con autotest su
# percorso presente/assente; main() la usa. Output invariato.
# Aggiunta 26/11 (turno 09:27): il caricamento del catalogo (json.loads(read_text)
# inline in main()) e' estratto in carica_catalogo(path, leggi=None), con lettura
# iniettabile e autotest dedicato (nessun file reale aperto); main() la usa.
# Output invariato.
# Aggiunta 26/11 (turno 09:50): l'orchestrazione di main() (analizza ->
# righe_report -> codice_uscita) e' estratta in report_da_dati(), funzione pura
# con autotest su dati sintetici e 'esiste' iniettabile; main() legge il disco e
# stampa solo le righe ricevute. Output invariato.
# Aggiunta 26/11 (turno 10:31): la scansione dei .sglraw su disco (prima
# path_raw_relativi(RADICE, RAW_ROOT.rglob("*.sglraw")) inline in main()) e'
# estratta in raw_su_disco(radice, raw_root, rglob=None), con rglob iniettabile e
# autotest dedicato senza toccare il disco; main() la usa. Output invariato.
# Aggiunta 26/11 (turno 17:45): classifica_inesistenti() riceve 'motivi' (default
# NOTI_RAW_MANCANTI) invece di leggere il globale, come fanno gia' le funzioni
# che stampano le righe; autotest dedicato con elenco di noti diverso. Output
# invariato.
# Non scrive nulla: solo diagnosi. Exit 0 se ogni raw su disco e' tracciato, 1
# altrimenti; le segnalazioni nuove sono informative e non cambiano l'exit code.
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest
import json
import sys
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
CATALOGO = RADICE / "GRAFICA/CATALOGO_GRAFICO.json"
RAW_ROOT = RADICE / "motore_nuovo/dati/sprite_raw"


def classifica_duplicati(riferimenti):
    """Divide i raw citati piu' volte in conflitti fra voci e ripetizioni in una voce.

    riferimenti e' path raw -> lista di id di catalogo, con una entry per
    riferimento: un elemento che ha 'raw' e 'raw_elenco' puntanti allo stesso file
    dara' il suo id due volte. Ritorna (fra_voci, in_voce):
    - fra_voci: raw con almeno due id DISTINTI (conflitto vero fra due voci);
    - in_voce:  raw con un solo id ripetuto (errore di una singola voce).
    Entrambi sono informativi: non cambiano l'exit.
    """
    fra_voci = {}
    in_voce = {}
    for r, ids in riferimenti.items():
        distinti = set(ids)
        if len(distinti) > 1:
            fra_voci[r] = sorted(distinti)
        elif len(ids) > 1:
            in_voce[r] = sorted(distinti)
    return fra_voci, in_voce


def normalizza_raw(r):
    """Forma canonica di un percorso raw, per confronti (funzione pura).

    './a/x.sglraw', 'a//x.sglraw' e 'a/b/../x.sglraw' puntano allo stesso file
    di 'a/x.sglraw': come stringhe pero' sono diversi, quindi il confronto con i
    raw su disco darebbe un falso 'RAW NON CATALOGATO' e due voci sullo stesso
    file non risulterebbero duplicate. Ritorna None se il valore non e' una
    stringa o e' vuoto (il chiamante lo ignora).
    """
    if not isinstance(r, str) or not r.strip():
        return None
    parti = []
    for p in r.replace("\\", "/").split("/"):
        if p in ("", "."):
            continue
        if p == ".." and parti and parti[-1] != "..":
            parti.pop()
            continue
        parti.append(p)
    return "/".join(parti)


def classifica_scritture(riferimenti):
    """Raggruppa i riferimenti raw per percorso canonico (funzione pura).

    Ritorna solo i percorsi canonici citati con piu' di una scrittura diversa:
    path canonico -> lista ordinata delle scritture trovate nel catalogo. La
    prima scrittura (quella gia' canonica) e' di solito quella corretta. E'
    una diagnosi informativa, non cambia l'exit.
    """
    gruppi = {}
    for r in riferimenti:
        n = normalizza_raw(r)
        if n is None:
            continue
        gruppi.setdefault(n, set()).add(r)
    return {n: sorted(v) for n, v in sorted(gruppi.items()) if len(v) > 1}


def conta_voci_con_raw(elementi):
    """Voci di catalogo con almeno un riferimento raw (funzione pura).

    Una voce puo' avere piu' raw (raw_elenco): contare i riferimenti distinti
    non equivale a contare le voci. Serve al riepilogo per non confondere i due.
    Un 'raw' vuoto o non-stringa e' ignorato, come un raw_elenco vuoto/assente.
    """
    return sum(
        1 for e in elementi
        if (isinstance(e.get("raw"), str) and e["raw"]) or (e.get("raw_elenco") or [])
    )


def raccogli_riferimenti(elementi):
    """Riferimenti raw del catalogo: path raw -> lista di id (funzione pura).

    Ogni riferimento produce una entry: una voce con 'raw' e 'raw_elenco' sullo
    stesso file dara' il suo id piu' volte (il chiamante distingue duplicato fra
    voci e ripetizione in voce). I valori non-stringa/vuoti di 'raw' sono ignorati.
    """
    riferimenti = {}
    for e in elementi:
        rid = e.get("id", "?")
        r = e.get("raw")
        if isinstance(r, str) and r:
            riferimenti.setdefault(r, []).append(rid)
        for r2 in e.get("raw_elenco") or []:
            if isinstance(r2, str) and r2:
                riferimenti.setdefault(r2, []).append(rid)
    return riferimenti


def raw_senza_voce(raws, riferimenti):
    """Raw su disco senza voce di catalogo, confronto su forma canonica (pura).

    Un riferimento scritto in modo equivalente ('./a/x.sglraw', 'a//x.sglraw')
    non deve produrre un falso orfano: il confronto usa normalizza_raw().
    Ritorna la lista ordinata dei path raw non tracciati.
    """
    canonici = {normalizza_raw(k) for k in riferimenti}
    return sorted(r for r in raws if normalizza_raw(r) not in canonici)


def riferimenti_inesistenti(riferimenti, esiste):
    """Riferimenti raw del catalogo senza file su disco (funzione pura).

    esiste e' un callable path_relativo -> bool: la funzione non tocca il disco.
    Ritorna la lista ordinata dei path mancanti.
    """
    return sorted(r for r in riferimenti if not esiste(r))


def analizza(raws, elementi, esiste):
    """Copertura inversa raw -> catalogo. Funzione pura (esiste e' un callable).

    raws:        sequenza di path relativi a sglavinated_src (raw su disco).
    elementi:    lista di voci del catalogo.
    esiste:      callback path_relativo -> bool (il file c'e' su disco).
    Ritorna un dict con: riferimenti, senza_voce, raw_inesistenti, duplicati
    (solo fra voci distinte), ripetuti_in_voce (una voce che cita due volte lo
    stesso raw), scritture (stesso file citato con piu' scritture diverse),
    voci_con_raw (voci con almeno un raw, distinto dal numero di riferimenti).
    senza_voce confronta i percorsi canonici, cosi' un riferimento scritto in
    modo equivalente non risulta un falso orfano.
    """
    riferimenti = raccogli_riferimenti(elementi)

    senza_voce = raw_senza_voce(raws, riferimenti)
    raw_inesistenti = riferimenti_inesistenti(riferimenti, esiste)
    duplicati, ripetuti_in_voce = classifica_duplicati(riferimenti)
    voci_con_raw = conta_voci_con_raw(elementi)
    return {
        "riferimenti": riferimenti,
        "senza_voce": senza_voce,
        "raw_inesistenti": raw_inesistenti,
        "duplicati": duplicati,
        "ripetuti_in_voce": ripetuti_in_voce,
        "scritture": classifica_scritture(riferimenti),
        "voci_con_raw": voci_con_raw,
    }


# Riferimenti 'raw' del catalogo senza file su disco che sono NOTI e attesi:
# pose dell'auto di Andrea (area bloccata, REGOLE_AUTO_GIOCATORE.md). Non si
# ricreano da qui (vera origine: git f2f3581 o ridisegno di Andrea). Un
# riferimento NUOVO senza file, non coperto da questo elenco, resta da decidere.
NOTI_RAW_MANCANTI = {
    "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw": "auto Andrea, area bloccata",
    "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx3.sglraw": "auto Andrea, area bloccata",
    "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_stop_sx1.sglraw": "auto Andrea, area bloccata (stop)",
    "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_stop_sx3.sglraw": "auto Andrea, area bloccata (stop)",
}


def classifica_inesistenti(inesistenti, motivi=None):
    """Divide i riferimenti 'raw' senza file in noti (attesi) e nuovi da decidere.

    motivi e' il dizionario dei noti: di default NOTI_RAW_MANCANTI, ma puo'
    essere passato dal chiamante. Cosi' la funzione e' pura e provabile con un
    elenco diverso senza toccare il globale (come righe_inesistenti/risolti).
    L'ordine e i risultati col default restano invariati.
    """
    if motivi is None:
        motivi = NOTI_RAW_MANCANTI
    noti = [r for r in inesistenti if r in motivi]
    nuovi = [r for r in inesistenti if r not in motivi]
    return noti, nuovi


def noti_risolti(presenti):
    """Noti raw-mancanti ricomparsi su disco (funzione pura).

    Se un raw dell'elenco NOTI_RAW_MANCANTI torna a esistere, l'eccezione non
    e' piu' necessaria: segnalarlo evita che l'elenco resti stantio e nasconda
    un cambiamento reale. Diagnosi informativa, non cambia l'exit.
    """
    visti = set(presenti)
    return sorted(r for r in NOTI_RAW_MANCANTI if r in visti)


RAW_PREFIX = RAW_ROOT.relative_to(RADICE).as_posix() + "/"


def percorso_raw_valido(r):
    """Un riferimento 'raw' del catalogo e' ben formato se sta sotto sprite_raw/."""
    return (
        isinstance(r, str)
        and r.startswith(RAW_PREFIX)
        and r.endswith(".sglraw")
        and ".." not in r.split("/")
    )


def classifica_percorsi(riferimenti):
    """Divide i riferimenti 'raw' in ben formati e sospetti (funzione pura).

    Un riferimento fuori da motore_nuovo/dati/sprite_raw/ (vecchia sede, GRAFICA/,
    percorso assoluto, '..', estensione sbagliata) non verrebbe visto come mancante
    se il file esiste altrove: resterebbe invisibile all'audit delle licenze.
    Qui si segnala, senza cambiare l'exit (come gli altri rilievi informativi).
    """
    validi = sorted(r for r in riferimenti if percorso_raw_valido(r))
    sospetti = sorted(r for r in riferimenti if not percorso_raw_valido(r))
    return validi, sospetti


def righe_riepilogo(res, raws, noti, nuovi, sospetti, validi, risolti):
    """Righe di sintesi finale del report (funzione pura).

    Erano f-string sparse in main(): qui si provano in isolamento ordine e
    formato, e main() si limita a stamparle. Nessun accesso al disco.
    """
    return [
        f"raw del motore: {len(raws)}; tracciati nel catalogo: {len(raws) - len(res['senza_voce'])}; senza voce: {len(res['senza_voce'])}",
        f"voci di catalogo con raw: {res['voci_con_raw']}; riferimenti raw distinti: {len(res['riferimenti'])}",
        f"riferimenti 'raw' senza file su disco: {len(res['raw_inesistenti'])} (noti {len(noti)}, da decidere {len(nuovi)})",
        f"noti raw-mancanti ora presenti su disco: {len(risolti)}",
        f"raw citati da piu' voci: {len(res['duplicati'])}",
        f"raw ripetuti dentro una sola voce: {len(res['ripetuti_in_voce'])}",
        f"raw citati con piu' scritture dello stesso file: {len(res['scritture'])}",
        f"riferimenti 'raw' con percorso sospetto: {len(sospetti)} (ben formati {len(validi)})",
    ]


def righe_inesistenti(noti, nuovi, riferimenti, motivi):
    """Righe di stampa dei riferimenti 'raw' senza file su disco (funzione pura).

    Erano due cicli print in main(): qui si provano in isolamento formato e
    ordine. Prima i noti (con il motivo atteso fra parentesi), poi i nuovi.
    motivi: path noto -> motivo (il chiamante passa NOTI_RAW_MANCANTI).
    """
    righe = [
        f"RAW MANCANTE NOTO {r} {riferimenti[r]} ({motivi[r]})" for r in noti
    ]
    righe += [
        f"RAW IN CATALOGO MA NON SU DISCO {r} {riferimenti[r]}" for r in nuovi
    ]
    return righe


def righe_risolti(risolti, motivi):
    """Righe dei noti raw-mancanti ricomparsi su disco (funzione pura).

    Era una f-string nel ciclo di main(): qui si provano in isolamento formato e
    ordine. motivi: path noto -> motivo (il chiamante passa NOTI_RAW_MANCANTI).
    """
    return [
        f"RAW NOTO ORA PRESENTE {r} (aggiornare NOTI_RAW_MANCANTI: {motivi[r]})"
        for r in risolti
    ]


def righe_classificazione(duplicati, ripetuti_in_voce, scritture):
    """Righe dei rilievi su raw citati piu' volte / con piu' scritture (pura).

    Erano tre cicli print in main(): qui si provano in isolamento formato e
    ordine (prima i duplicati fra voci, poi i ripetuti in una sola voce, poi le
    scritture diverse dello stesso file), ogni gruppo con le chiavi ordinate.
    Nessun accesso al disco.
    """
    righe = []
    for r, ids in sorted(duplicati.items()):
        righe.append(f"RAW CITATO DA PIU' VOCI {r} {ids}")
    for r, ids in sorted(ripetuti_in_voce.items()):
        righe.append(f"RAW RIPETUTO IN UNA VOCE {r} {ids}")
    for canonico, elenco in sorted(scritture.items()):
        righe.append(f"RAW STESSO FILE SCRITTURE DIVERSE {canonico} {elenco}")
    return righe


def righe_senza_voce(senza_voce):
    """Righe dei raw su disco senza voce di catalogo (funzione pura).

    Era un ciclo print in main(): qui si prova in isolamento il formato.
    Nessun accesso al disco.
    """
    return [f"RAW NON CATALOGATO {r}" for r in senza_voce]


def righe_sospetti(sospetti, riferimenti):
    """Righe dei riferimenti 'raw' con percorso sospetto (funzione pura).

    Era un ciclo print in main(): qui si provano in isolamento formato e
    ordine. riferimenti: path raw -> lista di id (il chiamante passa
    res['riferimenti']). Nessun accesso al disco.
    """
    return [
        f"RAW PERCORSO SOSPETTO {r} {riferimenti[r]}" for r in sospetti
    ]


def classifica_report(res, raws):
    """Classificazioni intermedie del report (funzione pura).

    Raggruppa i calcoli derivati usati da righe_report(): noti/nuovi fra gli
    inesistenti, validi/sospetti fra i riferimenti, e noti risolti su disco.
    Nessun accesso al disco; raws e' la lista dei path raw presenti.
    """
    noti, nuovi = classifica_inesistenti(res["raw_inesistenti"])
    validi, sospetti = classifica_percorsi(res["riferimenti"])
    return {
        "noti": noti,
        "nuovi": nuovi,
        "validi": validi,
        "sospetti": sospetti,
        "risolti": noti_risolti(raws),
    }


def righe_report(res, raws):
    """Righe complete del report, nello stesso ordine di main() (funzione pura).

    Compone le sezioni gia' testate (senza_voce, inesistenti, classificazione,
    sospetti, risolti, riepilogo) usando classifica_report() per i derivati.
    Nessun accesso al disco; main() si limita a stamparle e a decidere l'exit.
    """
    c = classifica_report(res, raws)
    noti, nuovi = c["noti"], c["nuovi"]
    validi, sospetti, risolti = c["validi"], c["sospetti"], c["risolti"]
    righe = []
    righe.extend(righe_senza_voce(res["senza_voce"]))
    righe.extend(righe_inesistenti(noti, nuovi, res["riferimenti"], NOTI_RAW_MANCANTI))
    righe.extend(righe_classificazione(
        res["duplicati"], res["ripetuti_in_voce"], res["scritture"]
    ))
    righe.extend(righe_sospetti(sospetti, res["riferimenti"]))
    righe.extend(righe_risolti(risolti, NOTI_RAW_MANCANTI))
    righe.extend(righe_riepilogo(res, raws, noti, nuovi, sospetti, validi, risolti))
    return righe


def codice_uscita(res):
    """Exit code del report: 1 se ci sono raw su disco senza voce, 0 altrimenti.

    Le altre segnalazioni (riferimenti mancanti, duplicati, sospetti, scritture)
    sono informative e non cambiano l'exit, come da policy del tool.
    """
    return 1 if res["senza_voce"] else 0


def path_raw_relativi(radice, paths):
    """Path raw assoluti -> path relativi posix ordinati (funzione pura).

    Era il generatore inline in main(): qui si prova in isolamento la forma
    (relative_to + as_posix + ordinamento). Il disco e' acceso dal chiamante
    con rglob(); questa funzione trasforma solo i Path che riceve.
    """
    return sorted(p.relative_to(radice).as_posix() for p in paths)


def raw_su_disco(radice, raw_root, rglob=None):
    """Scansiona i .sglraw su disco e li restituisce relativi/ordinati (pura).

    main() faceva inline path_raw_relativi(RADICE, RAW_ROOT.rglob("*.sglraw")):
    la scansione sta ora in un solo punto, provabile in autotest con un rglob
    iniettabile che restituisce Path finti. rglob e' un callable pattern -> Path;
    quando non e' passato si usa raw_root.rglob. Nessun disco toccato dal
    chiamante che inietta rglob; la trasformazione resta in path_raw_relativi().
    """
    if rglob is None:
        rglob = lambda pattern: raw_root.rglob(pattern)
    return path_raw_relativi(radice, rglob("*.sglraw"))


def esiste_raw(radice):
    """Callable path_relativo -> bool per verificare un raw su disco (funzione pura).

    Era la lambda inline in main() passata come 'esiste' ad analizza(): qui si
    prova in isolamento che il callable risolva il path rispetto alla radice
    data. La funzione non accede al disco: restituisce solo il callable, che il
    chiamante invoca sui riferimenti del catalogo.
    """
    return lambda r: (radice / r).is_file()


def carica_catalogo(path, leggi=None):
    """Carica CATALOGO_GRAFICO.json come dict, con lettura iniettabile (pura).

    main() faceva json.loads(CATALOGO.read_text(encoding="utf-8")): il parsing
    sta ora in un solo punto, provabile in autotest senza toccare il disco.
    leggi e' un callable path -> testo (default Path.read_text, che chiude da
    solo); l'esistenza del file resta di competenza di main().
    """
    if leggi is None:
        leggi = lambda p: Path(p).read_text(encoding="utf-8")
    return json.loads(leggi(path))


def report_da_dati(elementi, raws, esiste):
    """Componi analizza -> righe_report -> codice_uscita (funzione pura).

    main() faceva questa sequenza inline: analizzare i raw con le voci del
    catalogo, produrre le righe del report e decidere l'exit. Qui la sequenza
    sta in un solo punto, provabile in autotest con dati sintetici e 'esiste'
    iniettabile; nessun accesso al disco. Ritorna (righe, codice).
    """
    res = analizza(raws, elementi, esiste)
    return righe_report(res, raws), codice_uscita(res)


def autotest():
    """Prove della funzione pura con dati sintetici, nessun file reale letto."""
    elementi = [
        {"id": "a", "raw": "raw/a.sglraw"},
        {"id": "b", "raw": "raw/b.sglraw", "raw_elenco": ["raw/c.sglraw"]},
        {"id": "c", "raw": "raw/a.sglraw"},  # duplicato di a
        {"id": "d", "raw": "raw/fantasma.sglraw"},  # raw inesistente
    ]
    # Su disco ci sono solo a, b, c; manca disco "orfano" e manca fantasma.
    presenti = {"raw/a.sglraw", "raw/b.sglraw", "raw/c.sglraw"}
    res = analizza(
        ["raw/a.sglraw", "raw/b.sglraw", "raw/c.sglraw", "raw/orfano.sglraw"],
        elementi,
        lambda r: r in presenti,
    )
    prove = [
        ("senza_voce", res["senza_voce"] == ["raw/orfano.sglraw"]),
        ("raw_inesistenti", res["raw_inesistenti"] == ["raw/fantasma.sglraw"]),
        ("duplicati", res["duplicati"].get("raw/a.sglraw") == ["a", "c"]),
        ("n_riferimenti", len(res["riferimenti"]) == 4),
        ("voci_con_raw", res["voci_con_raw"] == 4),
    ]
    # Una sola voce che cita lo stesso raw due volte (raw + raw_elenco) va in
    # in_voce, non fra i duplicati fra voci; due voci distinte restano duplicati.
    fra, dentro = classifica_duplicati({
        "raw/x.sglraw": ["v1", "v2"],  # due voci -> duplicato
        "raw/y.sglraw": ["v3", "v3"],  # una voce due volte -> in_voce
    })
    ok_dup = (
        fra == {"raw/x.sglraw": ["v1", "v2"]}
        and dentro == {"raw/y.sglraw": ["v3"]}
        and res["ripetuti_in_voce"] == {}
    )
    prove.append(("classifica_duplicati", ok_dup))
    ok_norm = (
        normalizza_raw("./raw/a.sglraw") == "raw/a.sglraw"
        and normalizza_raw("raw//a.sglraw") == "raw/a.sglraw"
        and normalizza_raw("raw/b/../a.sglraw") == "raw/a.sglraw"
        and normalizza_raw("raw/../../fuori.sglraw") == "../fuori.sglraw"
        and normalizza_raw("") is None
        and normalizza_raw(None) is None
    )
    prove.append(("normalizza_raw", ok_norm))
    ok_senza_voce_pura = (
        raw_senza_voce(
            ["raw/a.sglraw", "raw/b.sglraw", "raw/orfano.sglraw"],
            {"./raw/a.sglraw": ["v1"], "raw/b.sglraw": ["v2"]},
        )
        == ["raw/orfano.sglraw"]
        and raw_senza_voce([], {"raw/a.sglraw": ["v1"]}) == []
        and raw_senza_voce(["raw/a.sglraw"], {}) == ["raw/a.sglraw"]
    )
    prove.append(("raw_senza_voce", ok_senza_voce_pura))
    ok_inesistenti_pura = (
        riferimenti_inesistenti(
            {"raw/a.sglraw": ["v1"], "raw/manca.sglraw": ["v2"], "raw/b.sglraw": ["v3"]},
            lambda r: r in {"raw/a.sglraw", "raw/b.sglraw"},
        )
        == ["raw/manca.sglraw"]
        and riferimenti_inesistenti({}, lambda r: True) == []
        and riferimenti_inesistenti({"raw/a.sglraw": ["v1"]}, lambda r: False)
        == ["raw/a.sglraw"]
    )
    prove.append(("riferimenti_inesistenti", ok_inesistenti_pura))
    ok_scritture = (
        classifica_scritture({
            "raw/a.sglraw": ["v1"],
            "./raw/a.sglraw": ["v2"],  # stesso file, scrittura diversa
            "raw/b.sglraw": ["v3"],
        })
        == {"raw/a.sglraw": ["./raw/a.sglraw", "raw/a.sglraw"]}
        and classifica_scritture({"raw/a.sglraw": ["v1"]}) == {}
        and classifica_scritture({}) == {}
    )
    prove.append(("classifica_scritture", ok_scritture))
    ok_conta = (
        conta_voci_con_raw([
            {"id": "a", "raw": "raw/a.sglraw"},
            {"id": "b", "raw_elenco": ["raw/b.sglraw", "raw/c.sglraw"]},
            {"id": "c", "raw": "", "raw_elenco": []},
            {"id": "d", "raw": None, "raw_elenco": ["raw/d.sglraw"]},
            {"id": "e"},
        ])
        == 3
        and conta_voci_con_raw(elementi) == 4
        and conta_voci_con_raw([]) == 0
    )
    prove.append(("conta_voci_con_raw", ok_conta))
    ok_racc = (
        raccogli_riferimenti([
            {"id": "a", "raw": "raw/a.sglraw"},
            {"id": "b", "raw_elenco": ["raw/b.sglraw", "raw/c.sglraw"]},
            {"id": "c", "raw": "raw/a.sglraw"},
            {"id": "d", "raw": "", "raw_elenco": [None, "raw/d.sglraw"]},
        ])
        == {
            "raw/a.sglraw": ["a", "c"],
            "raw/b.sglraw": ["b"],
            "raw/c.sglraw": ["b"],
            "raw/d.sglraw": ["d"],
        }
        and raccogli_riferimenti([]) == {}
    )
    prove.append(("raccogli_riferimenti", ok_racc))
    noti, nuovi = classifica_inesistenti([
        "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw",
        "motore_nuovo/dati/sprite_raw/giocatore/pose/nuovo_raw.sglraw",
    ])
    noti_custom, nuovi_custom = classifica_inesistenti(
        ["raw/n1.sglraw", "raw/n2.sglraw"],
        {"raw/n1.sglraw": "motivo finto"},
    )
    ok_class = (
        noti == ["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"]
        and nuovi == ["motore_nuovo/dati/sprite_raw/giocatore/pose/nuovo_raw.sglraw"]
        and noti_custom == ["raw/n1.sglraw"]
        and nuovi_custom == ["raw/n2.sglraw"]
        and classifica_inesistenti(["raw/x.sglraw"], {}) == ([], ["raw/x.sglraw"])
        and classifica_inesistenti([]) == ([], [])
    )
    prove.append(("classifica_inesistenti", ok_class))
    ok_inesistenti = (
        righe_inesistenti(
            ["raw/x.sglraw"], ["raw/y.sglraw"],
            {"raw/x.sglraw": ["v1"], "raw/y.sglraw": ["v2"]},
            {"raw/x.sglraw": "area bloccata"},
        )
        == [
            "RAW MANCANTE NOTO raw/x.sglraw ['v1'] (area bloccata)",
            "RAW IN CATALOGO MA NON SU DISCO raw/y.sglraw ['v2']",
        ]
        and righe_inesistenti([], [], {}, {}) == []
    )
    prove.append(("righe_inesistenti", ok_inesistenti))
    ok_risolti = (
        noti_risolti(["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"])
        == ["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"]
        and noti_risolti(["motore_nuovo/dati/sprite_raw/giocatore/pose/nuovo.sglraw"]) == []
    )
    prove.append(("noti_risolti", ok_risolti))
    ok_percorsi = (
        percorso_raw_valido("motore_nuovo/dati/sprite_raw/sponsor/x.sglraw")
        and not percorso_raw_valido("GRAFICA/sponsor/x.sglraw")
        and not percorso_raw_valido("motore_nuovo/dati/sprite_raw/../fuori.sglraw")
        and not percorso_raw_valido("motore_nuovo/dati/sprite_raw/x.png")
        and not percorso_raw_valido("")
        and not percorso_raw_valido("/motore_nuovo/dati/sprite_raw/x.sglraw")
    )
    validi, sospetti = classifica_percorsi({
        "motore_nuovo/dati/sprite_raw/a.sglraw": ["a"],
        "GRAFICA/vecchio.sglraw": ["b"],
    })
    v_vuoti, s_vuoti = classifica_percorsi({})
    ok_percorsi = (
        ok_percorsi
        and validi == ["motore_nuovo/dati/sprite_raw/a.sglraw"]
        and sospetti == ["GRAFICA/vecchio.sglraw"]
        and v_vuoti == [] and s_vuoti == []
    )
    prove.append(("classifica_percorsi", ok_percorsi))
    res_finto = {
        "senza_voce": ["raw/orfano.sglraw"],
        "voci_con_raw": 3,
        "riferimenti": {"raw/a.sglraw": ["a"], "raw/b.sglraw": ["b"]},
        "raw_inesistenti": ["raw/manca1.sglraw", "raw/manca2.sglraw"],
        "duplicati": {"raw/a.sglraw": ["a", "c"]},
        "ripetuti_in_voce": {"raw/b.sglraw": ["b"]},
        "scritture": {"raw/a.sglraw": ["./raw/a.sglraw", "raw/a.sglraw"]},
    }
    righe = righe_riepilogo(
        res_finto, ["raw/a.sglraw", "raw/b.sglraw", "raw/orfano.sglraw"],
        ["raw/manca1.sglraw"], ["raw/manca2.sglraw"],
        ["GRAFICA/vecchio.sglraw"], ["raw/a.sglraw"], ["raw/noto.sglraw"],
    )
    vuote = righe_riepilogo(
        {"senza_voce": [], "voci_con_raw": 0, "riferimenti": {},
         "raw_inesistenti": [], "duplicati": {}, "ripetuti_in_voce": {},
         "scritture": {}},
        [], [], [], [], [], [],
    )
    ok_righe = righe == [
        "raw del motore: 3; tracciati nel catalogo: 2; senza voce: 1",
        "voci di catalogo con raw: 3; riferimenti raw distinti: 2",
        "riferimenti 'raw' senza file su disco: 2 (noti 1, da decidere 1)",
        "noti raw-mancanti ora presenti su disco: 1",
        "raw citati da piu' voci: 1",
        "raw ripetuti dentro una sola voce: 1",
        "raw citati con piu' scritture dello stesso file: 1",
        "riferimenti 'raw' con percorso sospetto: 1 (ben formati 1)",
    ] and len(vuote) == 8 and vuote[0] == "raw del motore: 0; tracciati nel catalogo: 0; senza voce: 0"
    prove.append(("righe_riepilogo", ok_righe))
    ok_risolti_righe = (
        righe_risolti(
            ["raw/x.sglraw"],
            {"raw/x.sglraw": "area bloccata"},
        )
        == ["RAW NOTO ORA PRESENTE raw/x.sglraw (aggiornare NOTI_RAW_MANCANTI: area bloccata)"]
        and righe_risolti([], {}) == []
    )
    prove.append(("righe_risolti", ok_risolti_righe))
    ok_class_righe = (
        righe_classificazione(
            {"raw/b.sglraw": ["b1", "b2"], "raw/a.sglraw": ["a1", "a2"]},
            {"raw/c.sglraw": ["c1"]},
            {"raw/d.sglraw": ["./raw/d.sglraw", "raw/d.sglraw"]},
        )
        == [
            "RAW CITATO DA PIU' VOCI raw/a.sglraw ['a1', 'a2']",
            "RAW CITATO DA PIU' VOCI raw/b.sglraw ['b1', 'b2']",
            "RAW RIPETUTO IN UNA VOCE raw/c.sglraw ['c1']",
            "RAW STESSO FILE SCRITTURE DIVERSE raw/d.sglraw "
            "['./raw/d.sglraw', 'raw/d.sglraw']",
        ]
        and righe_classificazione({}, {}, {}) == []
    )
    prove.append(("righe_classificazione", ok_class_righe))
    ok_senza_voce = (
        righe_senza_voce(["raw/a.sglraw", "raw/b.sglraw"])
        == [
            "RAW NON CATALOGATO raw/a.sglraw",
            "RAW NON CATALOGATO raw/b.sglraw",
        ]
        and righe_senza_voce([]) == []
    )
    prove.append(("righe_senza_voce", ok_senza_voce))
    ok_sospetti = (
        righe_sospetti(
            ["GRAFICA/vecchio.sglraw", "raw/../x.sglraw"],
            {
                "GRAFICA/vecchio.sglraw": ["b"],
                "raw/../x.sglraw": ["a", "c"],
            },
        )
        == [
            "RAW PERCORSO SOSPETTO GRAFICA/vecchio.sglraw ['b']",
            "RAW PERCORSO SOSPETTO raw/../x.sglraw ['a', 'c']",
        ]
        and righe_sospetti([], {}) == []
    )
    prove.append(("righe_sospetti", ok_sospetti))
    res_report = {
        "senza_voce": ["raw/orfano.sglraw"],
        "voci_con_raw": 2,
        "riferimenti": {
            "raw/a.sglraw": ["a"],
            "GRAFICA/vecchio.sglraw": ["b"],
            "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw": ["c"],
            "raw/manca_nuovo.sglraw": ["m1"],
        },
        "raw_inesistenti": [
            "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw",
            "raw/manca_nuovo.sglraw",
        ],
        "duplicati": {"raw/a.sglraw": ["a", "d"]},
        "ripetuti_in_voce": {},
        "scritture": {},
    }
    raws_report = [
        "raw/a.sglraw",
        "raw/orfano.sglraw",
        "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw",
    ]
    cr = classifica_report(res_report, raws_report)
    ok_cr = (
        cr["noti"] == ["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"]
        and cr["nuovi"] == ["raw/manca_nuovo.sglraw"]
        and cr["validi"] == [
            "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw",
        ]
        and cr["sospetti"] == [
            "GRAFICA/vecchio.sglraw",
            "raw/a.sglraw",
            "raw/manca_nuovo.sglraw",
        ]
        and cr["risolti"] == ["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"]
    )
    prove.append(("classifica_report", ok_cr))
    rr = righe_report(res_report, raws_report)
    noti_r, nuovi_r = classifica_inesistenti(res_report["raw_inesistenti"])
    validi_r, sosp_r = classifica_percorsi(res_report["riferimenti"])
    risolti_r = noti_risolti(raws_report)
    atteso_report = []
    atteso_report.extend(righe_senza_voce(res_report["senza_voce"]))
    atteso_report.extend(righe_inesistenti(noti_r, nuovi_r, res_report["riferimenti"], NOTI_RAW_MANCANTI))
    atteso_report.extend(righe_classificazione(res_report["duplicati"], res_report["ripetuti_in_voce"], res_report["scritture"]))
    atteso_report.extend(righe_sospetti(sosp_r, res_report["riferimenti"]))
    atteso_report.extend(righe_risolti(risolti_r, NOTI_RAW_MANCANTI))
    atteso_report.extend(righe_riepilogo(res_report, raws_report, noti_r, nuovi_r, sosp_r, validi_r, risolti_r))
    prove.append(("righe_report", rr == atteso_report))
    ok_exit = (
        codice_uscita({"senza_voce": ["raw/orfano.sglraw"]}) == 1
        and codice_uscita({"senza_voce": []}) == 0
        and codice_uscita(res_finto) == 1
        and codice_uscita(res_report) == 1
    )
    prove.append(("codice_uscita", ok_exit))
    ok_relativi = (
        path_raw_relativi(Path("/r"), [Path("/r/b.sglraw"), Path("/r/a.sglraw")])
        == ["a.sglraw", "b.sglraw"]
        and path_raw_relativi(Path("/r"), [Path("/r/x/y.sglraw")])
        == ["x/y.sglraw"]
        and path_raw_relativi(Path("/r"), []) == []
    )
    prove.append(("path_raw_relativi", ok_relativi))
    # raw_su_disco: scansione con rglob iniettabile, nessun file reale letto.
    ok_disco = (
        raw_su_disco(
            Path("/r"), Path("/r/raw"),
            rglob=lambda p: [Path("/r/raw/b.sglraw"), Path("/r/raw/a.sglraw")],
        )
        == ["raw/a.sglraw", "raw/b.sglraw"]
        and raw_su_disco(Path("/r"), Path("/r/raw"), rglob=lambda p: [Path("/r/raw/x/y.sglraw")])
        == ["raw/x/y.sglraw"]
        and raw_su_disco(Path("/r"), Path("/r/raw"), rglob=lambda p: []) == []
    )
    prove.append(("raw_su_disco", ok_disco))
    # esiste_raw restituisce un callable che risolve il path rispetto alla
    # radice data; si prova con una radice finta, senza toccare il disco.
    class _RadiceFinta:
        def __init__(self, presenti):
            self.presenti = presenti

        def __truediv__(self, nome):
            return _FileFinto(nome in self.presenti)

    class _FileFinto:
        def __init__(self, presente):
            self.presente = presente

        def is_file(self):
            return self.presente

    esiste = esiste_raw(_RadiceFinta({"raw/a.sglraw"}))
    ok_esiste = (
        esiste("raw/a.sglraw") is True
        and esiste("raw/b.sglraw") is False
        and esiste_raw(_RadiceFinta(set()))("raw/a.sglraw") is False
    )
    prove.append(("esiste_raw", ok_esiste))
    # carica_catalogo: parsing con lettura iniettabile, nessun file reale aperto.
    ok_carica = (
        carica_catalogo("finto.json", leggi=lambda p: '{"elementi": [{"id": "x"}]}')
        == {"elementi": [{"id": "x"}]}
        and carica_catalogo("finto.json", leggi=lambda p: '{"elementi": []}')
        == {"elementi": []}
    )
    prove.append(("carica_catalogo", ok_carica))
    # report_da_dati: orchestrazione provata in isolamento, senza disco.
    el_orfano = [{"id": "a", "raw": "raw/a.sglraw"}]
    raws_orfano = ["raw/a.sglraw", "raw/orfano.sglraw"]
    esiste_a = lambda r: r in {"raw/a.sglraw"}
    righe_con, codice_con = report_da_dati(el_orfano, raws_orfano, esiste_a)
    atteso_con = righe_report(
        analizza(raws_orfano, el_orfano, esiste_a), raws_orfano
    )
    righe_ok, codice_ok = report_da_dati(
        [{"id": "a", "raw": "raw/a.sglraw"}], ["raw/a.sglraw"], lambda r: True
    )
    ok_orchestra = (
        righe_con == atteso_con
        and codice_con == 1
        and any(r.startswith("RAW NON CATALOGATO raw/orfano.sglraw") for r in righe_con)
        and codice_ok == 0
        and not any(r.startswith("RAW NON CATALOGATO") for r in righe_ok)
    )
    prove.append(("report_da_dati", ok_orchestra))
    for nome, ok in prove:
        print(f"AUTOTEST {nome}: {'OK' if ok else 'FALLITO'}")
    return 0 if all(ok for _, ok in prove) else 1


def main():
    cat = carica_catalogo(CATALOGO)
    raws = raw_su_disco(RADICE, RAW_ROOT)
    righe, codice = report_da_dati(cat["elementi"], raws, esiste_raw(RADICE))
    for riga in righe:
        print(riga)
    return codice


if __name__ == "__main__":
    if "--autotest" in sys.argv[1:]:
        raise SystemExit(autotest())
    raise SystemExit(main())
