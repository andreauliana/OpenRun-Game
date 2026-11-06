#!/usr/bin/env python3
"""Aggiunge le voci di parapetto e guardrail al catalogo grafico."""
import json
import os

cat_path = os.path.join(os.path.dirname(__file__), "..", "Libreria", "assets_originali", "CATALOGO_GRAFICO.json")
cat_path = os.path.abspath(cat_path)

with open(cat_path, "r", encoding="utf-8") as f:
    cat = json.load(f)

parapetto_entry = {
    "id": "oggetto.lv_01_neon.parapetto",
    "tipo": "oggetto_bordo_strada",
    "stato": "pronto",
    "file": "assets_originali/scenari/comuni/oggetti/parapetto.png",
    "raw": "motore_nuovo/dati/sprite_raw/scenari/parapetto.sglraw",
    "livelli": ["lv_01_neon"],
    "nota": "D6: variante A. Parapetto ponte, stile neutro, 32x48 px. Disegnato da zero, nessun materiale Sega."
}

guardrail_entry = {
    "id": "oggetto.lv_01_neon.guardrail",
    "tipo": "oggetto_bordo_strada",
    "stato": "pronto",
    "file": "assets_originali/scenari/comuni/oggetti/guardrail.png",
    "raw": "motore_nuovo/dati/sprite_raw/scenari/guardrail.sglraw",
    "livelli": ["lv_01_neon"],
    "nota": "D6: variante A. Guardrail autostrada, stile neutro, 48x32 px. Disegnato da zero, nessun materiale Sega."
}

cat["elementi"].append(parapetto_entry)
cat["elementi"].append(guardrail_entry)

with open(cat_path, "w", encoding="utf-8") as f:
    json.dump(cat, f, indent=2, ensure_ascii=False)

print("Catalog updated. Elements count:", len(cat["elementi"]))