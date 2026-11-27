#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Handoff ULTIMO_TURNO.md del @grafico: riscritto a ogni turno.
# 27/11 09:37: si scrive solo ULTIMO_TURNO.md (il turno non chiede STATUS_ o REVIEW).
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
DEST = RADICE / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
CONTENUTO = """\
# ULTIMO TURNO — @grafico (27/11/2026 09:37, Turno 2, giro 258)
- **Blocco/passo:** V47 VG3 ∞ (giro 258) — confronto date PNG/raw in `GRAFICA/scenari/`; tutto allineato, 0 da convertire. Manutenzione rotazione: `vg3_raw_senza_catalogo.py`.
- **Esito VG3:** 6/6 skyline allineati; 165 PNG (133 confrontati, 32 non motore); 80 oggetti senza raw 0; altri 47 raw / 46 allineati, 1 PNG piu' nuovo a pari contenuto (rumore); doppia estensione 0; 5 pose `_b` giocatore bloccata (area di Andrea, intoccata).
- **File cambiati:**
  - `UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py` — le due chiusure `reporta` dell'autotest di `esegui()` (risultato costante) estratte nella funzione pura `reporta_fisso(righe, codice)`, usata nei due `esegui()` di prova, nota in testa, autotest dedicato (risultato costante al variare degli argomenti, lista vuota). Output identico al baseline.
  - `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py` — contenuto aggiornato per l'handoff.
  - `UFFICIO/bacheche/grafico/diario.md` (append 1 riga), `UFFICIO/bacheche/grafico/RIASSUNTO.md` e `UFFICIO/out/grafico/ULTIMO_TURNO.md` — riscritti.
- **Comandi/exit code (eseguiti in questo turno, 27/11 09:37):**
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` → exit 0 (0 da convertire; 6/6 skyline; 165 PNG/133 confrontati/32 non motore; 5 pose _b bloccata).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` → exit 0 (155/155, 4 refs noti auto bloccata, 0 da decidere; output identico al baseline).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"` → exit 0 (3 noti non-asset, 0 da decidere).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` → exit 0 (43 prove, +1 `reporta_fisso`).
  - `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py" --autotest` → exit 0 (43 prove); `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py" --autotest` → exit 0 (37 prove).
- **Gate:** non eseguito (27/11 09:37); il blocco non lo chiede.
- **Catalogo/licenze/stato:** invariati; 0 risorse esterne; 0 Sega; PNG/raw di Andrea intoccati; nessun asset nuovo proposto né approvato. Prossimo tool a rotazione: `vg3_png_non_catalogati.py`.
"""

if __name__ == "__main__":
    DEST.write_text(CONTENUTO, encoding="utf-8")
    print(f"OK {DEST}")
