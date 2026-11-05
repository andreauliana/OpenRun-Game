# Esito turno developer — 20/10/2026 20:59

Blocco: R1 [~] (tracciati 6 piste) — compiti @developer conclusi; in attesa review Junior punti 1-3, chiusura CP-005 Capo, ri-verifica Mac Andrea (D12=A).

Gate non eseguito in questo turno (attesa terzi).

Ultimo verde noto: 20/10 19:50 gate_t1.sh soak exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK (rettilineo 55-71%, curva max 0.72-0.98, giro guidato 6/6, fuori strada 0%), ms/frame medio ~0.19 max 7.252 (limite 20), soak 30000 frame, replay 2x IDENTICO (log UFFICIO/Officina/Test area/gate_t1.log).

Codice verificato su file:
- motore_nuovo/src/road/road.cpp: raccordo entrata/uscita default min(n/3,60), deriva distribuita solo su segmenti con curva != 0, mai sui rettilinei; colline quota assoluta, camera = quota sotto auto + CAM_HEIGHT.
- motore_nuovo/dati/tracciati/tratti.json: 8 tratti CC-BY-4.0 (rettilineo_corto, rettilineo_lungo, curva_dolce_dx/sx, curva_stretta_dx/sx, esse, tornante, collina_su, collina_giu).
- motore_nuovo/dati/tracciati/regole.json: 6 livelli + default, lette da test/prova_gate.cpp al posto delle soglie hardcoded.
- 6 motore_nuovo/dati/livelli/*/livello.json: campo tracciato (lista di nomi di tratto con override opzionali), loader espande da tratti.json con fallback su segmenti.
- motore_nuovo/src/render/renderer.cpp: drawSprites zRel con std::fmod su totalLen, sprite non spariscono al wrap di playerZ.

ANDREA_REVIEW T1-Q3/T1-Q5: Mac 19/10 09:39 exit 13, giro guidato fallito 5/6 livelli (solo lv_02_atomico OK). Ottimizzazione percorsi richiesta: alternanza curve/rettilinei, un parametro alla volta, soglie non abbassate. Coperto da R1 in attesa review.

Prossimo dopo R1 prima di R2: 60 Hz (dt in secondi, default 60 Hz, opzione 50 Hz, ctest verde a 60 Hz; ANDREA_REVIEW 19 ott Parte 1-2).

Scritture: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_TURNO_20OTT_2059.md. Nessuna scrittura in motore_nuovo/ o tools/ questo turno. Nessun REPORT_TURNO né VOCE_SCAMBIO (attesa terzi).
