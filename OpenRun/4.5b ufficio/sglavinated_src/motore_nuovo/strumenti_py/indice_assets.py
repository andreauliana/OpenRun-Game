#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Indice unico degli asset di assets_originali/ (libreria interna).

Uso, dalla radice progetto, un solo comando senza pipe:
    python3 motore_nuovo/strumenti_py/indice_assets.py

Scrive assets_originali/INDICE_ASSET.json (un record per file: percorso, categoria,
livello, variante, dimensioni PNG, byte, sha1, licenza e stato dal manifest piu' vicino,
scelta di Andrea, avvisi) e assets_originali/INDICE_ASSET.md (riassunto, lacune, avvisi).
Solo libreria standard. Rilanciare dopo ogni generazione di asset.
"""
import hashlib, json, os, re, struct, sys, time

RADICE = "assets_originali"
SALTA = {".DS_Store", "__pycache__", "INDICE_ASSET.json", "INDICE_ASSET.md"}

# Scelte gia' prese da Andrea (DECISIONI_ANDREA.md).
SCELTE = [
    ("scenari/", r"varianteA", "D6: variante A per tutti i livelli"),
    ("ui/", r"varianteB", "D7: UI variante B (Lofi Jungle, bianco e nero)"),
    ("font/antonio", r".", "D4: Antonio Bold per titoli e fumetti"),
    ("sprite/auto_gt", r".", "D5: auto GT europea anni 70-90, 3 colori"),
    ("sponsor/", r".", "D2b: Curve Pericolose, Brinis Udine e altri sponsor inventati"),
]

# Cosa serve al primo livello giocabile: (nome, prefisso, testo nel percorso, minimo file PNG).
SERVE = [
    ("auto giocatore", "sprite/auto_gt/", "", 3),
    ("traffico / altri veicoli", "sprite/traffico/", "", 1),
    ("cartelloni sponsor (7 sponsor x 3 stili)", "sponsor/", "cartellone_", 21),
    ("skyline per livello", "scenari/", "skyline.png", 3),
    ("oggetti bordo strada", "scenari/", "oggetti/obj_", 10),
    ("HUD", "ui/hud/", "", 1),
    ("menu", "ui/menu/", "", 1),
    ("font pixel UI", "ui/font/", "", 1),
    ("personaggio Gianni (sprite)", "sprite/personaggi/gianni/", "", 1),
    ("personaggio Gesu' (sprite)", "sprite/personaggi/gesu/", "", 1),
    ("UFO (sprite)", "sprite/ufo/", "", 1),
]


def dimensioni_png(p):
    try:
        with open(p, "rb") as f:
            h = f.read(24)
        if h[:8] == b"\x89PNG\r\n\x1a\n":
            return struct.unpack(">II", h[16:24])
    except OSError:
        pass
    return None


def manifest_vicino(rel, cache):
    d = os.path.dirname(rel)
    while True:
        m = os.path.join(RADICE, d, "manifest.json")
        if m not in cache:
            try:
                with open(m, encoding="utf-8") as f:
                    cache[m] = json.load(f)
            except (OSError, ValueError):
                cache[m] = None
        if cache[m] is not None:
            return cache[m]
        if not d:
            return {}
        d = os.path.dirname(d)


def main():
    if not os.path.isdir(RADICE):
        print("assets_originali/ non trovata: lanciare dalla radice progetto (sglavinated_src)")
        return 4
    cache, record, generatori = {}, [], {}
    for cartella, dirs, files in os.walk(RADICE):
        dirs[:] = sorted(d for d in dirs if d not in SALTA)
        for nome in sorted(files):
            if nome in SALTA:
                continue
            p = os.path.join(cartella, nome)
            rel = os.path.relpath(p, RADICE).replace(os.sep, "/")
            st = os.stat(p)
            if nome.startswith("genera") and nome.endswith(".py"):
                generatori[os.path.dirname(rel)] = st.st_mtime
            man = manifest_vicino(rel, cache)
            r = {"percorso": "assets_originali/" + rel,
                 "categoria": rel.split("/")[0] if "/" in rel else "(radice)",
                 "tipo": os.path.splitext(nome)[1].lstrip(".").lower(),
                 "byte": st.st_size,
                 "licenza": man.get("licenza"),
                 "stato": man.get("stato") or "generato",
                 "originale": man.get("originale")}
            m = re.search(r"lv_\d\d_\w+", rel)
            if m:
                r["livello"] = m.group(0)
            m = re.search(r"variante([A-C])", rel)
            if m:
                r["variante"] = m.group(1)
            if r["tipo"] == "png":
                dim = dimensioni_png(p)
                if dim:
                    r["w"], r["h"] = dim
                with open(p, "rb") as f:
                    r["sha1"] = hashlib.sha1(f.read()).hexdigest()
            r["mtime"] = int(st.st_mtime)
            flag = []
            if re.search(r" copy|\(\d+\)|_old|\.bak", nome):
                flag.append("copia_o_backup")
            if r["tipo"] == "png" and st.st_size < 200:
                flag.append("minuscolo_probabile_segnaposto")
            if not r["licenza"] and r["tipo"] in ("png", "ttf") :
                flag.append("senza_licenza")
            for pref, rx, rif in SCELTE:
                if rel.startswith(pref) and re.search(rx, rel):
                    r["scelta"] = rif
            r["flag"] = flag
            record.append(r)

    per_hash = {}
    for r in record:
        if "sha1" in r:
            per_hash.setdefault(r["sha1"], []).append(r["percorso"])
    for r in record:
        g = per_hash.get(r.get("sha1"), [])
        if len(g) > 1:
            r["flag"].append("duplicato_di:" + next(x for x in g if x != r["percorso"]))
    for r in record:
        if r["tipo"] != "png":
            continue
        for d, t in generatori.items():
            if r["percorso"].startswith("assets_originali/" + d + "/") and r["mtime"] + 1 < t:
                r["flag"].append("da_rigenerare")
                break

    lacune = []
    for nome, pref, chiave, minimo in SERVE:
        n = sum(1 for r in record if r["tipo"] == "png"
                and r["percorso"].startswith("assets_originali/" + pref) and chiave in r["percorso"])
        lacune.append({"serve": nome, "trovati": n, "minimo": minimo, "ok": n >= minimo})

    adesso = time.strftime("%Y-%m-%d %H:%M")
    with open(os.path.join(RADICE, "INDICE_ASSET.json"), "w", encoding="utf-8") as f:
        json.dump({"generato": adesso, "n_file": len(record), "lacune": lacune, "file": record},
                  f, ensure_ascii=False, indent=1)

    cat = {}
    for r in record:
        c = cat.setdefault(r["categoria"], [0, 0, 0, 0])
        c[0] += 1; c[1] += r["tipo"] == "png"; c[2] += r["byte"]; c[3] += bool(r["flag"])
    avvisi = [r for r in record if r["flag"]]
    md = ["# Indice asset (generato da strumenti_py/indice_assets.py — non modificare a mano)", "",
          f"Aggiornato: {adesso} — {len(record)} file in assets_originali/.", "",
          "## Cosa serve per il primo livello giocabile", "",
          "| Serve | Trovati | Minimo | Stato |", "|---|---|---|---|"]
    md += [f"| {l['serve']} | {l['trovati']} | {l['minimo']} | {'OK' if l['ok'] else 'MANCA'} |" for l in lacune]
    md += ["", "## Per categoria", "", "| Categoria | File | PNG | Byte | Con avvisi |", "|---|---|---|---|---|"]
    md += [f"| {k} | {v[0]} | {v[1]} | {v[2]} | {v[3]} |" for k, v in sorted(cat.items())]
    md += ["", f"## Avvisi ({len(avvisi)})", ""]
    md += [f"- `{r['percorso']}`: {', '.join(r['flag'])}" for r in avvisi[:80]]
    if len(avvisi) > 80:
        md.append(f"- ... altri {len(avvisi) - 80} nel JSON")
    md += ["", "## Scelte di Andrea in uso", ""]
    md += [f"- {s}" for s in sorted({r['scelta'] for r in record if 'scelta' in r})]
    with open(os.path.join(RADICE, "INDICE_ASSET.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(md) + "\n")
    print(f"indice: {len(record)} file, {len(avvisi)} con avvisi, lacune {sum(1 for l in lacune if not l['ok'])}/{len(lacune)}")
    for l in lacune:
        if not l["ok"]:
            print(f"  MANCA: {l['serve']} ({l['trovati']}/{l['minimo']})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
