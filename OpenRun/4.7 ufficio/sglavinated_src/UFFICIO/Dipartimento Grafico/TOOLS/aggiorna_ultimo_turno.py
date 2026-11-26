#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
DEST = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
DEST_STATUS = RADICE / "UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md"
CONTENUTO = """\
# ULTIMO TURNO — @grafico (26/11/2026 19:16, Turno 1, giro 228)
- **Blocco/passo:** V47 VG3 ∞ (giro 228), passo ∞ "PNG di Andrea convertiti": controllo date PNG/raw e refactor di un tool di diagnosi; nessun asset convertito (area allineata).
- **File cambiati:** `UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py` (nota in testa, funzione pura `report_da_disco` con carica/scansiona/esiste iniettabili, uso in `main()`, autotest +1), `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py`, `UFFICIO/bacheche/grafico/diario.md` (una riga), `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md` e `STATUS_GRAFICO_PER_CAPO.md` (handoff esistente aggiornato).
- **Comando 1:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0; output identico al baseline (155/155 raw tracciati, 4 noti area auto, 0 da decidere).
- **Comando 2:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` — exit 0, 32 prove OK (+1 `report_da_disco`).
- **Comando 3:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0, area allineata (0 da convertire).
- **Comando 4:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` — exit 0, 33 prove OK.
- **Comando 5:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 noti non-asset, 0 da decidere).
- **Comando 6:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0, 28 prove OK.
- **Comando 7:** `python3 "UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py"` — riscrive i due handoff di `UFFICIO/out/grafico/`.
- **Gate:** non eseguito (26/11/2026 19:16); il blocco non chiede gate.
- **Licenze/provenienza:** invariate; zero risorse esterne, zero materiale Sega; PNG e raw di Andrea non toccati.
- **Stato approvazione:** diagnosi interna, non proposta di asset; nessuna review richiesta dal blocco.
"""
STATO = """\
# STATUS_GRAFICO_PER_CAPO — @grafico (26/11/2026 19:16, Turno 1, giro 228)
- **Blocco/passo:** V47 VG3 ∞ — confronto date/contenuto PNG di `GRAFICA/scenari/` coi `.sglraw`; produzione su `vg3_raw_senza_catalogo.py` (diverso dal turno prima: `vg3_date_png_raw.py`).
- **Stato:** area mia allineata, 0 conversioni. Skyline 6/6; 165 PNG (133 confrontati, 32 non motore), 80 oggetti senza raw 0, altri 47 raw/46 allineati, 1 png piu' nuovo a pari contenuto, doppia estensione 0. 155 raw/155 tracciati, 0 senza voce; 153 voci/159 riferimenti, 4 refs noti area auto bloccata, 0 risolti, 0 duplicati/ripetuti/scritture diverse/sospetti. 3 PNG non catalogati noti non-asset, 0 da decidere. 5 pose `_b` con raw e senza PNG: area Andrea, non azionabile.
- **Materiali modificati:** nessun PNG/raw/catalogo. Codice: `UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py` — la sequenza di letture disco in `main()` (carica_catalogo -> raw_su_disco -> esiste_raw -> report_da_dati) estratta in `report_da_disco(catalogo, radice, raw_root, carica=None, scansiona=None, esiste=None)`, pura con dipendenze iniettabili, usata in `main()`, nota in testa, autotest dedicato (dati finti, chiamate registrate); output invariato.
- **Catalogo/licenze:** `CATALOGO_GRAFICO.json` invariato; zero risorse esterne, zero materiale Sega.
- **Comandi ed exit code (26/11 19:16):**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155, 0 senza voce, 4 noti bloccati); output identico al baseline.
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` — exit 0 (32 prove OK, prima 31).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 0 (0 da convertire, 6/6 skyline, 165 PNG/133 confrontati, doppia estensione 0).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` — exit 0 (33 prove OK).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` — exit 0 (3 noti non-asset, 0 da decidere).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` — exit 0 (28 prove OK).
- **Lacune/blocchi:** auto del giocatore (5 pose `_b`, 3 `car-custom*`, 4 raw sx1/sx3) e cartelloni sponsor: area Andrea, non azionabile da qui.
- **Gate:** non eseguito (26/11 19:16). **Review richiesta:** nessuna (il blocco non chiede una REVIEW_).
- **Prossimo:** VG3 ∞ — ricontrollare PNG/raw nuovi; convertire solo se PNG piu' nuovo, mai area Andrea; prossimo tool `vg3_png_non_catalogati.py`.
"""

if __name__ == "__main__":
    DEST.write_text(CONTENUTO, encoding="utf-8")
    DEST_STATUS.write_text(STATO, encoding="utf-8")
    print(f"OK {DEST} {DEST_STATUS}")
