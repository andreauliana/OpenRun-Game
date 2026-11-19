from pathlib import Path

root = Path(__file__).resolve().parents[3]
out = root / "UFFICIO" / "out" / "grafico" / "ULTIMO_TURNO.md"
out.write_text(
    "# ULTIMO_TURNO — @grafico (19/11/2026 09:16)\n"
    "- **Blocco/passo:** V47 VG3 ∞ (giro 103) — confronto date/contenuto PNG `GRAFICA/scenari/` e gruppi vs `.sglraw`; conversione se più nuovi (script catalogo).\n"
    "- **File cambiati:** `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`.\n"
    "- **Comandi ed exit code:**\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py\"` — exit 0 (160 PNG: 133 confrontati, 27 non motore; scenari 86 raw, skyline 6 allineati, 0 da rigenerare; altri gruppi 47, 0 da convertire, contenuto diverso 0; giocatore 10 PNG, mancanti 5 pose `_b`, confronto saltato, area bloccata).\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py\"` — exit 0 (raw motore 155, tracciati catalogo 155, senza voce 0).\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py\"` — exit 0 (3 non catalogati in `GRAFICA/sprite/giocatore_pose/`: `car-custom.png`, `car-custom-dx.png`, `car-custom-sx.png`, area Andrea).\n"
    "- **Esito:** tutto allineato, nessuna conversione necessaria.\n"
    "- **Gate:** non eseguito (19/11 09:16).\n"
    "- **Licenze/Sega:** zero risorse esterne, zero materiale Sega.\n"
    "- **Prossimo:** VG3 ∞ — ricontrollare al prossimo turno.\n",
    encoding="utf-8",
)
print("ok", out)
