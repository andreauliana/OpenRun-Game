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
#   boa 16x32, barca 64x64, motoscafo 64x32, scoglio 48x24, faro 32x128, ombrellone 32x32.
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
    # Barca a vela 64x64 (Andrea 07/11: piu' grande e dettagliata): scafo
    # bianco con riga blu e oblo', tuga, albero con sartie, randa con stecche
    # e ombra a trama, fiocco, bandierina, scia di schiuma.
    t = Tela(64, 64)
    BLU, BLU_S, VETRO = c("2A4A9A"), c("1A2E66"), c("8FD8F0")
    t.rett(31, 2, 33, 47, LEGNO)            # albero
    t.rett(33, 2, 38, 5, ROSSO)             # bandierina
    for y in range(6, 45):                  # sartie: un filo sottile ai due lati
        t.p(31 - (y - 6) * 14 // 39, y, GRIGIO)
    for y in range(5, 44):                  # randa: stecche e ombra a trama verso la balumina
        w = (y - 5) * 24 // 39
        for x in range(33, 34 + w):
            ombra = x > 33 + w * 0.55 and (BAYER4[y % 4][x % 4] + 0.5) / 16 < (x - 33 - w * 0.55) / max(1, w * 0.45)
            t.p(x, y, VELA_O if ombra else VELA)
        if y in (14, 23, 32):
            t.rett(33, y, 34 + w, y + 1, VELA_O)
    t.rett(33, 43, 58, 45, LEGNO)           # boma
    for y in range(12, 44):                 # fiocco
        w = (y - 12) * 15 // 32
        for x in range(30 - w, 31):
            t.p(x, y, VELA if x > 30 - w * 0.6 else VELA_O)
    t.rett(36, 36, 56, 38, ROSSO)           # riga rossa sulla randa
    t.rett(24, 41, 40, 47, BIANCO)          # tuga con finestrini
    for x in (26, 31, 36):
        t.rett(x, 43, x + 3, 45, VETRO)
    for y in range(47, 57):                 # scafo: prua a destra, riga blu, oblo'
        r = (y - 47) * 2 // 3
        t.rett(4 + r, y, 62 - r * 2 // 3 - (3 if y > 52 else 0), y + 1, BIANCO if y < 51 else (BLU if y < 53 else BLU_S))
    for x in (16, 24, 32, 40):
        t.p(x, 49, VETRO); t.p(x + 1, 49, VETRO)
    for x in range(2, 62):                  # scia e schiuma a trama
        for y in range(57, 61):
            if (BAYER4[y % 4][x % 4] + 0.5) / 16 < (0.8 if y < 59 else 0.35):
                t.p(x, y, CHIARO if y > 58 else BIANCO)
    t.contorno(SCURO)
    return t


def motoscafo():
    # Motoscafo 64x32: scafo rosso e bianco, parabrezza, pilota, onda di prua.
    t = Tela(64, 32)
    VETRO, PELLE = c("8FD8F0"), c("E0A878")
    t.rett(30, 8, 34, 11, PELLE)            # testa del pilota
    t.rett(29, 11, 35, 16, c("2A4A9A"))
    for y in range(8, 16):                  # parabrezza inclinato
        t.rett(36 + (16 - y) // 2, y, 41 + (16 - y) // 2, y + 1, VETRO)
    t.rett(14, 14, 50, 17, BIANCO)          # coperta
    for y in range(17, 25):                 # scafo a cuneo, prua a destra
        r = (y - 17)
        t.rett(8 + r // 2, y, 60 - r * 2, y + 1, ROSSO if y < 20 else (BIANCO if y < 22 else c("8E2228")))
    t.rett(4, 18, 9, 22, SCURO)             # motore fuoribordo
    for x in range(0, 64):                  # scia bianca e onda di prua
        for y in range(25, 31):
            if (BAYER4[y % 4][x % 4] + 0.5) / 16 < (0.9 - x / 80 if y < 28 else 0.3):
                t.p(x, y, BIANCO if y < 28 else CHIARO)
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
    for nome, fn in (("boa", boa), ("barca", barca), ("motoscafo", motoscafo), ("scoglio", scoglio), ("faro", faro), ("ombrellone", ombrellone)):
        salva(fn(), comuni / f"{nome}.png", RAW / "comuni" / f"{nome}.sglraw", forza)


if __name__ == "__main__":
    main()
