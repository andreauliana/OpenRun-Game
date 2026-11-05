#!/usr/bin/env python3
# Parapetto del ponte e guardrail dell'autostrada (blocco PIST1, passo G-P1,
# compito @grafico, richiesta Claude 02/11). Due oggetti COMUNI a tutti i temi:
# colori neutri (grigi leggermente freddi, nessun colore saturo puro) che stanno
# bene su ogni livello. Disegnati da zero con Pillow, stessa tecnica di
# disegna_pubblico.py: nessun materiale Sega, licenza CC0-1.0.
# Piccola palette + dithering Bayer 4x4 SOLO sulla faccia larga del parapetto
# (>10 px, regola trama di ANDREA_REVIEW); le fasce del guardrail sono piu'
# strette di 10 px e restano piene (il metallo ha il riflesso pieno, non la trama).
# Produce: Libreria/assets_originali/scenari/comuni/oggetti/parapetto.png (32x48)
#          Libreria/assets_originali/scenari/comuni/oggetti/guardrail.png (48x32)
# Poi: python3 motore_nuovo/strumenti_py/png_a_raw.py  (scrive i .sglraw)
#      python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py
# Per ridisegnarli a mano: stesso nome e stessa misura, poi png_a_raw.py.
import os
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali",
                                   "scenari", "comuni", "oggetti"))
os.makedirs(OUT, exist_ok=True)

TRASP = (0, 0, 0, 0)
BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]

# Palette neutra (cemento e acciaio): nessun puro saturo, toni vicini.
BORDO = (38, 40, 45, 255)
OMBRA = (58, 60, 66, 255)
CEM_XSC = (92, 95, 102, 255)      # cemento in ombra
CEM_SC = (132, 135, 142, 255)     # cemento medio-scuro (secondo tono della trama)
CEM = (170, 173, 179, 255)        # cemento (primo tono della trama)
CEM_CH = (198, 200, 205, 255)     # cemento chiaro (cappello)
CAP_CH = (220, 222, 226, 255)     # riflesso del cappello
ACC_CH = (214, 218, 224, 255)     # acciaio chiaro (specchio)
ACC = (174, 180, 188, 255)        # acciaio
ACC_SC = (124, 130, 140, 255)     # acciaio in ombra
ACC_XSC = (86, 91, 100, 255)      # acciaio scuro (incavo della trave)
POSTO = (104, 108, 116, 255)      # palo del guardrail


def trama(rett, x0, y0, x1, y1, cA, cB, soglia=9):
    """Dithering Bayer 4x4 dentro il rettangolo (x1/y1 esclusi): zone larghe."""
    px = rett.load()
    for y in range(y0, y1):
        for x in range(x0, x1):
            px[x, y] = cA if BAYER[y % 4][x % 4] < soglia else cB


def parapetto():
    """Muro di contenimento del ponte, 32x48, visto di faccia."""
    img = Image.new("RGBA", (32, 48), TRASP)
    d = ImageDraw.Draw(img)
    d.rectangle([1, 1, 30, 46], fill=BORDO)          # sagoma con bordo 1 px
    d.rectangle([2, 2, 29, 5], fill=CEM_CH)           # cappello
    d.line([2, 2, 29, 2], fill=CAP_CH)                # riflesso in alto
    trama(img, 2, 6, 30, 44, CEM, CEM_SC, 9)          # faccia (zona larga: trama)
    d.rectangle([26, 6, 29, 43], fill=CEM_XSC)        # spessore destro
    for x in (9, 17):                                 # giunti di getto
        d.line([x, 6, x, 43], fill=CEM_SC)
    d.rectangle([2, 44, 29, 45], fill=CEM_XSC)        # zoccolo
    d.rectangle([0, 47, 31, 47], fill=OMBRA)          # ombra al suolo
    return img


def guardrail():
    """Guardrail dell'autostrada con due pali, 48x32, visto di faccia."""
    img = Image.new("RGBA", (48, 32), TRASP)
    d = ImageDraw.Draw(img)
    for x0 in (5, 33):                                # pali (dietro la trave)
        d.rectangle([x0, 7, x0 + 5, 30], fill=POSTO)
        d.line([x0, 7, x0, 30], fill=BORDO)
        d.line([x0 + 5, 7, x0 + 5, 30], fill=BORDO)
        d.rectangle([x0, 30, x0 + 5, 30], fill=BORDO)
    # Trave a W: fasce piu' strette di 10 px -> colori pieni, nessuna trama.
    d.rectangle([0, 2, 47, 24], fill=BORDO)
    d.rectangle([1, 3, 46, 4], fill=ACC_CH)           # specchio superiore
    d.rectangle([1, 5, 46, 12], fill=ACC)             # faccia alta
    d.rectangle([1, 13, 46, 15], fill=ACC_XSC)        # incavo centrale
    d.rectangle([1, 16, 46, 22], fill=ACC)            # faccia bassa
    d.rectangle([1, 23, 46, 23], fill=ACC_SC)         # piega inferiore
    for x0 in (5, 33):                                # bulloni sui pali
        d.rectangle([x0 + 1, 7, x0 + 2, 8], fill=ACC_CH)
        d.rectangle([x0 + 1, 18, x0 + 2, 19], fill=ACC_CH)
    d.rectangle([0, 31, 47, 31], fill=OMBRA)          # ombra al suolo
    return img


if __name__ == "__main__":
    for nome, img in (("parapetto", parapetto()), ("guardrail", guardrail())):
        p = os.path.join(OUT, nome + ".png")
        img.save(p)
        print(f"  [OK] {p} {img.size}")
