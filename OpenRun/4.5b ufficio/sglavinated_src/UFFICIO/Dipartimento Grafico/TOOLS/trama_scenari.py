#!/usr/bin/env python3
# Trama sugli skyline degli scenari (Andrea 29/10: le parti piatte sono troppo
# "flat"; pochi colori + dithering, cosi' il gioco resta portabile).
#
# Regole (dall'articolo di Divoom che ha mandato Andrea): trama solo sulle zone
# grandi (cielo, facciate, sfondi), mai sui dettagli piu' stretti di ~10 px, al
# massimo una o due zone per immagine; i bordi delle zone restano pieni.
# Ogni zona scelta prende una luce verticale (piu' scura in alto, piu' chiara in
# basso verso l'orizzonte) resa con 3 gradini dello stesso colore: scuro, base,
# chiaro. Tecniche: bayer (ordinato 4x4), atkinson (diffusione dell'errore),
# halftone (punti raggruppati 4x4).
#
# Il PNG originale NON viene toccato: si scrive skyline_trama.png accanto, e da
# quello il .sglraw che usa il gioco. Per tornare all'originale: --originale.
# Tecnica, forza e numero di zone per livello: Libreria/assets_originali/scenari/trama.json.
#
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py"            tutti i livelli
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py" lv_04_cromo
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py" --originale lv_04_cromo
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py" --anteprima   solo i PNG, raw non toccati
import json
import os
import struct
import sys
from PIL import Image

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.abspath(os.path.join(BASE, "..", "..", ".."))
ASSETS = os.path.join(SRC, "UFFICIO", "Dipartimento Grafico", "Libreria", "assets_originali")
RAW = os.path.join(SRC, "motore_nuovo", "dati", "sprite_raw", "scenari")
CONFIG = os.path.join(ASSETS, "scenari", "trama.json")

# Skyline usato in gioco da ogni livello (come in completa_deco1_lv123.py,
# completa_deco1_lv04_skyline.py e genera_scenari_lv456.py).
SKYLINE = {
    "lv_01_neon": "scenari/lv_01_neon/scenario_neon_01_varianteA/skyline.png",
    "lv_02_atomico": "scenari/lv_02_atomico/scenario_atomico_02_varianteA/skyline.png",
    "lv_03_foglie": "scenari/lv_03_foglie/scenario_foglie_03_varianteA/skyline.png",
    "lv_04_cromo": "skyline/varianteA_futurista/skyline.png",
    "lv_05_spray": "scenari/lv_05_spray/scenario_spray_05_varianteA/skyline.png",
    "lv_06_desktop": "scenari/lv_06_desktop/scenario_desktop_06_varianteA/skyline.png",
}
PREDEFINITO = {"tecnica": "bayer", "forza": 0.5, "zone": 2}

ZONA_MIN_PX = 120   # zone piu' piccole restano piene
ZONA_MIN_LATO = 10  # e anche quelle piu' strette di 10 px
SCURO, CHIARO = 0.82, 0.18  # gradino scuro (moltiplica) e chiaro (verso il bianco)

BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]
HALFTONE = [[12, 5, 6, 13], [4, 0, 1, 7], [11, 3, 2, 8], [15, 10, 9, 14]]  # punti che crescono dal centro


def gradino(c, g):
    r, gr, b, a = c
    if g == 0:
        return (int(r * SCURO), int(gr * SCURO), int(b * SCURO), a)
    if g == 2:
        if r + gr + b < 300:  # colore scuro: si schiarisce tenendo la tinta (col bianco diventerebbe grigio)
            return (min(255, int(r * 1.5) + 6), min(255, int(gr * 1.5) + 6), min(255, int(b * 1.5) + 6), a)
        return (r + int((255 - r) * CHIARO), gr + int((255 - gr) * CHIARO), b + int((255 - b) * CHIARO), a)
    return c


def zone(img):
    """Zone collegate (4 vicini) dello stesso colore opaco: lista di (colore, pixel)."""
    w, h = img.size
    px = img.load()
    vista = [[False] * w for _ in range(h)]
    out = []
    for y0 in range(h):
        for x0 in range(w):
            if vista[y0][x0] or px[x0, y0][3] < 255:
                continue
            c = px[x0, y0]
            pila, pixel = [(x0, y0)], []
            vista[y0][x0] = True
            while pila:
                x, y = pila.pop()
                pixel.append((x, y))
                for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
                    if 0 <= nx < w and 0 <= ny < h and not vista[ny][nx] and px[nx, ny] == c:
                        vista[ny][nx] = True
                        pila.append((nx, ny))
            out.append((c, pixel))
    return out


def scegli(tutte, quante):
    """Zone grandi dei colori che coprono piu' area, al massimo `quante` colori."""
    grandi = []
    for c, pixel in tutte:
        xs = [p[0] for p in pixel]
        ys = [p[1] for p in pixel]
        if len(pixel) >= ZONA_MIN_PX and max(xs) - min(xs) + 1 >= ZONA_MIN_LATO and max(ys) - min(ys) + 1 >= ZONA_MIN_LATO:
            grandi.append((c, pixel))
    area = {}
    for c, pixel in grandi:
        area[c] = area.get(c, 0) + len(pixel)
    colori = sorted(area, key=area.get, reverse=True)[:quante]
    return [z for z in grandi if z[0] in colori]


def trama(img, tecnica, forza, quante):
    out = img.copy()
    src, dst = img.load(), out.load()
    w, h = img.size
    for c, pixel in scegli(zone(img), quante):
        insieme = set(pixel)
        ys = [p[1] for p in pixel]
        top, fondo = min(ys), max(ys)
        errore = {}
        for x, y in sorted(pixel, key=lambda p: (p[1], p[0])):
            # Il bordo della zona resta pieno: forme nette, trama solo dentro.
            if any((nx, ny) not in insieme for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1))):
                continue
            t = (y - top) / max(1, fondo - top)
            v = 1.0 + forza * (2.0 * t - 1.0) + errore.pop((x, y), 0.0)  # 0 scuro .. 1 base .. 2 chiaro
            if tecnica == "atkinson":
                g = max(0, min(2, round(v)))
                e = (v - g) / 8.0
                for dx, dy in ((1, 0), (2, 0), (-1, 1), (0, 1), (1, 1), (0, 2)):
                    q = (x + dx, y + dy)
                    if q in insieme:
                        errore[q] = errore.get(q, 0.0) + e
            else:
                m = HALFTONE if tecnica == "halftone" else BAYER
                s = (m[y & 3][x & 3] + 0.5) / 16.0
                g = max(0, min(2, int(v + s)))  # v intero = quel gradino pieno; la parte frazionaria = densita' del gradino sopra
            dst[x, y] = gradino(src[x, y], g)
    return out


def scrivi_raw(img, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    w, h = img.size
    with open(path, "wb") as f:
        f.write(b"SGLR")
        f.write(struct.pack("<II", w, h))
        f.write(img.convert("RGBA").tobytes())


def main(argv):
    originale = "--originale" in argv
    anteprima = "--anteprima" in argv
    livelli = [a for a in argv if not a.startswith("--")] or list(SKYLINE)
    config = json.load(open(CONFIG)) if os.path.exists(CONFIG) else {}
    for lv in livelli:
        if lv not in SKYLINE:
            print(f"  [??] livello sconosciuto: {lv}")
            continue
        png = os.path.join(ASSETS, SKYLINE[lv])
        img = Image.open(png).convert("RGBA")
        raw = os.path.join(RAW, lv, "skyline.sglraw")
        if originale:
            scrivi_raw(img, raw)
            print(f"  [OK] {lv}: raw dall'originale {SKYLINE[lv]}")
            continue
        opz = {**PREDEFINITO, **config.get("predefinito", {}), **config.get("livelli", {}).get(lv, {})}
        if opz["tecnica"] == "nessuna":
            print(f"  [--] {lv}: nessuna trama (trama.json)")
            continue
        fatto = trama(img, opz["tecnica"], float(opz["forza"]), int(opz["zone"]))
        uscita = png[:-4] + "_trama.png"
        fatto.save(uscita)
        if not anteprima:
            scrivi_raw(fatto, raw)
        print(f"  [OK] {lv}: {opz['tecnica']} forza {opz['forza']} zone {opz['zone']} -> {os.path.relpath(uscita, ASSETS)}"
              + ("" if anteprima else " e skyline.sglraw"))


if __name__ == "__main__":
    main(sys.argv[1:])
