#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Riconverte gli oggetti bordo strada della Libreria nei raw che il motore legge
# (motore_nuovo/dati/sprite_raw/scenari/<livello>/<nome>.sglraw), cosi' i PNG
# ridisegnati dal grafico (es. oggetti alti 27/10) arrivano davvero nel gioco.
# Per ogni raw oggetto gia' presente cerca obj_<nome>.png nella variante A del
# livello e lo riscrive se il PNG e' diverso. Formato: "SGLR" + u32 LE w + u32 LE h + RGBA8888.
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py"
import struct
from pathlib import Path
from PIL import Image

SCENARI = Path("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari")
RAW = Path("motore_nuovo/dati/sprite_raw/scenari")


def png_variante_a(livello, nome):
    for variante in sorted((SCENARI / livello).glob("*varianteA")):
        png = variante / "oggetti" / f"obj_{nome}.png"
        if png.exists():
            return png
    return None


def main():
    n = 0
    for raw in sorted(RAW.glob("*/*.sglraw")):
        nome, livello = raw.stem, raw.parent.name
        if nome == "skyline":
            continue
        png = png_variante_a(livello, nome)
        if png is None:
            continue
        img = Image.open(png).convert("RGBA")
        dati = b"SGLR" + struct.pack("<II", img.width, img.height) + img.tobytes()
        if raw.read_bytes() == dati:
            continue
        raw.write_bytes(dati)
        print(f"aggiornato {raw} ({img.width}x{img.height}) da {png}")
        n += 1
    print(f"raw oggetti aggiornati: {n}")


if __name__ == "__main__":
    main()
