#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Converte il font pixel dell'HUD (assets_originali/ui/font/varianteB/font.png
+ font.json) in motore_nuovo/src/render/font_hud.h, cosi' il core disegna i
testi senza decodificare PNG. Uso, dalla radice sglavinated_src:
    python3 motore_nuovo/strumenti_py/font_a_header.py
Richiede Pillow. Rigenerare l'header ogni volta che cambia il font."""
import json
import sys
from PIL import Image

UI = "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/ui/"
META = UI + "font/varianteB/font.json"
OUT = "motore_nuovo/src/render/font_hud.h"


def main():
    meta = json.load(open(META, encoding="utf-8"))
    img = Image.open(UI + meta["sheet"].split("ui/")[-1] if "ui/" in meta["sheet"] else UI + meta["sheet"]).convert("RGBA")
    cw, ch = meta["cell_w"], meta["cell_h"]
    righe = []
    for code in range(32, 127):
        c = meta["chars"].get(chr(code))
        bits = [0] * ch
        if c:
            for y in range(ch):
                for x in range(cw):
                    r, g, b, a = img.getpixel((c["x"] + x, c["y"] + y))
                    if a >= 128 and r + g + b > 384:
                        bits[y] |= 1 << (cw - 1 - x)
        testo = repr(chr(code)) if code != 92 else "'\\\\'"
        righe.append("    {%s}, // %s" % (", ".join("0x%02X" % v for v in bits), testo))
    out = [
        "// SPDX-License-Identifier: MIT",
        "// GENERATO da motore_nuovo/strumenti_py/font_a_header.py: non modificare a mano.",
        "// Font pixel %s (%s), celle %dx%d, caratteri ASCII 32..126." % (meta["id"], meta["licenza"], cw, ch),
        "// Ogni riga e' una maschera di %d bit (bit alto = colonna sinistra)." % cw,
        "#pragma once",
        '#include "core/types.h"',
        "",
        "namespace sgl {",
        "constexpr i32 FONT_HUD_W = %d;" % cw,
        "constexpr i32 FONT_HUD_H = %d;" % ch,
        "constexpr u8 FONT_HUD[95][%d] = {" % ch,
        *righe,
        "};",
        "} // namespace sgl",
        "",
    ]
    open(OUT, "w", encoding="utf-8").write("\n".join(out))
    print("scritto", OUT)


if __name__ == "__main__":
    sys.exit(main())
