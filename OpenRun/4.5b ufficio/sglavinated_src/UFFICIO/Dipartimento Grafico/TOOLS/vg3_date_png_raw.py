#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Passo VG3 (@grafico): confronta i PNG di Andrea in
# GRAFICA/scenari/ con i raw che
# il motore legge (motore_nuovo/dati/sprite_raw/scenari/).
# Per ogni raw cerca il PNG sorgente (stessa regola di converti_oggetti_raw.py:
# varianteA/oggetti/obj_<nome>.png, oppure comuni/oggetti/<nome>.png) e dice se il
# PNG e' piu' nuovo del raw e se il raw e' ancora identico al PNG convertito.
# Gli skyline sono trame composte da trama_scenari.py: il confronto a byte diretto
# col PNG non vale. Si ricompone la trama in memoria (stesse regole e stesso
# trama.json) e si confronta col raw del gioco: se coincide, il raw e' allineato
# anche se il PNG ha una data piu' nuova (risalvataggio senza modifiche).
# Per gli altri gruppi 1:1 (rivali, traffico, incrocio, pubblico, ui, sponsor) vale
# la stessa regola: contenuto diverso = da convertire. Un PNG senza raw non blocca
# l'exit: non e' un asset che il motore legge (es. residuo auto_giocatore_sbandata).
# Del gruppo giocatore (area bloccata, auto di Andrea) non si converte nulla: si
# controlla che ogni PNG abbia il suo raw (controllo_pose_giocatore) e, se ci sono
# tutte le pose, anche che il raw sia ancora il PNG ritagliato come in
# converti_pose_auto.py; non fa mai fallire l'exit.
# In fondo c'e' il controllo di copertura: ogni PNG di GRAFICA deve essere o
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
# Le righe "png piu' nuovo ma contenuto uguale" (i risalvataggi del 16/11 20:24)
# sono rumore: senza --verbose si stampano solo i conteggi, il dettaglio per file
# si vede con --verbose. L'ultima riga stampa il codice di uscita (exit 0 = allineato).
import fnmatch
import json
import struct
import sys
from datetime import datetime
from pathlib import Path

from PIL import Image

RADICE = Path(__file__).resolve().parents[3]
SCENARI = RADICE / "GRAFICA/scenari"
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
    "lv_01_neon": "scenari/lv_01_neon/skyline.png",
    "lv_02_atomico": "scenari/lv_02_atomico/skyline.png",
    "lv_03_foglie": "scenari/lv_03_foglie/skyline.png",
    "lv_04_cromo": "scenari/lv_04_cromo/skyline.png",
    "lv_05_spray": "scenari/lv_05_spray/skyline.png",
    "lv_06_desktop": "scenari/lv_06_desktop/skyline.png",
}
ASSETS = RADICE / "GRAFICA"
# PNG senza raw e senza riferimento nel motore (residui di vecchie varianti, non
# cancellabili da qui). Si segnalano ma non contano come guasto: nessuna conversione.
RESIDUI_NON_MOTORE = {"sprite/rivali/auto_giocatore_sbandata.png"}
# PNG di GRAFICA che il motore non legge come sprite: anteprime UI generate,
# font (il motore usa il .ttf), area bloccata di Andrea (auto e pose), varianti
# legacy non usate. Pattern sul percorso relativo a GRAFICA/.
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
    "scenari/*/skyline_trama.png",  # intermedio scritto da trama_scenari.py, non letto dal motore
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
    png = SCENARI / livello / "oggetti" / f"obj_{nome}.png"
    return png if png.exists() else None


def raw_atteso(png):
    """Byte del raw che il motore si aspetta, o None se il PNG non si legge."""
    try:
        with Image.open(png) as im:
            rgba = im.convert("RGBA")
            return b"SGLR" + struct.pack("<II", rgba.width, rgba.height) + rgba.tobytes()
    except (OSError, ValueError):
        return None


def modulo_trama():
    """Modulo trama_scenari.py (stesse regole dello skyline), o None se non c'e'."""
    try:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import trama_scenari  # noqa: PLC0415 -- stesso TOOLS/, nessuna installazione
        return trama_scenari
    except (ImportError, OSError, SyntaxError):
        return None


def opzioni_skyline(livello, modulo):
    """Tecnica/forza/zone dello skyline, come le legge trama_scenari.py."""
    config = {}
    percorso = ASSETS / "scenari" / "trama.json"
    if percorso.exists():
        try:
            config = json.loads(percorso.read_text())
        except (OSError, ValueError):
            config = {}
    return {
        **modulo.PREDEFINITO,
        **config.get("predefinito", {}),
        **config.get("livelli", {}).get(livello, {}),
    }


def raw_skyline_atteso(png, livello, modulo):
    """Byte del raw skyline composto (trama inclusa), o None se non ricomponibile."""
    if modulo is None:
        return None
    try:
        with Image.open(png) as im:
            img = im.convert("RGBA")
    except (OSError, ValueError):
        return None
    opz = opzioni_skyline(livello, modulo)
    if opz["tecnica"] != "nessuna":
        img = modulo.trama(img, opz["tecnica"], float(opz["forza"]), int(opz["zone"]))
    return b"SGLR" + struct.pack("<II", img.width, img.height) + img.convert("RGBA").tobytes()


def png_oggetti():
    """Inverso di png_sorgente(): ogni PNG di oggetto con il raw che gli spetta."""
    for png in sorted(SCENARI.glob("comuni/oggetti/*.png")):
        yield png, "comuni", png.stem
    # Dal 16/11 gli oggetti stanno direttamente in scenari/<livello>/oggetti/
    # (prima erano in scenari/<livello>/scenario_*/oggetti/).
    for png in sorted(SCENARI.glob("*/oggetti/obj_*.png")):
        livello = png.parents[1].name
        if livello == "comuni":
            continue
        yield png, livello, png.stem[len("obj_"):]


def png_altri(raw, gruppo):
    """PNG sorgente per un raw dei gruppi 1:1 (stesso nome, cartella di GRAFICA)."""
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
    """Percorsi (relativi a GRAFICA/) dei PNG gia' confrontati con un raw."""
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
# quando si guardavano solo i PNG di GRAFICA.
GRUPPI_RAW = {"giocatore", "scenari", "sponsor"} | {g for _, g in ALTRI}


def controllo_gruppi_raw():
    ignoti = 0
    for cartella in sorted(p for p in RAW_ROOT.iterdir() if p.is_dir()):
        if cartella.name not in GRUPPI_RAW:
            ignoti += 1
            print(f"RAW-GRUPPO-IGNOTO {cartella.name}: nessuna regola di conversione")
    print(f"gruppi di raw noti: {len(GRUPPI_RAW)}, sconosciuti: {ignoti}")
    return ignoti


POSE = ["dritta"] + [f"sx{k}" for k in range(1, 5)] + [f"dx{k}" for k in range(1, 5)]
POSE += ["salita", "discesa"]


def byte_raw(im):
    """Byte del raw SGLR per un'immagine RGBA gia' ritagliata."""
    return b"SGLR" + struct.pack("<II", im.width, im.height) + im.tobytes()


def dettaglio_diff(atteso, grezzo, raw):
    """Quanto e' diverso il raw dal disegno di Andrea: misure, pixel, date."""
    if not grezzo.startswith(b"SGLR") or len(grezzo) < 12:
        return f"raw {data(raw)} non in formato SGLR: da rifare con converti_pose_auto.py"
    larg, alt = struct.unpack("<II", grezzo[4:12])
    dati_atteso, dati_raw = atteso[12:], grezzo[12:]
    if len(dati_atteso) != len(dati_raw):
        w_att, h_att = struct.unpack("<II", atteso[4:12])
        return (
            f"raw {data(raw)} {larg}x{alt}, il png ritagliato e' {w_att}x{h_att}:"
            " il raw e' di un altro disegno, da riconvertire"
        )
    diversi = sum(
        1 for i in range(0, len(dati_raw), 4) if dati_raw[i:i + 4] != dati_atteso[i:i + 4]
    )
    pixel = (larg * alt) or 1
    return (
        f"raw {data(raw)} {100.0 * diversi / pixel:.1f}% pixel diversi dal png"
        f"{'' if diversi else ' (solo ordine/versione)'}"
    )


def conta_pose(pose_png, stop_png):
    """Conteggi distinti per l'area bloccata: PNG presenti vs coppie confrontabili.

    'png' conta i file che esistono (pose base + varianti stop); 'coppie' conta le
    pose base che hanno anche lo stop, le sole che entrano nel confronto del riquadro
    di ritaglio (converti_pose_auto.py ritaglia tutte le pose insieme). Tenere i due
    numeri separati evita di scambiare i file presenti per le coppie pronte.
    """
    coppie_stop = [p for p in stop_png if p in pose_png]
    return {
        "png": len(pose_png) + len(stop_png),
        "base": len(pose_png),
        "stop": len(stop_png),
        "coppie": len(pose_png) + len(coppie_stop),
    }


def controllo_pose_giocatore():
    """Sola diagnosi dell'area bloccata: pose dell'auto di Andrea.

    Due esiti, nessuno converte nulla da qui (file bloccati):
    - un raw senza PNG sorgente = disegno originale perso, non rigenerabile, da riavere
      o da ridisegnare;
    - un PNG ridisegnato e non convertito = raw vecchio (prima la copertura lo
      classificava solo come "non motore", quindi il disallineamento non si vedeva).
    Il confronto ripete la regola di converti_pose_auto.py: tutte le pose ritagliate
    sull'unione delle parti non trasparenti, gli stop sullo stesso riquadro della posa.
    Il riquadro dipende da tutte le pose insieme: se ne manca una il confronto non e'
    riproducibile e si dichiara, invece di dire "da riconvertire" a torto.
    """
    cartella_png = ASSETS / "sprite/giocatore_pose"

    def nome_png(p):
        return "auto_stop.png" if p == "dritta" else f"auto_stop_{p}.png"

    pose_png = [p for p in POSE if (cartella_png / f"auto_{p}.png").exists()]
    stop_png = [p for p in POSE if (cartella_png / nome_png(p)).exists()]
    esaminati = 0
    mancanti = []  # raw senza nessun PNG sorgente
    orfani = []    # raw di uno stop il cui PNG c'e' ma la posa no: la conversione lo cancella
    for raw in sorted((RAW_ROOT / "giocatore/pose").glob("*.sglraw")):
        esaminati += 1
        if raw.stem.startswith("auto_stop_"):
            p = "dritta" if raw.stem == "auto_stop_dritta" else raw.stem[len("auto_stop_"):]
            if p not in stop_png:
                mancanti.append(raw.stem)
            elif p not in pose_png:
                orfani.append(raw.stem)
        else:
            p = raw.stem[len("auto_"):]
            if p not in pose_png:
                mancanti.append(raw.stem)
    conta = conta_pose(pose_png, stop_png)
    print(
        f"giocatore: raw pose {esaminati}, PNG presenti {conta['png']} "
        f"(pose base {conta['base']}, stop {conta['stop']}), "
        f"coppie pronte {conta['coppie']}, "
        f"mancanti {len(mancanti)}: {', '.join(mancanti) if mancanti else '-'}"
    )
    if mancanti:
        print(
            "giocatore: area bloccata (auto di Andrea), nessuna conversione da qui."
            " Rilanciare converti_pose_auto.py senza questi PNG cancella il raw:"
            " da riavere da git f2f3581 o da ridisegnare."
        )
    if orfani:
        print(
            f"giocatore: stop con PNG ma senza la posa (raw cancellato a una conversione)"
            f" {len(orfani)}: {', '.join(orfani)}"
        )
    if mancanti or orfani:
        print(
            "giocatore: riquadro di ritaglio non riproducibile (dipende da tutte le pose):"
            " confronto contenuto png/raw saltato finche' i PNG mancanti non tornano."
        )
        return len(mancanti)
    immagini = {}
    try:
        for p in pose_png:
            with Image.open(cartella_png / f"auto_{p}.png") as im:
                immagini[p] = im.convert("RGBA")
    except (OSError, ValueError):
        print("giocatore: pose PNG illeggibili, confronto contenuto saltato")
        return 0
    box = None
    for im in immagini.values():
        b = im.getchannel("A").getbbox()
        if b:
            box = b if box is None else (
                min(box[0], b[0]), min(box[1], b[1]), max(box[2], b[2]), max(box[3], b[3])
            )
    # PNG sorgente -> byte che converti_pose_auto.py scriverebbe nel raw.
    attesi = {}
    for p, im in immagini.items():
        attesi[f"auto_{p}"] = byte_raw(im.crop(box) if box else im)
        stop = cartella_png / nome_png(p)
        if stop.exists():
            try:
                with Image.open(stop) as sim:
                    srgba = sim.convert("RGBA")
                    attesi[stop.stem] = byte_raw(srgba.crop(box) if box else srgba)
            except (OSError, ValueError):
                print(f"giocatore: {stop.stem}.png non apribile da PIL")
    disallineati = []
    confrontati = 0
    for raw in sorted((RAW_ROOT / "giocatore/pose").glob("*.sglraw")):
        nome = "auto_stop" if raw.stem == "auto_stop_dritta" else raw.stem
        if nome not in attesi:
            continue
        confrontati += 1
        grezzo = raw.read_bytes()
        if grezzo != attesi[nome]:
            disallineati.append(f"{nome} ({dettaglio_diff(attesi[nome], grezzo, raw)})")
    if disallineati:
        print(
            f"giocatore: pose presenti con png diverso dal raw {len(disallineati)}: "
            f"{', '.join(disallineati)}"
        )
        print(
            "giocatore: il raw non e' piu' il png (ridisegno non convertito). Conversione"
            " bloccata da qui (auto di Andrea): da segnalare ad Andrea, non rigenerare il"
            " disegno e non lanciare converti_pose_auto.py al posto suo."
        )
    else:
        print(f"giocatore: raw pose confrontati e allineati {confrontati}")
    return len(mancanti)


def controllo_copertura():
    """Ogni PNG di GRAFICA o e' confrontato, o e' non-motore noto, o e' sospetto."""
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
    print(f"png in GRAFICA: {totale} (confrontati con un raw {totale - noti - fuori}, non motore noti {noti})")
    return fuori


def main():
    verbose = "--verbose" in sys.argv
    totale = diversi = senza_png = png_piu_nuovo = allineati = trame = 0
    righe = []
    noise = []
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
            noise.append(
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
    mod_trama = modulo_trama()
    for livello, rel in SKYLINE.items():
        png = ASSETS / rel
        raw = RAW / livello / "skyline.sglraw"
        if not png.exists() or not raw.exists():
            skyline_senza += 1
            print(f"SKYLINE-MANCA {livello}: png {png.exists()} raw {raw.exists()}")
            continue
        atteso = raw_skyline_atteso(png, livello, mod_trama)
        if atteso is None:
            # Senza regola di trama resta solo la data (comportamento di prima).
            if png.stat().st_mtime > raw.stat().st_mtime:
                skyline_png_nuovo += 1
                print(
                    f"SKYLINE-PNG-NUOVO {livello}: png {data(png)} raw {data(raw)}"
                    " (rilanciare trama_scenari.py)"
                )
            else:
                skyline_ok += 1
        elif raw.read_bytes() != atteso:
            skyline_png_nuovo += 1
            print(
                f"SKYLINE-DIVERSO {livello}: png {data(png)} raw {data(raw)}"
                " (rilanciare trama_scenari.py)"
            )
        else:
            skyline_ok += 1
    print(f"skyline controllati: {skyline_ok + skyline_png_nuovo + skyline_senza}")
    print(f"skyline allineati: {skyline_ok}")
    print(f"skyline da rigenerare: {skyline_png_nuovo}")
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
                noise.append(
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
    if verbose:
        for riga in noise:
            print(riga)
    elif noise:
        print(
            f"png piu' nuovi a pari contenuto: {len(noise)}"
            " (risalvataggi senza modifiche; dettaglio con --verbose)"
        )
    print(f"raw esaminati: {totale} (skyline a trama esclusi dal confronto a byte: {trame})")
    print(f"allineati: {allineati}")
    print(f"contenuto diverso da convertire: {diversi}")
    print(f"png piu' nuovi ma contenuto uguale: {png_piu_nuovo}")
    print(f"raw senza png sorgente: {senza_png}")
    fuori_copertura = controllo_copertura()
    gruppi_ignoti = controllo_gruppi_raw()
    controllo_pose_giocatore()
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
        # Prova del percorso negativo senza toccare GRAFICA o dati/sprite_raw:
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
        # 7 file PNG (3 pose base + 4 stop) ma solo 5 coppie pronte: il conteggio
        # dei file non va scambiato con quello delle coppie confrontabili.
        conta = conta_pose(["dritta", "sx1", "sx2"], ["sx1", "sx2", "sx3", "sx4"])
        ok_conta = (
            conta["png"] == 7 and conta["base"] == 3 and conta["stop"] == 4
            and conta["coppie"] == 5
        )
        ok = ok and ok_conta
        print("autotest conta_pose: " + ("OK" if ok_conta else "FALLITO"))
        print(f"exit {0 if ok else 1}")
        raise SystemExit(0 if ok else 1)
    codice = main()
    print(f"exit {codice}")
    raise SystemExit(codice)