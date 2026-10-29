# Esito turno PIST1 — 27/10/2026 22:24 — @developer

Blocco: PIST1 [ ] — percorsi più vari: ponti, colline, chicane, autostrada, città (task 1-2 @developer).
Stato di fine turno: avviato. Task 1 (nuovi tipi di tratto in tratti.json/regole.json) e task 2 (ogni livello usa almeno 3 tipi coerenti col tema) applicati ai dati; task 3 (@grafico: oggetti ponte/parapetto, autostrada/guardrail, città/facciate) e task 4 (@developer-junior: gate tratti) posticipati.

File modificati (dati, nessun sorgente C++ toccato):
- motore_nuovo/dati/tracciati/tratti.json: nuovi tratti chicane_dx (curva 0.85, 140 seg), chicane_sx (curva -0.85, 140 seg), ponte (larghezza 1, 400 seg), autostrada (larghezza 2, 520 seg), citta (curva 0.7, 200 seg), valle (collina -0.35, 220 seg); nota aggiornata. Licenza CC-BY-4.0, tracce originali, nessun materiale Sega.
- motore_nuovo/dati/livelli/lv_01_neon/livello.json: tracciato + autostrada, ponte.
- motore_nuovo/dati/livelli/lv_02_atomico/livello.json: tracciato + chicane_dx, chicane_sx.
- motore_nuovo/dati/livelli/lv_03_foglie/livello.json: tracciato + valle, chicane_sx.
- motore_nuovo/dati/livelli/lv_04_cromo/livello.json: tracciato + autostrada, ponte.
- motore_nuovo/dati/livelli/lv_05_spray/livello.json: tracciato + citta, chicane_dx.
- motore_nuovo/dati/livelli/lv_06_desktop/livello.json: tracciato + valle, ponte.

Note: larghezza per segmento già supportata dal core (game.cpp leggiCampiTratto clamp 1.0-2.0; road.cpp raccordo entrata/uscita min(n/3,60) senza scatti). I nomi di tracciato sconosciuti verrebbero saltati con fallback su "segmenti", quindi nessun rischio di crash.

Build/test/gate: NON eseguibili in questo turno (tool shell non disponibile: comandi respinti). gate non eseguito. Validazione JSON e cmake --build --target prova_gate da eseguire al prossimo turno con shell disponibile.

Vincoli rispettati: core indipendente da SDL, determinismo invariato (stesso seme = stesso tracciato), replay invariato, nessun materiale Sega, nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Mac (D19=B in ANDREA_REVIEW.md).

Scritture: bacheche/developer/diario.md (riga 31), bacheche/developer/RIASSUNTO.md (riscritto, 10 righe), out/developer/ESITO_PIST1_TRATTI_27OTT.md.

--- 29/10/2026 ~06:30 (developer, questo turno)
Gate sh UFFICIO/Officina/TOOL/gate_t1.sh (non soak) eseguito 29/10/2026 ~06:30, comando singolo, timeout 12 min:
- cmake configure OK, cmake --build target prova_gate OK.
- lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo: prova_gate exit 0 + percorso exit 0.
- lv_05_spray: prova_gate exit 0, percorso lv_05 TIMEOUT >12 min (giro guidato non termina, nessun exit code dello script); lv_06_desktop non raggiunto.
Esito: gate NON completato / non verde. Sospetto regressione del giro guidato sui nuovi tratti citta/chicane di lv_05 (PIST1 task 1-2, dati modificati 27/10 sera).
Prossimo turno: isolare lv_05 (confrontare tracciato/segmenti vs lv_01-04) e correggere; nessun lavoro Pi 400/RetroPie prima di ok Mac (D19=B in ANDREA_REVIEW.md).
