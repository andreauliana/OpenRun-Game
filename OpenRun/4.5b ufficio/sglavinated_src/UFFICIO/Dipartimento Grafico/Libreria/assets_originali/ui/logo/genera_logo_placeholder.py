#!/usr/bin/env python3
# Logo SEGNAPOSTO "OPEN RUN" per il menu (Andrea 08/11): solo la scritta,
# niente altre parole ne' decorazioni. Andrea lo sostituira' col suo logo:
# basta ridisegnare logo_openrun.png (stesso nome) e rilanciare questo script
# con --solo-raw, oppure convertire a mano in sglraw.
# Uso (da sglavinated_src): python3 "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/ui/logo/genera_logo_placeholder.py" [--solo-raw]
import struct, sys
from pathlib import Path
from PIL import Image

QUI = Path(__file__).resolve().parent
RADICE = QUI.parents[5]
PNG = QUI / "logo_openrun.png"
RAW = RADICE / "motore_nuovo" / "dati" / "sprite_raw" / "ui" / "logo_openrun.sglraw"

G = {
 'O': [" ##### ", "##   ##", "##   ##", "##   ##", "##   ##", "##   ##", " ##### "],
 'P': ["###### ", "##   ##", "##   ##", "###### ", "##     ", "##     ", "##     "],
 'E': ["#######", "##     ", "##     ", "###### ", "##     ", "##     ", "#######"],
 'N': ["##   ##", "###  ##", "#### ##", "## ####", "##  ###", "##   ##", "##   ##"],
 'R': ["###### ", "##   ##", "##   ##", "###### ", "## ##  ", "##  ## ", "##   ##"],
 'U': ["##   ##", "##   ##", "##   ##", "##   ##", "##   ##", "##   ##", " ##### "],
 ' ': ["    "] * 7,
}
S = 3                    # pixel per punto del glifo
BIANCO = (240, 240, 248, 255)
OMBRA = (30, 20, 60, 255)

def disegna():
    testo = "OPEN RUN"
    larg = sum(len(G[c][0]) + 1 for c in testo) - 1
    w, h = larg * S + 2, 7 * S + 2      # +2: bordo e ombra di 1-2 px
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    px = img.load()
    punti = set()
    x0 = 0
    for c in testo:
        for r, riga in enumerate(G[c]):
            for k, ch in enumerate(riga):
                if ch == '#':
                    for dy in range(S):
                        for dx in range(S):
                            punti.add((x0 + k * S + dx, r * S + dy))
        x0 += (len(G[c][0]) + 1) * S
    for (x, y) in punti:                     # ombra in basso a destra
        for dx, dy in ((1, 1), (2, 2), (2, 1), (1, 2)):
            if (x + dx, y + dy) not in punti and x + dx < w and y + dy < h:
                px[x + dx, y + dy] = OMBRA
    for (x, y) in punti:
        px[x, y] = BIANCO
    img.save(PNG)

def a_raw():
    img = Image.open(PNG).convert("RGBA")
    RAW.parent.mkdir(parents=True, exist_ok=True)
    with open(RAW, "wb") as f:
        f.write(b"SGLR" + struct.pack("<II", img.width, img.height) + img.tobytes())

if __name__ == "__main__":
    if "--solo-raw" not in sys.argv:
        disegna()
    a_raw()
    print(PNG, "->", RAW)
