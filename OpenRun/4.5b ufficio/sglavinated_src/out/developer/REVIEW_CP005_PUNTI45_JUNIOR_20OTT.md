# Review CP-005 — punti 4-5 (developer-builder-junior)
Review: @developer su @developer-junior (autore distinto dal revisore). CP-005 APERTO.
Data: 2026-10-20 16:11. Blocco R1, stato [~], gate verde 20/10 14:53.

## File esaminati
- motore_nuovo/test/prova_gate.cpp: verificaPercorso (riga 565), verificaEspansioneTracciato (riga 718), dispatch modo "espansione" (riga 935).
- motore_nuovo/dati/livelli/*/livello.json: campo tracciato (campione lv_02_atomico, lv_05_spray).
- motore_nuovo/dati/tracciati/tratti.json (8 tratti), regole.json (6 livelli + default; licenza CC-BY-4.0, dati originali, nessun materiale Sega).
Contesto (non in merito a questa review): compito 3 regole.json in prova_gate.cpp e' mio (@developer), righe 521-556.

## Esito: POSITIVA CON RILIEVO NON BLOCCANTE

### Requisito punto 4 — gate stampa tratto + segmento iniziale violato
PARZIALE. verificaPercorso stampa per livello metriche aggregate (rettilineo %, rettilineo piu' lungo, curva max dx/sx, fuori strada %) e le relative VIOLAZIONE (righe 603-640), ma NON stampa il nome del tratto né il segmento iniziale di ciascuna violazione. Il requisito "quale TRATTO viola quale regola (nome tratto + segmento iniziale)" non e' implementato. Non blocca il gate verde attuale (6/6 percorsi OK, fuori strada 0%), ma va completato prima di dare compito 4 per chiuso.

### Requisito punto 4 — test espansione tracciati
OK. verificaEspansioneTracciato carica tratti.json e i 6 livello.json, somma le lunghezze dei tratti e verifica segmenti >= somma e multiplo esatto (modo "espansione", riga 935). Stampa esito per livello.

### Requisito punto 5 — 6 livello.json con campo tracciato
OK. Presente in lv_02_atomico e lv_05_spray (campione) e coerente con gate verde 6/6.

## Gate di riferimento (non rieseguito in questo turno: nessuna modifica di codice dopo il verde)
sh UFFICIO/Officina/TOOL/gate_t1.sh — 20/10 14:53, exit 0, 6/6 GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio ~0.17 max 0.26 (<=20), gara/accelerazione/ribaltamento/sterzo/curve/screenshot P1 OK. Log: UFFICIO/Officina/Test area/gate_t1.log.

## Rilievo concreto al Junior (@developer-junior)
Completare verificaPercorso stampando, per ogni riga VIOLAZIONE, il nome del tratto e il segmento iniziale coinvolto (mappatura segmenti->tratto dal campo tracciato di livello.json), come richiesto dal compito 4 di R1.

## Prossimi passi (non di questo ruolo)
Chiusura CP-005 da parte del Capo (gate verde + entrambe le review registrate); ri-verifica runner Mac di Andrea (D12=A) prima di R2 / 60 Hz.
