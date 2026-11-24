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
# Ogni DIVERSO nel ciclo dei raw porta un motivo (classifica_raw): 'contenuto' e' un
# ridisegno di Andrea da convertire, 'dimensione' un raw di un altro disegno, 'corrotto'
# un raw troncato/ senza header SGLR da rigenerare. Senza il motivo tutte e tre le
# cause sembravano lo stesso "da convertire" e l'azione giusta non si distingueva.
# Un raw con header SGLR e dimensioni uguali al PNG ma buffer pixel troncato (o con
# byte in piu') e' 'corrotto', non 'dimensione': prima la lunghezza diversa finiva
# sotto 'dimensione' e sembrava un disegno di un'altra misura, quando e' lo stesso
# disegno danneggiato da rigenerare.
# Un PNG con doppia estensione in un gruppo del motore (es. x.png.png) si segnala con
# ALTRI-PNG-DOPPIA-ESTENSIONE e il nome vero: lo stem lo faceva passare per x.png, che
# ha il suo raw, e il residuo restava illeggibile. Non si rinomina e non si converte
# da qui: la decisione e' di Andrea.
# Il confronto del PNG col nome corretto e il suo raw sta in confronto_nome_ok(): la
# riga 'allineato al raw' di main() e lo stato sintetico leggono la stessa funzione,
# cosi' non possono contraddirsi se una delle due diagnosi viene modificata.
# I tre percorsi del residuo a doppia estensione (base, nome corretto, raw del nome
# corretto) si costruiscono in percorsi_doppia(): prima la base veniva ricalcolata a
# ogni riga del blocco e stem diversi avrebbero fatto parlare di file diversi.
# Se lo skyline di un livello manca (PNG sparito, raw ancora presente) si aggiunge
# SKYLINE-RESIDUO: dice se il raw e' ancora identico a skyline_trama.png, cioe' se lo
# sfondo in gioco e' quello giusto e manca solo il disegno di Andrea da ripristinare.
# La riga SKYLINE-MANCA porta ora uno stato (classifica_skyline): 'manca-disegno'
# (sfondo giusto, manca solo il PNG di Andrea) e' distinto da 'manca-sfondo' (raw di
# un altro disegno): prima entrambi dicevano solo SKYLINE-MANCA e non si distingueva
# la decisione di Andrea da un guasto da rigenerare.
# La sintesi degli skyline (controllati/allineati/da rigenerare/senza) sta in
# righe_skyline(): il totale era ricalcolato a mano in main(), cosi' un contatore
# aggiunto al ciclo poteva restare fuori dal conteggio.
# Lo stesso per la sintesi dei gruppi 1:1 in righe_altri(): otto contatori e quattro
# righe, prima sparse come print in main() con la doppia estensione concatenata a mano.
# Le code delle due righe del residuo .png.png (dettaglio per stato, esito 'risolto' o
# 'decide Andrea') stanno in suffisso_dettaglio_doppia() e suffisso_stato_doppia():
# prima erano due ternari annidati nei print di main(), non provabili a parte.
# Le cinque righe di chiusura (raw esaminati, allineati, da convertire, png piu' nuovi,
# senza png sorgente) stanno in righe_finali(): prima erano print sparsi in fondo a
# main(), cosi' un contatore nuovo poteva restare fuori dal totale.
# Il blocco dei PNG risalvati a pari contenuto (dettaglio riga per riga con --verbose,
# solo conteggio senza) sta in righe_rumore(): prima l'if/elif era in main(), cosi' le
# due diagnosi dello stesso rumore potevano dividersi e mostrare numeri diversi.
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


def classifica_raw(atteso, grezzo):
    """Perche' un raw non e' identico al PNG atteso. Funzione pura.

    Ritorna 'uguale' | 'corrotto' | 'dimensione' | 'contenuto':
    - corrotto:   raw senza header SGLR valido, o con l'header delle dimensioni
                  uguale al PNG ma il buffer pixel troncato/piu' lungo, da rigenerare;
    - dimensione: testata delle dimensioni diversa dal PNG (raw di un altro disegno);
    - contenuto:  stesse dimensioni e lunghezza ma pixel diversi (ridisegno di Andrea).
    Serve a non scambiare un raw danneggiato per un disegno nuovo, e viceversa.
    """
    if grezzo == atteso:
        return "uguale"
    if not grezzo.startswith(b"SGLR") or len(grezzo) < 12:
        return "corrotto"
    if len(atteso) >= 12 and grezzo[4:12] != atteso[4:12]:
        return "dimensione"
    if len(grezzo) != len(atteso):
        # Header e dimensioni coincidono: se il buffer pixel non e' della lunghezza
        # giusta il raw e' troncato/con byte extra, non un disegno piu' grande.
        return "corrotto"
    return "contenuto"


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


def residuo_skyline(livello, raw):
    """Se lo skyline originale e' sparito, dice cosa resta: l'intermedio con la trama.

    trama_scenari.py non tocca il PNG di Andrea: scrive skyline_trama.png accanto e
    converte quello. Se il PNG originale manca ma l'intermedio c'e', il raw si puo'
    ancora rifare identico (lo sfondo in gioco non cambia) e l'unica cosa persa e' il
    disegno da ridisegnare/ripristinare. Solo diagnosi: non rigenera lo sfondo di Andrea.
    """
    intermedio = SCENARI / livello / "skyline_trama.png"
    if not intermedio.exists():
        print(f"SKYLINE-RESIDUO {livello}: nessun skyline_trama.png, il raw non e' rifacibile da qui")
        return
    atteso = raw_atteso(intermedio)
    if atteso is None:
        print(f"SKYLINE-RESIDUO {livello}: skyline_trama.png {data(intermedio)} non apribile da PIL")
    elif raw.exists() and raw.read_bytes() == atteso:
        print(
            f"SKYLINE-RESIDUO {livello}: skyline_trama.png {data(intermedio)} identico al raw"
            f" {data(raw)}: lo sfondo in gioco e' quello, manca solo il PNG di Andrea"
        )
    else:
        print(
            f"SKYLINE-RESIDUO {livello}: skyline_trama.png {data(intermedio)} diverso dal raw"
            f" {data(raw)}: da decidere con Andrea"
        )


def raw_e_trama(livello, raw):
    """Il raw dello skyline e' ancora skyline_trama.png (sfondo ricostruibile)."""
    intermedio = SCENARI / livello / "skyline_trama.png"
    if not intermedio.exists() or not raw.exists():
        return False
    atteso = raw_atteso(intermedio)
    return atteso is not None and raw.read_bytes() == atteso


def classifica_skyline(png_esiste, raw_esiste, raw_uguale_trama):
    """Stato di uno skyline nella diagnosi VG3 (funzione pura).

    Prima i casi con PNG o raw mancante stampavano tutti 'SKYLINE-MANCA' e non si
    capiva se mancava solo il disegno di Andrea (lo sfondo in gioco e' giusto) o
    anche lo sfondo (raw di un altro disegno, da decidere con Andrea).
    Ritorna: 'allineato' | 'da-rigenerare' | 'manca-disegno' | 'manca-sfondo' | 'raw-manca'.
    """
    if not raw_esiste:
        return "raw-manca"
    if png_esiste:
        return "allineato" if raw_uguale_trama else "da-rigenerare"
    return "manca-disegno" if raw_uguale_trama else "manca-sfondo"


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


def base_doppia_estensione(png):
    """'lato_furgone_bianco.png.png' -> 'lato_furgone_bianco'; None se il nome e' normale.

    Serve a non confondere il residuo con l'asset vero: stampando lo stem, un
    'x.png.png' senza raw appariva come 'x.png' e sembrava il file con raw regolare.
    """
    if png.name.endswith(".png.png"):
        return png.name[: -len(".png.png")]
    return None


def percorsi_doppia(png, raw):
    """(base, PNG col nome corretto, raw del nome corretto) di un residuo .png.png.

    Ritorna (None, None, None) se il nome non e' a doppia estensione. Funzione pura:
    i tre percorsi venivano costruiti uno per riga dentro il blocco di diagnosi di
    main() e la base ricalcolata; se uno degli stem fosse cambiato, le righe della
    stessa diagnosi avrebbero potuto riferirsi a file diversi. Il raw sta accanto
    al file grezzo (cartella del gruppo), il nome corretto accanto al PNG.
    """
    base = base_doppia_estensione(png)
    if base is None:
        return None, None, None
    return base, png.parent / f"{base}.png", raw.parent / f"{base}.sglraw"


def stato_doppia_estensione(png, raw_base):
    """Cosa e' il file a doppia estensione rispetto al raw del nome corretto.

    Ritorna (stato, dettaglio): 'doppione' se i byte coincidono col raw (risalvataggio
    inutile), 'ridisegno' se il contenuto e' diverso (allora il file e' un disegno nuovo
    di Andrea salvato col nome sbagliato), 'illeggibile' o 'senza-raw' negli altri casi.
    Serve a distinguere i due casi prima di chiedere ad Andrea di rinominare: senza
    questo confronto il residuo sembrava sempre "da decidere".
    """
    atteso = raw_atteso(png)
    if atteso is None:
        return "illeggibile", "non apribile da PIL"
    if not raw_base.exists():
        return "senza-raw", f"raw {raw_base.stem}.sglraw manca"
    grezzo = raw_base.read_bytes()
    if grezzo == atteso:
        return "doppione", f"identico al raw {data(raw_base)}"
    return "ridisegno", dettaglio_diff(atteso, grezzo, raw_base)


def confronto_nome_ok(nome_ok, raw_base):
    """Stato del PNG col nome corretto rispetto al suo raw (funzione pura).

    Ritorna 'allineato' | 'nome-corretto-manca' | 'illeggibile' | 'raw-manca' |
    'raw-da-rigenerare'. La coppia nome-corretto/raw prima veniva confrontata due
    volte: con un booleano inline in main() (la riga 'allineato al raw') e con i
    controlli dentro stato_risoluzione_doppia() (lo stato sintetico). Se una delle
    due cambiava, le due righe potevano contraddirsi. Tenendola qui le due diagnosi
    leggono la stessa verita'. Non scrive nulla: PNG e raw restano di Andrea.
    """
    if not nome_ok.exists():
        return "nome-corretto-manca"
    atteso = raw_atteso(nome_ok)
    if atteso is None:
        return "illeggibile"
    if not raw_base.exists():
        return "raw-manca"
    return "allineato" if raw_base.read_bytes() == atteso else "raw-da-rigenerare"


def stato_risoluzione_doppia(png, raw_base, nome_ok):
    """Stato sintetico del residuo .png.png, separato dall'azione (funzione pura).

    'risolto' solo quando il nome corretto esiste, il suo raw e' allineato e il
    .png.png e' un doppione (lo stesso disegno): l'asset letto dal motore e' gia'
    quello giusto. 'ridisegno-da-decidere' quando il .png.png e' un disegno diverso
    (il caso del furgone del 20/11): va rinominato/convertito con Andrea. Negli
    altri casi si riportano i motivi gia' noti, cosi' un nome mancante non sembra
    un ridisegno. Non scrive nulla: PNG e raw restano di Andrea.
    """
    confronto = confronto_nome_ok(nome_ok, raw_base)
    if confronto != "allineato":
        return confronto
    stato, _ = stato_doppia_estensione(png, raw_base)
    if stato == "doppione":
        return "risolto"
    if stato == "ridisegno":
        return "ridisegno-da-decidere"
    return stato


def suffisso_dettaglio_doppia(stato):
    """Coda della riga con lo stato del .png.png, in un punto solo (funzione pura).

    La coda dipendeva da un ternario annidato dentro il print di main(): un caso
    nuovo di stato_doppia_estensione restava senza spiegazione e sembrava un
    errore. Qui i tre esiti noti hanno la loro frase e gli altri restano vuoti.
    """
    if stato == "doppione":
        return ": doppione inutile, nessun ridisegno da convertire"
    if stato == "ridisegno":
        return ": e' un disegno diverso dal raw, va col nome corretto e convertito"
    return ""


def suffisso_stato_doppia(sintesi):
    """Coda della riga 'stato <sintesi>' del residuo .png.png (funzione pura).

    Solo 'risolto' dice che non c'e' nulla da fare; ogni altro stato resta una
    decisione di Andrea (rinominare o convertire). Prima il testo stava in un
    ternario dentro main(), cosi' non si poteva provare senza toccare GRAFICA.
    """
    if sintesi == "risolto":
        return " (asset corretto gia' allineato, nessuna conversione VG3)"
    return " (decide Andrea: rinominare/convertire)"


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


def codice_esito(
    diversi, skyline_png_nuovo, skyline_senza, orfani,
    altri_diversi, altri_senza, altri_orfani, fuori_copertura, gruppi_ignoti,
):
    """Codice di uscita VG3: 0 solo se non c'e' nulla da convertire o da decidere.

    Funzione pura: raccoglie in un punto solo la regola che main() usava inline.
    Cosi' un contatore aggiunto o tolto non richiede di riscrivere la condizione
    a mano nel return, e l'autotest fissa i casi che fanno passare/fallire il gate.
    """
    problemi = (
        diversi + skyline_png_nuovo + skyline_senza + orfani
        + altri_diversi + altri_senza + altri_orfani + fuori_copertura + gruppi_ignoti
    )
    return 0 if problemi == 0 else 1


def righe_skyline(ok, da_rigenerare, senza):
    """Le quattro righe di sintesi skyline di main(), in un punto solo (funzione pura).

    Il totale 'skyline controllati' era la somma ricalcolata a mano dentro main():
    un contatore aggiunto al ciclo poteva non entrare nel totale. Qui la somma e la
    formattazione stanno insieme, e l'autotest fissa formato e totale.
    """
    return [
        f"skyline controllati: {ok + da_rigenerare + senza}",
        f"skyline allineati: {ok}",
        f"skyline da rigenerare: {da_rigenerare}",
        f"skyline senza png o raw: {senza}",
    ]


def righe_altri(esaminati, allineati, diversi, nuovo, senza, orfani, doppia, residui):
    """Le quattro righe di sintesi dei gruppi 1:1, in un punto solo (funzione pura).

    Prima erano quattro print con otto contatori sparsi in main(): la riga dei PNG
    senza raw si portava dietro il conteggio della doppia estensione come stringa
    concatenata, cosi' cambiare un contatore voleva dire ricomporre la frase a mano.
    """
    return [
        f"altri gruppi: raw esaminati {esaminati} (allineati {allineati})",
        f"altri gruppi: da convertire {diversi}, png piu' nuovi a pari contenuto {nuovo}",
        f"altri gruppi: raw senza png {senza}, png senza raw {orfani}"
        f" (di cui doppia estensione {doppia})",
        f"altri gruppi: residui noti non usati dal motore {residui}",
    ]


def righe_finali(totale, trame, allineati, diversi, png_piu_nuovo, senza_png):
    """Le cinque righe di chiusura di main(), in un punto solo (funzione pura).

    Prima erano cinque print sparsi in fondo a main(): il totale 'raw esaminati'
    e la parentesi dei skyline a trama esclusi venivano riscritti a mano a ogni
    modifica dei contatori. Qui formattazione e conteggi stanno insieme e
    l'autotest fissa il formato esatto e il caso tutto zero.
    """
    return [
        f"raw esaminati: {totale} (skyline a trama esclusi dal confronto a byte: {trame})",
        f"allineati: {allineati}",
        f"contenuto diverso da convertire: {diversi}",
        f"png piu' nuovi ma contenuto uguale: {png_piu_nuovo}",
        f"raw senza png sorgente: {senza_png}",
    ]


def righe_rumore(noise, verbose):
    """Le righe dei PNG risalvati a pari contenuto, in un punto solo (funzione pura).

    Prima l'if/elif stava in main(): con --verbose si stampava una riga per file,
    senza si stampava solo il conteggio. Le due diagosi dello stesso rumore potevano
    dividersi e raccontare numeri diversi. Qui la regola e' una sola e l'autotest
    fissa i tre casi (dettaglio, conteggio, vuoto). Non tocca GRAFICA ne' i raw.
    """
    if verbose:
        return list(noise)
    if noise:
        return [
            f"png piu' nuovi a pari contenuto: {len(noise)}"
            " (risalvataggi senza modifiche; dettaglio con --verbose)"
        ]
    return []


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
            motivo = classifica_raw(atteso, raw.read_bytes())
            righe.append(
                f"DIVERSO    {livello}/{nome}: png {data(png)} raw {data(raw)} [{motivo}]"
            )
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
            stato = classifica_skyline(png.exists(), raw.exists(), raw_e_trama(livello, raw))
            print(f"SKYLINE-MANCA {livello}: png {png.exists()} raw {raw.exists()} [{stato}]")
            residuo_skyline(livello, raw)
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
    for riga in righe_skyline(skyline_ok, skyline_png_nuovo, skyline_senza):
        print(riga)
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
    altri_orfani = altri_residui = altri_doppia = 0
    for png, gruppo, raw in png_altri_orfani():
        if not raw.exists():
            if png.relative_to(ASSETS).as_posix() in RESIDUI_NON_MOTORE:
                altri_residui += 1
                print(f"ALTRI-RESIDUO {gruppo}/{png.stem}: png {data(png)} (nessun raw, non usato)")
                continue
            altri_orfani += 1
            doppia, nome_ok, raw_base = percorsi_doppia(png, raw)
            if doppia is not None:
                altri_doppia += 1
                print(
                    f"ALTRI-PNG-DOPPIA-ESTENSIONE {gruppo}/{png.name}: png {data(png)}"
                    " (doppia estensione: il catalogo non lo vede)"
                )
                print(
                    f"ALTRI-PNG-DOPPIA-ESTENSIONE {gruppo}/{png.name}: nome atteso {doppia}.png;"
                    f" raw {doppia}.sglraw "
                    + (f"esiste ({data(raw_base)})" if raw_base.exists() else "manca")
                    + ": rinominare o convertire decide Andrea"
                )
                stato, dettaglio = stato_doppia_estensione(png, raw_base)
                print(
                    f"ALTRI-PNG-DOPPIA-ESTENSIONE {gruppo}/{png.name}: {stato} ({dettaglio})"
                    + suffisso_dettaglio_doppia(stato)
                )
                if nome_ok.exists():
                    allineato = confronto_nome_ok(nome_ok, raw_base) == "allineato"
                    print(
                        f"ALTRI-PNG-DOPPIA-ESTENSIONE {gruppo}/{png.name}: il nome corretto"
                        f" {doppia}.png esiste ({data(nome_ok)}), "
                        + ("allineato al raw" if allineato else "diverso dal raw")
                        + ": il .png.png non e' l'asset che il motore legge"
                    )
                sintesi = stato_risoluzione_doppia(png, raw_base, nome_ok)
                print(
                    f"ALTRI-PNG-DOPPIA-ESTENSIONE {gruppo}/{png.name}: stato {sintesi}"
                    + suffisso_stato_doppia(sintesi)
                )
            else:
                print(f"ALTRI-PNG-SENZA-RAW {gruppo}/{png.name}: png {data(png)}")
    for riga in righe_altri(
        altri_tot, altri_ok, altri_diversi, altri_nuovo,
        altri_senza, altri_orfani, altri_doppia, altri_residui,
    ):
        print(riga)
    for riga in righe_rumore(noise, verbose):
        print(riga)
    for riga in righe_finali(totale, trame, allineati, diversi, png_piu_nuovo, senza_png):
        print(riga)
    fuori_copertura = controllo_copertura()
    gruppi_ignoti = controllo_gruppi_raw()
    controllo_pose_giocatore()
    return codice_esito(
        diversi, skyline_png_nuovo, skyline_senza, orfani,
        altri_diversi, altri_senza, altri_orfani, fuori_copertura, gruppi_ignoti,
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
            atteso_buono = raw_atteso(buono) or b""
        print("autotest raw_atteso: " + ("OK" if ok else "FALLITO"))
        # classifica_raw: distingue ridisegno (contenuto), raw di altro disegno
        # (dimensione) e raw troncato (corrotto); senza il motivo sembravano uguali.
        cls = (
            classifica_raw(atteso_buono, atteso_buono),
            classifica_raw(atteso_buono, atteso_buono[:8]),
            classifica_raw(atteso_buono, b"SGLR" + struct.pack("<II", 3, 3) + atteso_buono[12:]),
            classifica_raw(atteso_buono, b"SGLR" + struct.pack("<II", 2, 2) + bytes(16)),
        )
        ok_cls = cls == ("uguale", "corrotto", "dimensione", "contenuto")
        ok = ok and ok_cls
        print("autotest classifica_raw: " + ("OK" if ok_cls else "FALLITO"))
        # Stesso header e dimensioni ma buffer pixel troncato: e' 'corrotto' e non
        # 'dimensione' (prima sembrava un disegno di un'altra misura).
        troncato = atteso_buono[:14]
        ok_troncato = classifica_raw(atteso_buono, troncato) == "corrotto"
        ok = ok and ok_troncato
        print("autotest classifica_raw-troncato: " + ("OK" if ok_troncato else "FALLITO"))
        # Buffer pixel piu' lungo del dovuto: stesso esito 'corrotto' (raw danneggiato,
        # non un disegno di misura diversa).
        lungo = atteso_buono + b"\x00" * 4
        ok_lungo = classifica_raw(atteso_buono, lungo) == "corrotto"
        ok = ok and ok_lungo
        print("autotest classifica_raw-lungo: " + ("OK" if ok_lungo else "FALLITO"))
        # 7 file PNG (3 pose base + 4 stop) ma solo 5 coppie pronte: il conteggio
        # dei file non va scambiato con quello delle coppie confrontabili.
        conta = conta_pose(["dritta", "sx1", "sx2"], ["sx1", "sx2", "sx3", "sx4"])
        ok_conta = (
            conta["png"] == 7 and conta["base"] == 3 and conta["stop"] == 4
            and conta["coppie"] == 5
        )
        ok = ok and ok_conta
        print("autotest conta_pose: " + ("OK" if ok_conta else "FALLITO"))
        ok_doppia = (
            base_doppia_estensione(Path("x/lato_furgone_bianco.png.png"))
            == "lato_furgone_bianco"
            and base_doppia_estensione(Path("x/lato_furgone_bianco.png")) is None
        )
        ok = ok and ok_doppia
        print("autotest base_doppia_estensione: " + ("OK" if ok_doppia else "FALLITO"))
        # percorsi_doppia: un unico posto per base, nome corretto e raw del nome
        # corretto; il raw sta accanto al file grezzo, non accanto al PNG. Nome normale
        # deve dare tutti None, cosi' il blocco di diagnosi non viene aperto.
        png_doppio = Path("sprite/traffico_incrocio/lato_furgone_bianco.png.png")
        raw_gruppo = Path("motore_nuovo/dati/sprite_raw/traffico_incrocio/x.sglraw")
        base_p, nome_p, raw_p = percorsi_doppia(png_doppio, raw_gruppo)
        base_n, nome_n, raw_n = percorsi_doppia(Path("sprite/traffico/a.png"), raw_gruppo)
        ok_percorsi = (
            base_p == "lato_furgone_bianco"
            and nome_p == Path("sprite/traffico_incrocio/lato_furgone_bianco.png")
            and raw_p == Path("motore_nuovo/dati/sprite_raw/traffico_incrocio/lato_furgone_bianco.sglraw")
            and base_n is None and nome_n is None and raw_n is None
        )
        ok = ok and ok_percorsi
        print("autotest percorsi_doppia: " + ("OK" if ok_percorsi else "FALLITO"))
        # Doppia estensione: lo stesso disegno del raw e' un doppione, uno diverso e'
        # un ridisegno da rinominare; senza il confronto i due casi si confondevano.
        # Copre anche raw mancante e PNG illeggibile: non vanno scambiati per doppione.
        with tempfile.TemporaryDirectory() as td:
            uguale = Path(td) / "uguale.png.png"
            diverso = Path(td) / "diverso.png.png"
            rotto = Path(td) / "rotto.png.png"
            raw_base = Path(td) / "uguale.sglraw"
            raw_manca = Path(td) / "manca.sglraw"
            Image.new("RGBA", (3, 3), (11, 22, 33, 255)).save(uguale)
            raw_base.write_bytes(raw_atteso(uguale) or b"")
            Image.new("RGBA", (3, 3), (200, 100, 50, 255)).save(diverso)
            rotto.write_bytes(b"non-png")
            stato_uguale = stato_doppia_estensione(uguale, raw_base)[0]
            stato_diverso = stato_doppia_estensione(diverso, raw_base)[0]
            stato_senza_raw = stato_doppia_estensione(uguale, raw_manca)[0]
            stato_illeggibile = stato_doppia_estensione(rotto, raw_base)[0]
        ok_stato = (
            stato_uguale == "doppione"
            and stato_diverso == "ridisegno"
            and stato_senza_raw == "senza-raw"
            and stato_illeggibile == "illeggibile"
        )
        ok = ok and ok_stato
        print("autotest stato_doppia_estensione: " + ("OK" if ok_stato else "FALLITO"))
        # stato_risoluzione_doppia: il doppione innocuo e' 'risolto', un disegno
        # diverso e' da decidere con Andrea; nome mancante e raw stonato restano
        # distinti e non vengono scambiati per un ridisegno.
        with tempfile.TemporaryDirectory() as td:
            cartella = Path(td)
            doppio = cartella / "lato.png.png"
            corretto = cartella / "lato.png"
            raw_ok = cartella / "lato.sglraw"
            Image.new("RGBA", (3, 3), (40, 80, 120, 255)).save(corretto)
            raw_ok.write_bytes(raw_atteso(corretto) or b"")
            Image.new("RGBA", (3, 3), (40, 80, 120, 255)).save(doppio)
            doppione = stato_risoluzione_doppia(doppio, raw_ok, corretto)
            Image.new("RGBA", (3, 3), (200, 30, 60, 255)).save(doppio)
            ridisegno = stato_risoluzione_doppia(doppio, raw_ok, corretto)
            senza_nome = stato_risoluzione_doppia(doppio, raw_ok, cartella / "assente.png")
            raw_ok.write_bytes(b"SGLR" + struct.pack("<II", 9, 9) + bytes(36))
            raw_stonato = stato_risoluzione_doppia(doppio, raw_ok, corretto)
        ok_risoluzione = (
            doppione == "risolto"
            and ridisegno == "ridisegno-da-decidere"
            and senza_nome == "nome-corretto-manca"
            and raw_stonato == "raw-da-rigenerare"
        )
        ok = ok and ok_risoluzione
        print("autotest stato_risoluzione_doppia: " + ("OK" if ok_risoluzione else "FALLITO"))
        # confronto_nome_ok: unica fonte per la riga 'allineato al raw' di main() e
        # per lo stato sintetico. Copre i cinque casi senza toccare GRAFICA: nome
        # mancante, PNG illeggibile, raw mancante, raw da rigenerare e allineato.
        with tempfile.TemporaryDirectory() as td:
            cartella = Path(td)
            corretto = cartella / "lato.png"
            raw_ok = cartella / "lato.sglraw"
            raw_assente = cartella / "assente.sglraw"
            Image.new("RGBA", (3, 3), (60, 90, 130, 255)).save(corretto)
            raw_ok.write_bytes(raw_atteso(corretto) or b"")
            caso_allineato = confronto_nome_ok(corretto, raw_ok)
            caso_senza_nome = confronto_nome_ok(cartella / "assente.png", raw_ok)
            caso_senza_raw = confronto_nome_ok(corretto, raw_assente)
            rotto = cartella / "rotto.png"
            rotto.write_bytes(b"non-png")
            caso_illeggibile = confronto_nome_ok(rotto, raw_ok)
            raw_ok.write_bytes(b"SGLR" + struct.pack("<II", 3, 3) + bytes(36))
            caso_stonato = confronto_nome_ok(corretto, raw_ok)
        ok_confronto = (
            caso_allineato == "allineato"
            and caso_senza_nome == "nome-corretto-manca"
            and caso_senza_raw == "raw-manca"
            and caso_illeggibile == "illeggibile"
            and caso_stonato == "raw-da-rigenerare"
        )
        ok = ok and ok_confronto
        print("autotest confronto_nome_ok: " + ("OK" if ok_confronto else "FALLITO"))
        # classifica_skyline: 'manca solo il disegno' (sfondo giusto) e 'manca anche
        # lo sfondo' stampavano entrambi SKYLINE-MANCA; vanno distinti, come pure
        # raw mancante e i due casi con PNG presente.
        cls_sky = (
            classifica_skyline(False, True, True),
            classifica_skyline(False, True, False),
            classifica_skyline(True, True, True),
            classifica_skyline(True, True, False),
            classifica_skyline(False, False, False),
        )
        ok_sky_cls = cls_sky == (
            "manca-disegno", "manca-sfondo", "allineato", "da-rigenerare", "raw-manca"
        )
        ok = ok and ok_sky_cls
        print("autotest classifica_skyline: " + ("OK" if ok_sky_cls else "FALLITO"))
        # raw_e_trama: caso reale (lv_04_cromo ha skyline_trama allineato al raw) e
        # caso negativo (livello inesistente / raw mancante).
        ok_raw_trama = (
            raw_e_trama("lv_04_cromo", RAW / "lv_04_cromo" / "skyline.sglraw") is True
            and raw_e_trama("lv_inesistente", RAW / "lv_01_neon" / "skyline.sglraw") is False
        )
        ok = ok and ok_raw_trama
        print("autotest raw_e_trama: " + ("OK" if ok_raw_trama else "FALLITO"))
        # codice_esito: 0 solo quando tutti i contatori di problema sono zero;
        # basta un solo orfano/residuo/diverso per fare exit 1.
        ok_esito = (
            codice_esito(0, 0, 0, 0, 0, 0, 0, 0, 0) == 0
            and codice_esito(1, 0, 0, 0, 0, 0, 0, 0, 0) == 1
            and codice_esito(0, 0, 0, 0, 0, 0, 1, 0, 0) == 1
            and codice_esito(0, 0, 0, 0, 0, 0, 0, 0, 1) == 1
        )
        ok = ok and ok_esito
        print("autotest codice_esito: " + ("OK" if ok_esito else "FALLITO"))
        # righe_skyline: formato esatto delle quattro righe e totale = somma dei
        # tre contatori (prima la somma era ricalcolata a mano in main()).
        righe_sky = righe_skyline(6, 0, 0)
        righe_sky_vuote = righe_skyline(0, 0, 0)
        ok_righe_sky = (
            righe_sky == [
                "skyline controllati: 6",
                "skyline allineati: 6",
                "skyline da rigenerare: 0",
                "skyline senza png o raw: 0",
            ]
            and righe_sky_vuote[0] == "skyline controllati: 0"
            and righe_skyline(4, 1, 1)[0] == "skyline controllati: 6"
        )
        ok = ok and ok_righe_sky
        print("autotest righe_skyline: " + ("OK" if ok_righe_sky else "FALLITO"))
        # righe_altri: formato esatto delle quattro righe dei gruppi 1:1, con la
        # doppia estensione dentro la riga dei PNG senza raw (prima era concatenata
        # a mano) e i contatori tutti a zero.
        righe_alt = righe_altri(47, 45, 0, 2, 0, 1, 1, 0)
        righe_alt_vuote = righe_altri(0, 0, 0, 0, 0, 0, 0, 0)
        ok_righe_alt = (
            righe_alt == [
                "altri gruppi: raw esaminati 47 (allineati 45)",
                "altri gruppi: da convertire 0, png piu' nuovi a pari contenuto 2",
                "altri gruppi: raw senza png 0, png senza raw 1 (di cui doppia estensione 1)",
                "altri gruppi: residui noti non usati dal motore 0",
            ]
            and righe_alt_vuote[2]
            == "altri gruppi: raw senza png 0, png senza raw 0 (di cui doppia estensione 0)"
            and len(righe_alt_vuote) == 4
        )
        ok = ok and ok_righe_alt
        print("autotest righe_altri: " + ("OK" if ok_righe_alt else "FALLITO"))
        # righe_finali: formato esatto delle cinque righe di chiusura e dei contatori,
        # prima cinque print sparsi in fondo a main(). Il caso zero fissa che nessuna
        # riga sparisca quando non c'e' nulla da convertire.
        righe_fin = righe_finali(86, 6, 1, 0, 79, 0)
        righe_fin_vuote = righe_finali(0, 0, 0, 0, 0, 0)
        ok_fin = (
            righe_fin == [
                "raw esaminati: 86 (skyline a trama esclusi dal confronto a byte: 6)",
                "allineati: 1",
                "contenuto diverso da convertire: 0",
                "png piu' nuovi ma contenuto uguale: 79",
                "raw senza png sorgente: 0",
            ]
            and righe_fin_vuote[0]
            == "raw esaminati: 0 (skyline a trama esclusi dal confronto a byte: 0)"
            and len(righe_fin_vuote) == 5
        )
        ok = ok and ok_fin
        print("autotest righe_finali: " + ("OK" if ok_fin else "FALLITO"))
        # suffisso_dettaglio_doppia: le code delle tre diagnosi del .png.png, prima
        # in un ternario annidato dentro main(); gli stati senza spiegazione restano
        # vuoti (non vanno scambiati per un ridisegno).
        ok_det = (
            suffisso_dettaglio_doppia("doppione")
            == ": doppione inutile, nessun ridisegno da convertire"
            and suffisso_dettaglio_doppia("ridisegno")
            == ": e' un disegno diverso dal raw, va col nome corretto e convertito"
            and suffisso_dettaglio_doppia("illeggibile") == ""
            and suffisso_dettaglio_doppia("senza-raw") == ""
        )
        ok = ok and ok_det
        print("autotest suffisso_dettaglio_doppia: " + ("OK" if ok_det else "FALLITO"))
        # suffisso_stato_doppia: solo 'risolto' chiude il caso, ogni altro stato
        # resta da decidere con Andrea.
        ok_stato_txt = (
            suffisso_stato_doppia("risolto")
            == " (asset corretto gia' allineato, nessuna conversione VG3)"
            and suffisso_stato_doppia("ridisegno-da-decidere")
            == " (decide Andrea: rinominare/convertire)"
            and suffisso_stato_doppia("nome-corretto-manca")
            == " (decide Andrea: rinominare/convertire)"
        )
        ok = ok and ok_stato_txt
        print("autotest suffisso_stato_doppia: " + ("OK" if ok_stato_txt else "FALLITO"))
        # Skyline: il PNG grezzo non si confronta col raw (la trama lo ricompone),
        # quindi la prova negativa e' che la ricomposizione segua il disegno: due PNG
        # diversi devono dare raw SGLR diversi. Se trama_scenari.py non si importa il
        # confronto degli skyline ripiega sulla data e la prova non si puo' fare.
        modulo = modulo_trama()
        if modulo is None:
            print("autotest raw_skyline_atteso: SALTO (trama_scenari.py non importabile)")
        else:
            with tempfile.TemporaryDirectory() as td:
                a = Path(td) / "sk_a.png"
                b = Path(td) / "sk_b.png"
                Image.new("RGBA", (8, 8), (12, 34, 56, 255)).save(a)
                Image.new("RGBA", (8, 8), (210, 120, 40, 255)).save(b)
                ra = raw_skyline_atteso(a, "lv_01_neon", modulo) or b""
                rb = raw_skyline_atteso(b, "lv_01_neon", modulo) or b""
            ok_sky = ra[:4] == b"SGLR" and rb[:4] == b"SGLR" and ra != rb
            ok = ok and ok_sky
            print("autotest raw_skyline_atteso: " + ("OK" if ok_sky else "FALLITO"))
        # righe_rumore: i due modi di mostrare i risalvataggi (dettaglio con
        # --verbose, conteggio senza) leggono la stessa lista; caso vuoto = nessuna
        # riga in entrambi i modi. Prima il blocco if/elif stava in main().
        rum = ["A (contenuto uguale)", "B (contenuto uguale)"]
        ok_rumore = (
            righe_rumore(rum, True) == rum
            and righe_rumore(rum, False) == [
                "png piu' nuovi a pari contenuto: 2"
                " (risalvataggi senza modifiche; dettaglio con --verbose)"
            ]
            and righe_rumore([], False) == []
            and righe_rumore([], True) == []
        )
        ok = ok and ok_rumore
        print("autotest righe_rumore: " + ("OK" if ok_rumore else "FALLITO"))
        print(f"exit {0 if ok else 1}")
        raise SystemExit(0 if ok else 1)
    codice = main()
    print(f"exit {codice}")
    raise SystemExit(codice)