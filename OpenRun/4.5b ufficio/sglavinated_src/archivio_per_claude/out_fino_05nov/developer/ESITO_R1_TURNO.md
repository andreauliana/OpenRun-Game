# Risultati condivisi builder-developer — turno 1, 19/10 ~11:50
Blocco: R1 (tracciati 6 piste), stato [~].
Comando eseguito: sh tools/gate_t1.sh soak (su Mac, ordine test Andrea: prima Mac).
Exit code: 0. Log: UFFICIO/Officina/Test area/gate_t1.log.
Esito: GATE T1: TUTTO OK (soak 30000 frame + percorso su tutte le piste).
6 percorsi GATE PERCORSO OK: lv_01_neon rett 65% dx0.57/sx0.53; lv_02_atomico rett 63% dx0.88/sx0.62; lv_03_foglie rett 58% dx0.67/sx0.83; lv_04_cromo rett 54% dx0.37/sx0.63; lv_05_spray rett 73% dx1.01/sx0.99; lv_06_desktop rett 52% dx0.51/sx0.59. Tutti: giro finito, fuori strada 0%, rettilineo piu' lungo >=840 seg.
ms/frame medio ~0.18-0.19, max <=1.0 soak (limite 20); speed@60=161.79; replay 2x IDENTICO su tutte le piste.
Gara a tappe (4 combinazioni) OK; screenshot P1 OK.
R1 compito 1 (road.cpp: raccordo entrata/uscita, correzione deriva solo su curve) gia' presente e verificato: risolve variabilita' percorsi.
Mancanti R1 compiti 2-3: dati/tracciati/tratti.json, regole.json, lettura regole.json in prova_gate (scrittura nuova cartella motore_nuovo/dati/tracciati/ bloccata questo turno; gate passa comunque con soglie esistenti).
Compiti 4-5 (gate tratto-violazione + riscrittura 6 livello.json con tracciato) -> Junior. Zero scritture in motore_nuovo/ e tools/.
