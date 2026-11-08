#!/usr/bin/env python3
# Pubblico all'arrivo (Andrea 29/10: "gente che attende l'arrivo") — sprite originali.
# Disegnati da zero con Pillow: due file di persone dietro una transenna
# bianca e rossa, alcune con la bandiera a scacchi. Nessun asset Sega. Licenza CC0-1.0.
# Due gruppi (a, b) in due pose: braccia giu' e braccia su. Il motore alterna
# le pose per farli esultare e mette i gruppi ai lati della strada, prima e
# dopo il traguardo.
# Produce: Libreria/assets_originali/sprite/pubblico/pubblico_<gruppo>_<posa>.png (40x26)
# Poi: python3 motore_nuovo/strumenti_py/png_a_raw.py (scrive i .sglraw).
# Per ridisegnarli a mano: stesso nome e stessa misura, poi png_a_raw.py.
import os
import random
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.abspath(os.path.join(BASE, "..", "Libreria", "assets_originali", "sprite", "pubblico"))
os.makedirs(OUT, exist_ok=True)

W, H = 40, 26
PELLE = [(240, 200, 160), (200, 144, 96), (138, 90, 58)]
CAPELLI = [(32, 24, 16), (106, 64, 32), (224, 192, 96), (60, 60, 64)]
MAGLIE = [(224, 48, 48), (48, 96, 224), (240, 208, 48), (48, 176, 80),
          (240, 240, 240), (208, 64, 192), (64, 200, 224), (240, 128, 48)]
GAMBE = (40, 40, 56)
ROSSO, BIANCO, NERO = (208, 32, 32), (244, 244, 244), (20, 20, 20)


def scurisci(c, k=0.72):
    return tuple(int(v * k) for v in c)


def persona(d, x, piedi, p, su, scuro):
    """Persona larga 5 px: testa 3x3, maglia 5x7, gambe; braccia giu' o su."""
    pelle, capelli, maglia, bandiera = p
    if scuro:
        pelle, capelli, maglia = scurisci(pelle), scurisci(capelli), scurisci(maglia)
    testa = piedi - 14
    d.rectangle([x, piedi - 3, x + 1, piedi], fill=GAMBE)
    d.rectangle([x + 3, piedi - 3, x + 4, piedi], fill=GAMBE)
    d.rectangle([x, testa + 4, x + 4, piedi - 4], fill=maglia)
    d.rectangle([x + 1, testa + 1, x + 3, testa + 3], fill=pelle)
    d.line([x + 1, testa, x + 3, testa], fill=capelli)
    if su:  # braccia alzate, mani sopra la testa
        d.line([x - 1, testa - 2, x - 1, testa + 5], fill=maglia)
        d.line([x + 5, testa - 2, x + 5, testa + 5], fill=maglia)
        d.point([(x - 1, testa - 3), (x + 5, testa - 3)], fill=pelle)
        if bandiera:  # bandiera a scacchi sventolata in alto
            for i in range(4):
                for j in range(3):
                    d.point((x + 6 + i, testa - 6 + j), fill=BIANCO if (i + j) % 2 else NERO)
            d.line([x + 5, testa - 6, x + 5, testa - 3], fill=NERO)
    else:
        d.line([x - 1, testa + 5, x - 1, testa + 9], fill=maglia)
        d.line([x + 5, testa + 5, x + 5, testa + 9], fill=maglia)
        if bandiera:  # bandiera tenuta bassa
            for i in range(3):
                for j in range(3):
                    d.point((x + 6 + i, testa + 4 + j), fill=BIANCO if (i + j) % 2 else NERO)


def transenna(d):
    """Transenna in basso: corrimano bianco e pannelli rossi e bianchi."""
    d.line([0, H - 6, W - 1, H - 6], fill=BIANCO)
    for x in range(0, W, 4):
        d.rectangle([x, H - 5, x + 3, H - 2], fill=ROSSO if (x // 4) % 2 == 0 else BIANCO)
    d.line([0, H - 1, W - 1, H - 1], fill=scurisci(ROSSO, 0.5))


def gruppo(seme):
    rnd = random.Random(seme)
    dietro = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), False) for _ in range(5)]
    davanti = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), rnd.random() < 0.3) for _ in range(6)]
    pose = {}
    for posa, su in (("giu", False), ("su", True)):
        img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
        d = ImageDraw.Draw(img)
        # Fila dietro: piu' in alto e piu' scura; alza le braccia a scacchiera.
        for i, p in enumerate(dietro):
            persona(d, 5 + i * 6, H - 9, p, su and i % 2 == 0, True)
        for i, p in enumerate(davanti):
            persona(d, 2 + i * 6, H - 4, p, su and (i % 2 == 1 or p[3]), False)
        transenna(d)
        pose[posa] = img
    return pose


# --- Gruppi di supporto alla partenza (Andrea 08/11) ---------------------
# Le stesse persone dell'arrivo (stessi semi e colori), senza transenna: in
# piccoli capannelli a bordo strada, rilassati, che chiacchierano prima del
# via. Ogni gruppo ha un oggetto: a = stereo portatile, b = cassa da sedersi.
# Pose: relax (braccia giu', uno con le mani sui fianchi), parla (uno gesticola,
# fumetto a puntini sopra la testa), esulta (braccia su allo START).
# Produce: supporto_<gruppo>_<posa>.png (40x26), poi png_a_raw.py.
FUMETTO = (250, 250, 250)


def persona_relax(d, x, piedi, p, posa, scuro=False):
    """Come persona(), con pose da attesa: 'giu', 'fianchi', 'gesto', 'su', 'seduto'."""
    pelle, capelli, maglia, bandiera = p
    if scuro:
        pelle, capelli, maglia = scurisci(pelle), scurisci(capelli), scurisci(maglia)
    if posa == "seduto":  # seduto sulla cassa: gambe corte in avanti
        testa = piedi - 11
        d.rectangle([x, piedi - 1, x + 4, piedi], fill=GAMBE)
        d.rectangle([x, testa + 4, x + 4, piedi - 2], fill=maglia)
        d.rectangle([x + 1, testa + 1, x + 3, testa + 3], fill=pelle)
        d.line([x + 1, testa, x + 3, testa], fill=capelli)
        d.line([x - 1, testa + 5, x - 1, testa + 7], fill=maglia)
        d.line([x + 5, testa + 5, x + 5, testa + 7], fill=maglia)
        return testa
    if posa == "su":
        persona(d, x, piedi, p, True, scuro)
        return piedi - 14
    testa = piedi - 14
    d.rectangle([x, piedi - 3, x + 1, piedi], fill=GAMBE)
    d.rectangle([x + 3, piedi - 3, x + 4, piedi], fill=GAMBE)
    d.rectangle([x, testa + 4, x + 4, piedi - 4], fill=maglia)
    d.rectangle([x + 1, testa + 1, x + 3, testa + 3], fill=pelle)
    d.line([x + 1, testa, x + 3, testa], fill=capelli)
    if posa == "fianchi":  # mani sui fianchi: gomiti in fuori
        d.point([(x - 1, testa + 5), (x - 2, testa + 6), (x - 1, testa + 7),
                 (x + 5, testa + 5), (x + 6, testa + 6), (x + 5, testa + 7)], fill=maglia)
    elif posa == "gesto":  # gesticola: un braccio piegato in alto, mano aperta
        d.line([x - 1, testa + 5, x - 1, testa + 9], fill=maglia)
        d.line([x + 5, testa + 5, x + 6, testa + 3], fill=maglia)
        d.point((x + 7, testa + 2), fill=pelle)
    else:
        d.line([x - 1, testa + 5, x - 1, testa + 9], fill=maglia)
        d.line([x + 5, testa + 5, x + 5, testa + 9], fill=maglia)
    return testa


def fumetto(d, x, y):
    """Nuvoletta 7x5 con tre puntini: qualcuno sta parlando."""
    d.rectangle([x, y, x + 6, y + 3], fill=FUMETTO)
    d.point((x + 1, y + 4), fill=FUMETTO)
    d.point([(x + 1, y + 2), (x + 3, y + 2), (x + 5, y + 2)], fill=NERO)


def stereo(d, x, y):
    """Stereo portatile 9x5 appoggiato a terra: due casse e la maniglia."""
    d.rectangle([x, y + 1, x + 8, y + 5], fill=(50, 50, 60))
    d.line([x + 2, y, x + 6, y], fill=(150, 150, 160))
    for cx in (x + 2, x + 6):
        d.rectangle([cx - 1, y + 2, cx + 1, y + 4], fill=(120, 120, 130))
        d.point((cx, y + 3), fill=NERO)
    d.point((x + 4, y + 2), fill=(240, 80, 80))


def cassa(d, x, y):
    """Cassa di legno 8x5."""
    d.rectangle([x, y, x + 7, y + 4], fill=(150, 100, 50))
    d.line([x, y + 2, x + 7, y + 2], fill=(110, 70, 30))
    d.line([x, y, x + 7, y], fill=(190, 140, 80))


def ombra(d, x0, x1, y):
    d.line([x0, y, x1, y], fill=(0, 0, 0, 90))
    d.line([x0 + 1, y + 1, x1 - 1, y + 1], fill=(0, 0, 0, 60))


def supporto(seme, nome):
    rnd = random.Random(seme)
    # Stesse persone del gruppo dell'arrivo (stesso seme, stesso ordine).
    dietro = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), False) for _ in range(5)]
    davanti = [(rnd.choice(PELLE), rnd.choice(CAPELLI), rnd.choice(MAGLIE), rnd.random() < 0.3) for _ in range(6)]
    pose = {}
    piedi = H - 3
    for posa in ("relax", "parla", "esulta"):
        img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
        d = ImageDraw.Draw(img)
        su = posa == "esulta"
        if nome == "a":
            # Capannello di tre a sinistra, stereo per terra, due a destra.
            ombra(d, 1, 19, piedi + 1)
            ombra(d, 26, 38, piedi + 1)
            persona_relax(d, 6, piedi - 2, dietro[1], "su" if su else "giu", True)
            persona_relax(d, 2, piedi, davanti[0], "su" if su else "fianchi")
            t = persona_relax(d, 11, piedi, davanti[2], "su" if su else ("gesto" if posa == "parla" else "giu"))
            stereo(d, 17, piedi - 4)
            persona_relax(d, 27, piedi, davanti[4], "su" if su else "giu")
            t2 = persona_relax(d, 33, piedi, davanti[1], "su" if su else "fianchi")
            if posa == "parla":
                fumetto(d, 13, t - 7)
            elif posa == "relax":
                fumetto(d, 31, t2 - 7)
        else:
            # Uno seduto sulla cassa, due in piedi che parlano, uno appoggiato.
            ombra(d, 1, 23, piedi + 1)
            ombra(d, 28, 38, piedi + 1)
            cassa(d, 3, piedi - 4)
            persona_relax(d, 4, piedi - 4, davanti[3], "su" if su else "seduto")
            t = persona_relax(d, 12, piedi, davanti[0], "su" if su else ("gesto" if posa == "parla" else "giu"))
            persona_relax(d, 18, piedi, davanti[5], "su" if su else "fianchi")
            persona_relax(d, 31, piedi, dietro[2], "su" if su else "giu")
            if posa == "parla":
                fumetto(d, 14, t - 7)
            elif posa == "relax":
                fumetto(d, 20, t - 7)
        pose[posa] = img
    return pose


if __name__ == "__main__":
    for nome, seme in (("a", 2910), ("b", 1029)):
        for posa, img in gruppo(seme).items():
            p = os.path.join(OUT, f"pubblico_{nome}_{posa}.png")
            img.save(p)
            print(f"  [OK] {p} {img.size}")
        for posa, img in supporto(seme, nome).items():
            p = os.path.join(OUT, f"supporto_{nome}_{posa}.png")
            img.save(p)
            print(f"  [OK] {p} {img.size}")
