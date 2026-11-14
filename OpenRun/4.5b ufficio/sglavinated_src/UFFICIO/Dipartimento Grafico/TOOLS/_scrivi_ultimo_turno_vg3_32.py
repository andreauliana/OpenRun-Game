from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
p = ROOT / "UFFICIO/out/grafico/ULTIMO_TURNO.md"
p.write_text(
    "# ULTIMO_TURNO — @grafico (14/11/2026 09:34)\n"
    "- Blocco/passo: V47 VG3 ∞ giro 32 — confronto date PNG Libreria/scenari vs .sglraw e conversione se piu' nuovi.\n"
    "- File cambiati: `UFFICIO/bacheche/grafico/diario.md` (+1), `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.\n"
    "- Comandi:\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py\"` — exit 0 (188 PNG: 142 confrontati, 46 non motore, 0 fuori copertura; 87 raw scenari 81 a byte + 6 skyline a data; 54 altri gruppi allineati; 0 da convertire; gruppi raw noti 8, sconosciuti 0).\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py\"` — exit 0 (raw oggetti aggiornati 0, errori 0).\n"
    "- Esito: tutto allineato, nessun PNG Andrea piu' nuovo, nessuna conversione.\n"
    "- Residuo noto: `sprite/rivali/auto_giocatore_sbandata.png` senza raw, non usato dal motore (non cancellato).\n"
    "- Gate: non eseguito (14/11 09:34).\n"
    "- Licenze/provenienza: nessuna risorsa esterna; nessun materiale Sega.\n"
    "- Prossimo: VG3 ∞ — ricontrollare al prossimo turno.\n",
    encoding="utf-8",
)
print(p)
