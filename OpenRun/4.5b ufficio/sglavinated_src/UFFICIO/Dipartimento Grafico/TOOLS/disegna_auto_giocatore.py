#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# RIV punto 3 (Andrea 23/10 e 27/10): fotogrammi dell'auto del giocatore per il
# finto 3D alla OutRun, scelti dal motore in base a sterzo, curva e quota.
# Disegnati da zero con Pillow, nessun materiale Sega, nessun marchio reale.
# Stessa misura per tutti (64x32) cosi' il motore puo' scambiarli senza
# ricalcolare nulla. Palette piccola, pixel pieni, niente sfumature.
#
# Produce in assets_originali/sprite/auto_gt/ (variante colore D5 rosso corsa):
#   auto_giocatore_dritta.png
#   auto_giocatore_sterzo_sx_leggero.png / _forte.png
#   auto_giocatore_sterzo_dx_leggero.png / _forte.png
#   auto_giocatore_salita.png / auto_giocatore_discesa.png
# e i raw che il motore legge in
#   motore_nuovo/dati/sprite_raw/giocatore/<nome>.sglraw
# Formato raw: "SGLR" + u32 LE w + u32 LE h + RGBA8888 (come png_a_raw.py).
#
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_auto_giocatore.py"
import os
import struct

from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(BASE, "..", "..", ".."))
LIB = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali"))
OUT_PNG = os.path.join(LIB, "sprite", "auto_gt")
OUT_RAW = os.path.join(ROOT, "motore_nuovo", "dati", "sprite_raw", "giocatore")
os.makedirs(OUT_PNG, exist_ok=True)
os.makedirs(OUT_RAW, exist_ok=True)

W, H = 64, 32

# Variante "rosso_corsa" approvata (D5). Colori pieni, nessun saturo puro.
BODY = (196, 42, 38)
DARK = (108, 20, 22)
LUCE = (238, 120, 96)
GLASS = (26, 32, 54)
GLASS_L = (96, 130, 180)
GOMMA = (28, 28, 32)
CERCHIO = (170, 170, 162)
FANALE = (230, 58, 42)
FANALE_L = (252, 150, 110)
TARGA = (232, 230, 214)
CROMO = (206, 202, 188)
GIALLO = (238, 190, 44)
NERO = (14, 14, 18)


def base():
    """Auto vista da dietro: alettone, lunotto, fanali a fascia, targa,
    doppio scarico, gomme larghe. Sagoma GT bassa e larga."""
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    # Ombra a terra.
    d.ellipse([8, 27, 56, 31], fill=(0, 0, 0, 95))
    # Gomme posteriori, sporgono ai lati.
    d.rectangle([2, 16, 11, 29], fill=GOMMA)
    d.rectangle([53, 16, 62, 29], fill=GOMMA)
    d.line([3, 17, 3, 28], fill=(56, 56, 62))
    d.line([54, 17, 54, 28], fill=(56, 56, 62))
    d.rectangle([6, 20, 7, 25], fill=CERCHIO)   # cerchio sinistro
    d.rectangle([57, 20, 58, 25], fill=CERCHIO)  # cerchio destro
    # Carrozzeria: fianchi larghi in basso, spalle che rientrano.
    d.polygon([(5, 13), (9, 8), (55, 8), (59, 13), (59, 23), (5, 23)],
              fill=BODY, outline=DARK)
    d.line([10, 9, 54, 9], fill=LUCE)  # riflesso sul cofano
    # Abitacolo e lunotto.
    d.polygon([(15, 9), (19, 3), (45, 3), (49, 9)], fill=BODY, outline=DARK)
    d.polygon([(17, 8), (21, 4), (43, 4), (47, 8)], fill=GLASS)
    d.line([22, 4, 31, 4], fill=GLASS_L)
    # Alettone con due supporti.
    d.rectangle([9, 6, 55, 7], fill=DARK)
    d.line([10, 6, 54, 6], fill=BODY)
    d.rectangle([14, 8, 15, 8], fill=DARK)
    d.rectangle([48, 8, 49, 8], fill=DARK)
    # Fascia da corsa gialla sul tetto e sulla coda.
    d.rectangle([30, 4, 31, 7], fill=GIALLO)
    d.rectangle([33, 4, 34, 7], fill=GIALLO)
    d.rectangle([30, 9, 31, 12], fill=GIALLO)
    d.rectangle([33, 9, 34, 12], fill=GIALLO)
    # Fanali a fascia con linea luminosa.
    d.rectangle([6, 14, 18, 17], fill=FANALE, outline=NERO)
    d.rectangle([46, 14, 58, 17], fill=FANALE, outline=NERO)
    d.line([7, 15, 11, 15], fill=FANALE_L)
    d.line([53, 15, 57, 15], fill=FANALE_L)
    d.line([19, 15, 45, 15], fill=(168, 30, 28))
    # Targa e paraurti cromato.
    d.rectangle([27, 18, 37, 21], fill=TARGA, outline=NERO)
    d.rectangle([5, 22, 59, 24], fill=DARK)
    d.line([6, 23, 58, 23], fill=CROMO)
    # Doppio scarico.
    d.rectangle([22, 25, 25, 26], fill=NERO)
    d.rectangle([38, 25, 41, 26], fill=NERO)
    d.point([(23, 25)], fill=CROMO)
    d.point([(39, 25)], fill=CROMO)
    return img


def ruota(img, gradi):
    """Inclina l'auto come in curva (finto 3D): rotazione attorno al punto
    sotto l'asse posteriore, poi ritaglio alla stessa misura 64x32."""
    canvas = Image.new("RGBA", (80, 44), (0, 0, 0, 0))
    canvas.paste(img, (8, 6))
    rot = canvas.rotate(gradi, resample=Image.NEAREST, center=(40, 34))
    return rot.crop((8, 6, 8 + W, 6 + H))


def schiaccia(img, fy):
    """Salita/discesa: schiaccia o allunga in verticale, ancorata a terra,
    cosi' il fotogramma resta 64x32 e si puo' scambiare senza ricalcoli."""
    alt = max(1, int(round(H * fy)))
    scalata = img.resize((W, alt), resample=Image.NEAREST)
    out = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    out.paste(scalata, (0, H - alt))
    return out


def salva(nome, img):
    img.save(os.path.join(OUT_PNG, nome + ".png"))
    dati = b"SGLR" + struct.pack("<II", img.width, img.height) + img.tobytes()
    with open(os.path.join(OUT_RAW, nome + ".sglraw"), "wb") as f:
        f.write(dati)
    print(f"  [OK] {nome}.png {img.width}x{img.height} + {nome}.sglraw")


def main():
    dritta = base()
    # +gradi = inclinazione a sinistra, -gradi = a destra (OutRun: la carrozzeria
    # si piega verso l'esterno della curva).
    frames = [
        ("auto_giocatore_dritta", dritta),
        ("auto_giocatore_sterzo_sx_leggero", ruota(dritta, 6)),
        ("auto_giocatore_sterzo_sx_forte", ruota(dritta, 12)),
        ("auto_giocatore_sterzo_dx_leggero", ruota(dritta, -6)),
        ("auto_giocatore_sterzo_dx_forte", ruota(dritta, -12)),
        ("auto_giocatore_salita", schiaccia(dritta, 1.12)),
        ("auto_giocatore_discesa", schiaccia(dritta, 0.90)),
    ]
    for nome, img in frames:
        salva(nome, img)
    print(f"auto giocatore: {len(frames)} fotogrammi in {OUT_PNG}")
    print(f"raw: {OUT_RAW}")


if __name__ == "__main__":
    main()
