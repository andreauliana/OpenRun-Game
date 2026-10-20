# Esito turno developer — 20/10/2026 17:01 (blocco R1, nessuna esecuzione gate)

## Gate
`sh UFFICIO/Officina/TOOL/gate_t1.sh` NON eseguito in questo turno: shell negata (permission.rejected) — 'gate non eseguito'. Nessun build/test avviato.

## Ultimo gate verde di riferimento
20/10 14:53, `sh UFFICIO/Officina/TOOL/gate_t1.sh`, exit 0, GATE T1 TUTTO OK: 6/6 percorsi GATE PERCORSO OK (lv_01 63%/659/0.85-0.85, lv_02 65%/1440/0.72-0.98, lv_03 57%/590/0.75-0.95, lv_04 71%/1457/0.85-0.85, lv_05 55%/607/0.86-0.84, lv_06 68%/813/0.85-0.85), giro guidato finito su tutti e 6, fuori strada 0%, ms/frame medio ~0.17 max 0.26 (limite 20), replay 2x IDENTICO, gara/accelerazione/ribaltamento/sterzo/curve continue/screenshot P1 OK. Log: `UFFICIO/Officina/Test area/gate_t1.log`. Dettaglio: `out/developer/ESITO_TURNO_20OTT_1453.md`.

## Stato blocco R1 (@developer)
Compiti 1-3 conclusi e verificati su file (nessuna scrittura nuova in motore_nuovo/ e tools/ in questo turno):
- `motore_nuovo/src/road/road.cpp`: raccordo entrata/uscita a lunghezza propria (default min(n/3, 60)), correzione deriva solo su segmenti con curva != 0.
- `motore_nuovo/dati/tracciati/tratti.json`: 8 tratti nominati.
- `motore_nuovo/dati/tracciati/regole.json`: regole per lv_01..lv_06 + default (rettilineo 25-75%, rettilineo_piu_lungo_min 300, curva_max 1.0, curva_dx/sx_min 0.3, fuori_strada_max 0.01).
- `prova_gate` legge `regole.json` al posto delle soglie hardcodate.
- 6 `dati/livelli/*/livello.json` con campo `tracciato` (carattere tema invariato, lv_02_atomico riferimento).

## CP-005 e attese
- CP-005 APERTO (Checkpoint Revision/CP-005-tracciati-percorsi.md): gate verde ottenuto su tutte e 6 le piste; mia review su Junior punti 4-5 registrata (POSITIVA CON RILIEVO NON BLOCCANTE: compito 4 stampa tratto + segmento iniziale mancante, report `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`).
- In attesa: review Junior su punti 1-3, chiusura CP-005 da parte del Capo (gate verde + entrambe le review registrate), ri-verifica runner Mac di Andrea (D12 = A) prima di R2.
- Prossimo per Andrea dopo R1, prima di R2: conversione a 60 Hz (dt in secondi, frequenza predefinita 60 Hz, opzione 50 Hz, ctest verde a 60 Hz, replay 2x IDENTICO) per ANDREA_REVIEW 19/10 Parte 1 e 2. Nessun lavoro dedicato Pi 400/RetroPie prima dell'ok di Andrea su Mac (D12/D14).

## Scritture
bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ (questo file). Nessuna modifica a RICHIESTA_APERTA.md / SCAMBIO.md (Andrea e Claude). Nessuna scrittura in Checkpoint Revision/ (competenza del Capo).
