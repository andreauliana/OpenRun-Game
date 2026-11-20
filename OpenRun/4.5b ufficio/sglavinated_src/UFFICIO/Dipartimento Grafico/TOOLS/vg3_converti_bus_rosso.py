#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico, 20/11 10:18): converte SOLO il ridisegno di Andrea di
# lato_bus_rosso.png nel raw che legge il motore, riusando la funzione salva_raw()
# del generatore del catalogo (GRAFICA/sprite/traffico_incrocio/).
# Non si puo' lanciare genera_traffico_incrocio.py --solo-raw perche' attraversa
# anche lato_berlina_verde, il cui PNG non c'e' piu' (raw senza sorgente, da
# segnalare ad Andrea, non da ricreare). Qui si tocca un solo file.
# Uso (da sglavinated_src):
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_converti_bus_rosso.py"
import importlib.util
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
GEN = RADICE / "GRAFICA/sprite/traffico_incrocio/genera_traffico_incrocio.py"
QUI = GEN.parent
OUT = RADICE / "motore_nuovo/dati/sprite_raw/traffico_incrocio"


def carica_generatore():
    spec = importlib.util.spec_from_file_location("genera_traffico_incrocio", GEN)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def main():
    mod = carica_generatore()
    nome = "lato_bus_rosso"
    png = QUI / (nome + ".png")
    raw = OUT / (nome + ".sglraw")
    if not png.exists():
        print(f"MANCA PNG {png}: niente da convertire")
        return 1
    mod.salva_raw(png, raw)
    print(f"convertito {png.relative_to(RADICE)} -> {raw.relative_to(RADICE)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
