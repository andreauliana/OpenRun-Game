#!/usr/bin/env python3
# Blocco RIV — sprite auto rivali originali (OpenRun 4.6, @grafico).
# Disegnati da zero con Pillow. Nessun asset Sega. Licenza CC0-1.0.
# Produce: 4 rivali (vista dietro, modello/colore distinti),
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

PALETTES = {
    "rosso":   {"body": (200, 30, 30),   "dark": (120, 15, 15),  "glass": (25, 25, 40), "rim": (180, 180, 170), "light": (255, 60, 20)},
    "azzurro": {"body": (30, 90, 210),   "dark": (15, 45, 130),  "glass": (25, 25, 40), "rim": (200, 200, 210), "light": (0, 180, 255)},
    "giallo":  {"body": (240, 200, 0),   "dark": (140, 120, 0),  "glass": (25, 25, 40), "rim": (180, 180, 170), "light": (255, 40, 40)},
    "verde":   {"body": (40, 190, 70),   "dark": (20, 110, 40),  "glass": (25, 25, 40), "rim": (200, 200, 210), "light": (255, 200, 0)},
}

def retro(pal, w=40, h=20):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rectangle([4, 6, w - 4, h - 4], fill=pal["body"], outline=pal["dark"])
    d.polygon([(10, 6), (w - 10, 6), (w - 8, 10), (8, 10)], fill=pal["glass"], outline=pal["dark"])
    d.rectangle([6, 10, 9, 14], fill=pal["light"], outline=pal["dark"])
    d.rectangle([w - 9, 10, w - 6, 14], fill=pal["light"], outline=pal["dark"])
    d.line([4, h - 4, w - 4, h - 4], fill=pal["dark"], width=2)
    d.ellipse([6, h - 7, 11, h - 3], fill=pal["dark"])
    d.ellipse([w - 11, h - 7, w - 6, h - 3], fill=pal["dark"])
    d.ellipse([7, h - 6, 10, h - 3], fill=pal["rim"])
    d.ellipse([w - 10, h - 6, w - 7, h - 3], fill=pal["rim"])
    return img

def lato(pal, w=44, h=16):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.polygon([(4, h - 4), (6, 8), (12, 6), (w - 10, 6), (w - 6, 8), (w - 4, h - 4)], fill=pal["body"], outline=pal["dark"])
    d.polygon([(10, 8), (w - 12, 8), (w - 10, 10), (12, 10)], fill=pal["glass"], outline=pal["dark"])
    d.ellipse([8, h - 6, 14, h - 2], fill=pal["dark"])
    d.ellipse([10, h - 4, 12, h - 2], fill=pal["rim"])
    d.ellipse([w - 14, h - 6, w - 8, h - 2], fill=pal["dark"])
    d.ellipse([w - 12, h - 4, w - 10, h - 2], fill=pal["rim"])
    d.rectangle([6, 10, 9, 12], fill=pal["light"], outline=pal["dark"])
    return img

files = []
for name, pal in PALETTES.items():
    files.append(salvo(f"auto_rivale_{name}_retro.png", retro(pal)))
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
