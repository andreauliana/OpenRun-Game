#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# ridisegna_sponsor.py — MIG1 G1 (Andrea 08/11 sera): cartelloni sponsor leggibili.
# Stesso nome file e stessa misura 128x64 RGBA; cambiato il disegno:
#   - una parola corta e grande (font pixel 5x7 in scala, contorno scuro 1 px);
#   - 2-3 colori per stile (fg, txt + duo di sfondo), niente colori saturi puri;
#   - dithering Bayer 4x4 SOLO sullo sfondo;
#   - resto del nome piccolo in basso (il nome display resta identico).
# Prima/dopo in UFFICIO/Officina/Test area/grafico/mig1_g1/{prima,dopo}/.
# Marchi tutti inventati, nessun contenuto Sega. Licenza CC0-1.0.
import shutil
import pathlib

from PIL import Image

ROOT = pathlib.Path(__file__).resolve().parents[3]  # sglavinated_src
SPONSOR = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "sponsor"
ANTEPRIME = ROOT / "UFFICIO" / "Officina" / "Test area" / "grafico" / "mig1_g1"
STILI = ("neon", "atomico", "foglie")

# Parola grande + resto del nome (nome_display resta lo stesso)
NOMI = {
    "curve_pericolose": ("CURVE", "PERICOLOSE"),
    "brinis_udine": ("BRINIS", "UDINE"),
    "piston_high": ("PISTON", "HIGH"),
    "canna_bis": ("CANNA-BIS", ""),
    "slow_down": ("SLOW", "DOWN"),
    "erba_medica": ("ERBA", "MEDICA"),
    "green_box": ("GREEN", "BOX"),
}

# Palette per stile: duo di sfondo + colore principale + colore testo + contorno scuro
PALETTE = {
    "neon":    {"bg": (24, 8, 48),    "bg2": (36, 16, 68),   "fg": (214, 98, 206), "txt": (118, 230, 214), "scuro": (10, 4, 22)},
    "atomico": {"bg": (44, 52, 48),   "bg2": (58, 68, 62),   "fg": (206, 82, 86),  "txt": (232, 226, 204), "scuro": (18, 22, 20)},
    "foglie":  {"bg": (18, 44, 18),   "bg2": (30, 62, 28),   "fg": (168, 214, 88), "txt": (232, 180, 224), "scuro": (8, 20, 10)},
}

BAYER4 = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]

# Font pixel 5x7 (stessa fonte di disegna_risultati.py)
F5X7 = {
    "-": ["     ", "     ", "     ", "#####", "     ", "     ", "     "],
    "A": [" ### ", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
    "B": ["#### ", "#   #", "#### ", "#   #", "#   #", "#   #", "#### "],
    "C": [" ### ", "#    ", "#    ", "#    ", "#    ", "#    ", " ### "],
    "D": ["#### ", "#   #", "#   #", "#   #", "#   #", "#   #", "#### "],
    "E": ["#####", "#    ", "#    ", "#####", "#    ", "#    ", "#####"],
    "G": [" ### ", "#    ", "#    ", "# ###", "#   #", "#   #", " ### "],
    "H": ["#   #", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
    "I": ["#####", "  #  ", "  #  ", "  #  ", "  #  ", "  #  ", "#####"],
    "L": ["#    ", "#    ", "#    ", "#    ", "#    ", "#    ", "#####"],
    "M": ["#   #", "## ##", "# # #", "# # #", "#   #", "#   #", "#   #"],
    "N": ["#   #", "##  #", "# # #", "#  ##", "#   #", "#   #", "#   #"],
    "O": [" ### ", "#   #", "#   #", "#   #", "#   #", "#   #", " ### "],
    "P": ["#### ", "#   #", "#   #", "#### ", "#    ", "#    ", "#    "],
    "R": ["#### ", "#   #", "#   #", "#### ", "#  # ", "#   #", "#   #"],
    "S": [" ### ", "#    ", "#    ", " ### ", "    #", "#   #", " ### "],
    "T": ["#####", "  #  ", "  #  ", "  #  ", "  #  ", "  #  ", "  #  "],
    "U": ["#   #", "#   #", "#   #", "#   #", "#   #", "#   #", " ### "],
    "V": ["#   #", "#   #", "#   #", "#   #", "#   #", " # # ", "  #  "],
    "W": ["#   #", "#   #", "#   #", "# # #", "# # #", "## ##", "#   #"],
    "X": ["#   #", " # # ", "  #  ", " # # ", "#   #", "#   #", "#   #"],
    "Y": ["#   #", " # # ", "  #  ", "  #  ", "  #  ", "  #  ", "  #  "],
    "Z": ["#####", "    #", "   # ", "  #  ", " #   ", "#    ", "#####"],
}

W, H = 128, 64
CELL = 6  # 5 px glifo + 1 px spazio


def larghezza(testo, scala):
    if not testo:
        return 0
    return (len(testo) * CELL - 1) * scala


def disegna_testo(img, x, y, testo, scala, colore, contorno=None):
    for i, ch in enumerate(testo):
        glifo = F5X7.get(ch)
        if glifo is None:
            continue
        bx = x + i * CELL * scala
        for ry, riga in enumerate(glifo):
            for rx, c in enumerate(riga):
                if c != "#":
                    continue
                px0 = bx + rx * scala
                py0 = y + ry * scala
                if contorno is not None:
                    for ox in range(-1, scala + 1):
                        for oy in range(-1, scala + 1):
                            img.putpixel((px0 + ox, py0 + oy), contorno + (255,))
                for dy in range(scala):
                    for dx in range(scala):
                        img.putpixel((px0 + dx, py0 + dy), colore + (255,))


def genera(id_sponsor, stile):
    pal = PALETTE[stile]
    grande, piccolo = NOMI[id_sponsor]
    img = Image.new("RGBA", (W, H), pal["bg"] + (255,))
    p = img.load()
    for y in range(H):
        for x in range(W):
            if BAYER4[x % 4][y % 4] < 7:
                p[x, y] = pal["bg2"] + (255,)
    for x in range(W):
        for y in (0, 1, H - 2, H - 1):
            p[x, y] = pal["fg"] + (255,)
    for y in range(H):
        for x in (0, 1, W - 2, W - 1):
            p[x, y] = pal["fg"] + (255,)
    for x in range(2, W - 2):
        p[x, 2] = pal["scuro"] + (255,)
        p[x, H - 3] = pal["scuro"] + (255,)
    for y in range(2, H - 2):
        p[2, y] = pal["scuro"] + (255,)
        p[W - 3, y] = pal["scuro"] + (255,)
    scala = max(1, min(5, 116 // larghezza(grande, 1)))
    h_g = 7 * scala
    y_g = 30 - h_g // 2
    x_g = (W - larghezza(grande, scala)) // 2
    disegna_testo(img, x_g, y_g, grande, scala, pal["txt"], contorno=pal["scuro"])
    if piccolo:
        x_s = (W - larghezza(piccolo, 1)) // 2
        disegna_testo(img, x_s, 53, piccolo, 1, pal["fg"])
    return img


def main():
    prima = ANTEPRIME / "prima"
    dopo = ANTEPRIME / "dopo"
    prima.mkdir(parents=True, exist_ok=True)
    dopo.mkdir(parents=True, exist_ok=True)
    n = 0
    for id_sponsor in sorted(NOMI):
        sdir = SPONSOR / id_sponsor
        sdir.mkdir(exist_ok=True)
        for stile in STILI:
            dest = sdir / f"cartellone_{stile}.png"
            if dest.exists():
                shutil.copy(dest, prima / f"{id_sponsor}_{stile}.png")
            genera(id_sponsor, stile).save(dest)
            shutil.copy(dest, dopo / f"{id_sponsor}_{stile}.png")
            n += 1
    print(f"ridisegnati {n} cartelloni 128x64; anteprime in {prima.relative_to(ROOT)} e {dopo.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
