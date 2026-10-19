#!/usr/bin/env python3
"""Generatore deterministico asset UI per @developer (Blocco M).
Disegna da zero: 3 varianti di font pixel, lettering titolo, menu, HUD.
Risoluzione riferimento 320x224. Max 16 colori per elemento.
Nessun riferimento a font/asset Sega. Output: assets_originali/ui/ + motore_nuovo/dati/ui/.
"""
import json, pathlib, sys

ROOT = pathlib.Path(__file__).resolve().parents[3]  # sglavinated_src
UI = ROOT / "assets_originali" / "ui"
DATI_UI = ROOT / "motore_nuovo" / "dati" / "ui"

CELL_W, CELL_H = 6, 8          # cella carattere (5 px glyph + 1 spacing, 7 px + 1 spacing)
GLYPH_W, GLYPH_H = 5, 7        # glyph effettivo

# ---------------------------------------------------------------- palette
# Ogni variante: sfondo, pieno, contorno, ombra, accento, secondario (max 6 usati, sotto 16)
PALETTE = {
    "A": {  # Neon Retro (synthwave / arcade 80s)
        "bg":      (0x05, 0x05, 0x0A),
        "fill":    (0xFF, 0x00, 0xFF),  # magenta
        "outline": (0x00, 0xFF, 0xFF),  # ciano
        "shadow":  (0x4B, 0x00, 0x82),  # viola scuro
        "accent":  (0xFF, 0xFF, 0x00),  # giallo acido
        "second":  (0xFF, 0xFF, 0xFF),  # bianco
    },
    "B": {  # Lofi Jungle (organico, acidi su verde/viola)
        "bg":      (0x0A, 0x1A, 0x0A),
        "fill":    (0x00, 0xFF, 0x00),  # verde acido
        "outline": (0x80, 0x00, 0x80),  # viola
        "shadow":  (0x4B, 0x00, 0x4B),
        "accent":  (0xFF, 0x8C, 0x00),  # arancione
        "second":  (0xCC, 0xCC, 0xCC),
    },
    "C": {  # Sci-Fi Militare (pulito, cromature, finestre)
        "bg":      (0x0A, 0x0A, 0x1A),
        "fill":    (0xFF, 0xFF, 0xFF),  # bianco
        "outline": (0x00, 0x80, 0xFF),  # blu ciano
        "shadow":  (0x40, 0x40, 0x40),
        "accent":  (0xFF, 0x00, 0x00),  # rosso
        "second":  (0xC0, 0xC0, 0xC0),
    },
}

LICENZA_IMM = "CC-BY-4.0"
LICENZA_JSON = "MIT"
AUTORE = "@developer (reparto grafico, progetto Sglavinated)"
URL = "interno al progetto: assets_originali/ui/ (disegnato da zero, nessun asset Sega)"

# ---------------------------------------------------------------- font 5x7
# Dizionario char -> lista di 7 stringhe lunghe 5 ('#' = pieno, ' ' = vuoto)
F5x7 = {
" ": ["     ","     ","     ","     ","     ","     ","     "],
"!": ["  #  ","  #  ","  #  ","  #  ","  #  ","     ","  #  "],
'"': [" # # ","     ","     ","     ","     ","     ","     "],
"#": ["  #  ","  #  ","#####","  #  ","#####","  #  ","  #  "],
"$": [" ### ","#    ","# ### ","#    ","#    ","#    "," ### "],
"%": ["#   #","#  # ","  #  "," #   ","#  # ","#   #","#   #"],
"&": [" ### ","#   #","#   #"," ### ","#   #","#   #"," ### "],
"'": ["  #  ","     ","     ","     ","     ","     ","     "],
"(": ["  #  "," #   ","#    ","#    ","#    "," #   ","  #  "],
")": ["  #  ","   # ","    # ","    # ","    # ","   # ","  #  "],
"*": ["     ","  #  "," # # ","  #  "," # # ","  #  ","     "],
"+": ["     ","  #  ","  #  ","#####","  #  ","  #  ","     "],
",": ["     ","     ","     ","  #  ","  #  ","#    "," #   "],
"-": ["     ","     ","     ","#####","     ","     ","     "],
".": ["     ","     ","     ","     ","     ","  #  ","  #  "],
"/": ["    #","   # ","  #  "," #   ","#    ","     ","     "],
"0": [" ### ","#   #","#   #","#   #","#   #","#   #"," ### "],
"1": ["  #  "," ##  ","# #  ","  #  ","  #  ","  #  "," ### "],
"2": [" ### ","#   #","    #","   # ","  #  "," #   ","#####"],
"3": [" ### ","#   #","    #","  ## ","    #","#   #"," ### "],
"4": ["     ","#   #","#   #","#   #","#####","    #","    #"],
"5": ["#####","#    ","#    "," #### ","    #","#   #"," ### "],
"6": [" ### ","#    ","#    "," #### ","#   #","#   #"," ### "],
"7": ["#####","    #","   # ","  #  "," #   ","#    ","#    "],
"8": [" ### ","#   #","#   #"," ### ","#   #","#   #"," ### "],
"9": [" ### ","#   #","#   #"," #### ","    #","    #"," ### "],
":": ["     ","     ","  #  ","     ","  #  ","     ","     "],
";": ["     ","     ","  #  ","     ","  #  ","#    "," #   "],
"<": ["     ","   # ","  #  "," #   ","  #  ","   # ","     "],
"=": ["     ","     ","     ","#####","     ","#####","     "],
">": ["     "," #   ","  #  ","   # ","  #  "," #   ","     "],
"?": [" ### ","#   #","    #","   # ","  #  ","     ","  #  "],
"@": [" ### ","#   #","# # #","#   #","# ### ","#   #"," ### "],
"A": [" ### ","#   #","#   #","#####","#   #","#   #","#   #"],
"B": ["#### ","#   #","#### ","#   #","#   #","#   #","#### "],
"C": [" ### ","#    ","#    ","#    ","#    ","#    "," ### "],
"D": ["#### ","#   #","#   #","#   #","#   #","#   #","#### "],
"E": ["#####","#    ","#    ","##### ","#    ","#    ","#####"],
"F": ["#####","#    ","#    ","##### ","#    ","#    ","#    "],
"G": [" ### ","#    ","#    ","# ### ","#   #","#   #"," ### "],
"H": ["#   #","#   #","#   #","#####","#   #","#   #","#   #"],
"I": ["#####","  #  ","  #  ","  #  ","  #  ","  #  ","#####"],
"J": ["#####","    #","    #","    #","#   #","#   #"," ### "],
"K": ["#   #","#   #","#  # ","# #  ","#  # ","#   #","#   #"],
"L": ["#    ","#    ","#    ","#    ","#    ","#    ","#####"],
"M": ["#   #","## ##","# # #","# # #","#   #","#   #","#   #"],
"N": ["#   #","##  #","# # #","#  ##","#   #","#   #","#   #"],
"O": [" ### ","#   #","#   #","#   #","#   #","#   #"," ### "],
"P": ["#### ","#   #","#   #","#### ","#    ","#    ","#    "],
"Q": [" ### ","#   #","#   #","#   #","# # #","#  # "," ### "],
"R": ["#### ","#   #","#   #","#### ","#  # ","#   #","#   #"],
"S": [" ### ","#    ","#    "," ### ","    #","#   #"," ### "],
"T": ["#####","  #  ","  #  ","  #  ","  #  ","  #  ","  #  "],
"U": ["#   #","#   #","#   #","#   #","#   #","#   #"," ### "],
"V": ["#   #","#   #","#   #","#   #","#   #"," # # ","  #  "],
"W": ["#   #","#   #","#   #","# # #","# # #","## ##","#   #"],
"X": ["#   #"," # # ","  #  "," # # ","#   #","#   #","#   #"],
"Y": ["#   #"," # # ","  #  ","  #  ","  #  ","  #  ","  #  "],
"Z": ["#####","    #","   # ","  #  "," #   ","#    ","#####"],
"a": ["     "," ### ","#   #","#   #","#####","#   #","#   #"],
"b": ["#    ","#    ","#    ","#### ","#   #","#   #","#### "],
"c": ["     "," ### ","#    ","#    ","#    ","#    "," ### "],
"d": ["    #","    #","    #","#### ","#   #","#   #","#### "],
"e": ["     "," ### ","#    "," #### ","#   #","#   #"," ### "],
"f": ["     ","  #  ","  #  ","##### ","#   #","#   #","#   #"],
"g": ["     "," ### ","#   #","#   #","#####","    #"," ### "],
"h": ["#    ","#    ","#    ","#### ","#   #","#   #","#   #"],
"i": ["     ","  #  ","     ","  #  ","  #  ","  #  ","  #  "],
"j": ["     ","   # ","     ","   # ","   # ","#   #"," ### "],
"k": ["#    ","#   #","#  # ","# #  ","# #  ","#   #","#   #"],
"l": ["#    ","#    ","#    ","#    ","#    ","#    ","#    "],
"m": ["     ","#   #","#   #","#####","#   #","#   #","#   #"],
"n": ["     ","#   #","#   #","#####","#   #","#   #","#   #"],
"o": ["     "," ### ","#   #","#   #","#   #","#   #"," ### "],
"p": ["     ","#### ","#   #","#   #","#### ","#    ","#    "],
"q": ["     ","#### ","#   #","#   #","#### ","    #","    #"],
"r": ["     ","#    ","#    ","##   ","# #  ","#   #","#   #"],
"s": ["     "," ### ","#    "," ### ","    #","#   #"," ### "],
"t": ["     ","#   #","#   #","##### ","#   #","#   #","#   #"],
"u": ["     ","#   #","#   #","#   #","#   #","#   #"," ### "],
"v": ["     ","#   #","#   #","#   #","#   #"," # # ","  #  "],
"w": ["     ","#   #","#   #","#   #","# # #","# # #"," # # "],
"x": ["     ","#   #"," # # ","  #  "," # # ","#   #","#   #"],
"y": ["     ","#   #","#   #","#   #","##### ","    #"," ### "],
"z": ["     ","#####","   # ","  #  "," #   ","#    ","#####"],
"[": [" ####","#    ","#    ","#    ","#    ","#    ","#### "],
"\\": ["#    ","#    "," #   ","  #  ","   # ","    #","     "],
"]": [" ####","    #","    #","    #","    #","    #","#### "],
"^": ["     ","  #  "," # # ","#   #","     ","     ","     "],
"_": ["     ","     ","     ","     ","     ","     ","#####"],
"`": [" #   ","     ","     ","     ","     ","     ","     "],
"{": ["     ","#    ","# #  "," #   ","# #  ","#    ","     "],
"|": ["     ","  #  ","  #  ","  #  ","  #  ","  #  ","  #  "],
"}": ["     ","    #"," # # "," #   "," # # ","    #","     "],
"~": ["     ","     ","# # #","     ","     ","     ","     "],
}

ORDER = list(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`{|}~")
assert len(ORDER) <= 90, f"troppi caratteri: {len(ORDER)}"

# ---------------------------------------------------------------- titolo 7x9 (block)
# Lettere necessarie per "Sglavinated Edition": S g l a v i n a t e d E o (+ spazio)
T7x9 = {
"S": [" ####","#    ","#    "," ### ","    #","#   #"," #### ","     ","     "],
"g": ["     ","  ###"," #   #"," #   #","  ####","     #","  ### ","     ","     "],
"l": [" #   "," #   "," #   "," #   "," #   "," #   "," ### ","     ","     "],
"a": ["     ","  ## "," #   #"," #   #"," #####"," #   #"," #   #","     ","     "],
"v": [" #   #"," #   #"," #   #"," #   #","  # # ","  # # ","   #  ","     ","     "],
"i": [" #   "," #   ","     "," #   "," #   "," #   "," #   ","     ","     "],
"n": [" #   #"," ##  #"," # # #"," #  ##"," #   #"," #   #"," #   #","     ","     "],
"t": [" #   #"," #   #"," ##### "," #   #"," #   #"," #   #"," #   #","     ","     "],
"e": ["     ","  ###"," #    ","  ####"," #   #"," #   #","  ### ","     ","     "],
"d": ["     ","    # ","    # ","  ####"," #   #"," #   #","  ####","     ","     "],
"E": [" #####"," #    "," #    ","  #### "," #    "," #    "," ##### ","     ","     "],
"o": ["     ","  ### "," #   # "," #   # "," #   # "," #   # ","  ### ","     ","     "],
}
TITLE = "Sglavinated Edition"

# ---------------------------------------------------------------- helpers
def glyph_mask(rows):
    return [[c == '#' for c in row] for row in rows]

def draw_glyph(img, x, y, mask, color, scale=1):
    for ry, row in enumerate(mask):
        for rx, v in enumerate(row):
            if v:
                for dy in range(scale):
                    for dx in range(scale):
                        img.putpixel((x + rx*scale + dx, y + ry*scale + dy), color)

def rect(img, x, y, w, h, color):
    for py in range(y, y+h):
        for px in range(x, x+w):
            img.putpixel((px, py), color)

def new_canvas(w, h, color):
    from PIL import Image
    return Image.new("RGBA", (w, h), color)

def save(img, path):
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path)

def manifest(var, kind, files, note=""):
    return {
        "id": f"ui_{kind}_{var.lower()}",
        "tipo": kind,
        "variante": var,
        "descrizione": f"{kind} variante {var} - UI 320x224",
        "stato": "proposta",
        "licenza": LICENZA_IMM if kind in ("font","titolo","menu","hud") else LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "risoluzione": [320, 224],
        "colori_max": 16,
        "file": files,
        "note": note,
    }

# ---------------------------------------------------------------- font sheet
def build_font(var):
    pal = PALETTE[var]
    cols = 10
    rows = (len(ORDER) + cols - 1) // cols
    sheet_w = cols * CELL_W
    sheet_h = rows * CELL_H
    img = new_canvas(sheet_w, sheet_h, pal["bg"])
    mapping = {}
    for i, ch in enumerate(ORDER):
        cx = (i % cols) * CELL_W
        cy = (i // cols) * CELL_H
        mask = glyph_mask(F5x7[ch])
        # disegno glyph
        draw_glyph(img, cx + 1, cy + 1, mask, pal["fill"])
        # contorno 1px (solo dove c'è glyph, dilatazione 1)
        for ry, row in enumerate(mask):
            for rx, v in enumerate(row):
                if v:
                    for dy in (-1, 0, 1):
                        for dx in (-1, 0, 1):
                            if dx == 0 and dy == 0: continue
                            px, py = cx + 1 + rx + dx, cy + 1 + ry + dy
                            if 0 <= px < sheet_w and 0 <= py < sheet_h:
                                if img.getpixel((px, py))[:3] == pal["bg"]:
                                    img.putpixel((px, py), pal["outline"])
        mapping[ch] = {"x": cx, "y": cy, "w": GLYPH_W, "h": GLYPH_H,
                       "advance": CELL_W, "offset_x": 1, "offset_y": 1}
    font_json = {
        "id": f"font_pixel_{var.lower()}",
        "descrizione": f"font pixel 5x7 variante {var}, disegnato da zero",
        "stato": "proposta",
        "licenza": LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "sheet": f"font/variante{var}/font.png",
        "sheet_w": sheet_w, "sheet_h": sheet_h,
        "cell_w": CELL_W, "cell_h": CELL_H,
        "glyph_w": GLYPH_W, "glyph_h": GLYPH_H,
        "baseline": 6,
        "chars": mapping,
    }
    return img, font_json

# ---------------------------------------------------------------- titolo
def build_title(var):
    pal = PALETTE[var]
    # calcola larghezza: ogni lettera 8px cella (7 glyph + 1 spazio), scala 2 -> 16px/lettera
    scale = 2
    cell = 8
    w = len(TITLE) * cell * scale
    h = 9 * scale
    img = new_canvas(w, h, pal["bg"])
    x = 0
    for ch in TITLE:
        if ch == " ":
            x += cell * scale
            continue
        mask = glyph_mask(T7x9[ch])
        # ombra (offset 2,2) colore shadow
        draw_glyph(img, x + 2*scale, 2*scale, mask, pal["shadow"], scale=scale)
        # contorno: dilatazione del glyph 1px (in unità scala) colore outline
        for ry, row in enumerate(mask):
            for rx, v in enumerate(row):
                if v:
                    for dy in (-1, 0, 1):
                        for dx in (-1, 0, 1):
                            if dx == 0 and dy == 0: continue
                            px = x + (rx + dx)*scale
                            py = (ry + dy)*scale
                            if 0 <= px < w and 0 <= py < h:
                                if img.getpixel((px, py))[:3] == pal["bg"]:
                                    img.putpixel((px, py), pal["outline"])
        # riempito
        draw_glyph(img, x, 0, mask, pal["fill"], scale=scale)
        x += cell * scale
    title_json = {
        "id": f"titolo_{var.lower()}",
        "descrizione": f"lettering titolo 'Sglavinated Edition' variante {var}",
        "stato": "proposta",
        "licenza": LICENZA_IMM,
        "autore": AUTORE,
        "url": URL,
        "file": f"titolo/variante{var}/title.png",
        "w": w, "h": h,
        "testo": TITLE,
        "scala": scale,
        "posizione_suggerita": {"x": (320 - w)//2, "y": 24},
    }
    return img, title_json

# ---------------------------------------------------------------- menu preview
MENU_VOCI = [
    ("GUIDA", "start"),
    ("RADIO", "radio"),
    ("SHOW", "show"),
    ("OPZIONI", "opzioni"),
    ("CREDITI", "crediti"),
    ("ESCI", "quit"),
]

def build_menu_preview(var):
    pal = PALETTE[var]
    img = new_canvas(320, 224, pal["bg"])
    # bordo esterno
    rect(img, 20, 20, 280, 184, pal["outline"])
    rect(img, 22, 22, 276, 180, pal["bg"])
    # titolo piccolo
    t = "SGLavinated"
    mask = glyph_mask(F5x7[" "])  # placeholder, disegniamo testo con font 1x
    y = 40
    for i, (voce, az) in enumerate(MENU_VOCI):
        sel = (i == 0)
        bar_y = 60 + i * 22
        if sel:
            rect(img, 40, bar_y, 200, 16, pal["accent"])
            col = pal["bg"]
        else:
            col = pal["fill"]
        # disegna voce con font 5x7 scala 1
        draw_text(img, 46, bar_y + 2, voce, col, scale=1)
        draw_text(img, 230, bar_y + 2, az.upper(), pal["second"], scale=1)
    # indicatore RetroPad
    draw_text(img, 46, 200, "SU/GIU scegli  A conferma  B indietro", pal["second"], scale=1)
    menu_json = {
        "id": f"menu_{var.lower()}",
        "descrizione": f"menu principale variante {var}",
        "stato": "proposta",
        "licenza": LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "preview": f"menu/variante{var}/menu_preview.png",
        "risoluzione": [320, 224],
        "navigazione": "RetroPad: SU/GIU sposta selezione, A conferma, B indietro/indietro",
        "voci": [
            {"label": v, "azione": a, "x": 46, "y": 60 + i*22, "w": 200, "h": 16,
             "colore_sel": pal["accent"], "colore_testo": pal["fill"], "colore_disabilitato": pal["second"]}
            for i, (v, a) in enumerate(MENU_VOCI)
        ],
        "voce_iniziale": 0,
        "sfondo": f"menu/variante{var}/menu_preview.png",
    }
    return img, menu_json

def draw_text(img, x, y, testo, colore, scale=1, font_rows=F5x7):
    cx = x
    for ch in testo:
        if ch == " ":
            cx += CELL_W * scale
            continue
        mask = glyph_mask(font_rows[ch])
        draw_glyph(img, cx, y, mask, colore, scale=scale)
        cx += CELL_W * scale

# ---------------------------------------------------------------- HUD preview
def build_hud_preview(var):
    pal = PALETTE[var]
    img = new_canvas(320, 224, pal["bg"])
    # cornice HUD angoli
    rect(img, 8, 8, 120, 6, pal["outline"])
    rect(img, 8, 8, 6, 56, pal["outline"])
    rect(img, 8, 58, 120, 6, pal["outline"])
    rect(img, 122, 8, 6, 56, pal["outline"])
    # etichette e valori (mock)
    draw_text(img, 16, 12, "KM", pal["second"], scale=1)
    draw_text(img, 16, 24, "000", pal["fill"], scale=2)
    draw_text(img, 16, 48, "RAZZO", pal["second"], scale=1)
    rect(img, 16, 58, 60, 6, pal["outline"])
    rect(img, 18, 60, 44, 2, pal["accent"])
    # velocita' a destra
    draw_text(img, 200, 12, "VEL", pal["second"], scale=1)
    draw_text(img, 200, 24, "120", pal["fill"], scale=3)
    draw_text(img, 240, 44, "KM/H", pal["second"], scale=1)
    # indicatore show
    draw_text(img, 200, 80, "SHOW OFF", pal["accent"], scale=1)
    hud_json = {
        "id": f"hud_{var.lower()}",
        "descrizione": f"HUD di gioco variante {var}",
        "stato": "proposta",
        "licenza": LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "preview": f"hud/variante{var}/hud_preview.png",
        "risoluzione": [320, 224],
        "elementi": [
            {"id": "distanza", "tipo": "testo", "label": "KM", "valore": "000",
             "label_pos": [16, 12], "valore_pos": [16, 24], "scala_valore": 2,
             "colore": pal["fill"], "colore_label": pal["second"]},
            {"id": "carica_razzo", "tipo": "barra", "label": "RAZZO",
             "barra_pos": [16, 58], "barra_w": 60, "barra_h": 6,
             "colore_bordo": pal["outline"], "colore_riempimento": pal["accent"]},
            {"id": "velocita", "tipo": "testo", "label": "VEL", "valore": "120",
             "label_pos": [200, 12], "valore_pos": [200, 24], "scala_valore": 3,
             "colore": pal["fill"], "colore_label": pal["second"]},
            {"id": "unita_vel", "tipo": "testo", "testo": "KM/H", "pos": [240, 44],
             "scala": 1, "colore": pal["second"]},
            {"id": "show", "tipo": "testo", "testo": "SHOW OFF", "pos": [200, 80],
             "scala": 1, "colore": pal["accent"]},
        ],
    }
    return img, hud_json

# ---------------------------------------------------------------- main
def main():
    varianti = ["A", "B", "C"]
    for var in varianti:
        # font
        img, fj = build_font(var)
        fdir = UI / "font" / f"variante{var}"
        save(img, fdir / "font.png")
        (fdir / "font.json").write_text(json.dumps(fj, indent=2, ensure_ascii=False), encoding="utf-8")
        (fdir / "manifest.json").write_text(json.dumps(manifest(var, "font", ["font.png", "font.json"], "Font pixel 5x7 disegnato da zero."), indent=2, ensure_ascii=False), encoding="utf-8")
        # titolo
        img, tj = build_title(var)
        tdir = UI / "titolo" / f"variante{var}"
        save(img, tdir / "title.png")
        (tdir / "title.json").write_text(json.dumps(tj, indent=2, ensure_ascii=False), encoding="utf-8")
        (tdir / "manifest.json").write_text(json.dumps(manifest(var, "titolo", ["title.png", "title.json"], "Lettering titolo 'Sglavinated Edition' disegnato da zero."), indent=2, ensure_ascii=False), encoding="utf-8")
        # menu
        img, mj = build_menu_preview(var)
        mdir = UI / "menu" / f"variante{var}"
        save(img, mdir / "menu_preview.png")
        (mdir / "manifest.json").write_text(json.dumps(manifest(var, "menu", ["menu_preview.png"], "Mockup menu principale. Dati reali in motore_nuovo/dati/ui/menu.json."), indent=2, ensure_ascii=False), encoding="utf-8")
        # hud
        img, hj = build_hud_preview(var)
        hdir = UI / "hud" / f"variante{var}"
        save(img, hdir / "hud_preview.png")
        (hdir / "manifest.json").write_text(json.dumps(manifest(var, "hud", ["hud_preview.png"], "Mockup HUD. Dati reali in motore_nuovo/dati/ui/hud.json."), indent=2, ensure_ascii=False), encoding="utf-8")
        print(f"variante {var} OK")

    # JSON dati per @developer: selezione variante A di default (Andrea sceglie dopo)
    DATI_UI.mkdir(parents=True, exist_ok=True)
    with (UI / "font" / "varianteA" / "font.json").open(encoding="utf-8") as f:
        fontA = json.load(f)
    with (UI / "titolo" / "varianteA" / "title.json").open(encoding="utf-8") as f:
        titleA = json.load(f)
    with (UI / "menu" / "varianteA" / "manifest.json").open(encoding="utf-8") as f:
        manA = json.load(f)

    menu_json = {
        "id": "menu_principale",
        "descrizione": "Menu principale come dati (letto da motore_nuovo/src/ui/). Variante selezionata: A (default, da confermare da Andrea).",
        "stato": "proposta",
        "licenza": LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "variante": "A",
        "asset_dir": "../../assets_originali/ui",
        "font_sheet": "font/varianteA/font.png",
        "font_meta": "font/varianteA/font.json",
        "titolo_immagine": "titolo/varianteA/title.png",
        "titolo_meta": "titolo/varianteA/title.json",
        "preview": "menu/varianteA/menu_preview.png",
        "risoluzione": [320, 224],
        "navigazione": "RetroPad: SU/GIU sposta selezione, A conferma, B indietro",
        "voci": mj["voci"],
        "voce_iniziale": mj["voce_iniziale"],
        "colore_sel": mj["voci"][0]["colore_sel"],
    }
    (DATI_UI / "menu.json").write_text(json.dumps(menu_json, indent=2, ensure_ascii=False), encoding="utf-8")

    with (UI / "hud" / "varianteA" / "manifest.json").open(encoding="utf-8") as f:
        manH = json.load(f)
    hud_json = {
        "id": "hud_gioco",
        "descrizione": "HUD di gioco come dati (letto da motore_nuovo/src/ui/). Variante selezionata: A (default, da confermare da Andrea).",
        "stato": "proposta",
        "licenza": LICENZA_JSON,
        "autore": AUTORE,
        "url": URL,
        "variante": "A",
        "asset_dir": "../../assets_originali/ui",
        "preview": "hud/varianteA/hud_preview.png",
        "risoluzione": [320, 224],
        "elementi": hj["elementi"],
    }
    (DATI_UI / "hud.json").write_text(json.dumps(hud_json, indent=2, ensure_ascii=False), encoding="utf-8")

    print("menu.json + hud.json scritti in motore_nuovo/dati/ui/")

if __name__ == "__main__":
    main()
