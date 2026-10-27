#!/usr/bin/env python3
# Traffico comune (Andrea 27/10: "un poco di traffico comune") — sprite originali.
# Disegnati da zero con Pillow, vista da dietro. Nessun asset Sega. Licenza CC0-1.0.
# Forme diverse dalle auto da corsa dei rivali (utilitaria alta, furgone,
# berlina, camioncino) e colori smorzati, cosi' si distinguono a colpo d'occhio.
# Produce: Libreria/assets_originali/sprite/traffico/traffico_<nome>_retro.png
# Poi: python3 motore_nuovo/strumenti_py/png_a_raw.py (scrive i .sglraw).
import os
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "traffico"))
os.makedirs(OUT, exist_ok=True)

NERO = (20, 20, 26)
GOMMA = (34, 34, 38)
VETRO = (60, 80, 100)
VETRO_LUCE = (120, 150, 170)
FANALE = (220, 30, 30)
FANALE_LUCE = (255, 120, 90)
TARGA = (235, 235, 225)


def ruote(d, w, h, inset, rw=6, rh=4):
    d.rectangle([inset, h - rh, inset + rw, h - 1], fill=GOMMA)
    d.rectangle([w - 1 - inset - rw, h - rh, w - 1 - inset, h - 1], fill=GOMMA)


def fanali(d, x0, x1, y0, y1, w):
    d.rectangle([x0, y0, x1, y1], fill=FANALE, outline=NERO)
    d.rectangle([w - 1 - x1, y0, w - 1 - x0, y1], fill=FANALE, outline=NERO)
    d.point([(x0 + 1, y0 + 1), (w - 1 - x1 + 1, y0 + 1)], fill=FANALE_LUCE)


def targa(d, w, y):
    d.rectangle([w // 2 - 4, y, w // 2 + 3, y + 2], fill=TARGA, outline=NERO)


def utilitaria(corpo, scuro, w=34, h=26):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rounded_rectangle([3, 3, w - 4, h - 4], radius=4, fill=corpo, outline=scuro)
    d.rectangle([7, 5, w - 8, 11], fill=VETRO, outline=scuro)
    d.line([9, 6, 13, 6], fill=VETRO_LUCE)
    fanali(d, 4, 7, 13, 17, w)
    targa(d, w, 16)
    d.rectangle([3, h - 6, w - 4, h - 4], fill=scuro)  # paraurti
    ruote(d, w, h, 4)
    return img


def furgone(corpo, scuro, w=40, h=32):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rectangle([2, 1, w - 3, h - 4], fill=corpo, outline=scuro)
    d.line([w // 2, 2, w // 2, h - 6], fill=scuro)  # porte posteriori
    d.rectangle([5, 4, w // 2 - 3, 10], fill=VETRO, outline=scuro)
    d.rectangle([w // 2 + 2, 4, w - 6, 10], fill=VETRO, outline=scuro)
    fanali(d, 3, 6, 16, 23, w)
    targa(d, w, h - 9)
    d.rectangle([2, h - 6, w - 3, h - 4], fill=NERO)
    ruote(d, w, h, 4, rw=7)
    return img


def berlina(corpo, scuro, w=40, h=22):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.polygon([(10, 2), (w - 11, 2), (w - 7, 8), (6, 8)], fill=corpo, outline=scuro)
    d.polygon([(12, 3), (w - 13, 3), (w - 10, 7), (9, 7)], fill=VETRO)
    d.rectangle([3, 8, w - 4, h - 4], fill=corpo, outline=scuro)
    fanali(d, 4, 9, 10, 13, w)
    targa(d, w, 11)
    d.line([4, 15, w - 5, 15], fill=scuro)
    d.rectangle([3, h - 6, w - 4, h - 4], fill=scuro)
    ruote(d, w, h, 5)
    return img


def camioncino(corpo, scuro, cassone, w=42, h=28):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rectangle([9, 1, w - 10, 9], fill=corpo, outline=scuro)          # cabina
    d.rectangle([12, 3, w - 13, 8], fill=VETRO, outline=scuro)
    d.rectangle([2, 9, w - 3, h - 5], fill=cassone, outline=scuro)       # sponda
    d.line([3, 13, w - 4, 13], fill=scuro)
    fanali(d, 3, 6, 15, 20, w)
    targa(d, w, h - 10)
    d.rectangle([2, h - 6, w - 3, h - 4], fill=NERO)
    ruote(d, w, h, 3, rw=8)
    return img


VEICOLI = {
    "utilitaria_bianca": utilitaria((226, 226, 218), (120, 120, 116)),
    "furgone_grigio": furgone((150, 156, 162), (80, 84, 90)),
    "berlina_blu": berlina((40, 60, 120), (20, 30, 64)),
    "camioncino_beige": camioncino((200, 180, 140), (110, 96, 70), (170, 150, 112)),
}

if __name__ == "__main__":
    for nome, img in VEICOLI.items():
        p = os.path.join(OUT, f"traffico_{nome}_retro.png")
        img.save(p)
        print(f"  [OK] {p} {img.size}")
