"""Converte screenshot PPM (P6, binario) in PNG per la revisione del @grafico.

Uso (da radice sglavinated_src, percorsi relativi):
    python3 "UFFICIO/Dipartimento Grafico/TOOLS/ppm_a_png.py" OUTDIR FILE [FILE...]

Input : file .ppm generati dai gate (es. UFFICIO/Officina/Test area/auto1/*.ppm)
Output : OUTDIR/<nome>.png (cartella creata se manca; nessuna sovrascritture
         di sorgenti: i PPM restano intatti)
Solo standard library: nessuna dipendenza esterna.
"""
import os
import struct
import sys
import zlib


def leggi_ppm(percorso):
    with open(percorso, "rb") as f:
        dati = f.read()
    # Header P6: magic, eventuali commenti '#...', larghezza, altezza, maxval
    token = []
    i = 0
    while len(token) < 4:
        while i < len(dati) and dati[i : i + 1].isspace():
            i += 1
        if i < len(dati) and dati[i : i + 1] == b"#":
            while i < len(dati) and dati[i] not in (10, 13):
                i += 1
            continue
        inizio = i
        while i < len(dati) and not dati[i : i + 1].isspace():
            i += 1
        token.append(dati[inizio:i])
    if token[0] != b"P6":
        raise ValueError("%s: atteso P6, trovato %r" % (percorso, token[0]))
    w, h, maxval = int(token[1]), int(token[2]), int(token[3])
    if maxval != 255:
        raise ValueError("%s: maxval %d non supportato" % (percorso, maxval))
    i += 1  # singolo spazio dopo maxval
    pixel = dati[i : i + w * h * 3]
    if len(pixel) != w * h * 3:
        raise ValueError("%s: pixel incompleti (%d)" % (percorso, len(pixel)))
    return w, h, pixel


def scrivi_png(percorso, w, h, rgb):
    def chunk(tipo, corpo):
        c = struct.pack(">I", len(corpo)) + tipo + corpo
        return c + struct.pack(">I", zlib.crc32(tipo + corpo) & 0xFFFFFFFF)

    righe = bytearray()
    stride = w * 3
    for y in range(h):
        righe.append(0)  # filtro None
        righe.extend(rgb[y * stride : (y + 1) * stride])
    ihdr = struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)  # 8-bit, RGB
    png = (
        b"\x89PNG\r\n\x1a\n"
        + chunk(b"IHDR", ihdr)
        + chunk(b"IDAT", zlib.compress(bytes(righe), 9))
        + chunk(b"IEND", b"")
    )
    with open(percorso, "wb") as f:
        f.write(png)


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    outdir = argv[1]
    os.makedirs(outdir, exist_ok=True)
    scritti = 0
    for percorso in argv[2:]:
        w, h, rgb = leggi_ppm(percorso)
        nome = os.path.splitext(os.path.basename(percorso))[0] + ".png"
        dest = os.path.join(outdir, nome)
        scrivi_png(dest, w, h, rgb)
        print("OK %s -> %s (%dx%d)" % (percorso, dest, w, h))
        scritti += 1
    print("convertiti: %d" % scritti)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
