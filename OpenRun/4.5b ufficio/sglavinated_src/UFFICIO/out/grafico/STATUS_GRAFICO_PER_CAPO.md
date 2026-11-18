# STATUS_GRAFICO_PER_CAPO — @grafico (18/11/2026 12:08)

- Stato: allineato (VG3 ∞ giro 80); nessuna conversione necessaria. Nessun blocco mio aperto oltre VG3 (VG2 chiuso).
- Controllo turno: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/vg3_date_png_raw.py"` — stampa "exit 0" (145 PNG: 133 confrontati, 12 non motore, 0 fuori copertura; 86 raw scenari: 80 oggetti a byte + 6/6 skyline a contenuto; 47 altri gruppi; da convertire 0, 126 a pari contenuto; gruppi raw noti 8, sconosciuti 0; pose auto 2/2 allineate).
- Catalogo, due controlli in più: `vg3_raw_senza_catalogo.py` exit 0 (142 raw del motore, 142 tracciati, 0 senza voce); `vg3_png_non_catalogati.py` exit 0.
- Modifica del turno: `UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py` — lista `NOTI_NON_ASSET` con motivo (`sprite/giocatore_pose/car-custom-v2.png` = tavola sorgente delle pose di Andrea, area bloccata, non letta dal motore) e conteggio separato "da decidere"/"noti": prima 1 non catalogato, ora 0 da decidere.
- Catalogo JSON non toccato: le voci "mancante" con `raw` previsto restano, sono la convenzione accettata da `check_catalogo_grafico.py` (che esenta lo stato "mancante").
- Pose auto: 2 PNG + 2 raw allineati (auto_dritta, auto_sx4), altre pose "mancante"; area bloccata, non toccata.
- Esito: zero PNG/raw scritti, zero riscritture; catalogo e provenienza coerenti.
- Residui noti: `font.antonio` ttf assente, `gianni`/`gesu`/`ufo` disegni di Andrea, `lv_04_cromo/skyline_trama.png`, `sprite/rivali/auto_giocatore_sbandata.png`.
- Gate: non eseguito (fuori dal blocco VG3). Licenze: nessuna risorsa esterna, zero Sega.
- File turno: diario (+1 riga), RIASSUNTO, questo status. `ULTIMO_TURNO.md` non riscrivibile (permesso negato): vale questo file.
- Review richiesta: nessuna.
