#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Registro comune degli strumenti (script) che gli agenti costruiscono.

Uso, dalla radice progetto, un solo comando senza pipe:
    python3 motore_nuovo/strumenti_py/indice_strumenti.py
Scrive libreria/STRUMENTI.md: per ogni script nelle cartelle di lavoro, il ruolo
proprietario (l'unico che lo modifica), lo scopo (prima riga del commento in testa)
e il comando d'uso. Lo rigenera il ciclo a ogni giro: nessuno lo scrive a mano.
Segnala gli script senza descrizione e quelli fuori posto (bacheche/, out/).
"""
import os, re, sys

# Cartella -> ruolo che la scrive (come la tabella di AGENTS.md).
PROPRIETARI = [
    ("motore_nuovo/strumenti_py/", "sviluppatore"),
    ("motore_nuovo/test/", "collaudatore"),
    ("libreria/codice/", "sviluppatore"),
    ("libreria/test/", "collaudatore"),
    ("assets_originali/", "grafico"),
    ("tools/", "Claude"),
]
FUORI_POSTO = ("bacheche/", "out/")
SALTA = ("archivio_v4/", "riferimento_v4/", "build", "__pycache__")
USCITA = "libreria/STRUMENTI.md"

if not os.path.isdir("motore_nuovo"):
    print("FALLITO: lanciare dalla radice progetto (sglavinated_src).")
    sys.exit(4)

def descrivi(path):
    """Prima riga utile del commento in testa e prima riga di uso (python3/sh ...)."""
    scopo, uso = "", ""
    with open(path, encoding="utf-8", errors="replace") as f:
        testa = [next(f, "") for _ in range(25)]
    for r in testa:
        t = r.strip().lstrip("#").strip().strip('"').strip()
        if not t or t.startswith("!") or t.startswith("SPDX") or set(t) <= set("─-=*"):
            continue
        if not scopo and not re.match(r"^(import|from|def|class)\b", t):
            scopo = t
        m = re.search(r"((python3|sh) " + re.escape(path) + r"[^`]*?)(\s{2,}|$)", t)
        if m and not uso:
            uso = m.group(1).strip()
        if scopo and uso:
            break
    return scopo, uso

righe, avvisi = [], []
for radice, _, files in os.walk("."):
    for n in sorted(files):
        if not n.endswith((".py", ".sh")):
            continue
        p = os.path.join(radice, n)[2:]
        if any(s in p for s in SALTA):
            continue
        if p.startswith(FUORI_POSTO):
            avvisi.append(f"FUORI POSTO: {p} (gli strumenti non stanno in bacheche/ o out/)")
            continue
        ruolo = next((r for pre, r in PROPRIETARI if p.startswith(pre)), None)
        if ruolo is None:
            continue
        scopo, uso = descrivi(p)
        if not scopo:
            avvisi.append(f"SENZA DESCRIZIONE: {p} (aggiungere in testa scopo e comando d'uso)")
        righe.append((p, ruolo, scopo or "-", uso or (f"python3 {p}" if p.endswith(".py") else f"sh {p}")))

righe.sort(key=lambda x: (x[1], x[0]))
with open(USCITA, "w", encoding="utf-8") as f:
    f.write("# Strumenti del progetto (generato da indice_strumenti.py: non modificare a mano)\n\n")
    f.write("Tutti li usano; li modifica solo il ruolo proprietario. Per un cambio, chiedilo nel tuo diario.\n")
    f.write("Prima di scrivere uno strumento nuovo controlla qui se esiste gia'.\n\n")
    f.write("| Strumento | Proprietario | Scopo | Uso (dalla radice sglavinated_src) |\n|---|---|---|---|\n")
    for p, r, s, u in righe:
        f.write(f"| `{p}` | {r} | {s.replace('|', '/')} | `{u}` |\n")
    if avvisi:
        f.write("\n## Avvisi\n" + "".join(f"- {a}\n" for a in avvisi))
for a in avvisi:
    print(a)
print(f"strumenti registrati: {len(righe)}, avvisi: {len(avvisi)} -> {USCITA}")
