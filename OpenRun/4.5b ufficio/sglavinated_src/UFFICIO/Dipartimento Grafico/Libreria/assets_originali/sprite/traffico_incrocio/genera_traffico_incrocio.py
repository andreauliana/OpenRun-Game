#!/usr/bin/env python3
# Auto viste di lato per gli incroci a 90 gradi della citta' (Claude 08/11,
# segnaposto: Andrea le puo' ridisegnare con lo stesso nome e la stessa misura,
# poi rilanciare con --solo-raw). Muso a DESTRA; il motore le specchia per chi
# va a sinistra. Uso (da sglavinated_src):
#   python3 "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/sprite/traffico_incrocio/genera_traffico_incrocio.py" [--solo-raw]
import struct, sys
from pathlib import Path
from PIL import Image, ImageDraw

QUI = Path(__file__).resolve().parent
RADICE = QUI.parents[5]
OUT = RADICE / "motore_nuovo" / "dati" / "sprite_raw" / "traffico_incrocio"
NERO = (24, 22, 30, 255)
VETRO = (120, 170, 200, 255)
VETRO2 = (80, 120, 160, 255)
GOMMA = (30, 30, 34, 255)
CERCHIO = (170, 170, 180, 255)
FARO = (250, 240, 170, 255)
STOP = (220, 40, 40, 255)

def ruota(d, cx, cy, r):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=GOMMA, outline=NERO)
    d.rectangle([cx - 1, cy - 1, cx + 1, cy + 1], fill=CERCHIO)

def berlina(col, scuro):
    w, h = 48, 15
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0)); d = ImageDraw.Draw(im)
    d.polygon([(13, 5), (17, 1), (31, 1), (37, 5)], fill=col, outline=NERO)       # abitacolo
    d.polygon([(16, 5), (19, 2), (24, 2), (24, 5)], fill=VETRO)                   # finestrino dietro
    d.polygon([(26, 5), (26, 2), (30, 2), (34, 5)], fill=VETRO2)                  # finestrino davanti
    d.rounded_rectangle([1, 5, 46, 11], 2, fill=col, outline=NERO)                # carrozzeria
    d.line([(3, 9), (44, 9)], fill=scuro)                                         # fiancata
    d.line([(25, 6), (25, 9)], fill=scuro)                                        # porta
    d.rectangle([44, 6, 45, 7], fill=FARO); d.rectangle([2, 6, 3, 7], fill=STOP)
    ruota(d, 10, 11, 3); ruota(d, 37, 11, 3)
    return im

def furgone(col, scuro):
    w, h = 52, 19
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0)); d = ImageDraw.Draw(im)
    d.rounded_rectangle([1, 1, 42, 15], 2, fill=col, outline=NERO)                # vano
    d.polygon([(42, 4), (47, 7), (50, 9), (50, 15), (42, 15)], fill=col, outline=NERO)  # muso
    d.polygon([(43, 5), (46, 7), (48, 9), (43, 9)], fill=VETRO2)
    d.line([(3, 12), (49, 12)], fill=scuro)
    for x in range(6, 40, 8): d.line([(x, 3), (x, 10)], fill=scuro)               # nervature
    d.rectangle([49, 10, 50, 11], fill=FARO); d.rectangle([2, 10, 3, 12], fill=STOP)
    ruota(d, 10, 15, 3); ruota(d, 42, 15, 3)
    return im

def bus():
    # Bus a due piani rosso, da citta' (lv_05 "Urban, London, public transport").
    col, scuro = (196, 40, 44, 255), (130, 24, 30, 255)
    w, h = 64, 29
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0)); d = ImageDraw.Draw(im)
    d.rounded_rectangle([1, 1, 62, 25], 3, fill=col, outline=NERO)
    for y0 in (4, 14):                                                            # due file di finestrini
        for x in range(5, 58, 8): d.rectangle([x, y0, x + 5, y0 + 5], fill=VETRO if (x // 8) % 2 else VETRO2)
    d.line([(2, 12), (61, 12)], fill=(236, 220, 200, 255))                        # fascia crema
    d.rectangle([58, 14, 61, 22], fill=VETRO2)                                    # parabrezza
    d.line([(3, 23), (60, 23)], fill=scuro)
    d.rectangle([61, 20, 62, 21], fill=FARO); d.rectangle([1, 20, 2, 22], fill=STOP)
    ruota(d, 12, 25, 3); ruota(d, 50, 25, 3)
    return im

AUTO = {
    "lato_berlina_gialla": lambda: berlina((226, 190, 60, 255), (170, 130, 30, 255)),   # taxi
    "lato_berlina_verde": lambda: berlina((70, 140, 110, 255), (40, 96, 72, 255)),
    "lato_furgone_bianco": lambda: furgone((220, 222, 228, 255), (150, 152, 166, 255)),
    "lato_bus_rosso": bus,
}

def salva_raw(png, raw):
    im = Image.open(png).convert("RGBA")
    raw.parent.mkdir(parents=True, exist_ok=True)
    raw.write_bytes(b"SGLR" + struct.pack("<II", im.width, im.height) + im.tobytes())

if __name__ == "__main__":
    for nome, f in AUTO.items():
        png = QUI / (nome + ".png")
        if "--solo-raw" not in sys.argv: f().save(png)
        salva_raw(png, OUT / (nome + ".sglraw"))
        print(png.name)
