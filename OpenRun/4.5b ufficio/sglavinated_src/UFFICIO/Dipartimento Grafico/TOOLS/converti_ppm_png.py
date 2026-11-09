#!/usr/bin/env python3
"""Converte screenshot PPM (P6 320x224) in PNG per revisione @grafico.

Uso: python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_ppm_png.py"
Radice: pathlib.Path(__file__).resolve().parents[3] (sglavinated_src).
Input: screenshot PPM di junior in "UFFICIO/Officina/Test area/".
Output: PNG in "UFFICIO/Officina/Test area/grafico/g2_screenshot_check/".
Solo lettura degli input; nessuna sovrascrittura fuori dalla cartella output.
PNG scritto con stdlib (zlib + struct), nessuna dipendenza esterna.
"""
import struct
import zlib
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
AREA_TEST = RADICE / "UFFICIO" / "Officina" / "Test area"
DESTINAZIONE = AREA_TEST / "grafico" / "g2_screenshot_check"

COPPIE = [
    (AREA_TEST / "mig1_j2_pali" / "mig1_j2_pali" / "pali_f300.ppm", "g2_j2_pali_f300.png"),
    (AREA_TEST / "mig1_j2_pali" / "mig1_j2_pali" / "pali_f900.ppm", "g2_j2_pali_f900.png"),
    (AREA_TEST / "mig1_j2_pali" / "mig1_j2_pali" / "pali_f1700.ppm", "g2_j2_pali_f1700.png"),
    (AREA_TEST / "mig1_j3_sportellata" / "mig1_j3_sportellata" / "sportellata_colpo.ppm", "g2_j3_sportellata_colpo.png"),
    (AREA_TEST / "mig1_j3_sportellata" / "mig1_j3_sportellata" / "sportellata_dopo2s.ppm", "g2_j3_sportellata_dopo2s.png"),
    (AREA_TEST / "mig1_j3_sportellata" / "mig1_j3_sportellata" / "sportellata_dopo7s.ppm", "g2_j3_sportellata_dopo7s.png"),
]


def leggi_ppm(percorso: Path) -> tuple[int, int, bytes]:
    dati = percorso.read_bytes()
    # Tokenizza l'intestazione P6 saltando spazi e commenti '#'
    pos = 0
    token: list[bytes] = []
    while len(token) < 4:
        while pos < len(dati) and dati[pos : pos + 1].isspace():
            pos += 1
        if dati[pos : pos + 1] == b"#":
            while pos < len(dati) and dati[pos] not in (10, 13):
                pos += 1
            continue
        inizio = pos
        while pos < len(dati) and not dati[pos : pos + 1].isspace():
            pos += 1
        token.append(dati[inizio:pos])
    if token[0] != b"P6":
        raise ValueError(f"{percorso}: formato {token[0]!r}, atteso P6")
    larghezza, altezza, massimo = int(token[1]), int(token[2]), int(token[3])
    if massimo != 255:
        raise ValueError(f"{percorso}: maxval {massimo}, atteso 255")
    pos += 1  # unico whitespace dopo il maxval
    pixel = dati[pos : pos + larghezza * altezza * 3]
    if len(pixel) != larghezza * altezza * 3:
        raise ValueError(f"{percorso}: dati incompleti")
    return larghezza, altezza, pixel


def scrivi_png(percorso: Path, larghezza: int, altezza: int, pixel: bytes) -> None:
    righe = b"".join(
        b"\x00" + pixel[y * larghezza * 3 : (y + 1) * larghezza * 3]
        for y in range(altezza)
    )
    blocco = struct.pack(">IIBBBBB", larghezza, altezza, 8, 2, 0, 0, 0)

    def pezzo(tipo: bytes, contenuto: bytes) -> bytes:
        return struct.pack(">I", len(contenuto)) + tipo + contenuto + struct.pack(">I", zlib.crc32(tipo + contenuto) & 0xFFFFFFFF)

    png = b"\x89PNG\r\n\x1a\n"
    png += pezzo(b"IHDR", blocco)
    png += pezzo(b"IDAT", zlib.compress(righe, 9))
    png += pezzo(b"IEND", b"")
    percorso.write_bytes(png)


def main() -> None:
    DESTINAZIONE.mkdir(parents=True, exist_ok=True)
    for sorgente, nome in COPPIE:
        if not sorgente.is_file():
            print(f"MANCA {sorgente}")
            continue
        larghezza, altezza, pixel = leggi_ppm(sorgente)
        destinazione = DESTINAZIONE / nome
        scrivi_png(destinazione, larghezza, altezza, pixel)
        print(f"OK {sorgente.name} -> {destinazione.relative_to(RADICE)} {larghezza}x{altezza}")


if __name__ == "__main__":
    main()
