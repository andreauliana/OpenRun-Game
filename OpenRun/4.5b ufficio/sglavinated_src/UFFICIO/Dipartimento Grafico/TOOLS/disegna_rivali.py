#!/usr/bin/env python3
# Blocco RIV — sprite auto rivali originali (OpenRun 4.6, @grafico).
# Disegnati da zero con Pillow. Nessun asset Sega. Licenza CC0-1.0.
# Produce: 7 rivali (vista dietro, 48x24, colore e dettagli distinti; ridisegnati 29/10),
# 2 viste di lato (curve) e 1 fotogramma di sbandata auto giocatore.
import os, json
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
LIB = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali"))
OUT = os.path.join(LIB, "sprite", "rivali")
os.makedirs(OUT, exist_ok=True)

def salvo(nome, img):
    p = os.path.join(OUT, nome)
    img.save(p)
    print(f"  [OK] {p}")
    return p

# Colori delle 7 auto rivali (Andrea 29/10: "leggermente migliori nel
# design", una per rivale). Per cambiare un'auto basta ritoccare il PNG in
# Libreria (stesso nome e stessa misura) e rilanciare png_a_raw.py.
# body = carrozzeria, luce = riflesso sul cofano, dark = ombre e bordi,
# striscia = fascia da corsa (None = senza), alettone = 0 basso, 1 alto.
PALETTES = {
    "rosso":   {"body": (210, 32, 32),   "luce": (255, 110, 90),  "dark": (110, 12, 16), "striscia": (250, 250, 250), "alettone": 1},
    "azzurro": {"body": (40, 120, 235),  "luce": (130, 200, 255), "dark": (14, 46, 120), "striscia": None,            "alettone": 0},
    "giallo":  {"body": (245, 205, 10),  "luce": (255, 245, 150), "dark": (130, 96, 0),  "striscia": (30, 30, 36),    "alettone": 1},
    "verde":   {"body": (40, 190, 80),   "luce": (150, 250, 160), "dark": (14, 96, 40),  "striscia": None,            "alettone": 1},
    "bianco":  {"body": (232, 232, 240), "luce": (255, 255, 255), "dark": (120, 120, 140), "striscia": (220, 30, 40), "alettone": 0},
    "arancio": {"body": (250, 130, 20),  "luce": (255, 200, 120), "dark": (140, 60, 0),  "striscia": (30, 30, 36),    "alettone": 0},
    "viola":   {"body": (150, 60, 210),  "luce": (215, 150, 255), "dark": (70, 20, 110), "striscia": None,            "alettone": 1},
}
# Ordine dei rivali in gara (runner/main.cpp, prova_gate.cpp, png_a_raw.py):
ORDINE = ["rosso", "azzurro", "giallo", "verde", "bianco", "arancio", "viola"]

NERO = (16, 16, 22)
GOMMA = (30, 30, 34)
GOMMA_LUCE = (70, 70, 78)
VETRO = (30, 40, 70)
VETRO_LUCE = (110, 150, 200)
FANALE = (230, 20, 30)
FANALE_LUCE = (255, 150, 120)
TARGA = (236, 236, 226)
SCARICO = (150, 150, 160)


def retro(pal, w=48, h=24):
    """Coupe' da corsa vista da dietro, bassa e larga, pixel pieni (niente
    sfumature): alettone, lunotto, fanali a fascia, targa, scarichi, gomme."""
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    c = w // 2
    # Gomme: sporgono sotto e ai lati della carrozzeria.
    for x0 in (2, w - 10):
        d.rectangle([x0, 14, x0 + 7, h - 1], fill=GOMMA)
        d.line([x0 + 1, 15, x0 + 1, h - 2], fill=GOMMA_LUCE)
    # Abitacolo e lunotto (dietro la coda).
    d.polygon([(12, 9), (15, 1), (w - 16, 1), (w - 13, 9)], fill=pal["body"], outline=pal["dark"])
    d.polygon([(14, 8), (16, 2), (w - 17, 2), (w - 15, 8)], fill=VETRO)
    d.line([17, 2, 21, 2], fill=VETRO_LUCE)
    d.line([15, 6, 16, 4], fill=VETRO_LUCE)
    # Carrozzeria: fianchi larghi in basso, spalle che rientrano verso il tetto.
    d.polygon([(3, 12), (7, 9), (w - 8, 9), (w - 4, 12), (w - 4, 19), (3, 19)], fill=pal["body"], outline=pal["dark"])
    d.line([8, 10, w - 9, 10], fill=pal["luce"])           # riflesso sulla coda
    # Fascia da corsa: sul tetto e sulla coda (il lunotto resta libero).
    if pal["striscia"]:
        for x in (c - 3, c + 1):
            d.rectangle([x, 1, x + 1, 1], fill=pal["striscia"])
            d.rectangle([x, 9, x + 1, 18], fill=pal["striscia"])
    # Alettone sopra la coda, con due supporti.
    if pal["alettone"]:
        d.rectangle([4, 5, w - 5, 6], fill=pal["dark"])
        d.line([5, 5, w - 6, 5], fill=pal["body"])
        d.rectangle([10, 7, 11, 8], fill=pal["dark"])
        d.rectangle([w - 12, 7, w - 11, 8], fill=pal["dark"])
    else:
        d.line([8, 9, w - 9, 9], fill=pal["dark"])           # labbro sulla coda
    # Fanali a fascia con una linea luminosa che li unisce.
    d.rectangle([4, 11, 13, 13], fill=FANALE, outline=NERO)
    d.rectangle([w - 14, 11, w - 5, 13], fill=FANALE, outline=NERO)
    d.line([5, 12, 8, 12], fill=FANALE_LUCE)
    d.line([w - 13, 12, w - 10, 12], fill=FANALE_LUCE)
    d.line([14, 12, w - 15, 12], fill=(160, 20, 30))
    # Targa al centro.
    d.rectangle([c - 5, 14, c + 4, 16], fill=TARGA, outline=NERO)
    # Diffusore scuro e due scarichi.
    d.rectangle([4, 17, w - 5, 19], fill=pal["dark"])
    for x in (c - 8, c + 6):
        d.rectangle([x, 17, x + 2, 19], fill=NERO)
        d.point([(x + 1, 18)], fill=SCARICO)
    # Ombra a terra tra le gomme.
    d.line([10, h - 2, w - 11, h - 2], fill=NERO)
    return img

def lato(pal, w=44, h=16):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.polygon([(4, h - 4), (6, 8), (12, 6), (w - 10, 6), (w - 6, 8), (w - 4, h - 4)], fill=pal["body"], outline=pal["dark"])
    d.polygon([(10, 8), (w - 12, 8), (w - 10, 10), (12, 10)], fill=VETRO, outline=pal["dark"])
    d.ellipse([8, h - 6, 14, h - 2], fill=pal["dark"])
    d.ellipse([10, h - 4, 12, h - 2], fill=SCARICO)
    d.ellipse([w - 14, h - 6, w - 8, h - 2], fill=pal["dark"])
    d.ellipse([w - 12, h - 4, w - 10, h - 2], fill=SCARICO)
    d.rectangle([6, 10, 9, 12], fill=FANALE, outline=pal["dark"])
    return img

files = []
for name in ORDINE:
    files.append(salvo(f"auto_rivale_{name}_retro.png", retro(PALETTES[name])))
files.append(salvo("auto_rivale_rosso_lato.png", lato(PALETTES["rosso"])))
files.append(salvo("auto_rivale_azzurro_lato.png", lato(PALETTES["azzurro"])))

# Sbandata auto giocatore: auto GT inclissa + segni di sbandata (fumo/strisce)
img = Image.new("RGBA", (64, 32), (0, 0, 0, 0))
d = ImageDraw.Draw(img)
d.arc([8, 12, 56, 28], 20, 160, fill=(100, 100, 100, 170), width=3)
d.arc([10, 14, 54, 26], 30, 150, fill=(160, 160, 160, 150), width=2)
base = Image.new("RGBA", (48, 20), (0, 0, 0, 0))
bd = ImageDraw.Draw(base)
bd.polygon([(4, 16), (6, 8), (14, 6), (38, 6), (40, 8), (44, 16)], fill=(200, 30, 30), outline=(120, 15, 15))
bd.polygon([(10, 8), (36, 8), (36, 10), (12, 10)], fill=(25, 25, 40), outline=(120, 15, 15))
bd.rectangle([6, 10, 9, 12], fill=(255, 60, 20), outline=(120, 15, 15))
bd.rectangle([37, 10, 40, 12], fill=(255, 60, 20), outline=(120, 15, 15))
base = base.rotate(22, expand=True, resample=Image.NEAREST)
img.paste(base, (8, 6), base)
files.append(salvo("auto_giocatore_sbandata.png", img))

print(f"Sprite rivali disegnati: {len(files)}")
