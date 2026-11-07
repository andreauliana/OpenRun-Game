#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Paesaggio a zone (Andrea 06/11): segnaposto per erba, cespugli, alberi,
# edifici e frecce di curva, uno stile per livello, pochi colori per livello e
# trama Bayer 4x4 solo sulle facciate grandi (regole palette/dithering 29/10).
# Disegnati da zero, nessun materiale Sega.
#
# Andrea li ridisegna a mano: stessi nomi e stesse misure. Lo strumento NON
# sovrascrive mai un PNG che esiste gia' (i disegni di Andrea restano), salvo
# --forza. Scrive sempre il raw del motore (formato SGLR) dal PNG presente.
#
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_paesaggio.py"
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_paesaggio.py" --forza   (ridisegna i segnaposto)
#
# Misure (larghezza x altezza, pixel):
#   ciuffo 24x12, cespuglio 40x24, albero 48x96,
#   edificio_alto 64x128, edificio_medio 64x96, edificio_basso 96x64,
#   comuni/freccia_curva_dx e _sx 32x40.
import random
import struct
import sys
from pathlib import Path
from PIL import Image

SCENARI = Path("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari")
RAW = Path("motore_nuovo/dati/sprite_raw/scenari")

# Cartella "variante A" che il gioco usa gia' per gli oggetti di ogni livello.
CARTELLA = {
    "lv_01_neon": "scenario_neon_01_varianteA",
    "lv_02_atomico": "scenario_atomico_02_varianteA",
    "lv_03_foglie": "scenario_foglie_03_varianteA",
    "lv_04_cromo": "scenario_cromo_04_varianteA",
    "lv_05_spray": "scenario_spray_05_varianteA",
    "lv_06_desktop": "scenario_desktop_06_varianteA",
}

BAYER4 = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]
T = (0, 0, 0, 0)


def c(h, a=255):
    h = h.lstrip("#")
    return (int(h[0:2], 16), int(h[2:4], 16), int(h[4:6], 16), a)


class Tela:
    def __init__(self, w, h):
        self.w, self.h = w, h
        self.img = Image.new("RGBA", (w, h), T)
        self.px = self.img.load()

    def p(self, x, y, col):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.px[x, y] = col

    def rett(self, x0, y0, x1, y1, col):  # x1, y1 esclusi
        for y in range(max(0, y0), min(self.h, y1)):
            for x in range(max(0, x0), min(self.w, x1)):
                self.px[x, y] = col

    def trama(self, x0, y0, x1, y1, a, b, da=0.0, fino=1.0):
        """Bayer 4x4 dall'alto (quasi tutto a) al basso (quasi tutto b)."""
        alt = max(1, y1 - y0 - 1)
        for y in range(max(0, y0), min(self.h, y1)):
            soglia = da + (fino - da) * (y - y0) / alt
            for x in range(max(0, x0), min(self.w, x1)):
                self.px[x, y] = b if (BAYER4[y % 4][x % 4] + 0.5) / 16 < soglia else a

    def ellisse(self, cx, cy, rx, ry, col):
        for y in range(int(cy - ry) - 1, int(cy + ry) + 2):
            for x in range(int(cx - rx) - 1, int(cx + rx) + 2):
                if ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2 <= 1.0:
                    self.p(x, y, col)

    def contorno(self, col):
        """Bordo scuro di 1 px attorno alla sagoma: si legge anche piccolo."""
        dentro = [[self.px[x, y][3] > 0 for x in range(self.w)] for y in range(self.h)]
        for y in range(self.h):
            for x in range(self.w):
                if dentro[y][x]:
                    continue
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    xx, yy = x + dx, y + dy
                    if 0 <= xx < self.w and 0 <= yy < self.h and dentro[yy][xx]:
                        self.px[x, y] = col
                        break


# --- Stile di ogni livello: pochi colori, presi dal tema del livello. --------
STILI = {
    "lv_01_neon": dict(  # notte synthwave: palme, torri scure con finestre neon
        erba=[c("0A5A2A"), c("12A048"), c("6CFF9C")], cont=c("05101A"),
        albero="palma", tronco=[c("3A1E5A"), c("FF2BD6")], chioma=[c("004E5A"), c("00B8C8"), c("8CFFFF")],
        muro=[c("1A1840"), c("2C2866")], tetto=c("FF2BD6"), finestra=[c("0E0E26"), c("00E5FF"), c("FF2BD6"), c("FFE14D")],
        fiori=c("FF2BD6")),
    "lv_02_atomico": dict(  # deserto anni 50: cactus, case pastello googie
        erba=[c("8A6A30"), c("B89A54"), c("E8D9A0")], cont=c("4A3418"),
        albero="cactus", tronco=[c("2E6A3E"), c("8CC88A")], chioma=[c("2E6A3E"), c("4E9A5A"), c("8CC88A")],
        muro=[c("F2B8C6"), c("8FE3C8")], tetto=c("D04A4A"), finestra=[c("2A6A80"), c("9FE8FF"), c("FFF2B0"), c("FFF2B0")],
        fiori=c("FF6A8A")),
    "lv_03_foglie": dict(  # viale delle foglie: alberi tondi, fiori, case viola
        erba=[c("1F6F1F"), c("2E8B2E"), c("7ED957")], cont=c("0E2A10"),
        albero="tondo", tronco=[c("5A3418"), c("8A5A2A")], chioma=[c("1F6F1F"), c("38A838"), c("A8F060")],
        muro=[c("6A3A9A"), c("9A5AC8")], tetto=c("FFD100"), finestra=[c("2A0E3A"), c("FFD100"), c("FF7AD9"), c("A8F060")],
        fiori=c("FFD100")),
    "lv_04_cromo": dict(  # cromo: conifere scure, grattacieli d'acciaio
        erba=[c("3C4650"), c("5A6A72"), c("9AB0B8")], cont=c("0B1030"),
        albero="conifera", tronco=[c("2A2A30"), c("50505A")], chioma=[c("123038"), c("1E5060"), c("00D8FF")],
        muro=[c("5A6478"), c("98A4B8")], tetto=c("00D8FF"), finestra=[c("1A2030"), c("00D8FF"), c("E8F4FF"), c("00D8FF")],
        fiori=c("00D8FF")),
    "lv_05_spray": dict(  # periferia industriale: erbacce, alberi secchi, capannoni con graffiti
        erba=[c("4A5228"), c("6E7A3A"), c("A8B060")], cont=c("2A241C"),
        albero="secco", tronco=[c("3A2E24"), c("6A5644")], chioma=[c("4A5228"), c("6E7A3A"), c("A8B060")],
        muro=[c("7A3A2A"), c("9A9488")], tetto=c("5A5A66"), finestra=[c("2A2A30"), c("E01020"), c("00C878"), c("FFC800")],
        fiori=c("E01020")),
    "lv_06_desktop": dict(  # collina verde e cielo blu: alberi tondi, palazzi a "finestra"
        erba=[c("2C922C"), c("34A834"), c("8CE05A")], cont=c("10401A"),
        albero="tondo", tronco=[c("6A4A2A"), c("9A7040")], chioma=[c("1E7A28"), c("34A834"), c("A0F070")],
        muro=[c("C0C0C0"), c("E8E8E8")], tetto=c("0A3AA8"), finestra=[c("3A6EA5"), c("FFFFFF"), c("FFE14D"), c("FFFFFF")],
        fiori=c("FFFFFF")),
}


def ciuffo(s, rnd):
    t = Tela(24, 12)
    scuro, medio, chiaro = s["erba"]
    t.ellisse(12, 11, 11, 3, scuro)  # zolla
    for i in range(8):  # fili d'erba a ventaglio, 2 px: si leggono anche piccoli
        x = 3 + i * 2 + rnd.randint(0, 1)
        alto = rnd.randint(5, 11)
        piega = -1 if x < 10 else (1 if x > 14 else 0)
        for k in range(alto):
            xx = x + (piega if k > alto * 0.5 else 0)
            col = chiaro if k >= alto - 2 else (medio if k > 2 else scuro)
            t.p(xx, 10 - k, col)
            t.p(xx + 1, 10 - k, medio if k < alto - 1 else col)
    t.contorno(s["cont"])
    return t


def cespuglio(s, rnd):
    t = Tela(40, 24)
    scuro, medio, chiaro = s["chioma"] if s["albero"] in ("tondo", "conifera") else s["erba"]
    for cx, cy, r in ((12, 15, 9), (27, 14, 10), (20, 10, 9)):
        t.ellisse(cx, cy, r, r * 0.85, scuro)
    for cx, cy, r in ((12, 13, 6), (27, 12, 7), (20, 8, 6)):
        t.ellisse(cx, cy, r, r * 0.8, medio)
    for cx, cy in ((9, 9), (18, 5), (25, 8), (31, 10)):
        t.p(cx, cy, chiaro); t.p(cx + 1, cy, chiaro)
    for i in range(5):  # qualche fiore/bacca del colore del tema
        t.p(rnd.randint(6, 33), rnd.randint(6, 18), s["fiori"])
    t.rett(3, 22, 37, 24, scuro)
    t.contorno(s["cont"])
    return t


def albero(s, rnd):
    t = Tela(48, 96)
    tr, trc = s["tronco"]
    scuro, medio, chiaro = s["chioma"]
    tipo = s["albero"]
    if tipo == "palma":
        for y in range(28, 96):  # tronco leggermente curvo, ad anelli
            x = 24 + int(3 * ((96 - y) / 68) ** 2)
            t.rett(x - 3, y, x + 3, y + 1, trc if y % 6 == 0 else tr)
        for ang in range(7):  # foglie a raggiera
            dx = [-20, -14, -6, 0, 6, 14, 20][ang]
            for k in range(22):
                f = k / 21
                x = 27 + int(dx * f)
                y = 26 - int(10 * f) + int(16 * f * f) + (0 if abs(dx) > 8 else -int(4 * f))
                spess = 3 if f < 0.7 else 2
                t.rett(x - 1, y, x - 1 + spess, y + 2, medio)
                t.rett(x - 1, y + 2, x - 1 + spess, y + 3, scuro)
                t.p(x, y - 1, chiaro)
        t.ellisse(27, 27, 4, 3, scuro)
    elif tipo == "cactus":
        t.rett(19, 18, 29, 96, medio)
        t.rett(19, 18, 21, 96, scuro)
        t.rett(26, 20, 28, 96, chiaro)
        t.ellisse(24, 19, 5, 4, medio)
        for (x0, y0, y1, lato) in ((6, 34, 58, -1), (35, 44, 66, 1)):  # braccia
            t.rett(x0, y0, x0 + 7, y1, medio)
            t.rett(x0, y1 - 6, 24 if lato < 0 else x0 + 7, y1, medio)
            t.rett(x0 + 1, y0, x0 + 2, y1, chiaro)
            t.ellisse(x0 + 3.5, y0, 3.5, 3, medio)
        for y in range(24, 92, 6):
            t.p(23, y, chiaro)
        t.p(24, 13, s["fiori"]); t.p(23, 14, s["fiori"]); t.p(25, 14, s["fiori"])
    elif tipo == "conifera":
        t.rett(21, 76, 27, 96, tr)
        t.rett(21, 76, 23, 96, trc)
        for i, (y0, r) in enumerate(((4, 8), (20, 13), (38, 18), (56, 22))):
            for y in range(y0, y0 + 24):
                half = int(r * (y - y0) / 24) + 2
                t.rett(24 - half, y, 24 + half, y + 1, scuro if (y - y0) > 18 else medio)
            t.rett(24 - 1, y0 + 2, 24 + 1, y0 + 14, chiaro)
    elif tipo == "secco":
        t.rett(21, 30, 27, 96, tr)
        t.rett(21, 30, 23, 96, trc)
        rami = ((24, 40, -16, -18), (24, 34, 14, -20), (24, 52, 18, -12), (24, 60, -18, -10), (24, 30, -4, -24))
        for x0, y0, dx, dy in rami:
            for k in range(20):
                f = k / 19
                x, y = x0 + int(dx * f), y0 + int(dy * f)
                t.rett(x - (1 if f < 0.5 else 0), y, x + 1, y + 1, tr)
        for i in range(10):  # qualche foglia rada
            t.ellisse(rnd.randint(8, 40), rnd.randint(8, 40), 2, 1.5, medio)
    else:  # tondo
        t.rett(21, 56, 27, 96, tr)
        t.rett(21, 56, 23, 96, trc)
        t.rett(18, 92, 30, 96, tr)
        for cx, cy, r in ((24, 30, 20), (12, 42, 11), (36, 42, 11), (24, 14, 13)):
            t.ellisse(cx, cy, r, r * 0.9, scuro)
        for cx, cy, r in ((22, 27, 15), (13, 38, 7), (33, 37, 8), (23, 13, 9)):
            t.ellisse(cx, cy, r, r * 0.9, medio)
        for i in range(14):
            x, y = rnd.randint(10, 34), rnd.randint(6, 34)
            t.p(x, y, chiaro); t.p(x + 1, y, chiaro)
    t.contorno(s["cont"])
    return t


def edificio(s, rnd, w, h, piani_px, nome):
    t = Tela(w, h)
    muro_a, muro_b = s["muro"]
    spento, *accese = s["finestra"]
    margine = 2
    tetto_h = 4
    # Facciata: trama Bayer dal muro chiaro (alto) allo scuro (basso), un solo
    # campo tramato per immagine (regole dithering 29/10).
    t.trama(margine, tetto_h, w - margine, h, muro_b, muro_a, 0.15, 0.85)
    t.rett(margine, 0, w - margine, tetto_h, s["tetto"])
    t.rett(margine + 2, tetto_h, w - margine - 2, tetto_h + 1, s["cont"])
    # Finestre: griglia regolare, alcune accese col colore del tema.
    fw, fh = (4, 5) if w <= 64 else (6, 4)
    passo_x = fw + 3
    nx = (w - 2 * margine - 4) // passo_x
    off = (w - nx * passo_x) // 2 + 1
    y = tetto_h + 5
    while y + fh < h - 14:
        for i in range(nx):
            col = rnd.choice(accese) if rnd.random() < 0.45 else spento
            t.rett(off + i * passo_x, y, off + i * passo_x + fw, y + fh, col)
        y += piani_px
    # Ingresso e zoccolo.
    t.rett(margine, h - 6, w - margine, h, s["cont"])
    pw = 10 if w <= 64 else 16
    t.rett(w // 2 - pw // 2, h - 13, w // 2 + pw // 2, h, accese[0])
    t.rett(w // 2 - pw // 2 + 1, h - 12, w // 2 + pw // 2 - 1, h, spento)
    if nome == "edificio_alto" and s["albero"] in ("palma", "conifera"):  # antenna in cima
        t.rett(w // 2, 0, w // 2 + 1, 0, s["tetto"])
    t.contorno(s["cont"])
    return t


def freccia(dx):
    # Cartello di curva: pannello giallo, freccia nera, palo. dx=True -> punta a destra.
    t = Tela(32, 40)
    giallo, nero, palo = c("FFD100"), c("101010"), c("8A8A8A")
    t.rett(14, 22, 18, 40, palo)
    t.rett(1, 2, 31, 24, nero)
    t.rett(2, 3, 30, 23, giallo)
    for k in range(2):  # due chevron
        for i in range(9):
            xa = (7 + k * 9 + (i if i <= 4 else 8 - i)) if dx else (24 - k * 9 - (i if i <= 4 else 8 - i))
            t.rett(xa - 1, 5 + i * 2, xa + 2, 7 + i * 2, nero)
    return t


def salva(tela, png, raw, forza):
    png.parent.mkdir(parents=True, exist_ok=True)
    if forza or not png.exists():
        tela.img.save(png)
        print(f"disegnato {png}")
    img = Image.open(png).convert("RGBA")  # sempre dal PNG presente (anche quello di Andrea)
    dati = b"SGLR" + struct.pack("<II", img.width, img.height) + img.tobytes()
    raw.parent.mkdir(parents=True, exist_ok=True)
    if not raw.exists() or raw.read_bytes() != dati:
        raw.write_bytes(dati)
        print(f"raw {raw} ({img.width}x{img.height})")


def main():
    forza = "--forza" in sys.argv
    for livello, cartella in CARTELLA.items():
        s = STILI[livello]
        rnd = random.Random(livello)  # stesso disegno a ogni lancio
        base = SCENARI / livello / cartella / "oggetti"
        pezzi = {
            "ciuffo": ciuffo(s, rnd),
            "cespuglio": cespuglio(s, rnd),
            "albero": albero(s, rnd),
            "edificio_alto": edificio(s, rnd, 64, 128, 9, "edificio_alto"),
            "edificio_medio": edificio(s, rnd, 64, 96, 10, "edificio_medio"),
            "edificio_basso": edificio(s, rnd, 96, 64, 9, "edificio_basso"),
        }
        for nome, tela in pezzi.items():
            salva(tela, base / f"obj_{nome}.png", RAW / livello / f"{nome}.sglraw", forza)
    comuni = SCENARI / "comuni" / "oggetti"
    salva(freccia(True), comuni / "freccia_curva_dx.png", RAW / "comuni" / "freccia_curva_dx.sglraw", forza)
    salva(freccia(False), comuni / "freccia_curva_sx.png", RAW / "comuni" / "freccia_curva_sx.sglraw", forza)


if __name__ == "__main__":
    main()
