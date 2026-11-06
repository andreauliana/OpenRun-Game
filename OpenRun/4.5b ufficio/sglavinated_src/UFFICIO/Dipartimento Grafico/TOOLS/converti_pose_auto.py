#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Pose dell'auto del giocatore disegnate da Andrea (06/11): tre quarti
# dall'alto, 4 per lato + salita/discesa. Converte i PNG di
#   Libreria/assets_originali/sprite/giocatore_pose/auto_<posa>.png
# nei raw che il motore legge in motore_nuovo/dati/sprite_raw/giocatore/pose/.
# Pose: dritta, sx1..sx4 (1 = appena girata, 4 = sterzo pieno), dx1..dx4
# (facoltative: se mancano il gioco specchia le sx), salita, discesa.
# Tutte della stessa misura della dritta. Facoltativi: auto_stop.png e
# auto_stop_<posa>.png con i soli stop accesi (vedi in fondo). Lanciare da qualunque cartella:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_pose_auto.py"
import os
import struct

from PIL import Image

BASE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(BASE, "..", "..", ".."))
SRC = os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "giocatore_pose")
DST = os.path.join(ROOT, "motore_nuovo", "dati", "sprite_raw", "giocatore", "pose")
POSE = ["dritta"] + [f"sx{k}" for k in range(1, 5)] + [f"dx{k}" for k in range(1, 5)] + ["salita", "discesa"]
os.makedirs(DST, exist_ok=True)
# Tutte le pose vengono ritagliate sullo stesso riquadro (l'unione delle parti
# non trasparenti), cosi' i margini vuoti del PNG non contano e le pose restano
# allineate tra loro.
immagini = {}
for p in POSE:
    png = os.path.join(SRC, f"auto_{p}.png")
    raw = os.path.join(DST, f"auto_{p}.sglraw")
    if os.path.exists(png):
        immagini[p] = Image.open(png).convert("RGBA")
    elif os.path.exists(raw):
        os.remove(raw)
if immagini:
    misura = immagini.get("dritta", next(iter(immagini.values()))).size
    box = None
    for p, im in immagini.items():
        if im.size != misura:
            print(f"ATTENZIONE auto_{p}.png: {im.size[0]}x{im.size[1]}, la dritta e' {misura[0]}x{misura[1]}")
        b = im.getchannel("A").getbbox()
        if b:
            box = b if box is None else (min(box[0], b[0]), min(box[1], b[1]), max(box[2], b[2]), max(box[3], b[3]))
    for p, im in immagini.items():
        im = im.crop(box) if box else im
        raw = os.path.join(DST, f"auto_{p}.sglraw")
        with open(raw, "wb") as f:
            f.write(b"SGLR" + struct.pack("<II", im.width, im.height) + im.tobytes())
        print(f"scritto {raw} ({im.width}x{im.height})")
    # Stop accesi (facoltativi): auto_stop.png per la dritta, auto_stop_<posa>.png
    # per le altre. Stessa misura della posa, solo i fanali accesi, il resto
    # trasparente. Il gioco li sovrappone alla posa quando si frena.
    for p in POSE:
        nome = "auto_stop.png" if p == "dritta" else f"auto_stop_{p}.png"
        png = os.path.join(SRC, nome)
        raw = os.path.join(DST, f"auto_stop_{p}.sglraw")
        if not os.path.exists(png) or p not in immagini:
            if os.path.exists(raw):
                os.remove(raw)
            continue
        im = Image.open(png).convert("RGBA")
        if im.size != immagini[p].size:
            print(f"ATTENZIONE {nome}: {im.size[0]}x{im.size[1]}, la posa e' {immagini[p].size[0]}x{immagini[p].size[1]}")
        im = im.crop(box) if box else im
        with open(raw, "wb") as f:
            f.write(b"SGLR" + struct.pack("<II", im.width, im.height) + im.tobytes())
        print(f"scritto {raw} ({im.width}x{im.height})")
