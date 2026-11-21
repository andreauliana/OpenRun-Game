#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Livello desktop (lv_06): alberi, cespugli ed erba in stile icone anni '90
# (Andrea 07/11: "piu' pixelati, piu' carini e simpatici e diversi, stile
# windows 98"). Disegnati a meta' risoluzione e ingranditi 2x: pixel grossi,
# contorno nero, pochi colori pieni della tavolozza a 16 colori e trama a
# scacchi 50% per l'ombra, come le icone di allora. Disegni originali, nessun
# materiale di terzi.
#
# Misure finali uguali ai segnaposto (il motore ricava la taglia dall'altezza):
#   albero, albero_pino, albero_fiori 48x96; cespuglio, cespuglio_fiori 40x24;
#   ciuffo 24x12.
# Non sovrascrive i PNG esistenti salvo --forza (i disegni di Andrea restano).
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_paesaggio_w98.py"
import sys
from pathlib import Path
from PIL import Image

sys.path.insert(0, str(Path(__file__).parent))
from disegna_paesaggio import Tela, c, salva, SCENARI, RAW, CARTELLA  # noqa: E402

LIVELLO = "lv_06_desktop"
NERO, BIANCO = c("000000"), c("FFFFFF")
VERDE_S, VERDE, VERDE_L = c("008000"), c("00A800"), c("00FF00")
OLIVA, MARRONE, MARRONE_L = c("808000"), c("804000"), c("C08040")
ROSSO, GIALLO, ROSA, ROSA_L, VIOLA = c("FF0000"), c("FFFF00"), c("FF80C0"), c("FFC0E0"), c("C000C0")
TEAL = c("008080")


def scacchi(t, x, y):
    return (x + y) % 2 == 0


def piccola(w, h):
    return Tela(w // 2, h // 2)


def grande(t):
    """Ingrandisce 2x senza filtri: pixel grossi."""
    g = Tela(t.w * 2, t.h * 2)
    g.img = t.img.resize((t.w * 2, t.h * 2), Image.NEAREST)
    g.px = g.img.load()
    return g


def chioma(t, cerchi, scuro, medio, chiaro, luce=True):
    for cx, cy, r in cerchi:
        t.ellisse(cx, cy, r, r, medio)
    # ombra a scacchi in basso a destra, luce in alto a sinistra
    for y in range(t.h):
        for x in range(t.w):
            if t.px[x, y] != medio:
                continue
            dentro = min(((x - cx) ** 2 + (y - cy) ** 2) ** 0.5 - r for cx, cy, r in cerchi)
            sud_est = any((x - cx) + (y - cy) > r * 0.55 for cx, cy, r in cerchi if ((x - cx) ** 2 + (y - cy) ** 2) <= r * r)
            if sud_est and scacchi(t, x, y):
                t.p(x, y, scuro)
            elif luce and dentro < -1 and any((x - cx) + (y - cy) < -r * 0.95 for cx, cy, r in cerchi):
                t.p(x, y, chiaro)


def albero():
    t = piccola(48, 96)
    t.rett(10, 18, 14, 48, MARRONE)
    t.rett(10, 18, 11, 48, MARRONE_L)
    t.rett(8, 46, 16, 48, MARRONE)
    chioma(t, ((12, 13, 9), (6, 19, 5), (18, 19, 5), (12, 6, 6)), VERDE_S, VERDE, VERDE_L)
    t.p(7, 9, BIANCO); t.p(8, 8, BIANCO)              # riflesso da icona
    for x, y in ((15, 15), (8, 21), (17, 22)):        # mele
        t.p(x, y, ROSSO)
    t.contorno(NERO)
    return grande(t)


def albero_pino():
    t = piccola(48, 96)
    t.rett(11, 38, 13, 48, MARRONE)
    for y0, r in ((2, 4), (10, 7), (19, 9), (28, 11)):  # piani a gradini
        for y in range(y0, y0 + 11):
            half = 1 + (r * (y - y0)) // 10
            for x in range(12 - half, 12 + half):
                col = VERDE_S if (x >= 12 and scacchi(t, x, y)) else (TEAL if y - y0 > 8 else VERDE)
                t.p(x, y, col)
    t.p(12, 1, GIALLO); t.p(11, 2, GIALLO); t.p(12, 2, GIALLO)   # stellina in cima
    for x, y in ((9, 16), (15, 24), (8, 32), (14, 34)):        # palline colorate
        t.p(x, y, (ROSSO, GIALLO, VIOLA, BIANCO)[(x + y) % 4])
    t.contorno(NERO)
    return grande(t)


def albero_fiori():
    t = piccola(48, 96)
    t.rett(11, 18, 14, 48, MARRONE)
    t.rett(9, 34, 11, 35, MARRONE); t.rett(14, 31, 16, 32, MARRONE)  # rametti
    t.rett(8, 46, 17, 48, MARRONE)
    chioma(t, ((12, 15, 8), (6, 20, 5), (18, 20, 5), (8, 9, 5), (16, 9, 5)), VIOLA, ROSA, ROSA_L)
    for x, y in ((5, 14), (11, 7), (17, 13), (13, 20), (7, 23), (19, 19)):
        t.p(x, y, BIANCO)
    t.contorno(NERO)
    for x, y in ((4, 46), (19, 45), (21, 47)):  # petali caduti
        t.p(x, y, ROSA)
    return grande(t)


def cespuglio():
    t = piccola(40, 24)
    chioma(t, ((6, 8, 5), (13, 7, 6), (10, 4, 4)), VERDE_S, VERDE, VERDE_L)
    t.rett(1, 11, 19, 12, VERDE_S)
    t.contorno(NERO)
    return grande(t)


def cespuglio_fiori():
    t = piccola(40, 24)
    chioma(t, ((6, 8, 5), (14, 8, 5), (10, 5, 4)), VERDE_S, VERDE, VERDE_L, luce=False)
    for x, y, col in ((5, 5, ROSSO), (10, 3, GIALLO), (15, 6, ROSSO), (8, 9, GIALLO), (13, 9, BIANCO), (3, 8, BIANCO)):
        t.p(x, y, col)
    t.rett(1, 11, 19, 12, VERDE_S)
    t.contorno(NERO)
    return grande(t)


def ciuffo():
    t = piccola(24, 12)
    for x, h in ((1, 3), (3, 5), (5, 4), (7, 2), (9, 4), (10, 5)):
        t.rett(x, 6 - h, x + 1, 6, VERDE if h > 3 else VERDE_S)
        t.p(x, 6 - h, VERDE_L)
    t.contorno(NERO)
    return grande(t)


def main():
    forza = "--forza" in sys.argv
    base = SCENARI / LIVELLO / CARTELLA[LIVELLO] / "oggetti"
    for nome, fn in (("albero", albero), ("albero_pino", albero_pino), ("albero_fiori", albero_fiori),
                     ("cespuglio", cespuglio), ("cespuglio_fiori", cespuglio_fiori), ("ciuffo", ciuffo)):
        salva(fn(), base / f"obj_{nome}.png", RAW / LIVELLO / f"{nome}.sglraw", forza)


if __name__ == "__main__":
    main()
