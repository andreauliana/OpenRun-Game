#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Steering poses for the player car, built from Andrea's straight pose
# (06/11). They are PLACEHOLDERS for Andrea to redraw by hand: the script never
# overwrites a pose that already exists (use --sovrascrivi to force it).
#
# From auto_dritta.png (+ auto_stop.png) it writes auto_sx1..auto_sx4.png
# (+ auto_stop_sx1..4.png). Turning left, the car yaws:
#   - the upper rows (roof, further away) slide left more than the bottom rows;
#   - the gap left on the right becomes the right flank (darker body colour,
#     dark glass on the cabin rows);
#   - the front-left wheel shows up on the left, turned, a bit higher than the
#     rear wheel (it is further away).
# The game mirrors the sx poses for the right side. Then run
# converti_pose_auto.py as usual.
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_pose_sterzo.py"
import os
import sys

from PIL import Image

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "giocatore_pose")
SOVRASCRIVI = "--sovrascrivi" in sys.argv

# Maximum shift of the top row at full steering (level 4), in pixels of the
# drawing; the front wheel and the flank follow it.
TAGLIO_MAX = 9
GOMMA = (24, 22, 24, 255)
GOMMA_LUCE = (70, 66, 70, 255)
CERCHIO = (150, 146, 152, 255)


def bbox_righe(im):
    a = im.getchannel("A")
    x0, y0, x1, y1 = a.getbbox()
    return x0, y0, x1, y1


def scurisci(c, k):
    return (int(c[0] * k), int(c[1] * k), int(c[2] * k), 255)


def colore_corpo(im, y0, y1):
    # Most common opaque, fairly saturated colour of the lower body rows.
    conta = {}
    for y in range((y0 + y1) // 2, y1):
        for x in range(im.width):
            r, g, b, a = im.getpixel((x, y))
            if a < 200 or max(r, g, b) < 90 or max(r, g, b) - min(r, g, b) < 60:
                continue
            conta[(r, g, b)] = conta.get((r, g, b), 0) + 1
    if not conta:
        return (120, 20, 24, 255)
    r, g, b = max(conta, key=conta.get)
    return (r, g, b, 255)


def taglio(y, y0, y1, liv):
    # 0 at the bottom row, -liv/4 * TAGLIO_MAX at the top row.
    t = (y1 - 1 - y) / max(1, (y1 - 1 - y0))
    return -int(round(t * TAGLIO_MAX * liv / 4.0))


def genera(dritta, stop, liv):
    x0, y0, x1, y1 = bbox_righe(dritta)
    corpo = colore_corpo(dritta, y0, y1)
    fianco = scurisci(corpo, 0.62)
    fianco_luce = scurisci(corpo, 0.85)
    vetro = (40, 30, 44, 255)
    altezza = y1 - y0
    cabina_fine = y0 + int(altezza * 0.45)  # rows above this are roof/glass
    out = Image.new("RGBA", dritta.size, (0, 0, 0, 0))
    out_stop = Image.new("RGBA", dritta.size, (0, 0, 0, 0)) if stop else None
    for y in range(y0, y1):
        d = taglio(y, y0, y1, liv)
        riga = [dritta.getpixel((x, y)) for x in range(dritta.width)]
        opachi = [x for x in range(dritta.width) if riga[x][3] >= 128]
        for x in range(dritta.width):
            nx = x + d
            if 0 <= nx < dritta.width and riga[x][3] > 0:
                out.putpixel((nx, y), riga[x])
                if out_stop is not None:
                    s = stop.getpixel((x, y))
                    if s[3] > 0:
                        out_stop.putpixel((nx, y), s)
        if not opachi or d == 0:
            continue
        # Right flank: fills the gap between the shifted right edge and the
        # original one (the side of the car that turns towards the camera).
        destra = max(opachi)
        # The flank also sticks out past the rear corner (the car's length is
        # now seen at an angle): 0..2 px, more at the bottom.
        oltre = (liv + 1) // 2 if y >= cabina_fine else 0
        for x in range(destra + d + 1, min(dritta.width, destra + 1 + oltre)):
            if y < cabina_fine:
                c = vetro if y > y0 + 1 else fianco
            else:
                c = fianco_luce if (y - cabina_fine) == 0 else fianco
            out.putpixel((x, y), c)
    # Front-left wheel, turned: an oval peeking out on the left, ahead of the
    # rear wheel (higher), wider with more steering.
    larg = 2 + liv // 2                 # 2..4 px of tread visible
    alto = 4 + (liv >= 3)
    sinistra_bassa = min(x for x in range(dritta.width) if dritta.getpixel((x, y1 - 2))[3] >= 128)
    yb = y1 - 2 - (1 + liv // 2)        # higher than the rear wheel
    xb = sinistra_bassa + taglio(yb, y0, y1, liv) - larg + 1
    for dy in range(alto):
        for dx in range(larg):
            x, y = xb + dx, yb - alto + 1 + dy
            if not (0 <= x < out.width and 0 <= y < out.height):
                continue
            if out.getpixel((x, y))[3] >= 128 and dx == larg - 1:
                continue  # body stays in front of the wheel
            c = GOMMA_LUCE if dy == 0 else GOMMA
            if larg >= 3 and dx == 1 and 0 < dy < alto - 1:
                c = CERCHIO
            out.putpixel((x, y), c)
    return out, out_stop


def main():
    dritta_png = os.path.join(SRC, "auto_dritta.png")
    if not os.path.exists(dritta_png):
        print("manca auto_dritta.png")
        return 1
    dritta = Image.open(dritta_png).convert("RGBA")
    stop_png = os.path.join(SRC, "auto_stop.png")
    stop = Image.open(stop_png).convert("RGBA") if os.path.exists(stop_png) else None
    for liv in range(1, 5):
        dst = os.path.join(SRC, f"auto_sx{liv}.png")
        if os.path.exists(dst) and not SOVRASCRIVI:
            print(f"tengo {dst} (gia' disegnata)")
            continue
        im, im_stop = genera(dritta, stop, liv)
        im.save(dst)
        print(f"scritto {dst}")
        if im_stop is not None:
            im_stop.save(os.path.join(SRC, f"auto_stop_sx{liv}.png"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
