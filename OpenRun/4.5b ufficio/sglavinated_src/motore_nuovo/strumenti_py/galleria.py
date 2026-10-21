#!/usr/bin/env python3
"""Genera UFFICIO/Dipartimento Grafico/Libreria/assets_originali/galleria/galleria.html: foto di tutti i PNG per cartella (ogni team vede il lavoro degli altri)."""
import pathlib, html
proj = pathlib.Path(__file__).resolve().parent.parent.parent
base = proj/"UFFICIO"/"Dipartimento Grafico"/"Libreria"/"assets_originali"
out = base/"galleria"/"galleria.html"; out.parent.mkdir(exist_ok=True)
groups = {}
for f in sorted(base.rglob("*.png")):
    if "galleria" in f.parts: continue
    groups.setdefault(f.parent.relative_to(base).as_posix(), []).append(f)
for f in sorted((proj/"motore_nuovo"/"dati").rglob("*.png")):
    groups.setdefault("dati/"+f.parent.relative_to(proj/"motore_nuovo"/"dati").as_posix(), []).append(f)
rows = ["<meta charset=utf-8><title>Galleria v3</title><body style='background:#111;color:#ddd;font:14px sans-serif'><h1>Galleria v3 (aperta)</h1>"]
for g, fs in groups.items():
    rows.append(f"<h2>{html.escape(g)} ({len(fs)})</h2><div style='display:flex;flex-wrap:wrap;gap:8px'>")
    for f in fs:
        rel = pathlib.Path("../..")/f.relative_to(proj) if base in f.parents else pathlib.Path("../../motore_nuovo")/f.relative_to(proj/"motore_nuovo")
        rows.append(f"<figure style='margin:0'><img src='{html.escape(rel.as_posix())}' style='image-rendering:pixelated;height:96px;background:#222'><figcaption>{html.escape(f.name)}</figcaption></figure>")
    rows.append("</div>")
if not groups: rows.append("<p>Nessuna immagine ancora.</p>")
out.write_text("\n".join(rows), encoding="utf-8"); print("galleria:", sum(len(v) for v in groups.values()), "immagini ->", out)
