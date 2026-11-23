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
    riferimenti = {}  # path raw -> lista di id di catalogo che lo citano
    for e in elementi:
        rid = e.get("id", "?")
        r = e.get("raw")
        if isinstance(r, str):
            riferimenti.setdefault(r, []).append(rid)
        for r2 in e.get("raw_elenco") or []:
            riferimenti.setdefault(r2, []).append(rid)

    canonici = {normalizza_raw(k) for k in riferimenti}
    senza_voce = sorted(r for r in raws if normalizza_raw(r) not in canonici)
    raw_inesistenti = sorted(r for r in riferimenti if not esiste(r))
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


def classifica_inesistenti(inesistenti):
    """Divide i riferimenti 'raw' senza file in noti (attesi) e nuovi da decidere."""
    noti = [r for r in inesistenti if r in NOTI_RAW_MANCANTI]
    nuovi = [r for r in inesistenti if r not in NOTI_RAW_MANCANTI]
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
    noti, nuovi = classifica_inesistenti([
        "motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw",
        "motore_nuovo/dati/sprite_raw/giocatore/pose/nuovo_raw.sglraw",
    ])
    ok_class = (
        noti == ["motore_nuovo/dati/sprite_raw/giocatore/pose/auto_sx1.sglraw"]
        and nuovi == ["motore_nuovo/dati/sprite_raw/giocatore/pose/nuovo_raw.sglraw"]
    )
    prove.append(("classifica_inesistenti", ok_class))
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
    for nome, ok in prove:
        print(f"AUTOTEST {nome}: {'OK' if ok else 'FALLITO'}")
    return 0 if all(ok for _, ok in prove) else 1


def main():
    cat = json.loads(CATALOGO.read_text(encoding="utf-8"))
    raws = sorted(
        p.relative_to(RADICE).as_posix() for p in RAW_ROOT.rglob("*.sglraw")
    )
    res = analizza(raws, cat["elementi"], lambda r: (RADICE / r).is_file())

    for r in res["senza_voce"]:
        print("RAW NON CATALOGATO", r)
    noti, nuovi = classifica_inesistenti(res["raw_inesistenti"])
    for r in noti:
        print("RAW MANCANTE NOTO", r, res["riferimenti"][r], f"({NOTI_RAW_MANCANTI[r]})")
    for r in nuovi:
        print("RAW IN CATALOGO MA NON SU DISCO", r, res["riferimenti"][r])
    for r, ids in sorted(res["duplicati"].items()):
        print("RAW CITATO DA PIU' VOCI", r, ids)
    for r, ids in sorted(res["ripetuti_in_voce"].items()):
        print("RAW RIPETUTO IN UNA VOCE", r, ids)
    for canonico, scritture in sorted(res["scritture"].items()):
        print("RAW STESSO FILE SCRITTURE DIVERSE", canonico, scritture)
    validi, sospetti = classifica_percorsi(res["riferimenti"])
    for r in sospetti:
        print("RAW PERCORSO SOSPETTO", r, res["riferimenti"][r])
    risolti = noti_risolti(raws)
    for r in risolti:
        print("RAW NOTO ORA PRESENTE", r, f"(aggiornare NOTI_RAW_MANCANTI: {NOTI_RAW_MANCANTI[r]})")

    tracciati = res["riferimenti"]
    print(f"raw del motore: {len(raws)}; tracciati nel catalogo: {len(raws) - len(res['senza_voce'])}; senza voce: {len(res['senza_voce'])}")
    print(f"voci di catalogo con raw: {res['voci_con_raw']}; riferimenti raw distinti: {len(tracciati)}")
    print(f"riferimenti 'raw' senza file su disco: {len(res['raw_inesistenti'])} (noti {len(noti)}, da decidere {len(nuovi)})")
    print(f"noti raw-mancanti ora presenti su disco: {len(risolti)}")
    print(f"raw citati da piu' voci: {len(res['duplicati'])}")
    print(f"raw ripetuti dentro una sola voce: {len(res['ripetuti_in_voce'])}")
    print(f"raw citati con piu' scritture dello stesso file: {len(res['scritture'])}")
    print(f"riferimenti 'raw' con percorso sospetto: {len(sospetti)} (ben formati {len(validi)})")
    return 1 if res["senza_voce"] else 0


if __name__ == "__main__":
    if "--autotest" in sys.argv[1:]:
        raise SystemExit(autotest())
    raise SystemExit(main())
