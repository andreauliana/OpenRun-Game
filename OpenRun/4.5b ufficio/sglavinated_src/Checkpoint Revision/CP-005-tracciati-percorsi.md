# CP-005 — Tracciati modificabili per le 6 piste (blocco R1)

| Campo | Valore |
|---|---|
| Stato | CHIUSO |
| Aperto / ultima modifica | 2026-10-19 / 2026-10-22 |
| Ambito e criteri di completamento | Blocco R1 di `RICHIESTA_APERTA.md`: `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` verde su tutte e 6 le piste (giro guidato OK 6/6, riferimento lv_02_atomico rettilineo ~40% / curva max 0.86), `ctest` 17/17, replay 2x IDENTICO, ms/frame <= 20. Soglie di gate NON si abbassano: si correggono i tracciati (ANDREA_REVIEW.md T1-Q5, DEFAULT A). Criteri tutti soddisfatti e registrati. |
| Responsabile | builder-developer per `road.cpp` (raccordo entrata/uscita, deriva solo su curve), `dati/tracciati/tratti.json`, `dati/tracciati/regole.json`; developer-junior per gate `percorso` (tratto + segmento violato), test di espansione tracciati e riscrittura dei 6 `livello.json` con campo `tracciato`. |
| Revisore indipendente | developer-junior rivede builder-developer (punti 1-3, POSITIVA); builder-developer rivede developer-junior (punti 4-5, POSITIVA). Autore distinto dal revisore. |
| Percorsi autorizzati | builder-developer: `motore_nuovo/src/road/road.cpp`, `motore_nuovo/dati/tracciati/`, `UFFICIO/Officina/TOOL/`, `out/developer/`; developer-junior: `motore_nuovo/test/prova_gate.cpp`, `motore_nuovo/dati/livelli/*/livello.json`, `out/developer-junior/`. |
| Decisioni richieste ad Andrea | Nessuna. T1-Q5 DEFAULT A già registrato in `ANDREA_REVIEW.md` (allinea tutti i livelli al profilo guidabile lv_02_atomico, tuning un parametro alla volta, soglie invariate). |
| Handoff Designer (se grafica coinvolta) | Non applicabile per R1 (tracciati/codice). Gli sprite veri (traffico, personaggi, UFO) dipendono da G1/@grafico in parallelo; vedi `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. |
| Handoff Officina (se codice/build/test coinvolti) | `UFFICIO/Officina/STATUS_OFFICINA.md`: report Andrea 2026-10-19 09:39 (gate soak exit 13, giro guidato KO 5/6), poi gate verde registrato 20/10 e 21/10 (6/6 piste); ctest 2026-10-22 `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, 17/17 passati, log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`. |

## Sintesi

- Obiettivo: rendere tutti e 6 i tracciati guidabili (varietà curve/rettilinei per tratto) come richiesto da Andrea (T1-Q3/T1-Q5), senza abbassare le soglie.
- Esito corrente: tutti e 6 i livelli passano il giro guidato (gate verde 20/10 14:53, 20/10 19:23, 21/10 ~09:00); riferimento storico rosso superato (Andrea 19/10 09:39, exit 13, 5/6 falliti).
- Blocco o prossimo passo: CP-005 chiuso formalmente dal @capo. T1-RUN resta [~] in attesa della prova runner Mac di Andrea (D13 = B; D12/D14 = A; nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Mac).

## Modifiche

| Percorso/file | Scopo | Autore |
|---|---|---|
| `motore_nuovo/src/road/road.cpp` | raccordo entrata/uscita a lunghezza propria (campi `entrata`, `uscita`; default min(n/3, 60)); deriva solo su segmenti con curva != 0 | builder-developer |
| `motore_nuovo/src/road/road.h` | `SegmentDesc` con `entrata`/`uscita` | builder-developer |
| `motore_nuovo/src/game/game.cpp` | supporto campo `tracciato` di `livello.json`, espansione da `dati/tracciati/tratti.json`, fallback su `segmenti` | builder-developer |
| `motore_nuovo/dati/tracciati/tratti.json` | libreria tratti nominati (rettilineo, curva, esse, tornante, collina...) con curva/collina/lunghezza/entrata/uscita | builder-developer |
| `motore_nuovo/dati/tracciati/regole.json` | regole per pista (quota rettilinei min/max, rettilineo più lungo, curva massima, quota fuori strada max) lette da prova_gate | builder-developer |
| `motore_nuovo/test/prova_gate.cpp` | gate `percorso` stampa tratto + segmento iniziale violato per ogni pista; test espansione tracciati | developer-junior |
| `motore_nuovo/dati/livelli/*/livello.json` | campo `tracciato`: lista di nomi di tratto con override opzionali (compatibile se assente) | developer-junior |

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 2026-10-19 | `sh tools/gate_t1.sh soak` (Andrea, Mac) | exit 13 / FALLITO | giro guidato OK solo lv_02_atomico (40% / 0.86); falliti lv_01_neon, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop | `ANDREA_REVIEW.md` T1-Q3; `STATUS_OFFICINA.md` 09:39 |
| 2026-10-20 | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (developer, ~14:53 e ~19:23) | exit 0 | 6/6 GATE PERCORSO OK, ms/frame medio ~0.17 max 0.323, replay 2x IDENTICO | `UFFICIO/Officina/Test area/gate_t1.log`; `out/developer/ESITO_TURNO_20OTT_1453.md`, `ESITO_GATE_20OTT_1923.md` |
| 2026-10-21 | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (developer, ~09:00) | exit 0 | 6/6 GATE PERCORSO OK, ms/frame medio 0.291 max 16.094 (limite 20), replay 2x IDENTICO, speed@60=161.79 | `UFFICIO/Officina/Test area/gate_t1.log`; `out/developer/ESITO_GATE_21OTT_0900.md` |
| 2026-10-22 | `sh UFFICIO/Officina/TOOL/run_tests.sh` (ctest, developer) | exit 0 | 17/17 test passati, totale 210.63 s, 0 falliti | `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`; report `out/developer/ESITO_CTEST_CP005_22OTT_0940.md` |
| 2026-10-22 | Review indipendente developer-junior → developer punti 1-3 | POSITIVA | rilievi R1-OSS-1/2/3 non bloccanti; soglie gate invariate (`MAX_MS_FRAME = 20.0`) | `out/developer-junior/REVIEW_CP005_PUNTI123_DEVELOPER_2026-10-22.md` |
| 2026-10-22 | Review indipendente developer → developer-junior punti 4-5 (precedente, riconfermata) | POSITIVA | rilievo non bloccante su compito 4 | `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md` |

## Review indipendente

- Revisore: developer-junior → developer (punti 1-3), report `out/developer-junior/REVIEW_CP005_PUNTI123_DEVELOPER_2026-10-22.md`, esito POSITIVA, rilievi R1-OSS-1/2/3 tutti non bloccanti; developer → developer-junior (punti 4-5), report precedente `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md` e `out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md`, esito POSITIVA con rilievo non bloccante. Autore distinto dal revisore in entrambi i lati.
- File/diff esaminati: `motore_nuovo/src/road/road.h/.cpp`, `motore_nuovo/src/game/game.cpp`, `dati/tracciati/tratti.json`, `dati/tracciati/regole.json`, 6 `dati/livelli/*/livello.json`, `motore_nuovo/test/prova_gate.cpp`; diff ancorati al commit `2488a81`, working tree pulito.
- Esito: POSITIVA (entrambe le review incrociate).
- Rilievi e correzioni richieste: non bloccanti (raccordo entrata di fatto no-op su tratti 2..n, nessun impatto sui 6 livelli attuali; `leggiTracciato` salta silenziosamente nomi di tratto sconosciuti, coperto dal gate espansione rc 11; conteggio test riconciliato a 17/17 con la run ctest del 2026-10-22).
- Nuova review dopo correzione: non necessaria.
- Status di reparto consultati: `STATUS_OFFICINA.md` 2026-10-22 (ctest 17/17, exit 0, log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`) e `UFFICIO/Officina/Test area/gate_t1.log` (gate soak exit 0, 6/6, replay 2x IDENTICO).
- Handoff Designer verificato: non applicabile (R1 è codice/dati tracciati; sprite veri dipendono da G1/@grafico, vedi `out/grafico/STATUS_GRAFICO_PER_CAPO.md`).
- Handoff Officina verificato: 2026-10-22, ctest `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, 17/17 passati, report `out/developer/ESITO_CTEST_CP005_22OTT_0940.md`, log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`.

## Decisione di Andrea

- Stato: NON RICHIESTA (T1-Q5 DEFAULT A già registrato).
- Decisione e data: —

## Eventi

- `2026-10-19` — apertura: responsabile builder-developer + developer-junior, revisori incrociati diversi dagli autori, ambito e criteri assegnati (soglie invariate, gate soak 6/6 + ctest 17/17).
- `2026-10-22` — chiusura formale: review incrociata indipendente completa (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA) e ctest 17/17 verde (exit 0); gate soak 6/6 già registrato. Chiuso da Capo.

## Chiusura

- Esito finale: CHIUSO — gate soak verde registrato (6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame <= 20), ctest 17/17 verde (exit 0), review incrociata indipendente POSITIVA su entrambi i lati.
- Evidenze/review completate: gate `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (log `UFFICIO/Officina/Test area/gate_t1.log`, esiti 20/10 14:53, 20/10 19:23, 21/10 ~09:00); ctest `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 17/17 (log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`, report `out/developer/ESITO_CTEST_CP005_22OTT_0940.md`); review junior→developer punti 1-3 POSITIVA (`out/developer-junior/REVIEW_CP005_PUNTI123_DEVELOPER_2026-10-22.md`); review sviluppatore→junior punti 4-5 POSITIVA (`out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`).
- Decisione di Andrea o motivazione per cui non è richiesta: non richiesta per CP-005 (T1-Q5 DEFAULT A già registrato in ANDREA_REVIEW.md). T1-RUN resta [~] in attesa della prova runner Mac di Andrea (D13 = B; D12/D14 = A; nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Mac).
- Chiuso da Capo il `2026-10-22`: CP-005 chiuso.
