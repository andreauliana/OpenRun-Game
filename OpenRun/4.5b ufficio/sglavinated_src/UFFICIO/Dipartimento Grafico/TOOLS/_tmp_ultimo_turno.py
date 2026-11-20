from pathlib import Path

root = Path(__file__).resolve().parents[3]
out = root / "UFFICIO" / "out" / "grafico" / "ULTIMO_TURNO.md"
out.write_text(
    "# ULTIMO_TURNO — @grafico (20/11/2026 11:02)\n"
    "- **Blocco/passo:** V47 VG3 ∞ (giro 118) — confronto date/contenuto PNG `GRAFICA/` e gruppi vs `.sglraw`; conversione se più nuovi (script catalogo).\n"
    "- **File cambiati:** `UFFICIO/bacheche/grafico/diario.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`, `UFFICIO/out/grafico/ULTIMO_TURNO.md`. Nessun asset/raw modificato (nulla da convertire).\n"
    "- **Comandi ed exit code:**\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py\"` — exit 1 (solo flag `ALTRI-PNG-SENZA-RAW traffico_incrocio/lato_furgone_bianco.png` = `.png.png` di Andrea 10:42; 161 PNG, 134 confrontati, 27 non motore; 80 oggetti senza raw 0; skyline 6/6 allineati; altri gruppi 47 raw, 45 allineati, da convertire 0, png senza raw 1; contenuto diverso 0; gruppi raw noti 8/sconosciuti 0; giocatore 10 PNG, mancanti 5 pose `_b`, confronto saltato).\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py\"` — exit 0 (raw motore 155, tracciati catalogo 155, senza voce 0).\n"
    "  - `python3 \"UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py\"` — exit 0 (4 non catalogati: 3 car-custom in giocatore_pose area Andrea + `lato_furgone_bianco.png.png`).\n"
    "- **Esito:** tutto allineato, nessuna conversione; resta solo il `.png.png` residuo di Andrea, non toccato, in attesa di decisione.\n"
    "- **Gate:** non eseguito (20/11 11:02).\n"
    "- **Licenze/Sega:** zero risorse esterne, zero materiale Sega.\n"
    "- **Prossimo:** VG3 ∞ — attendere decisione Andrea su `lato_furgone_bianco.png.png`; ricontrollare nuovi PNG.\n",
    encoding="utf-8",
)
print("ok", out)
