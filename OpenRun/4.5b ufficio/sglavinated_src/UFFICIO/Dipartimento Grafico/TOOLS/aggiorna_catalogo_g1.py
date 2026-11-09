#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# aggiorna_catalogo_g1.py — MIG1 G1 (Andrea 08/11 sera): allinea CATALOGO_GRAFICO.json
# al ridisegno dei cartelloni sponsor (ridisegna_sponsor.py): stato pronto + nota G1.
# Le voci erano "approvato"/"D2/D2b": ora il disegno cambia e attende l'ok di Andrea,
# quindi lo stato corretto del catalogo e' "pronto". Percorsi e id restano gli stessi.
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]  # sglavinated_src
CAT = ROOT / "UFFICIO" / "Dipartimento Grafico" / "Libreria" / "assets_originali" / "CATALOGO_GRAFICO.json"
NOTA = "MIG1 G1: ridisegno leggibile (ridisegna_sponsor.py, 09/11); parola grande con contorno scuro, dithering solo su sfondo; in attesa ok Andrea"

data = json.loads(CAT.read_text())
el = data["elementi"]

n = 0
for e in el:
    if e.get("id", "").startswith("sponsor."):
        e["stato"] = "pronto"
        e["nota"] = NOTA
        n += 1

CAT.write_text(json.dumps(data, indent=3) + "\n")
print(f"aggiornate {n} voci sponsor (stato=pronto, nota MIG1 G1) su {len(el)} elementi")
