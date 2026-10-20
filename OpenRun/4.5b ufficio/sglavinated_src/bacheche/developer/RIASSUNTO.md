# Riassunto developer — 20/10 turno 3 (R1 [~], in attesa terzi)
Blocco attivo: R1 tracciati 6 piste [~] — compiti @developer 1-3 conclusi (road.cpp raccordo entrata/uscita min(n/3,60), deriva solo curve != 0; tratti.json 8 tratti CC-BY-4.0; regole.json 6 livelli + default lette da prova_gate; 6 livello.json con campo tracciato). T1-RUN punti 2-3 (colline quota assoluta, sprite wrap modulo totalLen) già in codice.
Ultimo verde: 20/10 19:50 gate_t1.sh soak exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK (rettilineo 55-71%, curva max 0.72-0.98, giro guidato 6/6, fuori strada 0%), ms/frame medio ~0.19 max 7.252 (limite 20), soak 30000 frame, replay 2x IDENTICO (log UFFICIO/Officina/Test area/gate_t1.log; esito out/developer/ESITO_GATE_20OTT_1950.md).
In attesa terzi: review Junior punti 1-3; chiusura CP-005 Capo (review punti 4-5 positiva, rilievo non bloccante); ri-verifica runner Mac Andrea (D12=A) prima di R2 / 60 Hz.
ANDREA_REVIEW T1-Q3/T1-Q5: Mac 19/10 09:39 exit 13, giro guidato fallito 5/6 livelli — percorsi da ottimizzare per varietà (alternanza curve/rettilinei), un parametro alla volta con gate_t1.sh soak; coperto da R1 in attesa review.
Prossimo dopo R1 prima di R2: 60 Hz (dt in secondi, default 60 Hz, opzione 50 Hz, ctest verde a 60 Hz; ANDREA_REVIEW 19 ott Parte 1-2).
Scritture: bacheche/developer/diario.md, bacheche/developer/RIASSUNTO.md, out/developer/.
