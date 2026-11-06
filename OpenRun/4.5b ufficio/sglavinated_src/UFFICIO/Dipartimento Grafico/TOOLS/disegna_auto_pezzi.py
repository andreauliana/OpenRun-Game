#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Auto del giocatore A PEZZI (Andrea 06/11): la struttura e' quella del vecchio
# modello a rettangoli (carrozzeria, abitacolo, ruote, alettone, fanali), ma ogni
# rettangolo diventa un piccolo pezzo in pixel art. Il motore li posiziona,
# inclina, solleva e ruota (parallasse = finto 3D), come faceva con i rettangoli.
# Palette piccola + dithering Bayer solo sulle aree grandi (carrozzeria).
# Disegnati da zero, nessun materiale Sega, nessun marchio reale.
#
# Produce in assets_originali/sprite/giocatore/ i PNG (Andrea puo' ridisegnarli,
# stesso nome e stessa misura) e i raw in motore_nuovo/dati/sprite_raw/giocatore/pezzi/.
# Un PNG gia' presente NON viene sovrascritto (salvo --forza): vince il disegno di Andrea.
#
# Lanciare da qualunque cartella:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_auto_pezzi.py" [--forza]
import os
import struct
import sys

from PIL import Image

BASE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(BASE, "..", "..", ".."))
OUT_PNG = os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "giocatore")
OUT_RAW = os.path.join(ROOT, "motore_nuovo", "dati", "sprite_raw", "giocatore", "pezzi")
os.makedirs(OUT_PNG, exist_ok=True)
os.makedirs(OUT_RAW, exist_ok=True)
FORZA = "--forza" in sys.argv

T = (0, 0, 0, 0)
# Stessi colori dell'auto disegnata dal @grafico il 06/11 (auto_gt/auto_giocatore_dritta.png),
# che fa da riferimento: rosso corsa, strisce gialle, fanali scuri, paraurti cromato.
NERO = (14, 14, 18, 255)
ROSSO = (196, 42, 38, 255)
ROSSO_SC = (108, 20, 22, 255)
ROSSO_CH = (238, 120, 96, 255)
GIALLO = (238, 190, 44, 255)
VETRO = (26, 32, 54, 255)
VETRO_CH = (96, 130, 180, 255)
GOMMA = (28, 28, 32, 255)
GOMMA_CH = (56, 56, 62, 255)
CROMO = (206, 202, 188, 255)
TARGA = (232, 230, 214, 255)
STOP = (255, 48, 40, 255)
STOP_CH = (252, 150, 110, 255)
BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]


def nuova(w, h):
    return Image.new("RGBA", (w, h), T)


def rett(img, x0, y0, x1, y1, col):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            img.putpixel((x, y), col)


def sfuma(img, x0, y0, x1, y1, alto, basso, da=0.55):
    # Da `alto` a `basso` con dithering Bayer 4x4 solo nella parte bassa (niente colori nuovi).
    for y in range(y0, y1 + 1):
        t = (y - y0) / max(1, y1 - y0)
        k = max(0.0, (t - da) / (1 - da)) * 16
        for x in range(x0, x1 + 1):
            img.putpixel((x, y), basso if BAYER[y % 4][x % 4] < k else alto)


def carrozzeria():
    # Retro dell'auto SENZA abitacolo e SENZA ruote: 52x15.
    w, h = 52, 15
    im = nuova(w, h)
    sfuma(im, 1, 1, w - 2, h - 5, ROSSO, ROSSO_SC)
    rett(im, 3, 0, w - 4, 0, ROSSO_SC)
    rett(im, 2, 1, w - 3, 1, ROSSO_CH)                # filo di luce sul bordo del baule
    rett(im, 0, 3, 0, h - 5, ROSSO_SC)
    rett(im, w - 1, 3, w - 1, h - 5, ROSSO_SC)
    rett(im, w // 2 - 4, 0, w // 2 - 3, 3, GIALLO)    # strisce
    rett(im, w // 2 + 2, 0, w // 2 + 3, 3, GIALLO)
    for x0 in (3, w - 14):                            # fanali spenti, bordo scuro
        rett(im, x0, 4, x0 + 10, 7, NERO)
        rett(im, x0 + 1, 5, x0 + 9, 6, ROSSO_SC)
        rett(im, x0 + 2, 5, x0 + 4, 5, ROSSO_CH)
    rett(im, w // 2 - 6, 7, w // 2 + 5, 10, NERO)     # targa
    rett(im, w // 2 - 5, 8, w // 2 + 4, 9, TARGA)
    rett(im, 1, h - 4, w - 2, h - 3, CROMO)           # paraurti
    rett(im, 4, h - 2, w - 5, h - 1, NERO)            # sottoscocca
    rett(im, 12, h - 2, 14, h - 1, GOMMA_CH)          # scarichi
    rett(im, w - 15, h - 2, w - 13, h - 1, GOMMA_CH)
    return im


def abitacolo():
    # Tetto e lunotto, 30x7: trapezio con le strisce che continuano.
    w, h = 30, 7
    im = nuova(w, h)
    for y in range(h):
        m = max(0, 3 - y)
        rett(im, m, y, w - 1 - m, y, ROSSO_SC if y == 0 else ROSSO)
    rett(im, 5, 2, w - 6, h - 2, VETRO)
    rett(im, 6, 2, 12, 2, VETRO_CH)                   # riflesso
    rett(im, w // 2 - 3, 0, w // 2 - 2, h - 1, GIALLO)
    rett(im, w // 2 + 1, 0, w // 2 + 2, h - 1, GIALLO)
    return im


def alettone():
    w, h = 46, 3
    im = nuova(w, h)
    rett(im, 0, 0, w - 1, 1, NERO)
    rett(im, 2, 0, w - 3, 0, ROSSO_SC)
    rett(im, 5, 2, 6, 2, NERO)                        # supporti
    rett(im, w - 7, 2, w - 6, 2, NERO)
    return im


def ruota(passo):
    # Ruota posteriore vista da dietro, 7x12; il battistrada scorre (3 passi).
    w, h = 7, 12
    im = nuova(w, h)
    rett(im, 0, 1, w - 1, h - 2, GOMMA)
    rett(im, 1, 0, w - 2, 0, GOMMA)
    rett(im, 1, h - 1, w - 2, h - 1, GOMMA)
    for y in range(1, h - 1):
        if (y + passo) % 3 == 0:
            rett(im, 1, y, w - 2, y, GOMMA_CH)
    return im


def fianco():
    # Fianco destro dell'auto visto di sbieco (si vede in curva), 20x16: dal
    # retro (colonna 0, alto come la carrozzeria) verso il muso (a destra), che
    # si rimpicciolisce in prospettiva. Ruota anteriore in fondo, appoggiata a
    # terra. Il motore lo specchia per il fianco sinistro e ne mostra solo una
    # parte quando lo sterzo e' leggero.
    w, h = 20, 16
    im = nuova(w, h)
    for x in range(w):
        top = 1 + x // 4                               # il tetto/cofano scende verso il muso
        bot = h - 2 - x // 7                           # la linea di terra sale in lontananza
        rett(im, x, top, x, bot, ROSSO)
        rett(im, x, bot - 1, x, bot, ROSSO_SC)         # ombra della fiancata
        rett(im, x, top, x, top, ROSSO_CH if x % 5 else ROSSO_SC)
        if 2 <= x < w - 7:
            rett(im, x, top + 1, x, top + 2, VETRO)    # finestrino laterale
        rett(im, x, bot + 1, x, bot + 1, NERO)         # sottoporta
    rett(im, 0, 1, 0, h - 2, ROSSO_SC)                 # spigolo col retro
    for x in range(w - 8, w - 2):                      # ruota anteriore, a terra
        bot = h - 2 - x // 7
        rett(im, x, bot - 3, x, bot + 1, GOMMA)
    return im


def fanale_acceso():
    w, h = 9, 2
    im = nuova(w, h)
    rett(im, 0, 0, w - 1, h - 1, STOP)
    rett(im, 1, 0, 3, 0, STOP_CH)
    return im


def salva(nome, im):
    png = os.path.join(OUT_PNG, nome + ".png")
    if os.path.exists(png) and not FORZA:
        im = Image.open(png).convert("RGBA")          # il PNG di Andrea vince
        print(f"tengo {png} (gia' presente)")
    else:
        im.save(png)
        print(f"scritto {png} ({im.width}x{im.height})")
    raw = os.path.join(OUT_RAW, nome + ".sglraw")
    with open(raw, "wb") as f:
        f.write(b"SGLR" + struct.pack("<II", im.width, im.height) + im.tobytes())


salva("carrozzeria", carrozzeria())
salva("abitacolo", abitacolo())
salva("alettone", alettone())
for p in range(3):
    salva(f"ruota_{p + 1}", ruota(p))
salva("fanale_acceso", fanale_acceso())
salva("fianco", fianco())
