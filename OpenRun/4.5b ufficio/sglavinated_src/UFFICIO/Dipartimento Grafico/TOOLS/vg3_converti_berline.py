#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico, 20/11 10:41): converte SOLO i ridisegni di Andrea di
# lato_berlina_gialla.png (10:30) e lato_berlina_verde.png (10:21) nei raw
# che legge il motore, riusando salva_raw() del generatore del catalogo
# (GRAFICA/sprite/traffico_incrocio/). Non ritocca i PNG ne' l'altro raw.
# lato_furgone_bianco.png.png (doppia estensione, 10:42) non e' un nome noto
# al catalogo: va segnalato ad Andrea, non rinominato ne' convertito qui.
# Uso (da sglavinated_src):
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_converti_berline.py"
import importlib.util
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
GEN = RADICE / "GRAFICA/sprite/traffico_incrocio/genera_traffico_incrocio.py"
QUI = GEN.parent
OUT = RADICE / "motore_nuovo/dati/sprite_raw/traffico_incrocio"
NOMI = ("lato_berlina_gialla", "lato_berlina_verde")


def carica_generatore():
    spec = importlib.util.spec_from_file_location("genera_traffico_incrocio", GEN)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def main():
    mod = carica_generatore()
    esito = 0
    for nome in NOMI:
        png = QUI / (nome + ".png")
        raw = OUT / (nome + ".sglraw")
        if not png.exists():
            print(f"MANCA PNG {png.relative_to(RADICE)}: niente da convertire")
            esito = 1
            continue
        prima = raw.stat().st_size if raw.exists() else 0
        mod.salva_raw(png, raw)
        dopo = raw.stat().st_size
        print(f"convertito {png.relative_to(RADICE)} -> {raw.relative_to(RADICE)} "
              f"({prima} -> {dopo} byte)")
    return esito


if __name__ == "__main__":
    raise SystemExit(main())
