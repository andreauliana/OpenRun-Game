#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# G1 PERC1: genera freccia_cresta_dx/sx (stessa misura/stile di freccia_curva, segno del dosso).
# Scrive PNG in Libreria/assets_originali/scenari/comuni/oggetti/ e raw SGLR in motore_nuovo/dati/sprite_raw/scenari/comuni/.
# Lanciare da sglavinated_src: python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_freccia_cresta.py"
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OBJ = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "scenari" / "comuni" / "oggetti"
RAW = ROOT / "motore_nuovo" / "dati" / "sprite_raw" / "scenari" / "comuni"
MAGIC = b"SGLR"


def leggi_png(path):
    data = Path(path).read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: non PNG")
    pos = 8
    idat = bytearray()
    ihdr = None
    while pos < len(data):
        (length,) = struct.unpack_from(">I", data, pos)
        pos += 4
        ctype = data[pos : pos + 4]
        pos += 4
        cdata = data[pos : pos + length]
        pos += length
        pos += 4
        if ctype == b"IHDR":
            ihdr = cdata
        elif ctype == b"IDAT":
            idat += cdata
        elif ctype == b"IEND":
            break
    w, h, bd, ct, comp, filt, inter = struct.unpack(">IIBBBBB", ihdr)
    if bd != 8 or inter != 0 or ct not in (2, 6):
        raise ValueError(f"{path}: formato non supportato")
    raw = zlib.decompress(bytes(idat))
    bpp = 4 if ct == 6 else 3
    stride = w * bpp
    out = bytearray(w * h * 4)
    prev = bytearray(stride)
    i = 0
    for y in range(h):
        f = raw[i]
        i += 1
        line = bytearray(raw[i : i + stride])
        i += stride
        if f == 1:
            for x in range(bpp, stride):
                line[x] = (line[x] + line[x - bpp]) & 0xFF
        elif f == 2:
            for x in range(stride):
                line[x] = (line[x] + prev[x]) & 0xFF
        elif f == 3:
            for x in range(bpp, stride):
                line[x] = (line[x] + ((line[x - bpp] + prev[x]) >> 1)) & 0xFF
            for x in range(min(bpp, stride)):
                line[x] = (line[x] + (prev[x] >> 1)) & 0xFF
        elif f == 4:
            for x in range(stride):
                a = line[x - bpp] if x >= bpp else 0
                b = prev[x]
                c = prev[x - bpp] if x >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 0xFF
        elif f != 0:
            raise ValueError(f"filtro {f}")
        prev = line
        base = y * w * 4
        if ct == 6:
            out[base : base + stride] = line
        else:
            for x in range(w):
                o = base + 4 * x
                s = 3 * x
                out[o : o + 3] = line[s : s + 3]
                out[o + 3] = 255
    return w, h, bytes(out)


def scrivi_png(path, w, h, rgba):
    def chunk(ctype, data):
        return struct.pack(">I", len(data)) + ctype + data + struct.pack(">I", zlib.crc32(ctype + data) & 0xFFFFFFFF)

    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw.extend(rgba[y * w * 4 : (y + 1) * w * 4])
    ihdr = struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(bytes(raw), 9)) + chunk(b"IEND", b"")
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


def draw_rect(buf, w, h, x0, y0, x1, y1, c):
    for y in range(max(0, y0), min(h, y1)):
        for x in range(max(0, x0), min(w, x1)):
            set_px(buf, w, h, x, y, c)


def genera(w, h, destra):
    # Palette piccola, coerente con segnali: giallo avviso, bordo scuro, rosso freccia, dosso bianco/grigio.
    TR = (0, 0, 0, 0)
    B = (20, 20, 24, 255)
    Y = (232, 216, 72, 255)
    YD = (184, 160, 40, 255)  # dithering/bordo interno
    R = (216, 56, 48, 255)
    RD = (160, 40, 32, 255)
    W = (240, 240, 232, 255)
    G = (168, 168, 160, 255)
    buf = bytearray(w * h * 4)
    # Sfondo trasparente già ok.
    # Cartello romboidale/ottagono semplificato (pieno, giallo) come area segnale.
    cx, cy = w // 2, h // 2
    r = min(w, h) // 2 - 1
    for y in range(h):
        for x in range(w):
            # distanza Manhattan per forma a diamante, con bordo
            d = abs(x - cx) + abs(y - cy)
            if d <= r - 1:
                # piccolo dithering Bayer 2x2 per non usare tinta piatta pura ovunque? manteniamo semplice
                col = Y if ((x + y) % 2 == 0) else YD
                set_px(buf, w, h, x, y, col)
            elif d == r:
                set_px(buf, w, h, x, y, B)
    # Freccia orizzontale
    ay = cy - 1
    if destra:
        # asta
        draw_rect(buf, w, h, cx - 5, ay, cx + 2, ay + 3, R)
        # punta
        for dy, span in enumerate([1, 2, 3, 2, 1], start=-2):
            yy = ay + 1 + dy
            xx = cx + 2 + (2 - abs(dy))
            if 0 <= yy < h and 0 <= xx < w:
                set_px(buf, w, h, xx, yy, R)
                if xx - 1 >= cx + 2:
                    set_px(buf, w, h, xx - 1, yy, RD)
        set_px(buf, w, h, cx + 4, ay + 1, R)
        set_px(buf, w, h, cx + 3, ay, R)
        set_px(buf, w, h, cx + 3, ay + 2, R)
    else:
        draw_rect(buf, w, h, cx - 1, ay, cx + 6, ay + 3, R)
        for dy, span in enumerate([1, 2, 3, 2, 1], start=-2):
            yy = ay + 1 + dy
            xx = cx - 1 - (2 - abs(dy))
            if 0 <= yy < h and 0 <= xx < w:
                set_px(buf, w, h, xx, yy, R)
                if xx + 1 <= cx - 1:
                    set_px(buf, w, h, xx + 1, yy, RD)
        set_px(buf, w, h, cx - 3, ay + 1, R)
        set_px(buf, w, h, cx - 2, ay, R)
        set_px(buf, w, h, cx - 2, ay + 2, R)
    # Segno del dosso: piccola gobba sotto la freccia
    by = ay + 5
    bump = [(0, 2), (1, 1), (2, 0), (3, 0), (4, 1), (5, 2)]
    bx = cx - 2
    for dx, dy in bump:
        set_px(buf, w, h, bx + dx, by + dy, W)
        set_px(buf, w, h, bx + dx, by + dy + 1, G)
    # base dosso
    draw_rect(buf, w, h, bx - 1, by + 3, bx + 7, by + 4, B)
    return bytes(buf)


def main():
    ref = OBJ / "freccia_curva_dx.png"
    if not ref.exists():
        raise SystemExit(f"riferimento mancante: {ref}")
    w, h, _ = leggi_png(ref)
    print(f"riferimento freccia_curva: {w}x{h}")
    for name, destra in [("freccia_cresta_dx", True), ("freccia_cresta_sx", False)]:
        rgba = genera(w, h, destra)
        png = OBJ / f"{name}.png"
        rawp = RAW / f"{name}.sglraw"
        scrivi_png(png, w, h, rgba)
        scrivi_raw(rawp, w, h, rgba)
        print(f"OK {png} ({w}x{h})")
        print(f"OK {rawp}")


if __name__ == "__main__":
    main()
