#!/usr/bin/env python3
"""Fallisce se dati/personaggi/* e CATALOGO_PERSONAGGI.md non coincidono, o se manca il manifest."""
import json, re, sys, pathlib
base = pathlib.Path(__file__).resolve().parent.parent
cat = (base/"docs"/"CATALOGO_PERSONAGGI.md").read_text(encoding="utf-8")
ids = set(re.findall(r"^\|\s*([a-z0-9_]+)\s*\|", cat, re.M)) - {"id"}
dirs = {p.name for p in (base/"dati"/"personaggi").glob("*") if p.is_dir()}
err = []
for d in sorted(dirs):
    m = base/"dati"/"personaggi"/d/"manifest.json"
    if d not in ids: err.append(f"{d}: cartella senza riga nel catalogo")
    if not m.exists(): err.append(f"{d}: manca manifest.json"); continue
    try: j = json.loads(m.read_text(encoding="utf-8"))
    except Exception as e: err.append(f"{d}: manifest non valido ({e})"); continue
    for k in ("id","ruolo","file","crediti","licenza"):
        if k not in j: err.append(f"{d}: manifest senza '{k}'")
for e in err: print("ERRORE", e)
print("catalogo:", len(ids), "righe,", len(dirs), "cartelle")
sys.exit(1 if err else 0)
