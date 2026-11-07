#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""disegna_risultati.py - genera la schermata risultati variante B (B/N 320x224) per R2.

Legge i testi da motore_nuovo/dati/ui/risultati.json (se esiste) altrimenti usa
dati incorporati coerenti con ENG1 (tutti maiuscole, font 5x7, max 320 px).
Output: assets_originali/ui/risultati/varianteB/risultati_preview.png

Regole: tema bianco e nero (R=G=B), max 16 colori, sfondo nero,
nessun dettaglio sotto i 2 px, nessun asset Sega.
"""
import json
import pathlib

from PIL import Image, ImageDraw

ROOT = pathlib.Path(__file__).resolve().parents[3]  # sglavinated_src
UI = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "ui"
OUT = UI / "risultati" / "varianteB"
DATI_UI = ROOT / "motore_nuovo" / "dati" / "ui"

NERO = (0, 0, 0)
BIANCO = (255, 255, 255)
GRIGIO = (160, 160, 160)

CELL_W, CELL_H = 6, 8
GLYPH_W, GLYPH_H = 5, 7
TARGET_W, TARGET_H = 320, 224

F5x7 = {
    " ": ["     ", "     ", "     ", "     ", "     ", "     ", "     "],
    "!": ["  #  ", "  #  ", "  #  ", "  #  ", "  #  ", "     ", "  #  "],
    "\"": [" # # ", "     ", "     ", "     ", "     ", "     ", "     "],
    "#": ["  #  ", "  #  ", "#####", "  #  ", "#####", "  #  ", "  #  "],
    "$": [" ### ", "#    ", "# ###", "#    ", "#    ", "#    ", " ### "],
    "%": ["#   #", "#  # ", "  #  ", " #   ", "#  # ", "#   #", "#   #"],
    "&": [" ### ", "#   #", "#   #", " ### ", "#   #", "#   #", " ### "],
    "'": ["  #  ", "     ", "     ", "     ", "     ", "     ", "     "],
    "(": ["  #  ", " #   ", "#    ", "#    ", "#    ", " #   ", "  #  "],
    ")": ["  #  ", "   # ", "    #", "    #", "    #", "   # ", "  #  "],
    "*": ["     ", "  #  ", " # # ", "  #  ", " # # ", "  #  ", "     "],
    "+": ["     ", "  #  ", "  #  ", "#####", "  #  ", "  #  ", "     "],
    ",": ["     ", "     ", "     ", "  #  ", "  #  ", "#    ", " #   "],
    "-": ["     ", "     ", "     ", "#####", "     ", "     ", "     "],
    ".": ["     ", "     ", "     ", "     ", "     ", "  #  ", "  #  "],
    "/": ["    #", "   # ", "  #  ", " #   ", "#    ", "     ", "     "],
    "0": [" ### ", "#   #", "#   #", "#   #", "#   #", "#   #", " ### "],
    "1": ["  #  ", " ##  ", "# #  ", "  #  ", "  #  ", "  #  ", " ### "],
    "2": [" ### ", "#   #", "    #", "   # ", "  #  ", " #   ", "#####"],
    "3": [" ### ", "#   #", "    #", "  ## ", "    #", "#   #", " ### "],
    "4": ["     ", "#   #", "#   #", "#   #", "#####", "    #", "    #"],
    "5": ["#####", "#    ", "#    ", " ####", "    #", "#   #", " ### "],
    "6": [" ### ", "#    ", "#    ", " ####", "#   #", "#   #", " ### "],
    "7": ["#####", "    #", "   # ", "  #  ", " #   ", "#    ", "#    "],
    "8": [" ### ", "#   #", "#   #", " ### ", "#   #", "#   #", " ### "],
    "9": [" ### ", "#   #", "#   #", " ####", "    #", "    #", " ### "],
    ":": ["     ", "     ", "  #  ", "     ", "  #  ", "     ", "     "],
    ";": ["     ", "     ", "  #  ", "     ", "  #  ", "#    ", " #   "],
    "<": ["     ", "   # ", "  #  ", " #   ", "  #  ", "   # ", "     "],
    "=": ["     ", "     ", "     ", "#####", "     ", "#####", "     "],
    ">": ["     ", " #   ", "  #  ", "   # ", "  #  ", " #   ", "     "],
    "?": [" ### ", "#   #", "    #", "   # ", "  #  ", "     ", "  #  "],
    "@": [" ### ", "#   #", "# # #", "#   #", "# ###", "#   #", " ### "],
    "A": [" ### ", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
    "B": ["#### ", "#   #", "#### ", "#   #", "#   #", "#   #", "#### "],
    "C": [" ### ", "#    ", "#    ", "#    ", "#    ", "#    ", " ### "],
    "D": ["#### ", "#   #", "#   #", "#   #", "#   #", "#   #", "#### "],
    "E": ["#####", "#    ", "#    ", "#####", "#    ", "#    ", "#####"],
    "F": ["#####", "#    ", "#    ", "#####", "#    ", "#    ", "#    "],
    "G": [" ### ", "#    ", "#    ", "# ###", "#   #", "#   #", " ### "],
    "H": ["#   #", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
    "I": ["#####", "  #  ", "  #  ", "  #  ", "  #  ", "  #  ", "#####"],
    "J": ["#####", "    #", "    #", "    #", "#   #", "#   #", " ### "],
    "K": ["#   #", "#   #", "#  # ", "# #  ", "#  # ", "#   #", "#   #"],
    "L": ["#    ", "#    ", "#    ", "#    ", "#    ", "#    ", "#####"],
    "M": ["#   #", "## ##", "# # #", "# # #", "#   #", "#   #", "#   #"],
    "N": ["#   #", "##  #", "# # #", "#  ##", "#   #", "#   #", "#   #"],
    "O": [" ### ", "#   #", "#   #", "#   #", "#   #", "#   #", " ### "],
    "P": ["#### ", "#   #", "#   #", "#### ", "#    ", "#    ", "#    "],
    "Q": [" ### ", "#   #", "#   #", "#   #", "# # #", "#  # ", " ### "],
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

def draw_text(img, x, y, testo, colore, scale=1):
    cx = x
    for ch in testo:
        if ch == " ":
            cx += CELL_W * scale
            continue
        mk = [[c == "#" for c in row] for row in F5x7.get(ch, F5x7[" "])]
        for ry, row in enumerate(mk):
            for rx, v in enumerate(row):
                if not v:
                    continue
                for dy in range(scale):
                    for dx in range(scale):
                        px, py = x if False else cx + rx * scale + dx, y + ry * scale + dy
                        if 0 <= px < TARGET_W and 0 <= py < TARGET_H:
                            img.putpixel((px, py), colore + (255,))
        cx += CELL_W * scale
    return cx

def rect(img, x, y, w, h, colore):
    for py in range(max(0, y), min(TARGET_H, y + h)):
        for px in range(max(0, x), min(TARGET_W, x + w)):
            img.putpixel((px, py), colore + (255,))

def cornice(img, x, y, w, h, spessore, colore):
    s = max(1, int(spessore))
    rect(img, x, y, w, s, colore)
    rect(img, x, y + h - s, w, s, colore)
    rect(img, x, y, s, h, colore)
    rect(img, x + w - s, y, s, h, colore)

def canvas(w, h, colore=NERO):
    return Image.new("RGBA", (w, h), colore + (255,))

def salva(img, path):
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path)

def carica_json_opzionale(path):
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        return None

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    img = canvas(TARGET_W, TARGET_H)

    # Carica dati se disponibili
    rj = carica_json_opzionale(DATI_UI / "risultati.json")
    if rj:
        titolo = rj.get("titolo", "RISULTATI")
        tuo_nome = rj.get("tuo_nome", "---")
        tuo_score = rj.get("tuo_score", "00000")
        tuo_time = rj.get("tuo_time", "0'00\"00")
        classifica = rj.get("classifica", [])
    else:
        titolo = "RISULTATI"
        tuo_nome = "PL1"
        tuo_score = "012345"
        tuo_time = "1'12\"34"
        classifica = [
            ("ACE", "015000", "1'05\"20"),
            ("BOB", "014200", "1'08\"10"),
            ("CLI", "013500", "1'10\"00"),
            ("DAN", "012800", "1'11\"50"),
            ("PL1", "012345", "1'12\"34"),
            ("EDO", "011000", "1'15\"00"),
            ("FIO", "009800", "1'18\"20"),
            ("GIO", "008500", "1'20\"10"),
            ("LEO", "007200", "1'25\"00"),
            ("MAX", "006000", "1'30\"00"),
        ]

    # Cornice esterna
    cornice(img, 10, 10, 300, 204, 2, BIANCO)

    # Titolo centrato
    tw = len(titolo) * CELL_W
    tx = (TARGET_W - tw) // 2
    draw_text(img, tx, 20, titolo, BIANCO, scale=2)

    # Divisorio sotto il titolo
    rect(img, 10, 50, 300, 2, GRIGIO)

    # Dati giocatore (sinistra)
    draw_text(img, 20, 60, "SCORE", GRIGIO)
    draw_text(img, 20, 70, tuo_score, BIANCO, scale=2)
    draw_text(img, 20, 95, "TIME", GRIGIO)
    draw_text(img, 20, 105, tuo_time, BIANCO, scale=1)
    draw_text(img, 20, 120, "NAME", GRIGIO)
    draw_text(img, 20, 130, tuo_nome, BIANCO, scale=2)

    # Classifica (destra)
    draw_text(img, 170, 60, "RANKING", GRIGIO)
    yy = 72
    for i, (nome, score, time_) in enumerate(classifica[:9]):
        pos = f"{i+1:>2}"
        draw_text(img, 170, yy, pos, GRIGIO)
        draw_text(img, 200, yy, nome, BIANCO)
        draw_text(img, 240, yy, score, GRIGIO)
        draw_text(img, 280, yy, time_, GRIGIO)
        yy += 12

    # Hint in basso
    hint = "PRESS START"
    hw = len(hint) * CELL_W
    draw_text(img, (TARGET_W - hw) // 2, 200, hint, GRIGIO)

    salva(img, OUT / "risultati_preview.png")

    # Audit colori
    ricaricato = Image.open(OUT / "risultati_preview.png")
    cols = ricaricato.convert("RGB").getcolors(1024)
    nomi = sorted(c for _, c in cols)
    cromatici = [c for c in nomi if not (c[0] == c[1] == c[2])]
    assert not cromatici, f"colori cromatici {cromatici}"
    assert len(nomi) <= 16, f"{len(nomi)} colori > 16"
    assert ricaricato.size == (TARGET_W, TARGET_H)
    print(f"OK risultati_preview.png  {ricaricato.size[0]}x{ricaricato.size[1]}  {len(nomi)} colori  {nomi}")
    print("fatto: schermata risultati variante B generata")

if __name__ == "__main__":
    main()
