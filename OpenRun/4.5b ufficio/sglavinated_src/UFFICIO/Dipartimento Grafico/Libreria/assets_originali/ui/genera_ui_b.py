#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""genera_ui_b.py - rigenera gli asset UI della VARIANTE B in bianco e nero (D7).

Decisione D7 (Andrea): variante B (Lofi Jungle) per menu/HUD, tema bianco e nero.
Questo script produce (solo variante B):

  assets_originali/ui/font/varianteB/font.png        font pixel 5x7, 2 colori
  assets_originali/ui/titolo/varianteB/title.png     lettering con Antonio Bold, 2 colori
  assets_originali/ui/menu/varianteB/menu_preview.png    320x224, da dati/ui/menu.json
  assets_originali/ui/hud/varianteB/hud_preview.png      320x224, da dati/ui/hud.json

Regole rispettate:
  - tema "bianco e nero": nessun colore cromatico (R=G=B su ogni pixel), max 16
    colori per immagine, sfondo nero [0,0,0];
  - risoluzione target 320x224 per le preview, leggibili su TV a bassa risoluzione
    (nessun dettaglio piu' fine di 2 px);
  - preview e motore restano coerenti: menu_preview e hud_preview sono disegnati
    LEGGENDO motore_nuovo/dati/ui/menu.json e hud.json (posizioni, colori, regole);
  - nessun font Sega ne' asset Sega: glifi 5x7 disegnati da zero, titolo
    rasterizzato con Antonio Bold (OFL-1.1, Impallari Type) gia' in progetto;
  - determinismo: nessun orologio di sistema, nessuna rete, output identico a
    ogni esecuzione (stesso Pillow/FreeType).

Codice MIT (decisione D1, licenza dichiarata in testa + manifest cartella).
PNG prodotti CC-BY-4.0 (vedi manifest.json delle singole cartelle).
Antonio Bold: OFL-1.1, Impallari Type - assets_originali/font/antonio/ (CREDITI.md).

Uso: python3 assets_originali/ui/genera_ui_b.py
"""
import json
import pathlib
import sys

from PIL import Image, ImageDraw, ImageFont

try:  # Pillow >= 9.1
    LANCZOS = Image.Resampling.LANCZOS
except AttributeError:  # pragma: no cover
    LANCZOS = Image.LANCZOS

ROOT = pathlib.Path(__file__).resolve().parents[5]  # sglavinated_src
UI = ROOT / "assets_originali" / "ui"
DATI_UI = ROOT / "motore_nuovo" / "dati" / "ui"
ANTONIO = ROOT / "assets_originali" / "font" / "antonio" / "Antonio-Bold.ttf"

NERO = (0, 0, 0)
BIANCO = (255, 255, 255)

CELL_W, CELL_H = 6, 8       # cella carattere (glyph 5x7 + 1 px di stacco)
GLYPH_W, GLYPH_H = 5, 7
FONT_COLS = 10

TARGET_W, TARGET_H = 320, 224
MAX_COLORI = 16
TITLE_MAX_W = 304           # larghezza titolo usata nella bozza, resta il target
UPS = 4                     # supersampling per il titolo rasterizzato

# ------------------------------------------------------------------ font 5x7
# Glifi disegnati da zero per il progetto (stessa tabelle della bozza gen_ui.py).
# Ogni riga esattamente 5 caratteri: '#' pieno, ' ' vuoto.
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
"a": ["     ", " ### ", "#   #", "#   #", "#####", "#   #", "#   #"],
"b": ["#    ", "#    ", "#    ", "#### ", "#   #", "#   #", "#### "],
"c": ["     ", " ### ", "#    ", "#    ", "#    ", "#    ", " ### "],
"d": ["    #", "    #", "    #", "#### ", "#   #", "#   #", "#### "],
"e": ["     ", " ### ", "#    ", " ####", "#   #", "#   #", " ### "],
"f": ["     ", "  #  ", "  #  ", "#####", "#   #", "#   #", "#   #"],
"g": ["     ", " ### ", "#   #", "#   #", "#####", "    #", " ### "],
"h": ["#    ", "#    ", "#    ", "#### ", "#   #", "#   #", "#### "],
"i": ["     ", "  #  ", "     ", "  #  ", "  #  ", "  #  ", "  #  "],
"j": ["     ", "   # ", "     ", "   # ", "   # ", "#   #", " ### "],
"k": ["#    ", "#   #", "#  # ", "# #  ", "#  # ", "#   #", "#   #"],
"l": ["#    ", "#    ", "#    ", "#    ", "#    ", "#    ", "#    "],
"m": ["     ", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
"n": ["     ", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
"o": ["     ", " ### ", "#   #", "#   #", "#   #", "#   #", " ### "],
"p": ["     ", "#### ", "#   #", "#   #", "#### ", "#    ", "#    "],
"q": ["     ", "#### ", "#   #", "#   #", "#### ", "    #", "    #"],
"r": ["     ", "#    ", "#    ", "##   ", "# #  ", "#   #", "#   #"],
"s": ["     ", " ### ", "#    ", " ### ", "    #", "#   #", " ### "],
"t": ["     ", "#   #", "#   #", "#####", "#   #", "#   #", "#   #"],
"u": ["     ", "#   #", "#   #", "#   #", "#   #", "#   #", " ### "],
"v": ["     ", "#   #", "#   #", "#   #", "#   #", " # # ", "  #  "],
"w": ["     ", "#   #", "#   #", "#   #", "# # #", "# # #", " # # "],
"x": ["     ", "#   #", " # # ", "  #  ", " # # ", "#   #", "#   #"],
"y": ["     ", "#   #", "#   #", "#   #", "#####", "    #", " ### "],
"z": ["     ", "#####", "   # ", "  #  ", " #   ", "#    ", "#####"],
"[": [" ####", "#    ", "#    ", "#    ", "#    ", "#    ", "#### "],
"\\": ["#    ", "#    ", " #   ", "  #  ", "   # ", "    #", "     "],
"]": [" ####", "    #", "    #", "    #", "    #", "    #", "#### "],
"^": ["     ", "  #  ", " # # ", "#   #", "     ", "     ", "     "],
"_": ["     ", "     ", "     ", "     ", "     ", "     ", "#####"],
"`": [" #   ", "     ", "     ", "     ", "     ", "     ", "     "],
"{": ["     ", "#    ", "# #  ", " #   ", "# #  ", "#    ", "     "],
"|": ["     ", "  #  ", "  #  ", "  #  ", "  #  ", "  #  ", "  #  "],
"}": ["     ", "    #", " # # ", " #   ", " # # ", "    #", "     "],
"~": ["     ", "     ", "# # #", "     ", "     ", "     ", "     "],
}

ORDER = list(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`{|}~")


def _normalizza():
    """Rende ogni glifo 5x7 esattamente (righe da 5, 7 righe): nessun pixel fuori cella."""
    for ch, rows in F5x7.items():
        out = []
        for r in rows:
            r = "".join(c for c in r if c in "# ")
            out.append((r + " " * GLYPH_W)[:GLYPH_W])
        assert len(out) == 7, f"glifo {ch!r}: {len(out)} righe"
        F5x7[ch] = out


# ------------------------------------------------------------------ disegno
def mask(rows):
    return [[c == "#" for c in row] for row in rows]


def draw_glyph(img, x, y, mk, colore, scale=1):
    w, h = img.size
    for ry, row in enumerate(mk):
        for rx, v in enumerate(row):
            if not v:
                continue
            for dy in range(scale):
                for dx in range(scale):
                    px, py = x + rx * scale + dx, y + ry * scale + dy
                    if 0 <= px < w and 0 <= py < h:
                        img.putpixel((px, py), colore + (255,))


def draw_text(img, x, y, testo, colore, scale=1):
    cx = x
    for ch in testo:
        if ch == " ":
            cx += CELL_W * scale
            continue
        mk = mask(F5x7.get(ch, F5x7[" "]))
        draw_glyph(img, cx, y, mk, colore, scale)
        cx += CELL_W * scale
    return cx


def rect(img, x, y, w, h, colore):
    x0, y0 = max(0, x), max(0, y)
    x1, y1 = min(img.size[0], x + w), min(img.size[1], y + h)
    for py in range(y0, y1):
        for px in range(x0, x1):
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


def carica_json(path):
    return json.loads(path.read_text(encoding="utf-8"))


def salva_json(path, data):
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


# ------------------------------------------------------------------ font
def build_font():
    """font.png: 2 colori, glifi bianchi su nero, stessa geometria di font.json."""
    meta = carica_json(UI / "font" / "varianteB" / "font.json")
    sheet_w = FONT_COLS * CELL_W
    rows_n = (len(ORDER) + FONT_COLS - 1) // FONT_COLS
    sheet_h = rows_n * CELL_H
    assert (meta["sheet_w"], meta["sheet_h"]) == (sheet_w, sheet_h), \
        f"font.json dichiara {meta['sheet_w']}x{meta['sheet_h']}, tabella {sheet_w}x{sheet_h}"
    assert meta["cell_w"] == CELL_W and meta["cell_h"] == CELL_H
    assert meta["glyph_w"] == GLYPH_W and meta["glyph_h"] == GLYPH_H
    assert len(meta["chars"]) == len(ORDER)
    img = canvas(sheet_w, sheet_h)
    for i, ch in enumerate(ORDER):
        cx = (i % FONT_COLS) * CELL_W
        cy = (i // FONT_COLS) * CELL_H
        assert meta["chars"][ch]["x"] == cx and meta["chars"][ch]["y"] == cy, \
            f"font.json: cella di {ch!r} spostata"
        draw_glyph(img, cx + 1, cy + 1, mask(F5x7[ch]), BIANCO)
    return img


# ------------------------------------------------------------------ titolo
def _cerca_size(testo):
    for s in range(56, 7, -1):
        f = ImageFont.truetype(str(ANTONIO), s)
        b = f.getbbox(testo)
        if b[2] - b[0] <= TITLE_MAX_W:
            return s
    raise RuntimeError("Antonio Bold non utilizzabile")


def build_title():
    """title.png: lettering 'Sglavinated Edition' con Antonio Bold, 2 colori."""
    if not ANTONIO.exists():
        raise RuntimeError(f"font mancante: {ANTONIO}")
    tj = carica_json(UI / "titolo" / "varianteB" / "title.json")
    testo = tj["testo"]
    size = _cerca_size(testo)

    # bbox a risoluzione target
    b1 = ImageFont.truetype(str(ANTONIO), size).getbbox(testo)
    w, h = b1[2] - b1[0], b1[3] - b1[1]
    # disegno a UPS e riduzione: curve pulite, poi soglia a 128 -> solo nero/bianco
    f4 = ImageFont.truetype(str(ANTONIO), size * UPS)
    b4 = f4.getbbox(testo)
    w4, h4 = max(1, b4[2] - b4[0]), max(1, b4[3] - b4[1])
    big = Image.new("L", (w4, h4), 0)
    ImageDraw.Draw(big).text((-b4[0], -b4[1]), testo, font=f4, fill=255)
    soglia = big.resize((w, h), LANCZOS).point(lambda v: 255 if v >= 128 else 0)

    img = canvas(w, h)
    img.paste(Image.new("RGBA", (w, h), BIANCO + (255,)), (0, 0), soglia)

    tj["w"], tj["h"] = w, h
    tj["scala"] = 1
    tj["posizione_suggerita"] = {"x": (TARGET_W - w) // 2, "y": 24}
    tj["font_usato"] = "assets_originali/font/antonio/Antonio-Bold.ttf (Antonio Bold, OFL-1.1, Impallari Type)"
    salva_json(UI / "titolo" / "varianteB" / "title.json", tj)
    return img


# ------------------------------------------------------------------ menu
def build_menu():
    """menu_preview.png 320x224: disegnato da motore_nuovo/dati/ui/menu.json."""
    mj = carica_json(DATI_UI / "menu.json")
    col = mj["colori"]
    img = canvas(TARGET_W, TARGET_H, tuple(col.get("sfondo", NERO)))

    c = mj.get("cornice")
    if c:
        cornice(img, c["x"], c["y"], c["w"], c["h"], c.get("spessore", 2), tuple(col["cornice"]))

    sel_idx = mj.get("voce_iniziale", 0)
    for i, v in enumerate(mj["voci"]):
        x, y, w, h = v["x"], v["y"], v["w"], v["h"]
        sel = (i == sel_idx)
        if sel:
            rect(img, x, y, w, h, tuple(col["sel"]))
        colore_label = tuple(col["testo_sel"] if sel else col["testo"])
        colore_azione = tuple(col["azione_sel"] if sel else col["azione"])
        if not sel and v.get("disabilitato"):
            colore_label = tuple(col["disabilitato"])
        ty = y + (h - GLYPH_H) // 2
        draw_text(img, x + 6, ty, v["label"], colore_label)
        az = str(v["azione"]).upper()
        draw_text(img, x + w - 6 - len(az) * CELL_W, ty, az, colore_azione)

    hint = mj.get("hint")
    if hint:
        draw_text(img, hint["pos"][0], hint["pos"][1], hint["testo"], tuple(col["hint"]))
    return img


# ------------------------------------------------------------------ hud
def build_hud():
    """hud_preview.png 320x224: disegnato da motore_nuovo/dati/ui/hud.json."""
    hj = carica_json(DATI_UI / "hud.json")
    img = canvas(TARGET_W, TARGET_H)

    for el in hj["elementi"]:
        tipo = el.get("tipo")
        if tipo == "cornice":
            cornice(img, el["x"], el["y"], el["w"], el["h"],
                    el.get("spessore", 2), tuple(el["colore"]))
        elif tipo == "testo":
            if "label" in el and "label_pos" in el:
                draw_text(img, el["label_pos"][0], el["label_pos"][1], el["label"],
                          tuple(el.get("colore_label", BIANCO)))
            if "valore" in el and "valore_pos" in el:
                draw_text(img, el["valore_pos"][0], el["valore_pos"][1], str(el["valore"]),
                          tuple(el.get("colore", BIANCO)), el.get("scala_valore", 1))
            if "testo" in el and "pos" in el:
                draw_text(img, el["pos"][0], el["pos"][1], el["testo"],
                          tuple(el.get("colore", BIANCO)), el.get("scala", 1))
        elif tipo == "barra":
            if "label" in el and "label_pos" in el:
                draw_text(img, el["label_pos"][0], el["label_pos"][1], el["label"],
                          tuple(el.get("colore_label", BIANCO)))
            bx, by = el["barra_pos"]
            bw, bh = el["barra_w"], el["barra_h"]
            sp = int(el.get("spessore_bordo", 2))
            cornice(img, bx, by, bw, bh, sp, tuple(el["colore_bordo"]))
            riemp = int(el.get("riempimento_px", 0))
            if riemp > 0:
                rect(img, bx + sp, by + sp, min(riemp, bw - 2 * sp), bh - 2 * sp,
                     tuple(el["colore_riempimento"]))
        else:
            raise RuntimeError(f"elemento HUD sconosciuto: {tipo!r}")
    return img


# ------------------------------------------------------------------ controlli
def audit(path, img, preview=False):
    cols = img.convert("RGB").getcolors(1024)
    assert cols, "piu' di 1024 colori"
    nomi = sorted(c for _, c in cols)
    cromatici = [c for c in nomi if not (c[0] == c[1] == c[2])]
    assert not cromatici, f"{path.name}: colori cromatici {cromatici} (tema B/N violato)"
    assert len(nomi) <= MAX_COLORI, f"{path.name}: {len(nomi)} colori > {MAX_COLORI}"
    if preview:
        assert img.size == (TARGET_W, TARGET_H), f"{path.name}: {img.size}"
    print(f"  OK {path.relative_to(ROOT)}  {img.size[0]}x{img.size[1]}  "
          f"{len(nomi)} colori  {nomi}")


def main():
    _normalizza()
    print("genera_ui_b.py - variante B in bianco e nero (decisione D7)")

    lavori = [
        (UI / "font" / "varianteB" / "font.png", build_font, False),
        (UI / "titolo" / "varianteB" / "title.png", build_title, False),
        (UI / "menu" / "varianteB" / "menu_preview.png", build_menu, True),
        (UI / "hud" / "varianteB" / "hud_preview.png", build_hud, True),
    ]
    for path, costruttore, preview in lavori:
        img = costruttore()
        salva(img, path)
        ricaricato = Image.open(path)
        audit(path, ricaricato, preview)
    print("fatto: 4 PNG rigenerati in bianco e nero")


if __name__ == "__main__":
    sys.exit(main())
