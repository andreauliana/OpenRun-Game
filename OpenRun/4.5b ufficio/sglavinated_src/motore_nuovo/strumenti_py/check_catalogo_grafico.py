#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Controlla assets_originali/CATALOGO_GRAFICO.json (lo scrive @grafico).

Uso, dalla radice progetto, un solo comando senza pipe:
    python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py
Fallisce (exit 1) se: id doppi, stato non previsto, file o raw inesistenti per un
elemento non 'mancante'. Avvisa (senza fallire) per i PNG di assets_originali/
cartelloni, sprite, oggetti e skyline che non compaiono nel catalogo.
"""
import glob, json, os, sys

# La Libreria del Dipartimento Grafico e' l'unica copia degli asset. Nel catalogo i "file"
# iniziano con assets_originali/ (relativi alla Libreria), i "raw" con motore_nuovo/.
LIBRERIA = "UFFICIO/Dipartimento Grafico/Libreria"
CATALOGO = LIBRERIA + "/assets_originali/CATALOGO_GRAFICO.json"


def percorso(f):
    return os.path.join(LIBRERIA, f) if f.startswith("assets_originali/") else f

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

fuori = [f for pat in ("assets_originali/sponsor/*/*.png", "assets_originali/sprite/**/*.png",
                       "assets_originali/scenari/*/*/oggetti/*.png", "assets_originali/scenari/*/*/skyline.png")
         for f in (os.path.relpath(g, LIBRERIA) for g in glob.glob(os.path.join(glob.escape(LIBRERIA), pat), recursive=True))
         if f not in usati]

conta = {}
for e in cat["elementi"]: conta[e.get("stato")] = conta.get(e.get("stato"), 0) + 1
for x in err: print("ERRORE", x)
print("catalogo grafico:", len(cat["elementi"]), "elementi;", ", ".join(f"{k} {v}" for k, v in sorted(conta.items())))
print(f"PNG non catalogati (varianti o doppioni da decidere): {len(fuori)}")
sys.exit(1 if err else 0)
