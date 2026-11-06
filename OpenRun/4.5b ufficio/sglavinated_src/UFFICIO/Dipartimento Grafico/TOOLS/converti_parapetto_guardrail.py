#!/usr/bin/env python3
"""Converte PNG in formato SGLR raw per il motore (parapetto e guardrail)."""
import struct
import zlib
from pathlib import Path

PROJECT = Path(__file__).resolve().parent.parent.parent
ASSETS = PROJECT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali"

PNG_FILES = [
    ASSETS / "scenari" / "comuni" / "oggetti" / "parapetto.png",
    ASSETS / "scenari" / "comuni" / "oggetti" / "guardrail.png",
]

OUTDIR = PROJECT / "motore_nuovo" / "dati" / "sprite_raw" / "scenari" / "comuni"
OUTDIR.mkdir(parents=True, exist_ok=True)

MAGIC = b"SGLR"

def leggi_png(path):
    data = Path(path).read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: non e' un PNG")
    pos = 8
    idat = bytearray()
    ihdr = None
    while pos < len(data):
        (length,) = struct.unpack_from(">I", data, pos)
        pos += 4
        ctype = data[pos:pos + 4]
        pos += 4
        cdata = data[pos:pos + length]
        pos += length
        pos += 4
        if ctype == b"IHDR":
            ihdr = cdata
        elif ctype == b"IDAT":
            idat += cdata
        elif ctype == b"IEND":
            break
    if ihdr is None:
        raise ValueError(f"{path}: nessun chunk IHDR")
    w, h, bd, ct, comp, filt, inter = struct.unpack(">IIBBBBB", ihdr)
    if bd != 8 or inter != 0 or ct not in (2, 6):
        raise ValueError(f"{path}: servono PNG 8-bit RGB/RGBA senza interlaccio")
    raw = zlib.decompress(bytes(idat))
    bpp = 4 if ct == 6 else 3
    stride = w * bpp
    out = bytearray(w * h * 4)
    prev = bytearray(stride)
    i = 0
    for y in range(h):
        f = raw[i]; i += 1
        line = bytearray(raw[i:i + stride]); i += stride
        if f == 1:
            for x in range(bpp, stride):
                line[x] = (line[x] + line[x - bpp]) & 0xFF
        elif f == 2:
            for x in range(stride):
                line[x] = (line[x] + prev[x]) & 0xFF
        elif f == 3:
            for x in range(stride):
                a = line[x - bpp] if x >= bpp else 0
                line[x] = (line[x] + ((a + prev[x]) >> 1)) & 0xFF
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
            raise ValueError(f"{path}: filtro {f} non supportato")
        prev = line
        base = y * w * 4
        if ct == 6:
            out[base:base + stride] = line
        else:
            for x in range(w):
                out[base + 4 * x] = line[3 * x]
                out[base + 4 * x + 1] = line[3 * x + 1]
                out[base + 4 * x + 2] = line[3 * x + 2]
                out[base + 4 * x + 3] = 255
    return w, h, bytes(out)

for png_path in PNG_FILES:
    if not png_path.exists():
        print(f"salta (manca): {png_path}")
        continue
    w, h, rgba = leggi_png(str(png_path))
    raw_path = OUTDIR / png_path.name.replace(".png", ".sglraw")
    with open(raw_path, "wb") as f:
        f.write(MAGIC)
        f.write(struct.pack("<II", w, h))
        f.write(rgba)
    print(f"scritto {raw_path} ({w}x{h})")