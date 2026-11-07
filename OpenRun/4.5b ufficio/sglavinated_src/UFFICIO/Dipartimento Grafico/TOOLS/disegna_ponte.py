#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Dintorni del ponte (Andrea 07/11: "entrata e uscita dal ponte piu'
# interessanti"): segnaposto comuni a tutti i temi per il mare e la riva sotto
# il ponte alto. Pochi colori fissi, trama Bayer 4x4 solo sulle superfici grandi
# (torre del faro, vela, scoglio). Disegnati da zero, nessun materiale Sega.
#
# Andrea li ridisegna a mano: stessi nomi e stesse misure. Lo strumento NON
# sovrascrive mai un PNG che esiste gia', salvo --forza. Scrive sempre il raw
# del motore (formato SGLR) dal PNG presente.
#
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_ponte.py"
#
# Misure (larghezza x altezza, pixel), tutte in scenari/comuni/oggetti/:
#   boa 16x32, barca 48x48, scoglio 48x24, faro 32x128, ombrellone 32x32.
# La misura a schermo si ricava dall'altezza del PNG (paesaggio.cpp).
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from disegna_paesaggio import Tela, c, salva, SCENARI, RAW, BAYER4  # noqa: E402

BIANCO, ROSSO, SCURO = c("F2F2EE"), c("D8343A"), c("1C1C28")
GRIGIO, CHIARO, GIALLO = c("7C7C8C"), c("B4B4C0"), c("FFD84A")
ROCCIA_S, ROCCIA, ROCCIA_L = c("4A4652"), c("6E6878"), c("948C9C")
VELA, VELA_O, LEGNO, CIANO = c("EDE6D2"), c("BDB49E"), c("7A4A2A"), c("2AB4C8")


def bayer(t, x0, y0, x1, y1, a, b, quota):
    """Trama verticale: a sinistra a, verso destra b per la quota indicata."""
    for y in range(y0, y1):
        for x in range(x0, x1):
            s = (x - x0) / max(1, x1 - x0 - 1)
            if s > 1 - quota and (BAYER4[y % 4][x % 4] + 0.5) / 16 < (s - (1 - quota)) / quota:
                t.p(x, y, b)
            else:
                t.p(x, y, a)


def boa():
    t = Tela(16, 32)
    t.rett(7, 2, 9, 9, SCURO)              # asta con la luce
    t.rett(6, 0, 10, 3, GIALLO)
    for y in range(9, 24):                  # corpo a cono, fasce rosse e bianche
        mezzo = 2 + (y - 9) * 5 // 15
        col = ROSSO if ((y - 9) // 4) % 2 == 0 else BIANCO
        t.rett(8 - mezzo, y, 8 + mezzo, y + 1, col)
    t.rett(1, 24, 15, 28, SCURO)            # galleggiante
    t.rett(0, 28, 16, 30, CHIARO)           # schiuma
    t.contorno(SCURO)
    return t


def barca():
    t = Tela(48, 48)
    t.rett(23, 2, 25, 36, LEGNO)            # albero
    for y in range(4, 34):                  # vela grande (triangolo) a trama sul lato in ombra
        w = (y - 4) * 18 // 30
        for x in range(25, 26 + w):
            ombra = x > 25 + w * 0.6 and (BAYER4[y % 4][x % 4] + 0.5) / 16 < 0.5
            t.p(x, y, VELA_O if ombra else VELA)
    for y in range(10, 34):                 # fiocco
        w = (y - 10) * 12 // 24
        t.rett(22 - w, y, 23, y + 1, VELA)
    t.rett(22, 26, 40, 28, ROSSO)           # riga sulla vela
    for y in range(36, 44):                 # scafo
        r = (y - 36) // 2
        t.rett(6 + r, y, 44 - r, y + 1, BIANCO if y < 39 else CIANO)
    t.rett(4, 44, 44, 46, CHIARO)           # scia
    t.contorno(SCURO)
    return t


def scoglio():
    t = Tela(48, 24)
    t.ellisse(24, 16, 22, 9, ROCCIA)
    t.ellisse(15, 13, 11, 9, ROCCIA)
    t.ellisse(32, 15, 10, 7, ROCCIA)
    for y in range(24):                     # luce dall'alto a sinistra, ombra a trama a destra
        for x in range(48):
            if t.px[x, y][3] == 0:
                continue
            if y < 10 and x < 22:
                t.p(x, y, ROCCIA_L)
            elif x > 30 and (BAYER4[y % 4][x % 4] + 0.5) / 16 < (x - 30) / 18:
                t.p(x, y, ROCCIA_S)
    t.rett(0, 21, 48, 24, CHIARO)           # schiuma alla base
    t.contorno(SCURO)
    return t


def faro():
    t = Tela(32, 128)
    t.rett(14, 0, 18, 4, ROSSO)             # cupola
    t.rett(10, 4, 22, 6, SCURO)
    t.rett(11, 6, 21, 16, GIALLO)           # lanterna accesa
    t.rett(15, 6, 17, 16, BIANCO)
    t.rett(8, 16, 24, 20, SCURO)            # ballatoio
    for y in range(20, 116):                # torre rastremata a fasce, ombra a trama
        mezzo = 6 + (y - 20) * 6 // 96
        col = ROSSO if ((y - 20) // 16) % 2 == 0 else BIANCO
        ombra = c("8E2228") if col == ROSSO else CHIARO
        bayer(t, 16 - mezzo, y, 16 + mezzo, y + 1, col, ombra, 0.45)
    for y in (40, 72, 100):                 # finestrelle
        t.rett(15, y, 17, y + 4, SCURO)
    t.rett(4, 116, 28, 120, GRIGIO)         # basamento
    t.rett(2, 120, 30, 128, ROCCIA)
    t.contorno(SCURO)
    return t


def ombrellone():
    t = Tela(32, 32)
    t.rett(15, 10, 17, 30, LEGNO)
    for y in range(2, 12):                  # tela a spicchi rossi e bianchi
        mezzo = 3 + (y - 2) * 13 // 10
        for x in range(16 - mezzo, 16 + mezzo):
            t.p(x, y, ROSSO if ((x - 16) // 4) % 2 == 0 else BIANCO)
    t.rett(6, 30, 26, 32, c("C8B07A"))      # ombra sulla sabbia
    t.contorno(SCURO)
    return t


def main():
    forza = "--forza" in sys.argv
    comuni = SCENARI / "comuni" / "oggetti"
    for nome, fn in (("boa", boa), ("barca", barca), ("scoglio", scoglio), ("faro", faro), ("ombrellone", ombrellone)):
        salva(fn(), comuni / f"{nome}.png", RAW / "comuni" / f"{nome}.sglraw", forza)


if __name__ == "__main__":
    main()
