#!/usr/bin/env python3
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
content = """# ULTIMO_TURNO — @grafico (11/11/2026 10:53)
- Blocco/passo: V47 VG2 giro 12 (20 frasi)
- File cambiati: `motore_nuovo/dati/frasi.json`, `UFFICIO/Dipartimento Grafico/TOOLS/vg2_giro12.py`
- Bacheca/out: `UFFICIO/bacheche/grafico/diario.md` (+1), `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`
- Comando: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/verifica_frasi_vg2.py"`
- Exit code: 0
- Gate: gate non eseguito
- Note: solo `linea1`/`linea2` EN modificate; `it1`/`it2` intatti; max 16, maiuscolo, no accenti.
"""
OUT.write_text(content, encoding="utf-8")
print("done")
