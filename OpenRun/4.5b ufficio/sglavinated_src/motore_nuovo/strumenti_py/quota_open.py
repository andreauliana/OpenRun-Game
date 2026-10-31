#!/usr/bin/env python3
"""Quota open source: % di byte (codice+asset+dati) con licenza aperta dichiarata. Soglia: 50%. out/ escluso (screenshot e log dei test sono temporanei, non parte del gioco). Uso: python3 quota_open.py [soglia]"""
import json, pathlib, sys
OPEN = {"MIT","BSD-2-Clause","BSD-3-Clause","Apache-2.0","GPL-3.0","GPL-3.0-or-later","LGPL-3.0","CC0-1.0","CC-BY-4.0","CC-BY-3.0","CC-BY-SA-4.0","OFL-1.1","Zlib"}
root = pathlib.Path(__file__).resolve().parent.parent          # motore_nuovo
proj = root.parent                                              # sglavinated_src
soglia = float(sys.argv[1]) if len(sys.argv) > 1 else 50.0
def lic_of(d):
    for p in (d, *d.parents):
        m = p/"manifest.json"
        if m.exists():
            try: return json.loads(m.read_text(encoding="utf-8")).get("licenza","")
            except Exception: return ""
        l = p/"LICENSE"
        if l.exists():
            t = l.read_text(encoding="utf-8", errors="ignore").splitlines()
            if t and t[0].startswith("SPDX-License-Identifier:"): return t[0].split(":",1)[1].strip()
        if p in (root, proj): break
    return ""
tot = {"aperto":0,"chiuso/sconosciuto":0}
righe = []
for base in (root/"src", root/"dati", proj/"UFFICIO"/"Dipartimento Grafico"/"Libreria"/"assets_originali", proj/"libreria"):
    if not base.exists(): continue
    for f in base.rglob("*"):
        if f.is_file() and "galleria" not in f.parts and f.suffix not in (".md",) and f.name != "INDICE.md" and f.name not in ("manifest.json",):
            n = f.stat().st_size; l = lic_of(f.parent)
            k = "aperto" if l in OPEN else "chiuso/sconosciuto"
            tot[k] += n
            if k != "aperto": righe.append(f"  senza licenza aperta: {f.relative_to(proj)} ({l or 'nessuna'})")
T = sum(tot.values())
q = 100*tot["aperto"]/T if T else 0
for r in righe[:20]: print(r)
print(f"quota open: {q:.1f}% ({tot['aperto']} di {T} byte), soglia {soglia}%")
sys.exit(0 if (T == 0 or q >= soglia) else 1)
