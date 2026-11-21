#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Controlla GRAFICA/CATALOGO_GRAFICO.json (lo scrive @grafico).

Uso, dalla radice progetto, un solo comando senza pipe:
    python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py
Fallisce (exit 1) se: id doppi, stato non previsto, file o raw inesistenti per un
elemento non 'mancante'. Avvisa (senza fallire) per i PNG di GRAFICA/
cartelloni, sprite, oggetti e skyline che non compaiono nel catalogo.
"""
import glob, json, os, sys

# GRAFICA/ e' l'unica copia degli asset. Nel catalogo i "file"
# iniziano con GRAFICA/ (relativi a sglavinated_src), i "raw" con motore_nuovo/.
LIBRERIA = "."  # i percorsi del catalogo partono da sglavinated_src (GRAFICA/...)
CATALOGO = LIBRERIA + "/GRAFICA/CATALOGO_GRAFICO.json"


def percorso(f):
    return os.path.join(LIBRERIA, f) if f.startswith("GRAFICA/") else f

if not os.path.exists(CATALOGO):
    print(f"FALLITO: {CATALOGO} non trovato. Lanciare dalla radice progetto (sglavinated_src).")
    sys.exit(4)

cat = json.load(open(CATALOGO, encoding="utf-8"))
stati = set(cat["_meta"]["stati"])
err, visti, usati = [], set(), set()
for e in cat["elementi"]:
    i = e.get("id", "?")
    if i in visti: err.append(f"{i}: id doppio")
    visti.add(i)
    if e.get("stato") not in stati: err.append(f"{i}: stato '{e.get('stato')}' non previsto")
    files = e.get("file") or []
    files = [files] if isinstance(files, str) else files
    for f in files + ([e["raw"]] if e.get("raw") else []):
        usati.add(f)
        if e.get("stato") != "mancante" and not os.path.exists(percorso(f)): err.append(f"{i}: file inesistente {f}")

fuori = [f for pat in ("GRAFICA/sponsor/*/*.png", "GRAFICA/sprite/**/*.png",
                       "GRAFICA/scenari/*/oggetti/*.png", "GRAFICA/scenari/*/skyline.png")
         for f in (os.path.relpath(g, LIBRERIA) for g in glob.glob(os.path.join(glob.escape(LIBRERIA), pat), recursive=True))
         if f not in usati]

conta = {}
for e in cat["elementi"]: conta[e.get("stato")] = conta.get(e.get("stato"), 0) + 1
for x in err: print("ERRORE", x)
print("catalogo grafico:", len(cat["elementi"]), "elementi;", ", ".join(f"{k} {v}" for k, v in sorted(conta.items())))
print(f"PNG non catalogati (varianti o doppioni da decidere): {len(fuori)}")
sys.exit(1 if err else 0)
