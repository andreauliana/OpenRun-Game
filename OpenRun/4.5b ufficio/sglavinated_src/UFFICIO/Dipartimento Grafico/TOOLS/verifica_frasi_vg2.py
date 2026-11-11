#!/usr/bin/env python3
"""Verifica regole frasi.json per VG2 (gruppo grafico).
Regole: max 2 righe, max 16 caratteri per riga (linea1/linea2), maiuscole,
no accenti nel testo EN, campi it1/it2 presenti. Esito: exit 0 se tutto OK.
"""
import json
import sys
from pathlib import Path

RADICE = Path(__file__).resolve().parents[3]
PERCORSO = RADICE / "motore_nuovo" / "dati" / "frasi.json"

def main() -> int:
    dati = json.loads(PERCORSO.read_text(encoding="utf-8"))
    errori = []
    for frase in dati["frasi"]:
        fid = frase["id"]
        for campo in ("linea1", "linea2"):
            testo = frase.get(campo, "")
            if len(testo) > 16:
                errori.append(f"id {fid}: {campo} > 16 ({len(testo)}): {testo!r}")
            if testo != testo.upper():
                errori.append(f"id {fid}: {campo} non maiuscolo: {testo!r}")
            if any(ch in testo for ch in "àèéìòùÀÈÉÌÒÙ"):
                errori.append(f"id {fid}: {campo} contiene accenti: {testo!r}")
        for campo in ("it1", "it2"):
            if campo not in frase or not frase[campo]:
                errori.append(f"id {fid}: manca {campo}")
        if not frase.get("personaggio"):
            errori.append(f"id {fid}: manca personaggio")
    totale = len(dati["frasi"])
    if totale != dati["totale"]:
        errori.append(f"totale campo {dati['totale']} != frasi presenti {totale}")
    if errori:
        for errore in errori:
            print(errore)
        return 1
    print(f"OK: {totale} frasi, regole rispettate (max 16, maiuscolo, no accenti, it1/it2 presenti)")
    return 0

if __name__ == "__main__":
    sys.exit(main())
