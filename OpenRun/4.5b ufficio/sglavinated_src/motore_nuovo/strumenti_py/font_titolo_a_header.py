#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Converte il font pixel Born2bSporty V2 (CC0, TTF disegnato su griglia di
16 px per em) in motore_nuovo/src/render/font_titolo.h: font grassetto per
titoli e numeri grandi dell'HUD (Andrea 09/11). Proporzionale: ogni carattere
ha la sua larghezza. Uso, dalla radice sglavinated_src:
    python3 motore_nuovo/strumenti_py/font_titolo_a_header.py
Richiede Pillow. Rigenerare l'header ogni volta che cambia il font."""
import sys
from PIL import Image, ImageDraw, ImageFont

TTF = "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/font/born2bsporty/Born2bSportyV2.ttf"
OUT = "motore_nuovo/src/render/font_titolo.h"
PPEM = 16  # 1 pixel del font = 64 unita' su 1024


def main():
    font = ImageFont.truetype(TTF, PPEM)
    glifi = []
    for code in range(32, 127):
        ch = chr(code)
        img = Image.new("1", (32, 32), 0)
        d = ImageDraw.Draw(img)
        d.fontmode = "1"  # niente antialias: pixel netti
        d.text((8, 8), ch, font=font, fill=1)
        avanz = round(font.getlength(ch))
        px = {(x - 8, y - 8) for y in range(32) for x in range(32) if img.getpixel((x, y))}
        glifi.append((ch, avanz, px))
    tutti = [p for _, _, px in glifi for p in px]
    y0 = min(y for _, y in tutti)
    y1 = max(y for _, y in tutti)
    x0 = min(x for x, _ in tutti)
    alto = y1 - y0 + 1
    largo = max(max((x for x, _ in px), default=0) - x0 + 1 for _, _, px in glifi)
    assert largo <= 16, "glifo piu' largo di 16 px"
    # Riga della cima delle cifre (per allineare i numeri dell'HUD).
    cima = min(y for x, y in glifi[ord("0") - 32][2]) - y0
    base = max(y for x, y in glifi[ord("0") - 32][2]) - y0
    righe = []
    for ch, avanz, px in glifi:
        bits = [0] * alto
        for x, y in px:
            bits[y - y0] |= 1 << (15 - (x - x0))
        testo = repr(ch) if ch != "\\" else "'\\\\'"
        righe.append("    {%d, {%s}}, // %s" % (avanz, ", ".join("0x%04X" % v for v in bits), testo))
    cifra = max(a for ch, a, _ in glifi if ch.isdigit())
    out = [
        "// SPDX-License-Identifier: MIT",
        "// GENERATO da motore_nuovo/strumenti_py/font_titolo_a_header.py: non modificare a mano.",
        "// Font pixel Born2bSporty V2 di JapanYoshi (CC0-1.0, pubblico dominio),",
        "// grassetto con tratti di 2 px, proporzionale, caratteri ASCII 32..126.",
        "// Ogni riga e' una maschera di 16 bit (bit alto = colonna sinistra).",
        "#pragma once",
        '#include "core/types.h"',
        "",
        "namespace sgl {",
        "constexpr i32 FONT_TITOLO_H = %d;      // righe per carattere" % alto,
        "constexpr i32 FONT_TITOLO_CIMA = %d;   // prima riga accesa delle cifre" % cima,
        "constexpr i32 FONT_TITOLO_BASE = %d;   // ultima riga accesa delle cifre (linea di base)" % base,
        "constexpr i32 FONT_TITOLO_CIFRA = %d;  // passo fisso delle cifre (numeri incolonnati)" % cifra,
        "constexpr i32 FONT_TITOLO_X0 = %d;     // colonna 0 della maschera rispetto alla penna" % x0,
        "struct GlifoTitolo { u8 avanzamento; u16 righe[FONT_TITOLO_H]; };",
        "constexpr GlifoTitolo FONT_TITOLO[95] = {",
        *righe,
        "};",
        "} // namespace sgl",
        "",
    ]
    open(OUT, "w", encoding="utf-8").write("\n".join(out))
    print("scritto", OUT, "altezza", alto, "cima cifre", cima, "passo cifre", cifra)


if __name__ == "__main__":
    sys.exit(main())
