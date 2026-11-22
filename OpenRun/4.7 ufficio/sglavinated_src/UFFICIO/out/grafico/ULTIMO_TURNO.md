# ULTIMO TURNO — @grafico (22/11/2026 11:58, Turno 2)
- Blocco e passo: V47 VG3 ∞ (giro 152) — confronto date PNG/raw in GRAFICA/scenari/; passo ∞ su file diverso dal turno prima (che fu `vg3_date_png_raw.py`).
- File cambiati: `UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py` (`analizza()` restituisce anche `voci_con_raw`: 153 voci con raw, non 159 riferimenti — due voci hanno `raw_elenco` da 4; nuova funzione pura `noti_risolti(presenti)` segnala un raw noto-mancante ricomparso su disco → aggiornare `NOTI_RAW_MANCANTI`; nota in testa, docstring, 2 prove nuove in `--autotest`; exit invariato); `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_ultimo_turno.py` (aggiornato); `UFFICIO/bacheche/grafico/diario.md` (una riga); `UFFICIO/bacheche/grafico/RIASSUNTO.md` (riscritto).
- Comando 1: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py" --autotest` (dopo la modifica) — exit 0 (8 prove OK: senza_voce, raw_inesistenti, duplicati, n_riferimenti, voci_con_raw, classifica_inesistenti, noti_risolti, classifica_percorsi).
- Comando 2: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_raw_senza_catalogo.py"` — exit 0 (155/155, 0 senza voce, 4 refs 'raw' senza file area auto bloccata, 0 risolti, 153 voci/159 riferimenti, 0 sospetti/159 ben formati).
- Comando 3: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — exit 1 (solo i due flag noti in area Andrea; area mia allineata, 0 da convertire).
- Gate: non eseguito (22/11 11:58).
- Catalogo/licenze: invariato; zero risorse esterne, zero Sega; nessun file GRAFICA/ toccato.
- Stato approvazione: nessun nuovo asset proposto; i due flag e i 4 raw mancanti restano decisione Andrea.
