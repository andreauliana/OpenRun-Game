#!/usr/bin/env python3
"""
Generatore asset BLOCCO F — Outrun Sglavinated Edition v3 (open)
Produce 3 varianti per: (a) sagoma auto giocatore, (b) skyline notturno sci-fi, (c) palette livello 1.
Tutto originale, disegnato da zero con Pillow. Nessun riferimento a asset Sega.
Licenza: CC0 1.0 (dominio pubblico) per tutto il prodotto da questo script.
"""

import json
import os
from PIL import Image, ImageDraw, ImageFont

BASE = os.path.dirname(os.path.abspath(__file__))

# ─────────────────────────────────────────────────────────────────────────────
# PALETTE BASE (max 16 colori per sprite)
# ─────────────────────────────────────────────────────────────────────────────

PAL_FUTURISTA = {
    "nero":       (10, 10, 18),
    "blu_scuro":  (15, 20, 45),
    "ciano":      (0, 220, 255),
    "magenta":    (255, 0, 180),
    "giallo":     (255, 230, 0),
    "bianco":     (240, 240, 255),
    "grigio":     (80, 85, 100),
    "rosso":      (255, 40, 60),
    "arancio":    (255, 120, 0),
    "verde":      (0, 255, 120),
    "viola":      (140, 0, 255),
    "rosa":       (255, 100, 150),
    "marrone":    (60, 30, 15),
    "azzurro":    (80, 150, 255),
    "verde_scuro":(0, 80, 40),
    "giallo_scuro": (180, 140, 0),
}

PAL_PUNK = {
    "nero":       (12, 12, 12),
    "grigio":     (70, 70, 70),
    "bianco":     (230, 230, 230),
    "rosso":      (220, 30, 30),
    "giallo":     (240, 200, 0),
    "verde":      (30, 180, 60),
    "blu":        (30, 80, 200),
    "arancio":    (230, 100, 0),
    "rosa":       (220, 60, 120),
    "viola":      (120, 40, 180),
    "ciano":      (0, 180, 200),
    "marrone":    (50, 25, 10),
    "grigio_chiaro": (150, 150, 150),
    "verde_scuro": (0, 60, 30),
    "blu_scuro":  (10, 20, 60),
    "giallo_scuro": (160, 120, 0),
}

PAL_50 = {
    "nero":       (15, 10, 20),
    "bianco":     (245, 240, 230),
    "rosso":      (200, 30, 50),
    "azzurro":    (100, 160, 220),
    "giallo":     (240, 210, 60),
    "verde":      (60, 160, 80),
    "arancio":    (230, 120, 30),
    "rosa":       (230, 130, 160),
    "ciano":      (80, 200, 220),
    "grigio":     (120, 120, 130),
    "marrone":    (70, 40, 20),
    "viola":      (130, 60, 160),
    "blu_scuro":  (30, 50, 100),
    "verde_scuro": (30, 80, 40),
    "giallo_scuro": (180, 140, 30),
    "rosa_scuro": (160, 60, 90),
}


def salva(img, percorso):
    img.save(percorso)
    print(f"  [OK] {percorso}")


def rifinisci_palette(img, P):
    """Rimpiazza ogni pixel con il colore della palette piu' vicino, così lo
    sheet resta a esattamente 16 colori (nessun antialiasing sui bordi)."""
    colori = [tuple(v) + (255,) for v in P.values()]
    out = Image.new("RGBA", img.size, (0, 0, 0, 0))
    ip = img.load()
    op = out.load()
    w, h = img.size
    for y in range(h):
        for x in range(w):
            r, g, b, a = ip[x, y]
            if a < 200:
                continue
            best = min(colori, key=lambda cc: (cc[0]-r)**2 + (cc[1]-g)**2 + (cc[2]-b)**2)
            op[x, y] = best
    return out


def scrivi_palette_json(cartella, nome_stile, P):
    """Scheda palette engine-ready: esattamente 16 colori con nomi e RGB."""
    colori = [{"nome": k, "rgb": list(v)} for k, v in P.items()]
    data = {
        "autore": "@asset (progetto Sglavinated)",
        "licenza": "CC0-1.0",
        "descrizione": f"Palette livello 1 stile {nome_stile}: 16 colori pronti per il motore (render/palette). Originale, disegnata da zero.",
        "files": ["palette.png", "palette.json"],
        "originale": True,
        "riferimenti_sega": False,
        "n_colori": len(colori),
        "colori": colori,
    }
    path = os.path.join(cartella, "palette.json")
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)
    print(f"  [OK] {path}")


def manifest(cartella, autore, licenza, descrizione, files):
    m = {
        "autore": autore,
        "licenza": licenza,
        "descrizione": descrizione,
        "files": files,
        "originale": True,
        "riferimenti_sega": False,
    }
    path = os.path.join(cartella, "manifest.json")
    with open(path, "w", encoding="utf-8") as f:
        json.dump(m, f, ensure_ascii=False, indent=2)
    print(f"  [OK] {path}")


# ─────────────────────────────────────────────────────────────────────────────
# (a) SAGOMA AUTO DEL GIOCATORE — 3 varianti
# ─────────────────────────────────────────────────────────────────────────────

def disegna_auto_varianteA():
    """Stile anni '50: razzo compatto, pin-up, colori pastello."""
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_50

    # Corpo principale (forma razzo anni '50)
    d.ellipse([8, 10, 56, 26], fill=P["rosso"], outline=P["nero"], width=1)
    # Cabina
    d.ellipse([22, 6, 42, 18], fill=P["ciano"], outline=P["nero"], width=1)
    # Finestrini
    d.ellipse([26, 8, 38, 16], fill=P["bianco"], outline=P["nero"], width=1)
    # Ali posteriori (stile anni '50)
    d.polygon([(4, 18), (14, 14), (14, 22), (4, 24)], fill=P["rosso"], outline=P["nero"])
    d.polygon([(60, 18), (50, 14), (50, 22), (60, 24)], fill=P["rosso"], outline=P["nero"])
    # Ruote
    d.ellipse([12, 22, 22, 30], fill=P["nero"], outline=P["grigio"], width=1)
    d.ellipse([42, 22, 52, 30], fill=P["nero"], outline=P["grigio"], width=1)
    d.ellipse([15, 24, 19, 28], fill=P["grigio"])
    d.ellipse([45, 24, 49, 28], fill=P["grigio"])
    # Fari anteriori
    d.ellipse([50, 14, 54, 18], fill=P["giallo"], outline=P["nero"], width=1)
    # Scarico posteriore
    d.rectangle([2, 16, 6, 22], fill=P["grigio"], outline=P["nero"])
    # Stella decorativa
    d.polygon([(30, 12), (31, 14), (33, 14), (31, 15), (32, 17), (30, 16), (28, 17), (29, 15), (27, 14), (29, 14)], fill=P["bianco"])

    cartella = os.path.join(BASE, "sprite", "auto", "varianteA_anni50")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "auto.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Sagoma auto giocatore stile anni '50: razzo compatto con ali posteriori, colori pastello, pin-up. Originale, disegnata da zero.",
            ["auto.png"])


def disegna_auto_varianteB():
    """Stile futurista: linee pulite, cromature, megastrutture."""
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_FUTURISTA

    # Corpo principale (basso, aerodinamico)
    d.polygon([(4, 20), (8, 14), (20, 10), (44, 10), (56, 14), (60, 20), (58, 24), (6, 24)],
              fill=P["blu_scuro"], outline=P["ciano"], width=1)
    # Linea cromatura superiore
    d.line([(8, 14), (20, 10), (44, 10), (56, 14)], fill=P["ciano"], width=1)
    # Cabina (bolla)
    d.ellipse([24, 6, 40, 14], fill=P["nero"], outline=P["ciano"], width=1)
    d.ellipse([27, 7, 37, 13], fill=P["azzurro"])
    # Fari a LED
    d.rectangle([52, 16, 58, 18], fill=P["ciano"])
    d.rectangle([52, 20, 58, 22], fill=P["ciano"])
    # Ruote (large, sportive)
    d.ellipse([10, 22, 22, 30], fill=P["nero"], outline=P["grigio"], width=1)
    d.ellipse([42, 22, 54, 30], fill=P["nero"], outline=P["grigio"], width=1)
    d.ellipse([13, 24, 19, 28], fill=P["grigio"])
    d.ellipse([45, 24, 51, 28], fill=P["grigio"])
    # Sospensioni
    d.line([(16, 22), (16, 20)], fill=P["grigio"], width=1)
    d.line([(48, 22), (48, 20)], fill=P["grigio"], width=1)
    # Effetto neon sotto
    d.line([(8, 25), (56, 25)], fill=P["magenta"], width=1)

    cartella = os.path.join(BASE, "sprite", "auto", "varianteB_futurista")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "auto.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Sagoma auto giocatore stile futurista: bassa, aerodinamica, cabina bolla, fari LED, neon sotto. Originale, disegnata da zero.",
            ["auto.png"])


def disegna_auto_varianteC():
    """Stile punk: collage, scritte spray, colori sporchi."""
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_PUNK

    # Corpo principale (angoloso, aggressivo)
    d.polygon([(6, 22), (10, 12), (24, 8), (40, 8), (54, 12), (58, 22), (56, 26), (8, 26)],
              fill=P["grigio"], outline=P["nero"], width=1)
    # Graffita spray
    d.line([(14, 14), (30, 14)], fill=P["rosso"], width=2)
    d.line([(16, 18), (28, 18)], fill=P["giallo"], width=1)
    # Cabina (angolosa)
    d.polygon([(26, 10), (38, 10), (42, 16), (22, 16)], fill=P["nero"], outline=P["bianco"], width=1)
    # Fari (storti, punk)
    d.ellipse([50, 14, 56, 18], fill=P["giallo"], outline=P["nero"], width=1)
    d.ellipse([8, 14, 14, 18], fill=P["rosso"], outline=P["nero"], width=1)
    # Ruote (diverse, stile punk)
    d.ellipse([10, 24, 20, 30], fill=P["nero"], outline=P["bianco"], width=1)
    d.ellipse([44, 24, 54, 30], fill=P["nero"], outline=P["bianco"], width=1)
    d.ellipse([12, 26, 18, 29], fill=P["grigio_chiaro"])
    d.ellipse([46, 26, 52, 29], fill=P["grigio_chiaro"])
    # Bullone decorativo
    d.ellipse([30, 20, 34, 24], fill=P["arancio"], outline=P["nero"], width=1)
    # Striscia spray
    d.line([(10, 28), (54, 28)], fill=P["verde"], width=1)

    cartella = os.path.join(BASE, "sprite", "auto", "varianteC_punk")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "auto.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Sagoma auto giocatore stile punk: angolosa, graffita spray, colori sporchi, ruote diverse. Originale, disegnata da zero.",
            ["auto.png"])


# ─────────────────────────────────────────────────────────────────────────────
# (b) SKYLINE NOTTURNO SCI-FI — 3 varianti
# ─────────────────────────────────────────────────────────────────────────────

def disegna_skyline_varianteA():
    """Stile futurista: megastrutture, linee pulite, cromature."""
    W, H = 256, 128
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_FUTURISTA

    # Sfondo gradiente notte
    for y in range(H):
        r = int(10 + (y / H) * 15)
        g = int(10 + (y / H) * 10)
        b = int(18 + (y / H) * 30)
        d.line([(0, y), (W, y)], fill=(r, g, b))

    # Lune
    d.ellipse([200, 10, 230, 40], fill=P["bianco"], outline=P["azzurro"], width=1)
    d.ellipse([205, 15, 225, 35], fill=P["azzurro"])

    # Edifici (megastrutture)
    buildings = [
        (10, 40, 30, 128, P["blu_scuro"]),
        (35, 60, 55, 128, P["blu_scuro"]),
        (60, 30, 85, 128, P["blu_scuro"]),
        (90, 50, 110, 128, P["blu_scuro"]),
        (115, 20, 145, 128, P["blu_scuro"]),
        (150, 45, 170, 128, P["blu_scuro"]),
        (175, 35, 195, 128, P["blu_scuro"]),
        (200, 55, 220, 128, P["blu_scuro"]),
        (225, 40, 245, 128, P["blu_scuro"]),
    ]
    for x1, y1, x2, y2, col in buildings:
        d.rectangle([x1, y1, x2, y2], fill=col, outline=P["ciano"], width=1)
        # Finestre
        for wy in range(y1 + 4, y2 - 4, 6):
            for wx in range(x1 + 3, x2 - 3, 5):
                if (wx + wy) % 3 == 0:
                    d.rectangle([wx, wy, wx + 2, wy + 3], fill=P["ciano"])
                elif (wx + wy) % 3 == 1:
                    d.rectangle([wx, wy, wx + 2, wy + 3], fill=P["giallo"])

    # Antenne
    for x in [20, 70, 130, 185, 235]:
        d.line([(x, 20), (x, 5)], fill=P["ciano"], width=1)
        d.ellipse([x - 1, 3, x + 1, 7], fill=P["rosso"])

    # Insegne neon inventate
    d.text((40, 70), "NEON", fill=P["magenta"])
    d.text((120, 40), "CYBER", fill=P["ciano"])
    d.text((180, 60), "TECH", fill=P["giallo"])

    cartella = os.path.join(BASE, "skyline", "varianteA_futurista")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "skyline.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Skyline notturno sci-fi stile futurista: megastrutture, finestre luminose, antenne, insegne neon inventate. Originale, disegnata da zero.",
            ["skyline.png"])


def disegna_skyline_varianteB():
    """Stile synthwave: griglie, neon, tramonto."""
    W, H = 256, 128
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_FUTURISTA

    # Sfondo gradiente tramonto synthwave
    for y in range(H):
        if y < H // 2:
            t = y / (H // 2)
            r = int(20 + t * 60)
            g = int(10 + t * 20)
            b = int(40 + t * 80)
        else:
            t = (y - H // 2) / (H // 2)
            r = int(80 - t * 60)
            g = int(30 - t * 20)
            b = int(120 - t * 80)
        d.line([(0, y), (W, y)], fill=(r, g, b))

    # Griglia prospettica (pavimento)
    for i in range(10):
        y = H // 2 + i * 8
        d.line([(0, y), (W, y)], fill=P["magenta"], width=1)
    for i in range(-5, 15):
        x = W // 2 + i * 20
        d.line([(x, H // 2), (W // 2 + (x - W // 2) * 2, H)], fill=P["magenta"], width=1)

    # Sole synthwave
    d.ellipse([96, 20, 160, 84], fill=P["giallo"], outline=P["arancio"], width=2)
    for i in range(5):
        y = 40 + i * 10
        d.line([(96, y), (160, y)], fill=P["arancio"], width=2)

    # Edifici stilizzati
    buildings = [
        (10, 60, 35, 128, P["nero"]),
        (40, 80, 60, 128, P["nero"]),
        (65, 50, 90, 128, P["nero"]),
        (95, 70, 120, 128, P["nero"]),
        (125, 45, 155, 128, P["nero"]),
        (160, 65, 185, 128, P["nero"]),
        (190, 55, 215, 128, P["nero"]),
        (220, 75, 245, 128, P["nero"]),
    ]
    for x1, y1, x2, y2, col in buildings:
        d.rectangle([x1, y1, x2, y2], fill=col, outline=P["ciano"], width=1)
        # Finestre neon
        for wy in range(y1 + 4, y2 - 4, 8):
            for wx in range(x1 + 3, x2 - 3, 6):
                if (wx * wy) % 5 == 0:
                    d.rectangle([wx, wy, wx + 2, wy + 4], fill=P["ciano"])
                elif (wx * wy) % 5 == 1:
                    d.rectangle([wx, wy, wx + 2, wy + 4], fill=P["magenta"])

    # Insegne neon
    d.text((15, 90), "SYNTH", fill=P["ciano"])
    d.text((100, 85), "WAVE", fill=P["magenta"])
    d.text((170, 95), "80s", fill=P["giallo"])

    cartella = os.path.join(BASE, "skyline", "varianteB_synthwave")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "skyline.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Skyline notturno sci-fi stile synthwave: tramonto, griglia prospettica, sole, edifici stilizzati con neon. Originale, disegnata da zero.",
            ["skyline.png"])


def disegna_skyline_varianteC():
    """Stile psichedelico: foglie, fumi, colori che cambiano."""
    W, H = 256, 128
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_PUNK

    # Sfondo gradiente psichedelico
    for y in range(H):
        t = y / H
        r = int(40 + 40 * (1 - t))
        g = int(20 + 60 * t)
        b = int(60 + 40 * (1 - t))
        d.line([(0, y), (W, y)], fill=(r, g, b))

    # Lune psichedelica
    d.ellipse([100, 10, 140, 50], fill=P["giallo"], outline=P["rosa"], width=2)
    d.ellipse([110, 20, 130, 40], fill=P["rosa"])

    # Edifici organici
    buildings = [
        (10, 50, 40, 128, P["verde_scuro"]),
        (45, 70, 70, 128, P["verde"]),
        (75, 40, 105, 128, P["verde_scuro"]),
        (110, 60, 140, 128, P["verde"]),
        (145, 35, 175, 128, P["verde_scuro"]),
        (180, 55, 210, 128, P["verde"]),
        (215, 45, 245, 128, P["verde_scuro"]),
    ]
    for x1, y1, x2, y2, col in buildings:
        d.rectangle([x1, y1, x2, y2], fill=col, outline=P["nero"], width=1)
        # Finestre organiche
        for wy in range(y1 + 5, y2 - 5, 10):
            for wx in range(x1 + 4, x2 - 4, 8):
                if (wx + wy) % 4 == 0:
                    d.ellipse([wx, wy, wx + 4, wy + 6], fill=P["giallo"])
                elif (wx + wy) % 4 == 1:
                    d.ellipse([wx, wy, wx + 4, wy + 6], fill=P["ciano"])

    # Foglie stilizzate
    for x, y in [(30, 30), (80, 25), (130, 20), (180, 30), (230, 25)]:
        d.ellipse([x, y, x + 12, y + 8], fill=P["verde"], outline=P["nero"], width=1)
        d.line([(x + 6, y + 4), (x + 10, y + 1)], fill=P["nero"], width=1)

    # Fumi
    for i in range(5):
        x = 50 + i * 40
        d.arc([x, 80, x + 30, 110], 0, 180, fill=P["grigio_chiaro"], width=2)

    # Insegne psichedeliche
    d.text((20, 100), "PEACE", fill=P["giallo"])
    d.text((90, 95), "LOVE", fill=P["rosa"])
    d.text((160, 105), "420", fill=P["ciano"])

    cartella = os.path.join(BASE, "skyline", "varianteC_psichedelico")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "skyline.png"))
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Skyline notturno sci-fi stile psichedelico: edifici organici, foglie stilizzate, fumi, colori che cambiano. Originale, disegnata da zero.",
            ["skyline.png"])


# ─────────────────────────────────────────────────────────────────────────────
# (c) PALETTE LIVELLO 1 — 3 varianti
# ─────────────────────────────────────────────────────────────────────────────

def disegna_palette_varianteA():
    """Palette livello 1 — stile sci-fi anni '50."""
    W, H = 256, 64
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_50

    colors = list(P.values())
    sw = W // len(colors)
    for i, col in enumerate(colors):
        d.rectangle([i * sw, 0, (i + 1) * sw, H], fill=col)
    img = rifinisci_palette(img, P)

    cartella = os.path.join(BASE, "palette", "livello1_varianteA_anni50")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "palette.png"))
    scrivi_palette_json(cartella, "sci-fi anni '50 (pastello)", PAL_50)
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Palette livello 1 stile sci-fi anni '50: 16 colori pastello, razzi, ufo di latta. Originale, disegnata da zero.",
            ["palette.png", "palette.json"])


def disegna_palette_varianteB():
    """Palette livello 1 — stile futurista."""
    W, H = 256, 64
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_FUTURISTA

    colors = list(P.values())
    sw = W // len(colors)
    for i, col in enumerate(colors):
        d.rectangle([i * sw, 0, (i + 1) * sw, H], fill=col)
    img = rifinisci_palette(img, P)

    cartella = os.path.join(BASE, "palette", "livello1_varianteB_futurista")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "palette.png"))
    scrivi_palette_json(cartella, "futurista (neon su scuro)", PAL_FUTURISTA)
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Palette livello 1 stile futurista: 16 colori neon su scuro, ciano, magenta, giallo acido. Originale, disegnata da zero.",
            ["palette.png", "palette.json"])


def disegna_palette_varianteC():
    """Palette livello 1 — stile punk."""
    W, H = 256, 64
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    P = PAL_PUNK

    colors = list(P.values())
    sw = W // len(colors)
    for i, col in enumerate(colors):
        d.rectangle([i * sw, 0, (i + 1) * sw, H], fill=col)
    img = rifinisci_palette(img, P)

    cartella = os.path.join(BASE, "palette", "livello1_varianteC_punk")
    os.makedirs(cartella, exist_ok=True)
    salva(img, os.path.join(cartella, "palette.png"))
    scrivi_palette_json(cartella, "punk (colori sporchi)", PAL_PUNK)
    manifest(cartella, "@asset (progetto Sglavinated)", "CC0-1.0",
            "Palette livello 1 stile punk: 16 colori sporchi, collage, scritte spray. Originale, disegnata da zero.",
            ["palette.png", "palette.json"])


# ─────────────────────────────────────────────────────────────────────────────
# MAIN
# ─────────────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    print("=== BLOCCO F: generazione asset originali ===")
    print()

    print("--- (a) Sagoma auto giocatore ---")
    disegna_auto_varianteA()
    disegna_auto_varianteB()
    disegna_auto_varianteC()
    print()

    print("--- (b) Skyline notturno sci-fi ---")
    disegna_skyline_varianteA()
    disegna_skyline_varianteB()
    disegna_skyline_varianteC()
    print()

    print("--- (c) Palette livello 1 ---")
    disegna_palette_varianteA()
    disegna_palette_varianteB()
    disegna_palette_varianteC()
    print()

    print("=== Completato! ===")
