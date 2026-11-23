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


def analizza(raws, elementi, esiste):
    """Copertura inversa raw -> catalogo. Funzione pura (esiste e' un callable).

    raws:        sequenza di path relativi a sglavinated_src (raw su disco).
    elementi:    lista di voci del catalogo.
    esiste:      callback path_relativo -> bool (il file c'e' su disco).
    Ritorna un dict con: riferimenti, senza_voce, raw_inesistenti, duplicati,
    voci_con_raw (voci con almeno un raw, distinto dal numero di riferimenti).
    """
    riferimenti = {}  # path raw -> lista di id di catalogo che lo citano
    for e in elementi:
        rid = e.get("id", "?")
        r = e.get("raw")
        if isinstance(r, str):
            riferimenti.setdefault(r, []).append(rid)
        for r2 in e.get("raw_elenco") or []:
            riferimenti.setdefault(r2, []).append(rid)

    senza_voce = sorted(r for r in raws if r not in riferimenti)
    raw_inesistenti = sorted(r for r in riferimenti if not esiste(r))
    duplicati = {r: ids for r, ids in riferimenti.items() if len(ids) > 1}
    # Una voce puo' avere piu' raw (raw_elenco): contare i riferimenti distinti
    # non equivale a contare le voci. Serve al riepilogo per non confondere i due.
    voci_con_raw = sum(
        1 for e in elementi
        if (isinstance(e.get("raw"), str) and e["raw"]) or (e.get("raw_elenco") or [])
    )
    return {
        "riferimenti": riferimenti,
        "senza_voce": senza_voce,
        "raw_inesistenti": raw_inesistenti,
        "duplicati": duplicati,
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
    print(f"riferimenti 'raw' con percorso sospetto: {len(sospetti)} (ben formati {len(validi)})")
    return 1 if res["senza_voce"] else 0


if __name__ == "__main__":
    if "--autotest" in sys.argv[1:]:
        raise SystemExit(autotest())
    raise SystemExit(main())
