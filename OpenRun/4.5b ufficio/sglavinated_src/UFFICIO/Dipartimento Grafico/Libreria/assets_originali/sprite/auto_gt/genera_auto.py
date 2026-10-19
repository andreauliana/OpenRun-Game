#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Genera sagoma auto GT europea anni '70-'90 per Sglavinated Edition v3.
# Ispirata a Citroën CX/DS, Ford Sierra/Cosworth/Capri/Escort RS.
# Disegnata da zero con Pillow, nessun asset Sega, nessun marchio reale.
# Sprite 64x32, max 16 colori, 3 varianti di colore.
import os, json
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))

# 3 varianti colore (stessa silhouette GT europea)
VARIANTS = {
    'neon_cyan': {
        'body': (0, 200, 220),      # carrozzeria ciano neon
        'dark': (0, 100, 120),       # ombre
        'window': (20, 30, 60),      # vetri scuri
        'chrome': (200, 220, 255),   # cromature
        'light_f': (255, 255, 180),  # fari anteriori
        'light_r': (255, 40, 40),    # fari posteriori
        'tire': (30, 30, 35),        # gomme
        'rim': (160, 170, 180),      # cerchi
        'stripe': (255, 0, 200),     # striscia laterale
        'under': (0, 255, 180),      # neon sotto
    },
    'rosso_corsa': {
        'body': (200, 30, 30),
        'dark': (120, 15, 15),
        'window': (25, 25, 40),
        'chrome': (220, 210, 190),
        'light_f': (255, 255, 200),
        'light_r': (255, 60, 20),
        'tire': (25, 25, 30),
        'rim': (180, 180, 170),
        'stripe': (255, 200, 0),
        'under': (255, 120, 0),
    },
    'argento_lunare': {
        'body': (170, 175, 185),
        'dark': (100, 105, 115),
        'window': (30, 35, 50),
        'chrome': (240, 240, 250),
        'light_f': (255, 255, 220),
        'light_r': (255, 30, 30),
        'tire': (20, 20, 25),
        'rim': (200, 200, 210),
        'stripe': (0, 180, 255),
        'under': (100, 140, 255),
    },
}

def draw_gt_car(pal):
    """Disegna la sagoma GT vista dall'alto/3quarti (stile sprite billboard racing)."""
    img = Image.new('RGBA', (64, 32), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)

    # Ombra sotto (neon underglow)
    d.ellipse([8, 26, 56, 31], fill=(*pal['under'], 80))

    # Carrozzeria principale — profilo basso e lungo (CX/Sierra)
    # Corpo: trapezio allungato
    body_pts = [(10, 24), (8, 18), (12, 10), (20, 7), (44, 7), (52, 10), (56, 18), (54, 24)]
    d.polygon(body_pts, fill=pal['body'], outline=pal['dark'])

    # Cofano lungo (CX style) — zona anteriore piu bassa
    d.polygon([(44, 9), (52, 12), (55, 18), (54, 20), (44, 18)], fill=pal['dark'])

    # Parabrezza e lunotto (vetri scuri, angolati come Sierra XR4i)
    d.polygon([(22, 9), (28, 8), (36, 8), (42, 9), (40, 14), (24, 14)], fill=pal['window'])

    # Spoiler posteriore discreto (Cosworth)
    d.rectangle([8, 8, 14, 10], fill=pal['chrome'])

    # Fari anteriori (rettangolari, Escort RS style)
    d.rectangle([52, 14, 56, 17], fill=pal['light_f'])
    d.rectangle([52, 20, 56, 23], fill=pal['light_f'])

    # Fari posteriori
    d.rectangle([8, 14, 11, 17], fill=pal['light_r'])
    d.rectangle([8, 20, 11, 23], fill=pal['light_r'])

    # Striscia laterale (racing stripe)
    d.line([(12, 18), (52, 18)], fill=pal['stripe'], width=1)

    # Ruote (larghe, seduta bassa)
    for wy in [11, 22]:
        # Ruota posteriore
        d.ellipse([10, wy, 18, wy+5], fill=pal['tire'])
        d.ellipse([12, wy+1, 16, wy+4], fill=pal['rim'])
        # Ruota anteriore
        d.ellipse([46, wy, 54, wy+5], fill=pal['tire'])
        d.ellipse([48, wy+1, 52, wy+4], fill=pal['rim'])

    # Cromatura paraurti
    d.line([(8, 24), (56, 24)], fill=pal['chrome'], width=1)

    return img

def main():
    for name, pal in VARIANTS.items():
        img = draw_gt_car(pal)
        out = os.path.join(BASE, f'auto_{name}.png')
        img.save(out)
        print(f'  {out}')

    manifest = {
        "nome": "auto_giocatore_gt",
        "descrizione": "Sagoma auto GT europea anni 70-90, ispirata a Citroen CX/DS, Ford Sierra/Cosworth/Capri/Escort RS. 3 varianti colore, stessa silhouette.",
        "autore": "genera_auto.py",
        "licenza": "CC0-1.0",
        "originale": True,
        "riferimenti_sega": False,
        "riferimenti_reali": "solo silhouette come ispirazione, nessun marchio/logo",
        "sprite": "64x32",
        "colori_max": 16,
        "file": [f"auto_{n}.png" for n in VARIANTS]
    }
    mp = os.path.join(BASE, 'manifest.json')
    with open(mp, 'w') as f:
        json.dump(manifest, f, indent=2, ensure_ascii=False)
    print(f'  {mp}')

if __name__ == '__main__':
    main()
