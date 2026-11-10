#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# G2 PERC1: genera parete_galleria (32x32, tile ripetibile, palette piccola + Bayer 4x4)
# e luce_galleria (16x8). Scrive PNG in Libreria/assets_originali/scenari/comuni/oggetti/
# e raw SGLR in motore_nuovo/dati/sprite_raw/scenari/comuni/.
# Lanciare da sglavinated_src: python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_galleria.py"
# Non sovrascrive i PNG che esistono gia' (Andrea li ridisegna, 10/11), salvo --forza.
# Dopo un PNG ridisegnato basta: python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py"
import struct
import sys
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OBJ = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "scenari" / "comuni" / "oggetti"
RAW = ROOT / "motore_nuovo" / "dati" / "sprite_raw" / "scenari" / "comuni"
MAGIC = b"SGLR"

# Bayer 4x4 ordinato (stesso ordine usato in trama_scenari.py)
BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]


def scrivi_png(path, w, h, rgba):
    def chunk(ctype, data):
        return (
            struct.pack(">I", len(data))
            + ctype
            + data
            + struct.pack(">I", zlib.crc32(ctype + data) & 0xFFFFFFFF)
        )

    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw.extend(rgba[y * w * 4 : (y + 1) * w * 4])
    ihdr = struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)
    data = (
        b"\x89PNG\r\n\x1a\n"
        + chunk(b"IHDR", ihdr)
        + chunk(b"IDAT", zlib.compress(bytes(raw), 9))
        + chunk(b"IEND", b"")
    )
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_bytes(data)


def scrivi_raw(path, w, h, rgba):
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    with open(path, "wb") as f:
        f.write(MAGIC)
        f.write(struct.pack("<II", w, h))
        f.write(rgba)


def set_px(buf, w, h, x, y, c):
    if 0 <= x < w and 0 <= y < h:
        i = (y * w + x) * 4
        buf[i : i + 4] = bytes(c)


def lerp(a, b, t):
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(4))


def dither2(c0, c1, x, y):
    # Sceglie fra due colori vicini secondo Bayer 4x4 per simulare una sfumatura.
    return c0 if BAYER[y & 3][x & 3] < 8 else c1


def genera_parete():
    # Parete galleria 32x32 tileable: pannelli/scanalature verticali scure, palette piccola.
    w, h = 32, 32
    buf = bytearray(w * h * 4)
    # Palette: grigi cemento freddi + ombra profonda (nessun colore saturo).
    D = (28, 30, 36, 255)    # scuro/giunto
    M0 = (56, 60, 68, 255)   # medio scuro
    M1 = (72, 76, 86, 255)   # medio chiaro
    L = (92, 96, 108, 255)   # luce
    for y in range(h):
        for x in range(w):
            # Giunti orizzontali sottili ogni 16 px (tileable) e verticali a pannelli.
            giunto_h = (y == 0) or (y == 15)
            giunto_v = (x % 8 == 0)
            # Gradiente verticale leggero (piu' chiaro in alto nel pannello).
            in_panel_y = y % 16
            t = in_panel_y / 15.0
            # base fra medio chiaro (alto) e medio scuro (basso), con Bayer
            if t < 0.5:
                base = dither2(M1, M0, x, y) if t > 0.25 else M1
            else:
                base = dither2(M0, D, x, y) if t > 0.75 else M0
            # colonna di luce debole al centro di ogni pannello 8px
            in_panel_x = x % 8
            if in_panel_x in (3, 4) and not giunto_h:
                base = dither2(base, L, x, y)
            if giunto_h or giunto_v:
                # giunto scuro con piccolo dithering ai bordi
                if giunto_h and giunto_v:
                    c = D
                elif giunto_h:
                    c = D if BAYER[y & 3][x & 3] < 11 else M0
                else:
                    c = D if BAYER[y & 3][x & 3] < 11 else base
            else:
                c = base
            set_px(buf, w, h, x, y, c)
    return w, h, bytes(buf)


def genera_luce():
    # Luce galleria 16x8: plafoniera semplice, alone chiaro su supporto scuro.
    w, h = 16, 8
    TR = (0, 0, 0, 0)
    buf = bytearray(w * h * 4)
    # sfondo trasparente
    for y in range(h):
        for x in range(w):
            set_px(buf, w, h, x, y, TR)
    # Palette piccola
    S = (36, 38, 44, 255)    # supporto
    C = (72, 76, 84, 255)    # corpo
    G = (188, 192, 176, 255) # luce spenta/calda chiara
    W = (240, 244, 232, 255) # centro luminoso
    # supporto/staffa in alto
    for x in range(6, 10):
        set_px(buf, w, h, x, 0, S)
    # corpo lampada
    for y in range(1, 6):
        for x in range(2, 14):
            edge = (x == 2 or x == 13 or y == 1 or y == 5)
            if edge:
                set_px(buf, w, h, x, y, C)
            else:
                # interno: centro piu' luminoso, bordo piu' caldo, con Bayer
                dx = abs(x - 7.5) / 5.5
                if dx < 0.35:
                    c = W if BAYER[y & 3][x & 3] < 12 else G
                elif dx < 0.7:
                    c = dither2(G, W, x, y)
                else:
                    c = dither2(C, G, x, y)
                set_px(buf, w, h, x, y, c)
    # agganci laterali
    set_px(buf, w, h, 2, 1, S)
    set_px(buf, w, h, 13, 1, S)
    return w, h, bytes(buf)


def main():
    forza = "--forza" in sys.argv
    for name, gen in [("parete_galleria", genera_parete), ("luce_galleria", genera_luce)]:
        w, h, rgba = gen()
        png = OBJ / f"{name}.png"
        rawp = RAW / f"{name}.sglraw"
        if png.exists() and not forza:
            print(f"tengo {png} (esiste gia'; --forza per rigenerarlo)")
            continue
        scrivi_png(png, w, h, rgba)
        scrivi_raw(rawp, w, h, rgba)
        print(f"OK {png} ({w}x{h})")
        print(f"OK {rawp}")


if __name__ == "__main__":
    main()
