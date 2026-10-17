#!/usr/bin/env python3
# genera_bloccoL.py — produzione scenari livelli (BLOCCO L, @scenografo)
# Disegnato da zero con Python 3 + Pillow. Originale, nessun materiale Sega.
# Licenza script: CC0-1.0
# Uso: python3 bacheche/scenografo/genera_bloccoL.py  (da sglavinated_src)
import os
import json
import random
from PIL import Image, ImageDraw

ROOT = "assets_originali/scenari"

LEVELS = {
    "lv_01_neon": {
        "scenario": "scenario_neon_01",
        "stile": "synthwave / C-wave",
        "seme": 4211,
        "palette": [
            ("fondo_scuro", (10, 10, 26)),
            ("ciano", (0, 255, 255)),
            ("magenta", (255, 0, 255)),
            ("giallo_acido", (204, 255, 0)),
            ("viola", (128, 0, 255)),
            ("blu_notte", (20, 20, 60)),
            ("grigio_neon", (180, 180, 220)),
            ("bianco", (255, 255, 255)),
            ("rosa_neon", (255, 105, 255)),
            ("arancio_neon", (255, 128, 0)),
            ("verde_neon", (128, 255, 128)),
            ("ocra", (180, 140, 60)),
            ("grigio_scuro", (60, 60, 80)),
            ("ciliegia", (220, 20, 60)),
            ("turchese", (64, 224, 208)),
            ("trasparente", (0, 0, 0)),
        ],
        "oggetti": ["palo_neon", "cassetta", "panchina", "insegna_neon", "lampione", "bidone", "segnale", "palina"],
    },
    "lv_02_atomico": {
        "scenario": "scenario_atomico_02",
        "stile": "sci-fi anni '50",
        "seme": 7731,
        "palette": [
            ("cielo_pastello", (220, 230, 240)),
            ("rosa_pastello", (255, 182, 193)),
            ("menta", (189, 252, 201)),
            ("giallo_burro", (255, 235, 150)),
            ("cromo", (200, 200, 205)),
            ("argento", (190, 190, 200)),
            ("blu_acciaio", (120, 140, 160)),
            ("bianco", (255, 255, 255)),
            ("rosso_atomico", (220, 80, 80)),
            ("arancio", (255, 160, 80)),
            ("viola_atomico", (200, 160, 220)),
            ("grigio_cemento", (140, 140, 145)),
            ("verde_pallido", (160, 210, 160)),
            ("ocra_pastello", (210, 180, 130)),
            ("blu_notte_atomico", (40, 50, 70)),
            ("trasparente", (0, 0, 0)),
        ],
        "oggetti": ["bancone_drive_in", "segnale_atomico", "panchina_cromata", "torre_razzo", "ufo_distanza", "cupola", "freccia", "segnale_stop"],
    },
    "lv_03_foglie": {
        "scenario": "scenario_foglie_03",
        "stile": "psichedelico / cannabinoidi",
        "seme": 2098,
        "palette": [
            ("fondo_scuro", (15, 10, 25)),
            ("verde_acido", (170, 255, 0)),
            ("viola", (160, 60, 220)),
            ("arancio", (255, 120, 30)),
            ("magenta_psico", (220, 40, 200)),
            ("giallo_psico", (255, 230, 40)),
            ("verde_foglia", (60, 180, 80)),
            ("blu_psico", (60, 100, 220)),
            ("rosa_psico", (240, 100, 180)),
            ("grigio_fumo", (120, 100, 140)),
            ("marrone_terra", (120, 80, 60)),
            ("ciano_psico", (40, 220, 200)),
            ("giallo_ocra", (200, 150, 40)),
            ("prugna", (100, 60, 120)),
            ("bianco_ghiaccio", (230, 230, 240)),
            ("trasparente", (0, 0, 0)),
        ],
        "oggetti": ["foglia_gigante", "insegna_fumosa", "panchina_curva", "albero_mini", "fumo", "fungo_psico", "segnale_foglia", "panchina_fumo"],
    },
}

VARIANTI = ["A", "B", "C"]
SKY_W, SKY_H = 256, 128
OBJ_W, OBJ_H = 32, 32
PAL_SW = 16


def palname(level, name):
    for n, rgb in LEVELS[level]["palette"]:
        if n == name:
            return rgb
    return (0, 0, 0)


def rng(level, variant):
    return random.Random(LEVELS[level]["seme"] + 1000 * (ord(variant) - 64))


def gradiente(draw, x0, y0, x1, y1, c_top, c_bottom, vertical=True):
    steps = (y1 - y0) if vertical else (x1 - x0)
    if steps <= 0:
        return
    for i in range(steps + 1):
        t = i / steps
        c = tuple(int(c_top[k] * (1 - t) + c_bottom[k] * t) for k in range(3))
        if vertical:
            draw.line([(x0, y0 + i), (x1, y0 + i)], fill=c)
        else:
            draw.line([(x0 + i, y0), (x0 + i, y1)], fill=c)


def cerchio(draw, cx, cy, r, colore, riempito=True, larg=2):
    box = [cx - r, cy - r, cx + r, cy + r]
    if riempito:
        draw.ellipse(box, fill=colore)
    else:
        draw.ellipse(box, outline=colore, width=larg)


def rett(draw, x, y, w, h, colore, riempito=True, larg=2):
    if riempito:
        draw.rectangle([x, y, x + w, y + h], fill=colore)
    else:
        draw.rectangle([x, y, x + w, y + h], outline=colore, width=larg)


def obj_palo_neon(draw, pal, x, y, s, r):
    col = pal["ciano"]; col2 = pal["magenta"]; base = pal["grigio_scuro"]
    rett(draw, x + 14 * s, y + 6 * s, 4 * s, 26 * s, base)
    cerchio(draw, x + 16 * s, y + 6 * s, 5 * s, col)
    rett(draw, x + 11 * s, y + 2 * s, 10 * s, 4 * s, col2)


def obj_cassetta(draw, pal, x, y, s, r):
    col = pal["grigio_neon"]; col2 = pal["viola"]
    rett(draw, x + 6 * s, y + 8 * s, 20 * s, 20 * s, col)
    rett(draw, x + 8 * s, y + 10 * s, 4 * s, 4 * s, col2)
    rett(draw, x + 20 * s, y + 10 * s, 4 * s, 4 * s, col2)


def obj_panchina(draw, pal, x, y, s, r):
    col = pal["grigio_scuro"]; col2 = pal["ciano"]
    rett(draw, x + 4 * s, y + 16 * s, 24 * s, 4 * s, col)
    rett(draw, x + 6 * s, y + 10 * s, 4 * s, 6 * s, col)
    rett(draw, x + 22 * s, y + 10 * s, 4 * s, 6 * s, col)
    rett(draw, x + 8 * s, y + 6 * s, 16 * s, 3 * s, col2)


def obj_insegna_neon(draw, pal, x, y, s, r):
    col = pal["magenta"]; col2 = pal["giallo_acido"]; base = pal["fondo_scuro"]
    rett(draw, x + 4 * s, y + 6 * s, 24 * s, 12 * s, base, riempito=False, larg=2)
    draw.line([(x + 6 * s, y + 10 * s), (x + 12 * s, y + 6 * s)], fill=col, width=2)
    draw.line([(x + 14 * s, y + 12 * s), (x + 20 * s, y + 8 * s)], fill=col2, width=2)
    draw.line([(x + 6 * s, y + 14 * s), (x + 10 * s, y + 16 * s)], fill=col, width=2)


def obj_lampione(draw, pal, x, y, s, r):
    col = pal["arancio_neon"]; base = pal["grigio_neon"]
    rett(draw, x + 15 * s, y + 4 * s, 2 * s, 28 * s, base)
    rett(draw, x + 8 * s, y + 4 * s, 16 * s, 4 * s, col)
    cerchio(draw, x + 16 * s, y + 8 * s, 4 * s, col)


def obj_bidone(draw, pal, x, y, s, r):
    col = pal["grigio_scuro"]; col2 = pal["ciliegia"]
    rett(draw, x + 10 * s, y + 10 * s, 12 * s, 18 * s, col)
    rett(draw, x + 8 * s, y + 8 * s, 16 * s, 4 * s, col2)
    rett(draw, x + 14 * s, y + 14 * s, 4 * s, 8 * s, col2)


def obj_segnale(draw, pal, x, y, s, r):
    col = pal["ciano"]; col2 = pal["magenta"]
    rett(draw, x + 12 * s, y + 4 * s, 2 * s, 20 * s, col)
    rett(draw, x + 6 * s, y + 10 * s, 20 * s, 6 * s, col2, riempito=False, larg=2)
    draw.line([(x + 8 * s, y + 13 * s), (x + 24 * s, y + 13 * s)], fill=col, width=2)


def obj_palina(draw, pal, x, y, s, r):
    col = pal["rosa_neon"]; col2 = pal["bianco"]
    rett(draw, x + 14 * s, y + 6 * s, 4 * s, 26 * s, col)
    for i in range(4):
        rett(draw, x + 10 * s, y + 6 * s + i * 6 * s, 4 * s, 3 * s, col2)


def obj_bancone_drive_in(draw, pal, x, y, s, r):
    col = pal["cromo"]; col2 = pal["rosso_atomico"]
    rett(draw, x + 4 * s, y + 14 * s, 24 * s, 14 * s, col)
    rett(draw, x + 6 * s, y + 10 * s, 20 * s, 6 * s, col2, riempito=False, larg=2)
    rett(draw, x + 10 * s, y + 4 * s, 4 * s, 6 * s, col)


def obj_segnale_atomico(draw, pal, x, y, s, r):
    col = pal["menta"]; col2 = pal["blu_acciaio"]
    cerchio(draw, x + 16 * s, y + 12 * s, 9 * s, col)
    rett(draw, x + 14 * s, y + 4 * s, 4 * s, 10 * s, col2)


def obj_panchina_cromata(draw, pal, x, y, s, r):
    col = pal["argento"]; col2 = pal["cromo"]
    rett(draw, x + 4 * s, y + 16 * s, 24 * s, 4 * s, col)
    rett(draw, x + 6 * s, y + 10 * s, 4 * s, 6 * s, col2)
    rett(draw, x + 22 * s, y + 10 * s, 4 * s, 6 * s, col2)


def obj_torre_razzo(draw, pal, x, y, s, r):
    col = pal["arancio"]; col2 = pal["rosso_atomico"]
    draw.polygon([(x + 16 * s, y + 4 * s), (x + 4 * s, y + 28 * s), (x + 28 * s, y + 28 * s)], fill=col)
    rett(draw, x + 6 * s, y + 26 * s, 8 * s, 4 * s, col2)
    rett(draw, x + 18 * s, y + 26 * s, 8 * s, 4 * s, col2)


def obj_ufo_distanza(draw, pal, x, y, s, r):
    col = pal["viola_atomico"]; col2 = pal["cromo"]
    draw.ellipse([x + 6 * s, y + 14 * s, x + 26 * s, y + 22 * s], fill=col)
    draw.ellipse([x + 10 * s, y + 10 * s, x + 22 * s, y + 16 * s], fill=col2)
    rett(draw, x + 14 * s, y + 6 * s, 4 * s, 6 * s, col2)


def obj_cupola(draw, pal, x, y, s, r):
    col = pal["rosa_pastello"]; col2 = pal["giallo_burro"]
    draw.pieslice([x + 4 * s, y + 8 * s, x + 28 * s, y + 32 * s], 180, 360, fill=col)
    rett(draw, x + 8 * s, y + 24 * s, 16 * s, 6 * s, col2)


def obj_freccia(draw, pal, x, y, s, r):
    col = pal["arancio"]; col2 = pal["bianco"]
    draw.polygon([(x + 16 * s, y + 4 * s), (x + 4 * s, y + 20 * s), (x + 28 * s, y + 20 * s)], fill=col)
    rett(draw, x + 14 * s, y + 20 * s, 4 * s, 10 * s, col2)


def obj_segnale_stop(draw, pal, x, y, s, r):
    col = pal["rosso_atomico"]; col2 = pal["bianco"]
    draw.polygon([(x + 16 * s, y + 4 * s), (x + 26 * s, y + 14 * s), (x + 26 * s, y + 26 * s),
                  (x + 16 * s, y + 36 * s), (x + 6 * s, y + 26 * s), (x + 6 * s, y + 14 * s)], fill=col)
    rett(draw, x + 12 * s, y + 14 * s, 8 * s, 8 * s, col2)


def obj_foglia_gigante(draw, pal, x, y, s, r):
    col = pal["verde_acido"]; col2 = pal["viola"]
    draw.ellipse([x + 4 * s, y + 4 * s, x + 28 * s, y + 28 * s], fill=col)
    draw.line([(x + 16 * s, y + 4 * s), (x + 16 * s, y + 28 * s)], fill=col2, width=2)
    draw.line([(x + 8 * s, y + 12 * s), (x + 24 * s, y + 20 * s)], fill=col2, width=2)


def obj_insegna_fumosa(draw, pal, x, y, s, r):
    col = pal["arancio"]; col2 = pal["grigio_fumo"]
    rett(draw, x + 6 * s, y + 12 * s, 20 * s, 10 * s, col, riempito=False, larg=2)
    draw.ellipse([x + 6 * s, y + 4 * s, x + 16 * s, y + 14 * s], fill=(120, 100, 140, 110))
    draw.ellipse([x + 16 * s, y + 2 * s, x + 26 * s, y + 12 * s], fill=(120, 100, 140, 110))


def obj_panchina_curva(draw, pal, x, y, s, r):
    col = pal["marrone_terra"]; col2 = pal["arancio"]
    draw.chord([x + 6 * s, y + 6 * s, x + 26 * s, y + 22 * s], 180, 360, fill=col)
    rett(draw, x + 8 * s, y + 20 * s, 16 * s, 4 * s, col2)


def obj_albero_mini(draw, pal, x, y, s, r):
    col = pal["verde_foglia"]; col2 = pal["marrone_terra"]
    rett(draw, x + 14 * s, y + 16 * s, 4 * s, 14 * s, col2)
    cerchio(draw, x + 16 * s, y + 12 * s, 10 * s, col)


def obj_fumo(draw, pal, x, y, s, r):
    draw.ellipse([x + 4 * s, y + 8 * s, x + 16 * s, y + 22 * s], fill=(120, 100, 140, 120))
    draw.ellipse([x + 12 * s, y + 4 * s, x + 24 * s, y + 16 * s], fill=(120, 100, 140, 120))
    draw.ellipse([x + 20 * s, y + 10 * s, x + 28 * s, y + 20 * s], fill=(120, 100, 140, 100))


def obj_fungo_psico(draw, pal, x, y, s, r):
    col = pal["magenta_psico"]; col2 = pal["giallo_psico"]
    draw.ellipse([x + 4 * s, y + 6 * s, x + 28 * s, y + 18 * s], fill=col)
    rett(draw, x + 14 * s, y + 14 * s, 4 * s, 14 * s, col2)
    for i in range(3):
        cerchio(draw, x + 8 * s + i * 8 * s, y + 12 * s, 2 * s, col2)


def obj_segnale_foglia(draw, pal, x, y, s, r):
    col = pal["giallo_ocra"]; col2 = pal["prugna"]
    rett(draw, x + 14 * s, y + 4 * s, 4 * s, 14 * s, col2)
    draw.polygon([(x + 16 * s, y + 4 * s), (x + 4 * s, y + 18 * s), (x + 28 * s, y + 18 * s)], fill=col)


def obj_panchina_fumo(draw, pal, x, y, s, r):
    col = pal["viola"]; col2 = pal["grigio_fumo"]
    rett(draw, x + 4 * s, y + 18 * s, 24 * s, 3 * s, col)
    draw.pieslice([x + 6 * s, y + 8 * s, x + 26 * s, y + 22 * s], 180, 360, fill=col2)
    rett(draw, x + 8 * s, y + 16 * s, 16 * s, 3 * s, col2)


OBJ_DRAW = {
    "palo_neon": obj_palo_neon,
    "cassetta": obj_cassetta,
    "panchina": obj_panchina,
    "insegna_neon": obj_insegna_neon,
    "lampione": obj_lampione,
    "bidone": obj_bidone,
    "segnale": obj_segnale,
    "palina": obj_palina,
    "bancone_drive_in": obj_bancone_drive_in,
    "segnale_atomico": obj_segnale_atomico,
    "panchina_cromata": obj_panchina_cromata,
    "torre_razzo": obj_torre_razzo,
    "ufo_distanza": obj_ufo_distanza,
    "cupola": obj_cupola,
    "freccia": obj_freccia,
    "segnale_stop": obj_segnale_stop,
    "foglia_gigante": obj_foglia_gigante,
    "insegna_fumosa": obj_insegna_fumosa,
    "panchina_curva": obj_panchina_curva,
    "albero_mini": obj_albero_mini,
    "fumo": obj_fumo,
    "fungo_psico": obj_fungo_psico,
    "segnale_foglia": obj_segnale_foglia,
    "panchina_fumo": obj_panchina_fumo,
}


def disegna_skyline(level, variante, r):
    pal = {n: c for n, c in LEVELS[level]["palette"]}
    img = Image.new("RGBA", (SKY_W, SKY_H), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    fondo = pal["fondo_scuro"] if "fondo_scuro" in pal else pal["cielo_pastello"]
    if "blu_notte" in pal:
        sky2 = pal["blu_notte"]
    elif "blu_notte_atomico" in pal:
        sky2 = pal["blu_notte_atomico"]
    else:
        sky2 = pal["fondo_scuro"]
    gradiente(draw, 0, 0, SKY_W, SKY_H, fondo, sky2)
    orizzonte = SKY_H - 26
    if level == "lv_01_neon":
        col_grid = pal["ciano"]
        for gx in range(0, SKY_W, 20):
            draw.line([(gx, orizzonte), (gx - 30, SKY_H)], fill=col_grid, width=1)
        draw.line([(0, orizzonte), (SKY_W, orizzonte)], fill=pal["giallo_acido"], width=2)
        bx = 10
        while bx < SKY_W:
            bw = r.randint(22, 40)
            bh = r.randint(30, 70)
            rett(draw, bx, orizzonte - bh, bw, bh, pal["grigio_scuro"])
            for wy in range(orizzonte - bh + 6, orizzonte - 4, 10):
                for wx in range(bx + 4, bx + bw - 4, 8):
                    if r.random() < 0.5:
                        draw.point((wx, wy), fill=pal["ciano"])
            bx += bw + 6
        sx, sy = SKY_W // 2, orizzonte - 10
        for i in range(6):
            yy = sy - 6 + i * 2
            col = pal["magenta"] if i % 2 == 0 else pal["giallo_acido"]
            draw.line([(sx - 18 + i * 2, yy), (sx + 18 - i * 2, yy)], fill=col, width=2)
    elif level == "lv_02_atomico":
        draw.line([(0, orizzonte), (SKY_W, orizzonte)], fill=pal["arancio"], width=2)
        bx = 6
        while bx < SKY_W:
            bw = r.randint(20, 36)
            bh = r.randint(24, 56)
            rett(draw, bx, orizzonte - bh, bw, bh, pal["cromo"])
            tetto = bh // 3
            draw.polygon([(bx, orizzonte - bh), (bx + bw // 2, orizzonte - bh - tetto), (bx + bw, orizzonte - bh)], fill=pal["rosa_pastello"])
            for wy in range(orizzonte - bh + 6, orizzonte - 4, 10):
                for wx in range(bx + 4, bx + bw - 4, 8):
                    if r.random() < 0.4:
                        draw.point((wx, wy), fill=pal["bianco"])
            bx += bw + 5
        draw.ellipse([150, orizzonte - 42, 190, orizzonte - 30], fill=pal["argento"])
        draw.ellipse([158, orizzonte - 48, 182, orizzonte - 40], fill=pal["cromo"])
        for fx in [40, 120, 200]:
            draw.polygon([(fx + 8, orizzonte - 24), (fx, orizzonte - 14), (fx + 16, orizzonte - 14)], fill=pal["rosso_atomico"])
    else:
        draw.line([(0, orizzonte), (SKY_W, orizzonte)], fill=pal["arancio"], width=2)
        bx = 6
        while bx < SKY_W:
            bw = r.randint(24, 44)
            bh = r.randint(20, 50)
            rett(draw, bx + bw // 2 - 3, orizzonte - bh, 6, bh, pal["marrone_terra"])
            col_fog = pal["verde_acido"] if r.random() < 0.5 else pal["viola"]
            draw.ellipse([bx, orizzonte - bh - 14, bx + bw, orizzonte - bh + 6], fill=col_fog)
            bx += bw + 7
        for _ in range(4):
            fx = r.randint(0, SKY_W - 24)
            fy = r.randint(10, orizzonte - 20)
            draw.ellipse([fx, fy, fx + 24, fy + 14], fill=(160, 60, 220, 90))
            draw.ellipse([fx + 10, fy - 6, fx + 30, fy + 8], fill=(160, 60, 220, 70))
    return img


def disegna_palette(level, variante, r):
    pal = LEVELS[level]["palette"]
    img = Image.new("RGBA", (PAL_SW * len(pal), PAL_SW), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    for i, (nome, rgb) in enumerate(pal):
        x = i * PAL_SW
        rett(draw, x, 0, PAL_SW - 1, PAL_SW - 1, rgb + (255,))
        rett(draw, x, 0, PAL_SW - 1, PAL_SW - 1, (255, 255, 255, 60), riempito=False, larg=1)
    return img


def genera_variante(level, variante):
    info = LEVELS[level]
    r = rng(level, variante)
    base = os.path.join(ROOT, f"{info['scenario']}_variante{variante}")
    obj_dir = os.path.join(base, "oggetti")
    os.makedirs(obj_dir, exist_ok=True)

    pal = {n: c for n, c in info["palette"]}

    sky = disegna_skyline(level, variante, r)
    sky.save(os.path.join(base, "skyline.png"))

    oggetti = info["oggetti"][:]
    r.shuffle(oggetti)
    n_oggetti = r.randint(4, 8)
    scelti = oggetti[:n_oggetti]
    oggetto_file = []
    for nome in scelti:
        img = Image.new("RGBA", (OBJ_W, OBJ_H), (0, 0, 0, 0))
        draw = ImageDraw.Draw(img)
        OBJ_DRAW[nome](draw, pal, 0, 0, 1, r)
        fname = f"obj_{nome}.png"
        img.save(os.path.join(obj_dir, fname))
        oggetto_file.append(os.path.join("oggetti", fname))

    pal_img = disegna_palette(level, variante, r)
    pal_img.save(os.path.join(base, "palette.png"))
    with open(os.path.join(base, "palette.json"), "w", encoding="utf-8") as f:
        json.dump([{"nome": n, "rgb": list(c)} for n, c in info["palette"]], f, ensure_ascii=False, indent=2)

    manifest = {
        "livello": level,
        "variante": variante,
        "scenario": info["scenario"],
        "stile": info["stile"],
        "autore": "@scenografo (progetto Sglavinated, disegnato da zero, nessun materiale Sega)",
        "licenza": "CC0-1.0",
        "originale": True,
        "riferimenti_sega": False,
        "palette_max_colori": 16,
        "risoluzione_skyline": [SKY_W, SKY_H],
        "oggetti_bordo_strada": scelti,
        "file": {
            "skyline": "skyline.png",
            "palette": "palette.png",
            "palette_dati": "palette.json",
            "oggetti": oggetto_file,
        },
        "note": "Atmosfera coerente con livello.json; nessun elemento Sega; generato da seme fisso (deterministico).",
    }
    with open(os.path.join(base, "manifest.json"), "w", encoding="utf-8") as f:
        json.dump(manifest, f, ensure_ascii=False, indent=2)
    return base, scelti, oggetto_file


def main():
    prodotti = []
    for level in LEVELS:
        for variante in VARIANTI:
            base, scelti, ofile = genera_variante(level, variante)
            prodotti.append((level, variante, base, scelti))
            print(f"[OK] {level} variante {variante}: {len(scelti)} oggetti -> {base}")
    riepilogo = {}
    for level, variante, base, scelti in prodotti:
        riepilogo.setdefault(level, []).append({"variante": variante, "cartella": base, "oggetti": scelti})
    with open(os.path.join(ROOT, "_riepilogo_varianti.json"), "w", encoding="utf-8") as f:
        json.dump(riepilogo, f, ensure_ascii=False, indent=2)
    print("\nVarianti pronte per scelta di Andrea (una per livello):")
    for level, v in riepilogo.items():
        print(f"  {level}: " + ", ".join(x["variante"] for x in v))


if __name__ == "__main__":
    main()
