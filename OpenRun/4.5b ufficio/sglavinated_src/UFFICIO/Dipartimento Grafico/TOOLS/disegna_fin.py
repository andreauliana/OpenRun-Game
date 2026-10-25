#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""disegna_fin.py - disegna le 3 scene di arrivo (blocco FIN) in bianco e nero.

Scene (FLUSSO_GIOCO.md sezione 4, proposte ancora da confermare da Andrea):
  lv_04_cromo: citta futurista, robot che premiano l'auto
  lv_05_spray: festa punk tra i muri a spray
  lv_06_desktop: il desktop si "spegne" e riparte con l'auto vincitrice

Tema bianco e nero (D7): solo nero/bianco/grigi (R=G=B), risoluzione 320x224.
Output: assets_originali/scenari/<livello>/<varianteA>/finale/finale.png
Uso: python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_fin.py"
"""
import pathlib

from PIL import Image, ImageDraw

ROOT = pathlib.Path(__file__).resolve().parents[3]  # sglavinated_src
GRAFICO = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali"

NERO = (0, 0, 0)
BIANCO = (255, 255, 255)
GRIGIO = (160, 160, 160)
GRIGIO_SCURO = (100, 100, 100)

W, H = 320, 224


def rettangolo(d, x, y, w, h, colore, riempito=False):
    if riempito:
        d.rectangle([x, y, x + w, y + h], fill=colore)
    else:
        d.rectangle([x, y, x + w, y + h], outline=colore, width=2)


def linea(d, x1, y1, x2, y2, colore, sp=2):
    d.line([x1, y1, x2, y2], fill=colore, width=sp)


def cerchio(d, cx, cy, r, colore, riempito=False, sp=2):
    if riempito:
        d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=colore)
    else:
        d.ellipse([cx - r, cy - r, cx + r, cy + r], outline=colore, width=sp)


def scena_cromo(d):
    """lv_04_cromo: citta futurista, robot che premiano l'auto."""
    # sfondo nero
    d.rectangle([0, 0, W, H], fill=NERO)
    # grattacieli futuristi (siluette bianche)
    for x, w, h in [(20, 40, 110), (70, 30, 90), (110, 45, 120), (165, 35, 100), (210, 40, 115), (260, 30, 85)]:
        rettangolo(d, x, H - h - 24, w, h, BIANCO, riempito=True)
        # finestre
        for wx in range(x + 6, x + w - 6, 10):
            for wy in range(H - h - 18, H - 30, 14):
                d.rectangle([wx, wy, wx + 4, wy + 4], fill=NERO)
    # linee di velocita / scie
    for i in range(3):
        linea(d, 0, 40 + i * 30, 320, 30 + i * 30, GRIGIO, sp=1)
    # robot al centro (premia l'auto)
    d.rectangle([130, 110, 160, 170], fill=BIANCO)  # corpo
    d.rectangle([136, 110, 154, 124], fill=NERO)    # testa
    d.rectangle([122, 130, 134, 160], fill=GRIGIO)  # braccio sx
    d.rectangle([156, 130, 168, 160], fill=GRIGIO)  # braccio dx
    d.rectangle([130, 160, 160, 174], fill=BIANCO)  # gambe
    # trofeo nella mano destra
    d.rectangle([156, 140, 172, 152], fill=BIANCO)
    d.ellipse([160, 144, 168, 152], fill=NERO)
    # auto in basso (vincitrice)
    d.rectangle([100, 180, 200, 204], fill=BIANCO)
    d.rectangle([110, 166, 190, 182], fill=BIANCO)  # tetto
    cerchio(d, 112, 200, 7, BIANCO, riempito=True)
    cerchio(d, 112, 200, 3, NERO, riempito=True)
    cerchio(d, 188, 200, 7, BIANCO, riempito=True)
    cerchio(d, 188, 200, 3, NERO, riempito=True)
    # etichetta
    d.text((100, 212), "LV04 CROMO - ARRIVO", fill=BIANCO)


def scena_spray(d):
    """lv_05_spray: festa punk tra i muri a spray."""
    d.rectangle([0, 0, W, H], fill=NERO)
    # muri a spray laterali
    d.rectangle([0, 60, 60, 200], fill=BIANCO)
    d.rectangle([260, 60, 320, 200], fill=BIANCO)
    # scritte graffiti (semuplici segni)
    for i in range(6):
        x = 10 + i * 10
        linea(d, x, 70, x + 30, 100, NERO, sp=3)
        linea(d, x + 30, 70, x, 100, NERO, sp=3)
    for i in range(6):
        x = 220 + i * 10
        linea(d, x, 130, x + 30, 100, NERO, sp=3)
        linea(d, x + 30, 130, x, 100, NERO, sp=3)
    # bombolette spray
    for x in [40, 280]:
        d.rectangle([x, 104, x + 14, 124], fill=GRIGIO)
        d.rectangle([x + 2, 100, x + 12, 106], fill=BIANCO)
    # note musicali (festa)
    for cx, cy in [(100, 100), (160, 90), (220, 110)]:
        d.rectangle([cx, cy, cx + 4, cy + 24], fill=BIANCO)
        cerchio(d, cx + 4, cy + 4, 6, BIANCO, riempito=True)
    # auto al centro (vincitrice)
    d.rectangle([105, 170, 215, 194], fill=BIANCO)
    d.rectangle([115, 156, 205, 172], fill=BIANCO)
    cerchio(d, 118, 190, 7, BIANCO, riempito=True)
    cerchio(d, 118, 190, 3, NERO, riempito=True)
    cerchio(d, 202, 190, 7, BIANCO, riempito=True)
    cerchio(d, 202, 190, 3, NERO, riempito=True)
    d.text((100, 212), "LV05 SPRAY - ARRIVO", fill=BIANCO)


def scena_desktop(d):
    """lv_06_desktop: desktop si spegne e riparte con l'auto vincitrice."""
    d.rectangle([0, 0, W, H], fill=BIANCO)  # sfondo desktop chiaro
    # barra delle applicazioni in basso
    d.rectangle([0, 204, W, 224], fill=GRIGIO_SCURO)
    # icone desktop
    for x, y in [(30, 30), (100, 30), (30, 100), (100, 100)]:
        d.rectangle([x, y, x + 28, y + 28], fill=GRIGIO)
        d.rectangle([x + 4, y + 4, x + 24, y + 24], fill=BIANCO)
        d.text((x + 6, y + 10), "APP", fill=NERO)
    # finestra aperta al centro
    d.rectangle([130, 40, 300, 160], fill=GRIGIO)
    d.rectangle([130, 40, 300, 56], fill=GRIGIO_SCURO)  # title bar
    d.text((136, 46), "WIN", fill=BIANCO)
    # simbolo di spegnimento (power button)
    d.rectangle([210, 70, 220, 90], fill=NERO)
    cerchio(d, 215, 64, 12, NERO, sp=3)
    # interruttore
    d.rectangle([170, 170, 180, 184], fill=BIANCO)
    d.rectangle([166, 166, 184, 174], fill=NERO)
    # auto in basso (vincitrice, in primo piano)
    d.rectangle([95, 180, 225, 204], fill=BIANCO)
    d.rectangle([105, 166, 215, 182], fill=BIANCO)
    cerchio(d, 108, 200, 7, GRIGIO, riempito=True)
    cerchio(d, 108, 200, 3, NERO, riempito=True)
    cerchio(d, 212, 200, 7, GRIGIO, riempito=True)
    cerchio(d, 212, 200, 3, NERO, riempito=True)
    d.text((100, 212), "LV06 DESKTOP - ARRIVO", fill=NERO)


def salva_scena(nome_cartella, nome_file, disegna):
    cartella = GRAFICO / "scenari" / nome_cartella
    cartella.mkdir(parents=True, exist_ok=True)
    img = Image.new("RGBA", (W, H), NERO + (255,))
    d = ImageDraw.Draw(img)
    disegna(d)
    path = cartella / nome_file
    img.save(path)
    print(f"  OK {path.relative_to(ROOT)}  {W}x{H}")


def main():
    print("disegna_fin.py - scene arrivo bianco e nero (blocco FIN)")
    salva_scena("lv_04_cromo/scenario_cromo_04_varianteA/finale", "finale.png", scena_cromo)
    salva_scena("lv_05_spray/scenario_spray_05_varianteA/finale", "finale.png", scena_spray)
    salva_scena("lv_06_desktop/scenario_desktop_06_varianteA/finale", "finale.png", scena_desktop)
    print("fatto: 3 scene di arrivo disegnate")


if __name__ == "__main__":
    main()
