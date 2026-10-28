#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Genera skyline + oggetti bordo strada per lv_04_cromo, lv_05_spray, lv_06_desktop (variante A).
# Disegnato da zero con Pillow, nessun materiale Sega, palette max 16 colori per oggetto.
# Lanciare da sglavinated_src/: python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_scenari_lv456.py"
from PIL import Image, ImageDraw
import json, struct, zlib
from pathlib import Path

SRC = Path(__file__).resolve().parents[3]  # sglavinated_src, qualunque sia la cartella da cui si lancia
ROOT = SRC / "UFFICIO/Dipartimento Grafico/Libreria/assets_originali"
SCENARI = ROOT / "scenari"
CATALOGO = ROOT / "CATALOGO_GRAFICO.json"
RAW_ROOT = SRC / "motore_nuovo/dati/sprite_raw"

def save_png(path, img):
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path, "PNG")

def write_sglraw(png_path, out_path):
    data = Path(png_path).read_bytes()
    assert data[:8] == b"\x89PNG\r\n\x1a\n", f"{png_path}: non PNG"
    pos = 8
    idat = bytearray(); ihdr = None
    while pos < len(data):
        (length,) = struct.unpack_from(">I", data, pos); pos += 4
        ctype = data[pos:pos+4]; pos += 4
        cdata = data[pos:pos+length]; pos += length; pos += 4
        if ctype == b"IHDR": ihdr = cdata
        elif ctype == b"IDAT": idat += cdata
        elif ctype == b"IEND": break
    w, h, bd, ct, comp, filt, inter = struct.unpack(">IIBBBBB", ihdr)
    raw = zlib.decompress(bytes(idat))
    bpp = 4 if ct == 6 else 3
    stride = w * bpp
    out = bytearray(w * h * 4)
    prev = bytearray(stride); i = 0
    for y in range(h):
        f = raw[i]; i += 1
        line = bytearray(raw[i:i+stride]); i += stride
        if f == 1:
            for x in range(bpp, stride): line[x] = (line[x] + line[x-bpp]) & 0xFF
        elif f == 2:
            for x in range(stride): line[x] = (line[x] + prev[x]) & 0xFF
        elif f == 3:
            for x in range(stride):
                a = line[x-bpp] if x >= bpp else 0
                line[x] = (line[x] + ((a + prev[x]) >> 1)) & 0xFF
        elif f == 4:
            for x in range(stride):
                a = line[x-bpp] if x >= bpp else 0
                b = prev[x]; c = prev[x-bpp] if x >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p-a), abs(p-b), abs(p-c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 0xFF
        prev = line
        base = y * w * 4
        if ct == 6:
            out[base:base+stride] = line
        else:
            for x in range(w):
                out[base+4*x] = line[3*x]; out[base+4*x+1] = line[3*x+1]
                out[base+4*x+2] = line[3*x+2]; out[base+4*x+3] = 255
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "wb") as f:
        f.write(b"SGLR"); f.write(struct.pack("<II", w, h)); f.write(bytes(out))
    return w, h

def disegna_skyline(path, colori, elementi):
    """Disegna skyline 256x128 con colori della palette (indici RGB diretti)."""
    img = Image.new("RGBA", (256, 128), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    for el in elementi:
        tipo = el[0]
        if tipo == "fondo":
            d.rectangle([0, 0, 255, 127], fill=colori[el[1]])
        elif tipo == "orizzonte":
            d.rectangle([0, 70, 255, 127], fill=colori[el[1]])
        elif tipo == "palazzo":
            x, y, w, h = el[1], el[2], el[3], el[4]
            d.rectangle([x, 70-h, x+w, 127], fill=colori[el[5]])
        elif tipo == "stella":
            cx, cy = el[1], el[2]
            d.ellipse([cx-2, cy-2, cx+2, cy+2], fill=colori[el[3]])
        elif tipo == "luna":
            cx, cy, r = el[1], el[2], el[3]
            d.ellipse([cx-r, cy-r, cx+r, cy+r], fill=colori[el[4]])
    save_png(path, img)
    return img

def disegna_oggetto(path, colori, disegna_fn):
    img = Image.new("RGBA", (48, 32), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    disegna_fn(d, colori)
    save_png(path, img)
    return img

# ---------- Palette ----------
P_CROMO = [(11,16,48),(74,85,96),(200,200,210),(0,216,255),(255,255,255),(120,120,120),(60,60,80),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0)]
P_SPRAY = [(90,90,102),(138,127,106),(224,16,32),(255,220,0),(190,190,190),(0,0,0),(255,255,255),(120,120,120),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0)]
P_DESKTOP = [(58,142,230),(52,168,52),(192,192,192),(255,255,255),(130,130,130),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0),(0,0,0)]

# ---------- lv_04_cromo: 4 oggetti (skyline esiste gia': skyline.futurista) ----------
objs_cromo = [
    ("guard_rail_cromo", lambda d,c: [
        d.rectangle([4, 14, 44, 18], fill=c[2]),
        d.line([4, 16, 44, 16], fill=c[3], width=2),
        d.rectangle([10, 6, 14, 14], fill=c[4]),
        d.rectangle([26, 6, 30, 14], fill=c[4]),
        d.rectangle([38, 6, 42, 14], fill=c[4]),
    ]),
    ("lampione_lama", lambda d,c: [
        d.rectangle([22, 4, 26, 30], fill=c[6]),
        d.rectangle([16, 6, 30, 10], fill=c[3]),
        d.line([24, 10, 24, 6], fill=c[4], width=2),
        d.ellipse([22, 4, 26, 8], fill=c[4]),
    ]),
    ("robot_servizio", lambda d,c: [
        d.rectangle([16, 10, 32, 24], fill=c[2]),
        d.rectangle([18, 6, 30, 10], fill=c[4]),
        d.rectangle([14, 24, 18, 30], fill=c[6]),
        d.rectangle([30, 24, 34, 30], fill=c[6]),
        d.ellipse([20, 10, 24, 14], fill=c[4]),
        d.ellipse([24, 10, 28, 14], fill=c[4]),
    ]),
    ("pilone_cromo", lambda d,c: [
        d.rectangle([22, 6, 26, 30], fill=c[2]),
        d.rectangle([16, 28, 32, 32], fill=c[3]),
        d.line([24, 6, 24, 12], fill=c[4], width=2),
        d.rectangle([18, 12, 30, 14], fill=c[4]),
    ]),
]

# ---------- lv_05_spray: skyline + 4 oggetti ----------
def skyline_spray(d, c):
    d.rectangle([0, 0, 255, 70], fill=c[0])
    d.rectangle([0, 70, 255, 127], fill=c[4])
    # capannoni
    d.rectangle([10, 40, 60, 70], fill=c[4])
    d.rectangle([70, 30, 110, 70], fill=c[4])
    d.rectangle([120, 45, 170, 70], fill=c[4])
    d.rectangle([180, 35, 230, 70], fill=c[4])
    d.rectangle([240, 50, 255, 70], fill=c[4])
    # ciminiere
    d.rectangle([45, 10, 55, 40], fill=c[4])
    d.rectangle([150, 5, 160, 45], fill=c[4])
    d.rectangle([210, 15, 220, 35], fill=c[4])
    # fumetti spray (scritte)
    d.rectangle([80, 50, 100, 62], fill=c[2])
    d.rectangle([190, 48, 210, 58], fill=c[3])

def objs_spray():
    return [
        ("bidone", lambda d,c: [
            d.rectangle([16, 10, 32, 28], fill=c[4]),
            d.rectangle([18, 8, 30, 12], fill=c[2]),
            d.rectangle([20, 28, 28, 30], fill=c[6]),
        ]),
        ("transenna", lambda d,c: [
            d.rectangle([8, 20, 40, 24], fill=c[2]),
            d.line([8, 22, 40, 22], fill=c[4], width=1),
            d.rectangle([14, 14, 18, 20], fill=c[4]),
            d.rectangle([30, 14, 34, 20], fill=c[4]),
        ]),
        ("cassonetto", lambda d,c: [
            d.rectangle([12, 12, 36, 28], fill=c[4]),
            d.rectangle([14, 10, 34, 14], fill=c[2]),
            d.line([24, 14, 24, 28], fill=c[6], width=2),
            d.rectangle([16, 26, 20, 30], fill=c[6]),
            d.rectangle([28, 26, 32, 30], fill=c[6]),
        ]),
        ("serranda_spray", lambda d,c: [
            d.rectangle([8, 8, 40, 28], fill=c[4]),
            d.line([8, 8, 40, 8], fill=c[2], width=2),
            d.line([8, 14, 40, 14], fill=c[3], width=1),
            d.line([8, 20, 40, 20], fill=c[2], width=1),
            d.rectangle([14, 10, 18, 28], fill=c[2]),
        ]),
    ]

# ---------- lv_06_desktop: skyline + 4 oggetti ----------
def skyline_desktop(d, c):
    d.rectangle([0, 0, 255, 60], fill=c[0])
    d.rectangle([0, 60, 255, 127], fill=c[1])
    # nuvole
    d.ellipse([30, 20, 80, 45], fill=c[3])
    d.ellipse([60, 25, 110, 50], fill=c[3])
    d.ellipse([160, 15, 210, 40], fill=c[3])
    d.ellipse([190, 20, 240, 45], fill=c[3])
    # finestre bevel
    d.rectangle([90, 80, 120, 100], fill=c[2])
    d.rectangle([140, 82, 170, 102], fill=c[2])
    d.rectangle([10, 85, 40, 105], fill=c[2])
    # icone
    d.rectangle([190, 80, 220, 108], fill=c[4])

def objs_desktop():
    return [
        ("pali_cavi", lambda d,c: [
            d.line([24, 6, 24, 30], fill=c[4], width=2),
            d.line([32, 6, 32, 30], fill=c[4], width=2),
            d.line([24, 8, 32, 10], fill=c[6], width=1),
            d.line([24, 14, 32, 16], fill=c[6], width=1),
            d.rectangle([21, 28, 27, 30], fill=c[2]),
            d.rectangle([29, 28, 35, 30], fill=c[2]),
        ]),
        ("case_computer", lambda d,c: [
            d.rectangle([14, 10, 34, 26], fill=c[2]),
            d.rectangle([16, 12, 32, 24], fill=c[4]),
            d.line([24, 14, 24, 22], fill=c[0], width=1),
            d.line([18, 18, 30, 18], fill=c[0], width=1),
        ]),
        ("delfino_cartone", lambda d,c: [
            d.ellipse([12, 14, 36, 24], fill=c[4]),
            d.ellipse([28, 12, 40, 20], fill=c[4]),
            d.rectangle([30, 16, 38, 18], fill=c[3]),
        ]),
        ("icona_cartella", lambda d,c: [
            d.rectangle([12, 10, 36, 26], fill=c[2]),
            d.rectangle([14, 12, 34, 20], fill=c[4]),
            d.line([16, 16, 32, 16], fill=c[0], width=1),
            d.line([16, 20, 32, 20], fill=c[0], width=1),
        ]),
    ]

creati = []
raw_creati = []

def aggiungi(lv, nome, img, tipo, stati_catalogo, note=""):
    base = f"scenari/{lv}/scenario_{lv.split('_')[1]}_{lv.split('_')[2]}_varianteA"
    # percorso file oggetto dentro la cartella scenario
    cartella = SCENARI / lv / base / "oggetti"
    cartella.mkdir(parents=True, exist_ok=True)
    fpath = cartella / f"obj_{nome}.png"
    save_png(fpath, img)
    creati.append((str(fpath), tipo))
    w, h = write_sglraw(fpath, RAW_ROOT / f"scenari/{lv}/{nome}.sglraw")
    raw_creati.append((str(RAW_ROOT / f"scenari/{lv}/{nome}.sglraw"), w, h))

# lv_04_cromo: solo oggetti (skyline esiste: skyline.futurista)
cartella_cromo = SCENARI / "lv_04_cromo" / "scenario_cromo_04_varianteA" / "oggetti"
cartella_cromo.mkdir(parents=True, exist_ok=True)
for nome, fn in objs_cromo:
    img = Image.new("RGBA", (48, 32), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    fn(d, P_CROMO)
    fpath = cartella_cromo / f"obj_{nome}.png"
    save_png(fpath, img)
    creati.append((str(fpath), "oggetto_bordo_strada"))
    w, h = write_sglraw(fpath, RAW_ROOT / f"scenari/lv_04_cromo/{nome}.sglraw")
    raw_creati.append((str(RAW_ROOT / f"scenari/lv_04_cromo/{nome}.sglraw"), w, h))

# lv_05_spray: skyline + oggetti
base5 = "scenari/lv_05_spray/scenario_spray_05_varianteA"
cartella5 = SCENARI / "lv_05_spray" / "scenario_spray_05_varianteA"
img5 = disegna_skyline(cartella5 / "skyline.png", P_SPRAY, [
    ("fondo", 0), ("orizzonte", 4)
])
skyline_spray(ImageDraw.Draw(img5), P_SPRAY)
save_png(cartella5 / "skyline.png", img5)
creati.append((f"{base5}/skyline.png", "skyline"))
w, h = write_sglraw(cartella5 / "skyline.png", RAW_ROOT / "scenari/lv_05_spray/skyline.sglraw")
raw_creati.append((str(RAW_ROOT / "scenari/lv_05_spray/skyline.sglraw"), w, h))

# palette per lv_05
palette5 = [{"nome": f"c{i}", "rgb": list(P_SPRAY[i])} for i in range(16)]
(cartella5 / "palette.json").write_text(json.dumps(palette5, indent=2))
cartella5.joinpath("palette.png").write_bytes(Image.new("RGBA", (16, 16)).tobytes())
# palette.png 16 colori
pal_img = Image.new("RGBA", (64, 16))
pd = ImageDraw.Draw(pal_img)
for i, c in enumerate(P_SPRAY):
    pd.rectangle([i*4, 0, i*4+3, 15], fill=c)
pal_img.save(cartella5 / "palette.png")
creati.append((f"{base5}/palette.png", "palette"))
creati.append((f"{base5}/palette.json", "palette_dati"))

man5 = {
    "livello": "lv_05_spray", "variante": "A", "scenario": "scenario_spray_05",
    "stile": "punk / muri a spray", "autore": "@grafico (disegnato da zero, nessun materiale Sega)",
    "licenza": "CC0-1.0", "originale": True, "riferimenti_sega": False, "palette_max_colori": 16,
    "risoluzione_skyline": [256, 128],
    "oggetti_bordo_strada": ["bidone", "transenna", "cassonetto", "serranda_spray"],
    "file": {
        "skyline": "skyline.png", "palette": "palette.png", "palette_dati": "palette.json",
        "oggetti": [f"oggetti/obj_{n}.png" for n, _ in objs_spray()]
    },
    "note": "Variante A (D6). Disegnato da zero con Pillow, seme fisso, deterministico."
}
(cartella5 / "manifest.json").write_text(json.dumps(man5, indent=2))
creati.append((f"{base5}/manifest.json", "manifest"))

for nome, fn in objs_spray():
    img = Image.new("RGBA", (48, 32), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    fn(d, P_SPRAY)
    cartella_o = cartella5 / "oggetti"
    cartella_o.mkdir(parents=True, exist_ok=True)
    fpath = cartella_o / f"obj_{nome}.png"
    save_png(fpath, img)
    creati.append((f"{base5}/oggetti/obj_{nome}.png", "oggetto_bordo_strada"))
    w, h = write_sglraw(fpath, RAW_ROOT / f"scenari/lv_05_spray/{nome}.sglraw")
    raw_creati.append((str(RAW_ROOT / f"scenari/lv_05_spray/{nome}.sglraw"), w, h))

# lv_06_desktop: skyline + oggetti
base6 = "scenari/lv_06_desktop/scenario_desktop_06_varianteA"
cartella6 = SCENARI / "lv_06_desktop" / "scenario_desktop_06_varianteA"
img6 = disegna_skyline(cartella6 / "skyline.png", P_DESKTOP, [
    ("fondo", 0), ("orizzonte", 1)
])
skyline_desktop(ImageDraw.Draw(img6), P_DESKTOP)
save_png(cartella6 / "skyline.png", img6)
creati.append((f"{base6}/skyline.png", "skyline"))
w, h = write_sglraw(cartella6 / "skyline.png", RAW_ROOT / "scenari/lv_06_desktop/skyline.sglraw")
raw_creati.append((str(RAW_ROOT / "scenari/lv_06_desktop/skyline.sglraw"), w, h))

palette6 = [{"nome": f"c{i}", "rgb": list(P_DESKTOP[i])} for i in range(16)]
(cartella6 / "palette.json").write_text(json.dumps(palette6, indent=2))
pal_img6 = Image.new("RGBA", (64, 16))
pd6 = ImageDraw.Draw(pal_img6)
for i, c in enumerate(P_DESKTOP):
    pd6.rectangle([i*4, 0, i*4+3, 15], fill=c)
pal_img6.save(cartella6 / "palette.png")
creati.append((f"{base6}/palette.png", "palette"))
creati.append((f"{base6}/palette.json", "palette_dati"))

man6 = {
    "livello": "lv_06_desktop", "variante": "A", "scenario": "scenario_desktop_06",
    "stile": "desktop Windows 95 vibe", "autore": "@grafico (disegnato da zero, nessun materiale Sega)",
    "licenza": "CC0-1.0", "originale": True, "riferimenti_sega": False, "palette_max_colori": 16,
    "risoluzione_skyline": [256, 128],
    "oggetti_bordo_strada": ["pali_cavi", "case_computer", "delfino_cartone", "icona_cartella"],
    "file": {
        "skyline": "skyline.png", "palette": "palette.png", "palette_dati": "palette.json",
        "oggetti": [f"oggetti/obj_{n}.png" for n, _ in objs_desktop()]
    },
    "note": "Variante A (D6). Disegnato da zero con Pillow, seme fisso, deterministico."
}
(cartella6 / "manifest.json").write_text(json.dumps(man6, indent=2))
creati.append((f"{base6}/manifest.json", "manifest"))

for nome, fn in objs_desktop():
    img = Image.new("RGBA", (48, 32), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    fn(d, P_DESKTOP)
    cartella_o = cartella6 / "oggetti"
    cartella_o.mkdir(parents=True, exist_ok=True)
    fpath = cartella_o / f"obj_{nome}.png"
    save_png(fpath, img)
    creati.append((f"{base6}/oggetti/obj_{nome}.png", "oggetto_bordo_strada"))
    w, h = write_sglraw(fpath, RAW_ROOT / f"scenari/lv_06_desktop/{nome}.sglraw")
    raw_creati.append((str(RAW_ROOT / f"scenari/lv_06_desktop/{nome}.sglraw"), w, h))

print(f"PNG creati: {len(creati)}")
for p, t in creati:
    print(f"  {t}: {p}")
print(f"RAW creati: {len(raw_creati)}")
for p, w, h in raw_creati:
    print(f"  {p} ({w}x{h})")
