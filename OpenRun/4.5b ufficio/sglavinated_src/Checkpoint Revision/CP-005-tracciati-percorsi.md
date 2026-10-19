# CP-005 — Tracciati modificabili per le 6 piste (blocco R1)

| Campo | Valore |
|---|---|
| Stato | APERTO |
| Aperto / ultima modifica | 2026-10-19 / 2026-10-19 |
| Ambito e criteri di completamento | Blocco R1 di `RICHIESTA_APERTA.md`: `sh tools/gate_t1.sh soak` verde su tutte e 6 le piste (giro guidato OK 6/6, riferimento lv_02_atomico rettilineo ~40% / curva max 0.86), `ctest` 17/17, replay 2x IDENTICO, ms/frame <= 20. Soglie di gate NON si abbassano: si correggono i tracciati (ANDREA_REVIEW.md T1-Q5, DEFAULT A). |
| Responsabile | builder-developer per `road.cpp` (raccordo entrata/uscita, deriva solo su curve), `dati/tracciati/tratti.json`, `dati/tracciati/regole.json`; developer-builder-junior per gate `percorso` (tratto + segmento violato), test di espansione tracciati e riscrittura dei 6 `livello.json` con campo `tracciato`. |
| Revisore indipendente | developer-builder-junior rivede builder-developer (punti 1-3); builder-developer rivede developer-builder-junior (punti 4-5). Autore distinto dal revisore. |
| Percorsi autorizzati | builder-developer: `motore_nuovo/src/road/road.cpp`, `motore_nuovo/dati/tracciati/`, `tools/`, `out/builder-developer/`; developer-builder-junior: `motore_nuovo/test/prova_gate.cpp`, `motore_nuovo/dati/livelli/*/livello.json`, `out/developer-builder-junior/`. |
| Decisioni richieste ad Andrea | Nessuna. T1-Q5 DEFAULT A già registrato in `ANDREA_REVIEW.md` (allinea tutti i livelli al profilo guidabile lv_02_atomico, tuning un parametro alla volta, soglie invariate). |
| Handoff Designer (se grafica coinvolta) | Non applicabile per R1 (tracciati/codice). Gli sprite veri (traffico, personaggi, UFO) dipendono da G1/designer in parallelo; vedi `out/designer/STATUS_GRAFICO_PER_CAPO.md`. |
| Handoff Officina (se codice/build/test coinvolti) | `UFFICIO/Officina/STATUS_OFFICINA.md` aggiornato 2026-10-19 09:39: report Andrea `sh tools/gate_t1.sh soak` su Mac, GATE T1 FALLITO (exit 13); gate core OK (smoke, soak 30000, accelerazione, ribaltamento, sterzo, curve, gara, screenshot P1); ms/frame medio ~0.19 max 0.94; replay 2x IDENTICO; giro guidato FALLITO 5/6 livelli con dettaglio per livello in `ANDREA_REVIEW.md` T1-Q3. |

## Sintesi

- Obiettivo: rendere tutti e 6 i tracciati guidabili (varietà curve/rettilinei per tratto) come richiesto da Andrea (T1-Q3/T1-Q5), senza abbassare le soglie.
- Esito corrente: gate rosso (Andrea 19/10 09:39, exit 13); solo lv_02_atomico passa il giro guidato.
- Blocco o prossimo passo: builder-developer esegue tuning road.cpp + tratti.json + regole.json, un parametro alla volta, rilancia `sh tools/gate_t1.sh soak`; poi developer-builder-junior completa gate/test/6 livello.json e peer review incrociata.

## Modifiche

| Percorso/file | Scopo | Autore |
|---|---|---|
| `motore_nuovo/src/road/road.cpp` | raccordo entrata/uscita a lunghezza propria (campi `entrata`, `uscita`; default min(n/3, 60)); deriva solo su segmenti con curva != 0 | builder-developer |
| `motore_nuovo/dati/tracciati/tratti.json` | libreria tratti nominati (rettilineo, curva, esse, tornante, collina...) con curva/collina/lunghezza/entrata/uscita | builder-developer |
| `motore_nuovo/dati/tracciati/regole.json` | regole per pista (quota rettilinei min/max, rettilineo più lungo, curva massima, quota fuori strada max) lette da prova_gate | builder-developer |
| `motore_nuovo/test/prova_gate.cpp` | gate `percorso` stampa tratto + segmento iniziale violato per ogni pista | developer-builder-junior |
| `motore_nuovo/test/prova_gate.cpp` | test: espansione di ogni `tracciato` dà numero segmenti = somma lunghezze | developer-builder-junior |
| `motore_nuovo/dati/livelli/*/livello.json` | campo `tracciato`: lista di nomi di tratto con override opzionali (compatibile se assente) | developer-builder-junior |

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 2026-10-19 | `sh tools/gate_t1.sh soak` (Andrea, Mac) | exit 13 / FALLITO | giro guidato OK solo lv_02_atomico (40% / 0.86); falliti lv_01_neon, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop | `ANDREA_REVIEW.md` T1-Q3; `STATUS_OFFICINA.md` 09:39 |

## Review indipendente

- Revisore: da designare per ambo i lati (developer-builder-junior su builder-developer; builder-developer su developer-builder-junior).
- File/diff esaminati: IN ATTESA (nessun diff ancora prodotto in questo turno).
- Esito: IN ATTESA
- Rilievi e correzioni richieste: —
- Nuova review dopo correzione: —
- Status di reparto consultati: `STATUS_OFFICINA.md` 2026-10-19 09:39 confrontato con `ANDREA_REVIEW.md` T1-Q3; `STATUS_GRAFICO_PER_CAPO.md` non applicabile a R1.
- Handoff Designer verificato: non applicabile.
- Handoff Officina verificato: 2026-10-19 09:39, report Andrea gate soak exit 13, misure registrate sopra.

## Decisione di Andrea

- Stato: NON RICHIESTA (T1-Q5 DEFAULT A già registrato).
- Decisione e data: —

## Eventi

- `2026-10-19` — apertura: responsabile builder-developer + developer-builder-junior, revisori incrociati diversi dagli autori, ambito e criteri assegnati (soglie invariate, gate soak 6/6 + ctest 17/17).

## Chiusura

- Esito finale: —
- Evidenze/review completate: —
- Decisione di Andrea o motivazione per cui non è richiesta: T1-Q5 DEFAULT A registrato in ANDREA_REVIEW.md; nessuna decisione nuova necessaria per procedere col tuning.
- Chiuso da Capo il `—`: —
