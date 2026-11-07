#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# AUTO1 G-A1 (@grafico, 07/11/2026): componenti dell'auto del giocatore.
# Regola: SOLO Andrea disegna le pose dell'auto (sprite/giocatore_pose/).
# Qui si disegnano invece i pezzi animati che il Renderer sovrappone ai
# fotogrammi carrozzeria (riserva, sprite/auto_gt/, 64x32):
#   ruota_1..3.png  12x10  ruota posteriore vista da dietro, 3 passi di
#                         rotazione; la destra e' la sinistra specchiata
#   luci_stop.png   12x4   fanale stop acceso, specchiato a destra
#   fumo_1..3.png   16x16  fumo scarico/danni, 3 passi, dithering Bayer 4x4
#   auto_giocatore.json    centro ruota/fanale/scarico per ogni fotogramma,
#                         in pixel dentro la griglia 64x32
# Palette piccola, pixel pieni, niente colori saturi puri, nessun materiale
# Sega. Originali disegnati da zero con Pillow.
#
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_componenti_auto.py"
import json
import math
import os
import struct

from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(BASE, "..", "..", ".."))
LIB = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali"))
OUT_PNG = os.path.join(LIB, "sprite", "giocatore")
OUT_RAW = os.path.join(ROOT, "motore_nuovo", "dati", "sprite_raw", "giocatore")
os.makedirs(OUT_PNG, exist_ok=True)
os.makedirs(OUT_RAW, exist_ok=True)

W, H = 64, 32  # griglia del fotogramma carrozzeria (sprite/auto_gt/)

GOMMA = (28, 28, 32)
GOMMA_L = (56, 56, 62)
GOMMA_O = (18, 18, 22)
CERCHIO = (170, 170, 162)
CERCHIO_L = (206, 204, 194)
CERCHIO_D = (96, 96, 92)
FANALE = (230, 58, 42)
FANALE_L = (252, 150, 110)
FANALE_D = (168, 30, 28)
FUMO = [(96, 96, 102), (128, 128, 134), (160, 160, 166)]

# Bayer 4x4 ordinato (stessa tecnica degli skyline, DIREZIONE_ARTISTICA).
BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]


def ruota(passo):
    """Ruota posteriore sinistra 12x10 vista da dietro: gomma scura,
    cerchio al centro e un tratto luminoso che gira col passo (0,1,2).
    La destra e' questa stessa immagine specchiata (flag del Renderer)."""
    img = Image.new("RGBA", (12, 10), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rectangle([1, 0, 10, 0], fill=GOMMA)
    d.rectangle([0, 1, 11, 8], fill=GOMMA)
    d.rectangle([1, 9, 10, 9], fill=GOMMA_O)
    d.line([1, 1, 1, 8], fill=GOMMA_L)
    d.line([10, 1, 10, 8], fill=GOMMA_L)
    d.rectangle([4, 4, 7, 7], fill=CERCHIO)
    d.rectangle([5, 5, 6, 6], fill=CERCHIO_D)
    if passo == 0:
        punti = [(4, 4), (5, 4)]      # in alto
    elif passo == 1:
        punti = [(7, 5), (7, 6)]      # a destra
    else:
        punti = [(5, 7), (6, 7)]      # in basso
    for p in punti:
        d.point(p, fill=CERCHIO_L)
    return img


def luci_stop():
    """Fanale stop acceso 12x4: rosso pieno con riga luminosa in alto.
    Specchiato a destra dal Renderer."""
    img = Image.new("RGBA", (12, 4), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, 11, 3], fill=FANALE)
    d.line([1, 0, 10, 0], fill=FANALE_L)
    d.line([0, 3, 11, 3], fill=FANALE_D)
    d.point([(0, 0), (11, 0), (0, 1), (0, 2), (11, 1), (11, 2)], fill=FANALE_D)
    return img


def fumo(passo):
    """Nuvola di fumo 16x16: 3 passi (compatto -> disperso), sagome piene
    ritagliate con dithering Bayer 4x4 sull'alpha, come da stile."""
    densita = (225, 185, 145)[passo]
    colore = FUMO[passo]
    mask = Image.new("L", (16, 16), 0)
    dm = ImageDraw.Draw(mask)
    if passo == 0:
        dm.ellipse([5, 8, 10, 13], fill=densita)
    elif passo == 1:
        dm.ellipse([3, 6, 10, 13], fill=densita)
        dm.ellipse([7, 9, 13, 15], fill=densita)
    else:
        dm.ellipse([2, 4, 9, 11], fill=densita)
        dm.ellipse([7, 7, 14, 13], fill=densita)
        dm.ellipse([4, 10, 10, 15], fill=densita)
    img = Image.new("RGBA", (16, 16), (0, 0, 0, 0))
    pm, px = mask.load(), img.load()
    for y in range(16):
        for x in range(16):
            a = pm[x, y]
            if a == 0:
                continue
            soglia = (BAYER[y % 4][x % 4] + 0.5) / 16.0 * 255.0
            if a >= soglia:
                px[x, y] = (colore[0], colore[1], colore[2], 255)
    return img


def salva(nome, img):
    img.save(os.path.join(OUT_PNG, nome + ".png"))
    dati = b"SGLR" + struct.pack("<II", img.width, img.height) + img.tobytes()
    with open(os.path.join(OUT_RAW, nome + ".sglraw"), "wb") as f:
        f.write(dati)
    print(f"  [OK] {nome}.png {img.width}x{img.height} + {nome}.sglraw")


# --- posizioni dei pezzi dentro il fotogramma 64x32 -------------------------
# Centri base (fotogramma dritto): ruote = centro dei vecchi pneumatici
# cuciti nella carrozzeria, fanali = centro dei fanali a fascia, scarichi =
# bocchette del doppio scarico. Gli stessi transform di
# disegna_auto_giocatore.py (rotazione attorno a 32,28; salita/discesa
# ancorata a terra) cosi' i pezzi restano attaccati al fotogramma ruotato.
POS_BASE = {
    "ruota_sx": (6.5, 24.0),
    "ruota_dx": (57.5, 24.0),
    "fanale_sx": (12.0, 15.5),
    "fanale_dx": (52.0, 15.5),
    "scarico_sx": (23.5, 25.5),
    "scarico_dx": (39.5, 25.5),
}

FOTOGRAMMI = [
    ("auto_giocatore_dritta", None),
    ("auto_giocatore_sterzo_sx_leggero", ("rot", 6)),
    ("auto_giocatore_sterzo_sx_forte", ("rot", 12)),
    ("auto_giocatore_sterzo_dx_leggero", ("rot", -6)),
    ("auto_giocatore_sterzo_dx_forte", ("rot", -12)),
    ("auto_giocatore_salita", ("squash", 1.12)),
    ("auto_giocatore_discesa", ("squash", 0.90)),
]


def trasforma(x, y, t):
    if t is None:
        return (x, y)
    if t[0] == "rot":
        rad = math.radians(t[1])
        c, s = math.cos(rad), math.sin(rad)
        dx, dy = x - 32.0, y - 28.0
        # rotazione antioraria in coordinate schermo (y verso il basso)
        return (32.0 + dx * c + dy * s, 28.0 - dx * s + dy * c)
    fy = t[1]
    alt = max(1, int(round(H * fy)))
    return (x, y * (alt / float(H)) + (H - alt))


def main():
    componenti = [
        ("ruota_1", ruota(0)),
        ("ruota_2", ruota(1)),
        ("ruota_3", ruota(2)),
        ("luci_stop", luci_stop()),
        ("fumo_1", fumo(0)),
        ("fumo_2", fumo(1)),
        ("fumo_3", fumo(2)),
    ]
    for nome, img in componenti:
        salva(nome, img)

    fotogrammi = {}
    for nome, t in FOTOGRAMMI:
        fotogrammi[nome] = {
            k: [round(v[0], 1), round(v[1], 1)]
            for k, v in ((k, trasforma(x, y, t)) for k, (x, y) in POS_BASE.items())
        }

    json_dati = {
        "versione": 1,
        "griglia": {"w": W, "h": H},
        "nota": (
            "AUTO1 G-A1 (@grafico): centri in pixel dentro il fotogramma "
            "64x32 di sprite/auto_gt/. ruota_dx e fanale_dx sono lo specchio "
            "di sx (Renderer con flag specchia); scarico_* e' dove parte il "
            "fumo. Le pose di Andrea (sprite/giocatore_pose/, 42x25) hanno "
            "ruote e fanali gia' disegnati: per esse non servono ruota/"
            "luci_stop, solo fumo verso il basso."
        ),
        "componenti": {
            "ruota": {"file": ["ruota_1.png", "ruota_2.png", "ruota_3.png"],
                      "misura": [12, 10]},
            "luci_stop": {"file": "luci_stop.png", "misura": [12, 4]},
            "fumo": {"file": ["fumo_1.png", "fumo_2.png", "fumo_3.png"],
                     "misura": [16, 16]},
        },
        "fotogrammi": fotogrammi,
    }
    percorso_json = os.path.join(OUT_PNG, "auto_giocatore.json")
    with open(percorso_json, "w", encoding="utf-8") as f:
        json.dump(json_dati, f, indent=1, ensure_ascii=False)
        f.write("\n")
    # Copia per il motore (A2 del @developer la legge da qui).
    with open(os.path.join(OUT_RAW, "auto_giocatore.json"), "w",
              encoding="utf-8") as f:
        json.dump(json_dati, f, indent=1, ensure_ascii=False)
        f.write("\n")
    print(f"  [OK] auto_giocatore.json ({len(fotogrammi)} fotogrammi)")

    print(f"componenti auto: {len(componenti)} PNG in {OUT_PNG}")
    print(f"raw + json: {OUT_RAW}")


if __name__ == "__main__":
    main()
