#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Elenca i PNG di GRAFICA che non compaiono in CATALOGO_GRAFICO.json.

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
NOTI_NON_ASSET = {}


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
    for f in sorted(NOTI_NON_ASSET):
        if f in fuori:
            print(f"  noto non-asset: {f} ({NOTI_NON_ASSET[f]})")
    da_decidere = [f for f in fuori if f not in NOTI_NON_ASSET]
    print(f"png non catalogati: {len(da_decidere)} (noti non-asset {len(fuori) - len(da_decidere)})")
    for f in da_decidere:
        print("  ", f)
    return 0


if __name__ == "__main__":
    sys.exit(main())
