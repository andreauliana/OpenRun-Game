#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico): confronta i PNG di Andrea in
# UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari/ con i raw che
# il motore legge (motore_nuovo/dati/sprite_raw/scenari/).
# Per ogni raw cerca il PNG sorgente (stessa regola di converti_oggetti_raw.py:
# varianteA/oggetti/obj_<nome>.png, oppure comuni/oggetti/<nome>.png) e dice se il
# PNG e' piu' nuovo del raw e se il raw e' ancora identico al PNG convertito.
# Gli skyline non si confrontano a byte: sono trame composte da trama_scenari.py.
# Per loro vale solo la data (PNG del disegno vs skyline.sglraw del gioco).
# Per gli altri gruppi 1:1 (rivali, traffico, incrocio, pubblico, ui, sponsor) vale
# la stessa regola: contenuto diverso = da convertire. Un PNG senza raw non blocca
# l'exit: non e' un asset che il motore legge (es. residuo auto_giocatore_sbandata).
# Il gruppo giocatore resta fuori del tutto: e' area bloccata (auto di Andrea).
# In fondo c'e' il controllo di copertura: ogni PNG della Libreria deve essere o
# confrontato con un raw, o appartenere a una categoria non-motore nota (anteprime
# UI, font, area bloccata di Andrea, varianti legacy). Un PNG nuovo in un percorso
# non mappato fa fallire il controllo: cosi' un ridisegno di Andrea fuori posto non
# resta invisibile.
# Non scrive nulla: solo diagnosi. Exit 0 se tutto allineato, 1 se c'e' da convertire
# o se un gruppo di dati/sprite_raw non ha una regola di conversione.
# Un PNG che PIL non riesce ad aprire (salvato a meta', troncato) non fa crashare il
# controllo: si segnala PNG-ILLEGGIBILE, conta come errore (exit 1), e gli altri esiti
# restano visibili.
# Lanciare da sglavinated_src/:
#   python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"
import fnmatch
import struct
from datetime import datetime
from pathlib import Path

from PIL import Image

RADICE = Path(__file__).resolve().parents[3]
SCENARI = RADICE / "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari"
RAW = RADICE / "motore_nuovo/dati/sprite_raw/scenari"
RAW_ROOT = RADICE / "motore_nuovo/dati/sprite_raw"
# Gruppi con PNG -> raw 1:1 fuori da scenari/. Il gruppo giocatore resta fuori:
# l'auto del giocatore la disegna Andrea e i suoi file sono bloccati (solo lettura).
ALTRI = (
    ("sprite/rivali", "rivali"),
    ("sprite/traffico", "traffico"),
    ("sprite/traffico_incrocio", "traffico_incrocio"),
    ("sprite/pubblico", "pubblico"),
    ("ui/logo", "ui"),
)
# Stesso elenco di trama_scenari.py: chi disegna lo skyline di ogni livello.
SKYLINE = {
    "lv_01_neon": "scenari/lv_01_neon/scenario_neon_01_varianteA/skyline.png",
    "lv_02_atomico": "scenari/lv_02_atomico/scenario_atomico_02_varianteA/skyline.png",
    "lv_03_foglie": "scenari/lv_03_foglie/scenario_foglie_03_varianteA/skyline.png",
    "lv_04_cromo": "skyline/varianteA_futurista/skyline.png",
    "lv_05_spray": "scenari/lv_05_spray/scenario_spray_05_varianteA/skyline.png",
    "lv_06_desktop": "scenari/lv_06_desktop/scenario_desktop_06_varianteA/skyline.png",
}
ASSETS = RADICE / "UFFICIO/Dipartimento Grafico/Libreria/assets_originali"
# PNG senza raw e senza riferimento nel motore (residui di vecchie varianti, non
# cancellabili da qui). Si segnalano ma non contano come guasto: nessuna conversione.
RESIDUI_NON_MOTORE = {"sprite/rivali/auto_giocatore_sbandata.png"}
# PNG della Libreria che il motore non legge come sprite: anteprime UI generate,
# font (il motore usa il .ttf), area bloccata di Andrea (auto e pose), varianti
# legacy non usate. Pattern sul percorso relativo a assets_originali/.
NON_MOTORE = (
    "ui/font/*/font.png",
    "ui/hud/*/hud_preview.png",
    "ui/menu/*/menu_preview.png",
    "ui/risultati/*/risultati_preview.png",
    "ui/splash/splash_avvio.png",
    "ui/titolo/*/title.png",
    "sprite/giocatore/*.png",
    "sprite/giocatore_pose/*.png",
    "sprite/auto_gt/*.png",
    "sprite/auto/variante*/auto.png",
    "palette/*/palette.png",
    "skyline/*/skyline_trama.png",
    "skyline/varianteB_*/skyline.png",
    "skyline/varianteC_*/skyline.png",
)


def data(p):
    return datetime.fromtimestamp(p.stat().st_mtime).strftime("%Y-%m-%d %H:%M")


def png_sorgente(nome, livello):
    if livello == "comuni":
        png = SCENARI / "comuni" / "oggetti" / f"{nome}.png"
        return png if png.exists() else None
    for variante in sorted((SCENARI / livello).glob("*varianteA")):
        png = variante / "oggetti" / f"obj_{nome}.png"
        if png.exists():
            return png
    return None


def raw_atteso(png):
    """Byte del raw che il motore si aspetta, o None se il PNG non si legge."""
    try:
        with Image.open(png) as im:
            rgba = im.convert("RGBA")
            return b"SGLR" + struct.pack("<II", rgba.width, rgba.height) + rgba.tobytes()
    except (OSError, ValueError):
        return None


def png_oggetti():
    """Inverso di png_sorgente(): ogni PNG di oggetto con il raw che gli spetta."""
    for png in sorted(SCENARI.glob("comuni/oggetti/*.png")):
        yield png, "comuni", png.stem
    for png in sorted(SCENARI.glob("*/scenario_*/oggetti/obj_*.png")):
        livello = png.parents[2].name
        yield png, livello, png.stem[len("obj_"):]


def png_altri(raw, gruppo):
    """PNG sorgente per un raw dei gruppi 1:1 (stesso nome, cartella della Libreria)."""
    for rel, g in ALTRI:
        if g == gruppo:
            png = ASSETS / rel / f"{raw.stem}.png"
            return png if png.exists() else None
    if gruppo == "sponsor":
        sponsor, _, stile = raw.stem.rpartition("_")
        png = ASSETS / "sponsor" / sponsor / f"cartellone_{stile}.png"
        return png if png.exists() else None
    return None


def png_altri_orfani():
    """Inverso: PNG dei gruppi 1:1 senza raw nel motore."""
    for rel, gruppo in ALTRI:
        for png in sorted((ASSETS / rel).glob("*.png")):
            yield png, gruppo, RAW_ROOT / gruppo / f"{png.stem}.sglraw"
    for png in sorted((ASSETS / "sponsor").glob("*/cartellone_*.png")):
        stile = png.stem[len("cartellone_"):]
        raw = RAW_ROOT / "sponsor" / f"{png.parent.name}_{stile}.sglraw"
        yield png, "sponsor", raw


def png_coperti():
    """Percorsi (relativi a assets_originali/) dei PNG gia' confrontati con un raw."""
    coperti = set()
    for png, _livello, _nome in png_oggetti():
        coperti.add(png.relative_to(ASSETS).as_posix())
    for rel, _gruppo in ALTRI:
        for png in (ASSETS / rel).glob("*.png"):
            coperti.add(png.relative_to(ASSETS).as_posix())
    for png in (ASSETS / "sponsor").glob("*/cartellone_*.png"):
        coperti.add(png.relative_to(ASSETS).as_posix())
    for rel in SKYLINE.values():
        if (ASSETS / rel).exists():
            coperti.add(rel)
    return coperti


def png_non_motore(rel):
    return any(fnmatch.fnmatchcase(rel, pat) for pat in NON_MOTORE)


# Gruppi di dati/sprite_raw che il motore legge. "giocatore" e' l'area bloccata di
# Andrea (conversione fatta a mano): resta l'unico gruppo non confrontato, ma deve
# comunque essere previsto qui. Un gruppo nuovo senza regola fa fallire il controllo:
# cosi' una cartella di raw aggiunta (o spostata) non resta invisibile come succedeva
# quando si guardavano solo i PNG della Libreria.
GRUPPI_RAW = {"giocatore", "scenari", "sponsor"} | {g for _, g in ALTRI}


def controllo_gruppi_raw():
    ignoti = 0
    for cartella in sorted(p for p in RAW_ROOT.iterdir() if p.is_dir()):
        if cartella.name not in GRUPPI_RAW:
            ignoti += 1
            print(f"RAW-GRUPPO-IGNOTO {cartella.name}: nessuna regola di conversione")
    print(f"gruppi di raw noti: {len(GRUPPI_RAW)}, sconosciuti: {ignoti}")
    return ignoti


def controllo_copertura():
    """Ogni PNG della Libreria o e' confrontato, o e' non-motore noto, o e' sospetto."""
    coperti = png_coperti()
    totale = noti = fuori = 0
    for png in sorted(ASSETS.rglob("*.png")):
        totale += 1
        rel = png.relative_to(ASSETS).as_posix()
        if rel in coperti:
            continue
        if png_non_motore(rel):
            noti += 1
            continue
        fuori += 1
        print(f"PNG-FUORI-COPERTURA {rel}: png {data(png)} (nessun raw e nessuna categoria nota)")
    print(f"png nella Libreria: {totale} (confrontati con un raw {totale - noti - fuori}, non motore noti {noti})")
    return fuori


def main():
    totale = diversi = senza_png = png_piu_nuovo = allineati = trame = 0
    righe = []
    for raw in sorted(RAW.glob("*/*.sglraw")):
        nome, livello = raw.stem, raw.parent.name
        totale += 1
        if nome == "skyline":
            trame += 1
            continue
        png = png_sorgente(nome, livello)
        if png is None:
            senza_png += 1
            righe.append(f"MANCA-PNG  {livello}/{nome}.sglraw ({data(raw)})")
            continue
        atteso = raw_atteso(png)
        if atteso is None:
            diversi += 1
            righe.append(f"PNG-ILLEGGIBILE {livello}/{nome}: png {data(png)} non apribile da PIL")
        elif raw.read_bytes() != atteso:
            diversi += 1
            righe.append(f"DIVERSO    {livello}/{nome}: png {data(png)} raw {data(raw)}")
        elif png.stat().st_mtime > raw.stat().st_mtime:
            png_piu_nuovo += 1
            righe.append(
                f"PNG-NUOVO  {livello}/{nome}: png {data(png)} raw {data(raw)} (contenuto uguale)"
            )
        else:
            allineati += 1
    for riga in righe:
        print(riga)
    # Giro inverso: un PNG nuovo di Andrea senza raw non compare nel ciclo sopra.
    orfani = png_visti = 0
    for png, livello, nome in png_oggetti():
        png_visti += 1
        raw = RAW / livello / f"{nome}.sglraw"
        if not raw.exists():
            orfani += 1
            print(f"PNG-SENZA-RAW {livello}/{nome}: png {data(png)}")
    print(f"png di oggetti controllati: {png_visti}")
    print(f"png di oggetti senza raw: {orfani}")
    skyline_png_nuovo = skyline_ok = skyline_senza = 0
    for livello, rel in SKYLINE.items():
        png = ASSETS / rel
        raw = RAW / livello / "skyline.sglraw"
        if not png.exists() or not raw.exists():
            skyline_senza += 1
            print(f"SKYLINE-MANCA {livello}: png {png.exists()} raw {raw.exists()}")
        elif png.stat().st_mtime > raw.stat().st_mtime:
            skyline_png_nuovo += 1
            print(
                f"SKYLINE-PNG-NUOVO {livello}: png {data(png)} raw {data(raw)}"
                " (rilanciare trama_scenari.py)"
            )
        else:
            skyline_ok += 1
    print(f"skyline controllati a data: {skyline_ok + skyline_png_nuovo + skyline_senza}")
    print(f"skyline allineati: {skyline_ok}")
    print(f"skyline con png piu' nuovo: {skyline_png_nuovo}")
    print(f"skyline senza png o raw: {skyline_senza}")
    # Gruppi 1:1 fuori da scenari/ (Andrea puo' ridisegnarli: si converte, non si ritocca).
    altri_tot = altri_ok = altri_diversi = altri_nuovo = altri_senza = 0
    for gruppo in [g for _, g in ALTRI] + ["sponsor"]:
        for raw in sorted((RAW_ROOT / gruppo).glob("*.sglraw")):
            altri_tot += 1
            png = png_altri(raw, gruppo)
            if png is None:
                altri_senza += 1
                print(f"ALTRI-MANCA-PNG {gruppo}/{raw.stem}: raw {data(raw)}")
                continue
            atteso = raw_atteso(png)
            if atteso is None:
                altri_diversi += 1
                print(f"ALTRI-ILLEGGIBILE {gruppo}/{raw.stem}: png {data(png)} non apribile da PIL")
            elif raw.read_bytes() != atteso:
                altri_diversi += 1
                print(f"ALTRI-DIVERSO   {gruppo}/{raw.stem}: png {data(png)} raw {data(raw)}")
            elif png.stat().st_mtime > raw.stat().st_mtime:
                altri_nuovo += 1
                print(
                    f"ALTRI-PNG-NUOVO {gruppo}/{raw.stem}: png {data(png)} raw {data(raw)}"
                    " (contenuto uguale)"
                )
            else:
                altri_ok += 1
    altri_orfani = altri_residui = 0
    for png, gruppo, raw in png_altri_orfani():
        if not raw.exists():
            if png.relative_to(ASSETS).as_posix() in RESIDUI_NON_MOTORE:
                altri_residui += 1
                print(f"ALTRI-RESIDUO {gruppo}/{png.stem}: png {data(png)} (nessun raw, non usato)")
                continue
            altri_orfani += 1
            print(f"ALTRI-PNG-SENZA-RAW {gruppo}/{png.stem}: png {data(png)}")
    print(f"altri gruppi: raw esaminati {altri_tot} (allineati {altri_ok})")
    print(f"altri gruppi: da convertire {altri_diversi}, png piu' nuovi a pari contenuto {altri_nuovo}")
    print(f"altri gruppi: raw senza png {altri_senza}, png senza raw {altri_orfani}")
    print(f"altri gruppi: residui noti non usati dal motore {altri_residui}")
    print(f"raw esaminati: {totale} (skyline a trama esclusi dal confronto a byte: {trame})")
    print(f"allineati: {allineati}")
    print(f"contenuto diverso da convertire: {diversi}")
    print(f"png piu' nuovi ma contenuto uguale: {png_piu_nuovo}")
    print(f"raw senza png sorgente: {senza_png}")
    fuori_copertura = controllo_copertura()
    gruppi_ignoti = controllo_gruppi_raw()
    return (
        0
        if diversi == 0
        and skyline_png_nuovo == 0
        and skyline_senza == 0
        and orfani == 0
        and altri_diversi == 0
        and altri_senza == 0
        and altri_orfani == 0
        and fuori_copertura == 0
        and gruppi_ignoti == 0
        else 1
    )


if __name__ == "__main__":
    import sys

    if "--autotest" in sys.argv:
        # Prova del percorso negativo senza toccare Libreria o dati/sprite_raw:
        # un PNG valido deve dare byte SGLR, uno troncato deve dare None.
        import tempfile

        with tempfile.TemporaryDirectory() as td:
            buono = Path(td) / "buono.png"
            rotto = Path(td) / "rotto.png"
            Image.new("RGBA", (2, 2), (10, 20, 30, 255)).save(buono)
            dati = buono.read_bytes()
            rotto.write_bytes(dati[: len(dati) // 2])
            ok = (raw_atteso(buono) or b"")[:4] == b"SGLR" and raw_atteso(rotto) is None
        print("autotest raw_atteso: " + ("OK" if ok else "FALLITO"))
        raise SystemExit(0 if ok else 1)
    raise SystemExit(main())