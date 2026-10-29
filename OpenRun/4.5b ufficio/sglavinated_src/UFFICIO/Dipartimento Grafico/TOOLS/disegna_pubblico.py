#!/usr/bin/env python3
# Pubblico all'arrivo (Andrea 29/10: "gente che attende l'arrivo") — sprite originali.
# Disegnati da zero con Pillow: due file di persone dietro una transenna
# bianca e rossa, alcune con la bandiera a scacchi. Nessun asset Sega. Licenza CC0-1.0.
# Due gruppi (a, b) in due pose: braccia giu' e braccia su. Il motore alterna
# le pose per farli esultare e mette i gruppi ai lati della strada, prima e
# dopo il traguardo.
# Produce: Libreria/assets_originali/sprite/pubblico/pubblico_<gruppo>_<posa>.png (40x26)
# Poi: python3 motore_nuovo/strumenti_py/png_a_raw.py (scrive i .sglraw).
# Per ridisegnarli a mano: stesso nome e stessa misura, poi png_a_raw.py.
import os
import random
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "pubblico"))
os.makedirs(OUT, exist_ok=True)

W, H = 40, 26
PELLE = [(240, 200, 160), (200, 144, 96), (138, 90, 58)]
CAPELLI = [(32, 24, 16), (106, 64, 32), (224, 192, 96), (60, 60, 64)]
MAGLIE = [(224, 48, 48), (48, 96, 224), (240, 208, 48), (48, 176, 80),
          (240, 240, 240), (208, 64, 192), (64, 200, 224), (240, 128, 48)]
GAMBE = (40, 40, 56)
ROSSO, BIANCO, NERO = (208, 32, 32), (244, 244, 244), (20, 20, 20)


def scurisci(c, k=0.72):
    return tuple(int(v * k) for v in c)


def persona(d, x, piedi, p, su, scuro):
    """Persona larga 5 px: testa 3x3, maglia 5x7, gambe; braccia giu' o su."""
    pelle, capelli, maglia, bandiera = p
    if scuro:
        pelle, capelli, maglia = scurisci(pelle), scurisci(capelli), scurisci(maglia)
    testa = piedi - 14
    d.rectangle([x, piedi - 3, x + 1, piedi], fill=GAMBE)
    d.rectangle([x + 3, piedi - 3, x + 4, piedi], fill=GAMBE)
    d.rectangle([x, testa + 4, x + 4, piedi - 4], fill=maglia)
    d.rectangle([x + 1, testa + 1, x + 3, testa + 3], fill=pelle)
    d.line([x + 1, testa, x + 3, testa], fill=capelli)
    if su:  # braccia alzate, mani sopra la testa
        d.line([x - 1, testa - 2, x - 1, testa + 5], fill=maglia)
        d.line([x + 5, testa - 2, x + 5, testa + 5], fill=maglia)
        d.point([(x - 1, testa - 3), (x + 5, testa - 3)], fill=pelle)
        if bandiera:  # bandiera a scacchi sventolata in alto
            for i in range(4):
                for j in range(3):
                    d.point((x + 6 + i, testa - 6 + j), fill=BIANCO if (i + j) % 2 else NERO)
            d.line([x + 5, testa - 6, x + 5, testa - 3], fill=NERO)
    else:
        d.line([x - 1, testa + 5, x - 1, testa + 9], fill=maglia)
        d.line([x + 5, testa + 5, x + 5, testa + 9], fill=maglia)
        if bandiera:  # bandiera tenuta bassa
            for i in range(3):
                for j in range(3):
                    d.point((x + 6 + i, testa + 4 + j), fill=BIANCO if (i + j) % 2 else NERO)


def transenna(d):
    """Transenna in basso: corrimano bianco e pannelli rossi e bianchi."""
    d.line([0, H - 6, W - 1, H - 6], fill=BIANCO)
    for x in range(0, W, 4):
        d.rectangle([x, H - 5, x + 3, H - 2], fill=ROSSO if (x // 4) % 2 == 0 else BIANCO)
    d.line([0, H - 1, W - 1, H - 1], fill=scurisci(ROSSO, 0.5))


def gruppo(seme):
    rnd = random.Random(seme)
    dietro = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), False) for _ in range(5)]
    davanti = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), rnd.random() < 0.3) for _ in range(6)]
    pose = {}
    for posa, su in (("giu", False), ("su", True)):
        img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
        d = ImageDraw.Draw(img)
        # Fila dietro: piu' in alto e piu' scura; alza le braccia a scacchiera.
        for i, p in enumerate(dietro):
            persona(d, 5 + i * 6, H - 9, p, su and i % 2 == 0, True)
        for i, p in enumerate(davanti):
            persona(d, 2 + i * 6, H - 4, p, su and (i % 2 == 1 or p[3]), False)
        transenna(d)
        pose[posa] = img
    return pose


if __name__ == "__main__":
    for nome, seme in (("a", 2910), ("b", 1029)):
        for posa, img in gruppo(seme).items():
            p = os.path.join(OUT, f"pubblico_{nome}_{posa}.png")
            img.save(p)
            print(f"  [OK] {p} {img.size}")
