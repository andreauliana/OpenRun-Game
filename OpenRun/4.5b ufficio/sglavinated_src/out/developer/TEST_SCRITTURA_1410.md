# Risultati turno builder-developer — 19/10/2026 14:10

Blocco: R1 (tracciati 6 piste), stato [~].

Comando eseguito: nessuno (shell negata in questa sessione).
Gate: sh tools/gate_t1.sh soak NON eseguito -> gate non eseguito.

Scrittura:
- motore_nuovo/dati/: NEGATA (tentativo dati/tracciati/tratti.json: Permission denied).
- motore_nuovo/test/prova_gate.cpp: non scrivibile.
- motore_nuovo/docs/FORMATI_DATI.md: non scrivibile.
- out/developer/: SCRIVIBILE (esito di questo turno).
- bacheche/developer/: SCRIVIBILE (diario, RIASSUNTO aggiornati).

Verifiche lettura (nessuna scrittura eseguita):
- compito 1 road.cpp: fatto (raccordo entrata/uscita a lunghezza propria, correzione deriva distribuita solo su curve != 0, rettilinei intatti).
- tutti e 6 i livello.json hanno gia' campo "tracciato" (nomi tratti usati: rettilineo_corto/lungo, curva_dolce_dx/sx, curva_stretta_dx/sx, collina_su/giu).
- loader game.cpp legge già motore_nuovo/dati/tracciati/tratti.json, ma il file manca -> fallback su "segmenti".

Bloccati:
- compiti 2-3: dati/tracciati/tratti.json e regole.json non creabili; lettura regole.json in prova_gate non implementabile da questo ruolo.

Ultimo esito gate (Andrea, 19/10 09:39, Mac): exit 13, GATE T1 FALLITO.
- giro guidato KO 5/6: lv_01_neon rett 13%, lv_03_foglie 18%, lv_04_cromo 23%, lv_05_spray 21%, lv_06_desktop 12% (soglia gate 25-75%).
- solo lv_02_atomico OK: rett 40%, curva max 0.86 (riferimento T1-Q5 DEFAULT A).
- core OK: ms/frame medio ~0.19 max 0.94 (limite 20), replay 2x IDENTICO su 6 livelli.

Azione T1-Q5 DEFAULT A (sblocco scrittura richiesto): creare dati/tracciati/tratti.json e regole.json, allineare percorsi al profilo guidabile lv_02_atomico, rilanciare sh tools/gate_t1.sh soak su tutte le 6 piste, poi ctest 17/17. Compiti 4-5 (Junior) restano in attesa.

Zero scritture in motore_nuovo/ e tools/ questo turno. Nessun REPORT_TURNO, nessuna VOCE_SCAMBIO.
