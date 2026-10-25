#!/usr/bin/env python3
# Aggiorna CATALOGO_GRAFICO.json: stati mancante->pronto per scenari lv_04/05/06 e aggiunge voci oggetto individuali.
import json
from pathlib import Path

CAT = Path("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/CATALOGO_GRAFICO.json")
data = json.loads(CAT.read_text())
el = data["elementi"]

def trova_idx(id_cercato):
    for i, e in enumerate(el):
        if e.get("id") == id_cercato:
            return i
    return -1

# 1. skyline lv_05_spray
i = trova_idx("scenario.lv_05_spray.skyline")
el[i]["stato"] = "pronto"
el[i]["file"] = "assets_originali/scenari/lv_05_spray/scenario_spray_05_varianteA/skyline.png"
el[i]["raw"] = "motore_nuovo/dati/sprite_raw/scenari/lv_05_spray/skyline.sglraw"
el[i]["nota"] = "Variante A (D6). Skyline punk: capannoni, ciminiere, muri a spray. Disegnato da zero, nessun materiale Sega."

# 2. skyline lv_06_desktop
i = trova_idx("scenario.lv_06_desktop.skyline")
el[i]["stato"] = "pronto"
el[i]["file"] = "assets_originali/scenari/lv_06_desktop/scenario_desktop_06_varianteA/skyline.png"
el[i]["raw"] = "motore_nuovo/dati/sprite_raw/scenari/lv_06_desktop/skyline.sglraw"
el[i]["nota"] = "Variante A (D6). Skyline desktop: prato, cielo con nuvole, finestre bevel. Disegnato da zero, nessun materiale Sega."

# 3. oggetti lv_04_cromo
i = trova_idx("scenario.lv_04_cromo.oggetti")
el[i]["stato"] = "pronto"
el[i]["file"] = [
    "assets_originali/scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_guard_rail_cromo.png",
    "assets_originali/scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_lampione_lama.png",
    "assets_originali/scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_robot_servizio.png",
    "assets_originali/scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_pilone_cromo.png",
]
el[i].pop("raw", None)
el[i]["nota"] = "Variante A (D6). 4 oggetti propri: guard-rail cromati, lampioni a lama, robot di servizio, pilone cromo. Nessun prestito. Vedi voci oggetto individuali per i raw."

# 4. oggetti lv_05_spray
i = trova_idx("scenario.lv_05_spray.oggetti")
el[i]["stato"] = "pronto"
el[i]["file"] = [
    "assets_originali/scenari/lv_05_spray/scenario_spray_05_varianteA/oggetti/obj_bidone.png",
    "assets_originali/scenari/lv_05_spray/scenario_spray_05_varianteA/oggetti/obj_transenna.png",
    "assets_originali/scenari/lv_05_spray/scenario_spray_05_varianteA/oggetti/obj_cassonetto.png",
    "assets_originali/scenari/lv_05_spray/scenario_spray_05_varianteA/oggetti/obj_serranda_spray.png",
]
el[i].pop("raw", None)
el[i]["nota"] = "Variante A (D6). 4 oggetti propri: bidoni, transenne, cassonetti, serranda spray. Nessun prestito. Vedi voci oggetto individuali per i raw."

# 5. oggetti lv_06_desktop
i = trova_idx("scenario.lv_06_desktop.oggetti")
el[i]["stato"] = "pronto"
el[i]["file"] = [
    "assets_originali/scenari/lv_06_desktop/scenario_desktop_06_varianteA/oggetti/obj_pali_cavi.png",
    "assets_originali/scenari/lv_06_desktop/scenario_desktop_06_varianteA/oggetti/obj_case_computer.png",
    "assets_originali/scenari/lv_06_desktop/scenario_desktop_06_varianteA/oggetti/obj_delfino_cartone.png",
    "assets_originali/scenari/lv_06_desktop/scenario_desktop_06_varianteA/oggetti/obj_icona_cartella.png",
]
el[i].pop("raw", None)
el[i]["nota"] = "Variante A (D6). 4 oggetti propri: pali con cavi, case-computer, delfini di cartone, icona cartella. Nessun prestito. Vedi voci oggetto individuali per i raw."

# 6. Aggiungi voci oggetto individuali (solo se non gia' presenti)
base_idx = trova_idx("scenario.lv_06_desktop.oggetti") + 1

nuovi = []
for lv, nome_file, nome_id, note in [
    ("lv_04_cromo", "guard_rail_cromo", "oggetto.lv_04_cromo.guard_rail_cromo", "guard-rail cromato, stile futurista/cromo"),
    ("lv_04_cromo", "lampione_lama", "oggetto.lv_04_cromo.lampione_lama", "lampione a lama, stile futurista/cromo"),
    ("lv_04_cromo", "robot_servizio", "oggetto.lv_04_cromo.robot_servizio", "robot di servizio, stile futurista/cromo"),
    ("lv_04_cromo", "pilone_cromo", "oggetto.lv_04_cromo.pilone_cromo", "pilone cromo, stile futurista/cromo"),
    ("lv_05_spray", "bidone", "oggetto.lv_05_spray.bidone", "bidone industriale, stile punk/spray"),
    ("lv_05_spray", "transenna", "oggetto.lv_05_spray.transenna", "transenna da cantiere, stile punk/spray"),
    ("lv_05_spray", "cassonetto", "oggetto.lv_05_spray.cassonetto", "cassonetto rifiuti, stile punk/spray"),
    ("lv_05_spray", "serranda_spray", "oggetto.lv_05_spray.serranda_spray", "serranda con scritte spray, stile punk"),
    ("lv_06_desktop", "pali_cavi", "oggetto.lv_06_desktop.pali_cavi", "pali con cavi, stile desktop Windows 95"),
    ("lv_06_desktop", "case_computer", "oggetto.lv_06_desktop.case_computer", "case computer, stile desktop Windows 95"),
    ("lv_06_desktop", "delfino_cartone", "oggetto.lv_06_desktop.delfino_cartone", "delfino di cartone, stile desktop"),
    ("lv_06_desktop", "icona_cartella", "oggetto.lv_06_desktop.icona_cartella", "icona cartella, stile desktop Windows 95"),
]:
    scenario_map = {
        "lv_04_cromo": "scenario_cromo_04_varianteA",
        "lv_05_spray": "scenario_spray_05_varianteA",
        "lv_06_desktop": "scenario_desktop_06_varianteA",
    }
    nome_scenario = scenario_map[lv]
    if trova_idx(nome_id) >= 0:
        # aggiorna percorso file/raw della voce esistente
        idx = trova_idx(nome_id)
        el[idx]["file"] = f"assets_originali/scenari/{lv}/{nome_scenario}/oggetti/obj_{nome_file}.png"
        el[idx]["raw"] = f"motore_nuovo/dati/sprite_raw/scenari/{lv}/{nome_file}.sglraw"
        el[idx]["stato"] = "pronto"
        continue
    nuovi.append({
        "id": nome_id,
        "tipo": "oggetto_bordo_strada",
        "stato": "pronto",
        "file": f"assets_originali/scenari/{lv}/{nome_scenario}/oggetti/obj_{nome_file}.png",
        "raw": f"motore_nuovo/dati/sprite_raw/scenari/{lv}/{nome_file}.sglraw",
        "livelli": [lv],
        "nota": f"Variante A (D6). {note}. Disegnato da zero, nessun materiale Sega."
    })

for n in nuovi:
    el.insert(base_idx, n)
    base_idx += 1

CAT.write_text(json.dumps(data, indent=3) + "\n")
print(f"Catalogo aggiornato: {len(el)} elementi (+{len(nuovi)} oggetti individuali)")
