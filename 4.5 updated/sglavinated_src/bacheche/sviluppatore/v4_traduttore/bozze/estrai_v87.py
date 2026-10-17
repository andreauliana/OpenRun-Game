#!/usr/bin/env python3
"""Estrae i 156 fumetti e le tabelle show dalla v87 (sola lettura).
Legge OPENCODE_REFERENCE_v74.json (riferimento generato da tools/gianni12.py)
e scrive:
  motore_nuovo/dati/frasi.json
  motore_nuovo/dati/personaggi/gianni/fumetti.json
  motore_nuovo/dati/personaggi/gesu/fumetti.json
Non tocca nulla della v87.
"""
import json, pathlib

PROJ = pathlib.Path(__file__).resolve().parent.parent.parent.parent  # sglavinated_src
REF = PROJ / "OPENCODE_REFERENCE_v74.json"
DATI = PROJ / "motore_nuovo" / "dati"

ref = json.loads(REF.read_text(encoding="utf-8"))
phrases = ref["phrases"]  # lista {id, char, line1, line2}
n = len(phrases)
assert n == 156, n
assert [p["id"] for p in phrases] == list(range(156))

# range personaggio dalla v87 (G_IDS / CHAR di gianni12.py)
GIANNI_IDS = (list(range(0, 27)) + list(range(34, 66)) + [85, 86, 87] +
              list(range(88, 105)) + [121, 122, 123, 124, 134, 135, 136,
                                    137, 138, 139, 140, 142, 143, 144, 145,
                                    146, 152, 153])
GIANNI_SET = set(GIANNI_IDS)
GESU_IDS = [i for i in range(156) if i not in GIANNI_SET]
assert len(GIANNI_IDS) + len(GESU_IDS) == 156
assert len(GIANNI_IDS) == 97 and len(GESU_IDS) == 59, (len(GIANNI_IDS), len(GESU_IDS))

def frasi_subset(ids):
    out = []
    by_id = {p["id"]: p for p in phrases}
    for i in ids:
        p = by_id[i]
        out.append({"id": p["id"], "linea1": p["line1"], "linea2": p["line2"]})
    return out

# ---------- dati/frasi.json (fonte unica) ----------
frasi_json = {
    "licenza": "CC-BY-4.0",
    "versione": 1,
    "fonte_v87": "tools/gianni12.py (lista PHR), OPENCODE_REFERENCE_v74.json (build a4422f78 / v85c), HANDOVER.md",
    "totale": 156,
    "id_min": 0,
    "id_max": 155,
    "note": "I testi sono della hack v87 (sola lettura); nel nuovo motore sono dati sostituibili. "
            "I limiti hardware della v87 (156 fumetti, budget Mega Drive) NON si portano.",
    "regole_testo_v87": {
        "maiuscole": True,
        "max_righe": 2,
        "max_larghezza_riga_px": 88,
        "font_riferimento": "DejaVuSans-Bold 8px (solo riferimento v87)",
        "no_accenti": True,
        "no_contenuto_sessuale": True,
        "no_insulti_gruppi": True,
        "no_istruzioni_dosi_modi_mix": True
    },
    "range_personaggio_v87": {
        "gianni": GIANNI_IDS,
        "gesu": GESU_IDS
    },
    "tabelle_show_v87": ref["tables"],
    "frasi": [{"id": p["id"], "personaggio": "gianni" if p["id"] in GIANNI_SET else "gesu",
               "linea1": p["line1"], "linea2": p["line2"]} for p in phrases]
}
(DATI / "frasi.json").write_text(json.dumps(frasi_json, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")

# ---------- per personaggio ----------
TABLES = ref["tables"]
G_TABLES = {
    "otab": TABLES["otab"],          # uscita di strada -> Gianni
    "ph_tab": TABLES["ph_tab"],      # periodico Gianni per stage (4 x 32)
    "g1t": TABLES["escalation"]["gianni"]["g1t"],
    "g2t": TABLES["escalation"]["gianni"]["g2t"],
    "g3t": TABLES["escalation"]["gianni"]["g3t"],
    "itab": [35, 36, 37, 38],        # inattivita' (Gianni; v87 ctab era codice morto)
}
J_TABLES = {
    "xtab": TABLES["xtab"],          # crash -> Gesu'
    "jtab": TABLES["jtab"],          # periodico Gesu'
    "j1t": TABLES["escalation"]["gesu"]["j1t"],
    "j2t": TABLES["escalation"]["gesu"]["j2t"],
    "j3t": TABLES["escalation"]["gesu"]["j3t"],
}

for nome, ids, tables in (("gianni", GIANNI_IDS, G_TABLES), ("gesu", GESU_IDS, J_TABLES)):
    d = DATI / "personaggi" / nome
    d.mkdir(parents=True, exist_ok=True)
    fumetti = {
        "licenza": "CC-BY-4.0",
        "personaggio": nome,
        "fonte_v87": "tools/gianni12.py (PHR/CHAR), OPENCODE_REFERENCE_v74.json",
        "n_frasi": len(ids),
        "id_frasi": ids,
        "frasi": frasi_subset(ids),
        "tabelle_show": tables
    }
    (d / "fumetti.json").write_text(json.dumps(fumetti, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(nome, "->", len(ids), "frasi, tabelle:", list(tables))

print("TOTALE frasi:", n, "| Gianni:", len(GIANNI_IDS), "| Gesu':", len(GESU_IDS))
