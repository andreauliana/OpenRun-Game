#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Elenca i PNG della Libreria che non compaiono in CATALOGO_GRAFICO.json.

Stessa regola di motore_nuovo/strumenti_py/check_catalogo_grafico.py (che li conta
e avvisa senza fallire): cartelloni, sprite, oggetti e skyline. Serve a decidere,
uno per uno, se sono varianti/doppioni da scartare o elementi da catalogare.

Uso, dalla radice sglavinated_src:
    python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"
Sola lettura: non scrive ne' modifica file. Exit 0 sempre.
"""
import glob
import json
import os
import pathlib
import sys

RADICE = pathlib.Path(__file__).resolve().parents[3]
LIBRERIA = RADICE / "UFFICIO/Dipartimento Grafico/Libreria"
CATALOGO = LIBRERIA / "assets_originali/CATALOGO_GRAFICO.json"

PATTERN = (
    "assets_originali/sponsor/*/*.png",
    "assets_originali/sprite/**/*.png",
    "assets_originali/scenari/*/*/oggetti/*.png",
    "assets_originali/scenari/*/*/skyline.png",
)


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
    print(f"png non catalogati: {len(fuori)}")
    for f in fuori:
        print("  ", f)
    return 0


if __name__ == "__main__":
    sys.exit(main())
