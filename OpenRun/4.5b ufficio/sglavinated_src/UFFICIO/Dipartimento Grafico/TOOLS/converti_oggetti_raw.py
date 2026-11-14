#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Riconverte gli oggetti bordo strada della Libreria nei raw che il motore legge
# (motore_nuovo/dati/sprite_raw/scenari/<livello>/<nome>.sglraw), cosi' i PNG
# ridisegnati dal grafico (es. oggetti alti 27/10) arrivano davvero nel gioco.
# Per ogni raw oggetto gia' presente cerca obj_<nome>.png nella variante A del
# livello e lo riscrive se il PNG e' diverso. Formato: "SGLR" + u32 LE w + u32 LE h + RGBA8888.
# Scrittura atomica (file temporaneo + os.replace): il motore non vede mai un raw
# a meta' se la conversione viene interrotta.
# Un PNG che PIL non riesce ad aprire (salvato a meta', troncato) non ferma gli
# altri: si segnala PNG-ILLEGGIBILE, si salta il file e l'exit finale e' 1.
# Dopo ogni scrittura il raw viene riletto e confrontato: VERIFICA-FALLITA se
# sul disco non c'e' esattamente quello che si e' scritto (exit 1).
# Lanciare da qualunque cartella (radice ricavata dal percorso dello script):
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py"
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py" --autotest
import os
import struct
import tempfile
from pathlib import Path
from PIL import Image

RADICE = Path(__file__).resolve().parents[3]
SCENARI = RADICE / "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari"
RAW = RADICE / "motore_nuovo/dati/sprite_raw/scenari"


def png_variante_a(livello, nome):
    for variante in sorted((SCENARI / livello).glob("*varianteA")):
        png = variante / "oggetti" / f"obj_{nome}.png"
        if png.exists():
            return png
    return None


def leggi_png(png):
    """Byte SGLR attesi per il PNG, o (None, None) se PIL non lo legge."""
    try:
        with Image.open(png) as im:
            rgba = im.convert("RGBA")
            return b"SGLR" + struct.pack("<II", rgba.width, rgba.height) + rgba.tobytes(), rgba.size
    except (OSError, ValueError):
        return None, None


def scrivi_raw(dst, dati):
    """Scrittura atomica: tmp nella stessa cartella, poi os.replace."""
    dst.parent.mkdir(parents=True, exist_ok=True)
    fd, tmp = tempfile.mkstemp(dir=str(dst.parent), prefix=dst.name + ".", suffix=".tmp")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(dati)
        os.replace(tmp, dst)
    except BaseException:
        if os.path.exists(tmp):
            os.unlink(tmp)
        raise


def converti_comuni(errori):
    """Oggetti comuni a tutti i temi (PIST1, passo G-P1 @grafico): parapetto del
    ponte e guardrail dell'autostrada, da scenari/comuni/oggetti/*.png a
    sprite_raw/scenari/comuni/<nome>.sglraw (lo stesso formato SGLR)."""
    src_dir = SCENARI / "comuni" / "oggetti"
    if not src_dir.is_dir():
        return 0
    dst_dir = RAW / "comuni"
    n = 0
    for png in sorted(src_dir.glob("*.png")):
        dati, size = leggi_png(png)
        if dati is None:
            errori.append(png)
            print(f"PNG-ILLEGGIBILE {png}: non apribile da PIL, saltato")
            continue
        dst = dst_dir / (png.stem + ".sglraw")
        if dst.exists() and dst.read_bytes() == dati:
            continue
        scrivi_raw(dst, dati)
        if dst.read_bytes() != dati:
            errori.append(dst)
            print(f"VERIFICA-FALLITA {dst}")
            continue
        print(f"scritto {dst} ({size[0]}x{size[1]}) da {png}")
        n += 1
    return n


def main():
    n = 0
    errori = []
    for raw in sorted(RAW.glob("*/*.sglraw")):
        nome, livello = raw.stem, raw.parent.name
        if nome == "skyline":
            continue
        png = png_variante_a(livello, nome)
        if png is None:
            continue
        dati, size = leggi_png(png)
        if dati is None:
            errori.append(png)
            print(f"PNG-ILLEGGIBILE {png}: non apribile da PIL, saltato ({raw.name} invariato)")
            continue
        if raw.read_bytes() == dati:
            continue
        scrivi_raw(raw, dati)
        if raw.read_bytes() != dati:
            errori.append(raw)
            print(f"VERIFICA-FALLITA {raw}")
            continue
        print(f"aggiornato {raw} ({size[0]}x{size[1]}) da {png}")
        n += 1
    n += converti_comuni(errori)
    print(f"raw oggetti aggiornati: {n}")
    print(f"errori: {len(errori)}")
    return 0 if not errori else 1


def autotest():
    """Percorso negativo e scrittura atomica, in una cartella temporanea."""
    with tempfile.TemporaryDirectory() as td:
        base = Path(td)
        buono = base / "buono.png"
        rotto = base / "rotto.png"
        Image.new("RGBA", (3, 2), (10, 20, 30, 255)).save(buono)
        rotto.write_bytes(buono.read_bytes()[:40])
        dati, size = leggi_png(buono)
        ok = (
            dati is not None
            and dati[:4] == b"SGLR"
            and struct.unpack("<II", dati[4:12]) == (3, 2)
            and size == (3, 2)
            and leggi_png(rotto) == (None, None)
        )
        dst = base / "nuova" / "prova.sglraw"
        scrivi_raw(dst, dati or b"")
        ok = ok and dati is not None and dst.read_bytes() == dati
        ok = ok and not [p for p in (base / "nuova").glob("*.tmp")]
        print("autotest conversione: " + ("OK" if ok else "FALLITO"))
        return 0 if ok else 1


if __name__ == "__main__":
    import sys

    if "--autotest" in sys.argv:
        raise SystemExit(autotest())
    raise SystemExit(main())
