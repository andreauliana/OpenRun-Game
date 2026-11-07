#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""genera_splash.py - schermata splash/avvio (R2 punto 3, passo 1 di N).

Tema B/N (decisione D7), risoluzione 320x224, testo rasterizzato con
Antonio Bold (OFL-1.1, Impallari Type) + soglia -> solo nero/bianco.
Output:
  UFFICIO/Dipartimento Grafico/Libreria/assets_originali/ui/splash/splash_avvio.png

Prima bozza: da approvare da Andrea. Il menu/logo restano segnaposto Andrea (LOOP1).
Il catalogo e la voce manifest vengono registrati al passo successivo.
"""
import pathlib
from PIL import Image, ImageDraw, ImageFont

try:
    LANCZOS = Image.Resampling.LANCZOS
except AttributeError:  # pragma: no cover
    LANCZOS = Image.LANCZOS

ROOT = pathlib.Path(__file__).resolve().parents[3]  # sglavinated_src (script in TOOLS/)
UI = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "ui"
ANTONIO = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "font" / "antonio" / "Antonio-Bold.ttf"

NERO = (0, 0, 0)
BIANCO = (255, 255, 255)
TARGET_W, TARGET_H = 320, 224
MAX_COLORI = 16


def canvas(w, h, colore=NERO):
    return Image.new("RGBA", (w, h), colore + (255,))


def salva(img, path):
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path)


def testo_soglia(testo, font, colore):
    """Rasterizza testo con Antonio Bold, soglia 128, restituisce RGBA colore su trasparente."""
    bbox = font.getbbox(testo)
    w = max(1, bbox[2] - bbox[0])
    h = max(1, bbox[3] - bbox[1])
    big_w = max(1, (bbox[2] - bbox[0]) * 4)
    big_h = max(1, (bbox[3] - bbox[1]) * 4)
    big = Image.new("L", (big_w, big_h), 0)
    ImageDraw.Draw(big).text((-bbox[0] * 4, -bbox[1] * 4), testo, font=font, fill=255)
    maschera = big.resize((w, h), LANCZOS).point(lambda v: 255 if v >= 128 else 0)
    out = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    out.paste(Image.new("RGBA", (w, h), colore + (255,)), (0, 0), maschera)
    return out


def main():
    if not ANTONIO.exists():
        raise RuntimeError(f"font mancante: {ANTONIO}")
    img = canvas(TARGET_W, TARGET_H)

    f_tit = ImageFont.truetype(str(ANTONIO), 40)
    f_car = ImageFont.truetype(str(ANTONIO), 22)
    f_pic = ImageFont.truetype(str(ANTONIO), 16)

    tit = testo_soglia("Sglavinated Edition", f_tit, BIANCO)
    img.paste(tit, ((TARGET_W - tit.size[0]) // 2, 34), tit)

    car = testo_soglia("LOADING", f_car, BIANCO)
    img.paste(car, ((TARGET_W - car.size[0]) // 2, 112), car)

    pic = testo_soglia("Caricamento asset...", f_pic, BIANCO)
    img.paste(pic, ((TARGET_W - pic.size[0]) // 2, 150), pic)

    # Barra di caricamento (contorno bianco).
    bx, by, bw, bh = 40, 186, 240, 10
    sp = 2
    for py in range(by, by + bh):
        for px in range(bx, bx + bw):
            if px < bx + sp or px >= bx + bw - sp or py < by + sp or py >= by + bh - sp:
                img.putpixel((px, py), BIANCO + (255,))

    out = pathlib.Path("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/ui/splash/splash_avvio.png")
    salva(img, out)
    ricaricato = Image.open(out)
    cols = ricaricato.convert("RGB").getcolors(1024)
    nomi = sorted(c for _, c in cols)
    cromatici = [c for c in nomi if not (c[0] == c[1] == c[2])]
    assert not cromatici, f"colori cromatici {cromatici}"
    assert len(nomi) <= MAX_COLORI, f"{len(nomi)} colori > {MAX_COLORI}"
    assert ricaricato.size == (TARGET_W, TARGET_H), ricaricato.size
    print(f"OK {out}  {ricaricato.size[0]}x{ricaricato.size[1]}  {len(nomi)} colori  {nomi}")
    print("fatto: splash B/N generato")


if __name__ == "__main__":
    main()
