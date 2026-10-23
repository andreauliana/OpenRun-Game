# CP-006 — Chiusura T1-RUN (Tappa 1, Gate T1)

| Campo | Valore |
|---|---|
| Stato | PRONTO PER ANDREA |
| Aperto / ultima modifica | 2026-10-23 / 2026-10-23 |
| Ambito e criteri di completamento | Chiusura formale del blocco T1-RUN (Tappa 1): gate T1 verde (`sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, 6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame <= 20), CP-005 chiusa con review incrociata indipendente positiva, prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok). Criteri tutti soddisfatti e registrati. CHIUSO richiede decisione di Andrea (D19 [APERTA]); con risposta mancante si applica DEFAULT A e si procede senza aspettare. |
| Responsabile | @capo (aggregazione esiti; non scrittura tecnica, non autovalutazione) |
| Revisore indipendente | review tecnica già incrociata e registrata in CP-005 (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA; autore distinto dal revisore in entrambi i lati). Nessuna nuova scrittura tecnica in questo dossier. |
| Percorsi autorizzati | Checkpoint Revision/ (dossier, registro, stato), out/capo/ (STATI_PROPOSTI.md) |
| Decisioni richieste ad Andrea | D19 [APERTA] in ANDREA_REVIEW.md: conferma chiusura T1-RUN [x] e stato PRONTO PER ANDREA (avvio R2 subordinato a decisione formale; nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac). DEFAULT A applicato (sì, procedere). Risposta di Andrea mancante -> stato PRONTO PER ANDREA, non CHIUSO (regola 8). |
| Handoff Designer | non applicabile (T1-RUN è codice/build/test; sprite veri dipendono da G1/@grafico, vedi out/grafico/STATUS_GRAFICO_PER_CAPO.md) |
| Handoff Officina | UFFICIO/Officina/STATUS_OFFICINA.md: report Andrea 19/10 09:39 (gate soak exit 13, giro guidato KO 5/6), poi gate verde 20/10 e 21/10 (6/6 piste); ctest 22/10 exit 0, 17/17 passati, log UFFICIO/Officina/build/Testing/Temporary/LastTest.log; gate soak log UFFICIO/Officina/Test area/gate_t1.log. |

## Sintesi

- Obiettivo: chiudere T1-RUN con prove e review già registrate, portare lo stato a PRONTO PER ANDREA in attesa di decisione Andrea su D19.
- Esito corrente: gate tecnico verde (6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10); prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok, registrato in ANDREA_REVIEW.md ultima riga/D19); CP-005 chiusa con review incrociata indipendente positiva su entrambi i lati.
- Blocco o prossimo passo: decisione Andrea su D19 (risposta mancante, DEFAULT A = sì). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A). Non toccare src/runner/ (D16).

## Modifiche

Nessuna modifica tecnica in questo turno (chiusura amministrativa di T1-RUN).

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 2026-10-19 | `sh tools/gate_t1.sh soak` (Andrea, Mac) | exit 13 / FALLITO | giro guidato OK solo lv_02_atomico (40% / 0.86); falliti lv_01_neon, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop | ANDREA_REVIEW.md T1-Q3; STATUS_OFFICINA.md 09:39 |
| 2026-10-20 / 2026-10-21 | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (developer) | exit 0 | 6/6 GATE PERCORSO OK, ms/frame medio <= 0.323 max <= 16.094 (limite 20), replay 2x IDENTICO | UFFICIO/Officina/Test area/gate_t1.log; out/developer/ESITO_TURNO_20OTT_1453.md, ESITO_GATE_20OTT_1923.md, ESITO_GATE_21OTT_0900.md |
| 2026-10-22 | `sh UFFICIO/Officina/TOOL/run_tests.sh` (ctest, developer) | exit 0 | 17/17 test passati, totale 210.63 s, 0 falliti | UFFICIO/Officina/build/Testing/Temporary/LastTest.log; out/developer/ESITO_CTEST_CP005_22OTT_0940.md |
| 2026-10-23 | Prova runner Mac Andrea (v91, SDL2) | runner ok | ctest 17/17 | ANDREA_REVIEW.md ultima riga (D19 [APERTA]) |

## Review indipendente

- Revisore: già registrata in CP-005 — developer-junior → developer punti 1-3 (report out/developer-junior/REVIEW_CP005_PUNTI123_DEVELOPER_2026-10-22.md, POSITIVA, rilievi R1-OSS-1/2/3 non bloccanti, soglie gate invariate); developer → developer-junior punti 4-5 (report out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md e out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md, POSITIVA con rilievo non bloccante). Autore distinto dal revisore in entrambi i lati.
- File/diff esaminati: motore_nuovo/src/road/road.h/.cpp, motore_nuovo/src/game/game.cpp, dati/tracciati/tratti.json, dati/tracciati/regole.json, 6 dati/livelli/*/livello.json, motore_nuovo/test/prova_gate.cpp; diff ancorati al commit 2488a81, working tree pulito (da CP-005).
- Esito: POSITIVA (entrambe le review incrociate).
- Nuova review dopo correzione: non necessaria.
- Status di reparto consultati: STATUS_OFFICINA.md (2026-10-22, ctest 17/17, exit 0, log UFFICIO/Officina/build/Testing/Temporary/LastTest.log); UFFICIO/Officina/Test area/gate_t1.log (gate soak exit 0, 6/6, replay 2x IDENTICO); out/grafico/STATUS_GRAFICO_PER_CAPO.md (2026-10-23 10:41, G1 check [x], exit 0, PNG non catalogati: 0 — non applicabile a T1-RUN).
- Handoff Designer verificato: non applicabile (R1/T1-RUN sono codice/dati; sprite veri dipendono da G1/@grafico).
- Handoff Officina verificato: 2026-10-22, ctest sh UFFICIO/Officina/TOOL/run_tests.sh exit 0, 17/17 passati, report out/developer/ESITO_CTEST_CP005_22OTT_0940.md, log UFFICIO/Officina/build/Testing/Temporary/LastTest.log.

## Decisione di Andrea

- Stato: IN ATTESA (D19 [APERTA], risposta mancante)
- Decisione e data: —
- DEFAULT applicato: A — T1-RUN [x] proposto, PRONTO PER ANDREA, si procede senza aspettare; avvio R2 subordinato a decisione formale; nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac.

## Eventi

- 2026-10-23 — apertura: responsabile @capo, ambito chiusura T1-RUN, criteri gate + CP-005 + prova Mac 23/10, revisore indipendente già registrato in CP-005; DEFAULT A applicato su D19.

## Chiusura

- Esito finale: PRONTO PER ANDREA — gate e review indipendenti registrati e positivi; prove richieste registrate; decisione di Andrea su D19 in attesa (DEFAULT A applicato, si procede senza aspettare).
- Evidenze/review completate: gate sh UFFICIO/Officina/TOOL/gate_t1.sh soak (log UFFICIO/Officina/Test area/gate_t1.log, esiti 20/10 14:53, 20/10 19:23, 21/10 ~09:00); ctest sh UFFICIO/Officina/TOOL/run_tests.sh exit 0 17/17 (log UFFICIO/Officina/build/Testing/Temporary/LastTest.log, report out/developer/ESITO_CTEST_CP005_22OTT_0940.md); review junior→developer punti 1-3 POSITIVA (out/developer-junior/REVIEW_CP005_PUNTI123_DEVELOPER_2026-10-22.md); review sviluppatore→junior punti 4-5 POSITIVA (out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md); prova runner Mac Andrea 23/10 ok (ANDREA_REVIEW.md ultima riga).
- Decisione di Andrea o motivazione per cui non è richiesta: non richiesta per la chiusura tecnica (CP-005, T1-Q5 DEFAULT A già registrato); D19 [APERTA] attende risposta formale di Andrea per passare da PRONTO PER ANDREA a CHIUSO (regola 8).
- Chiuso da Capo il —: non ancora (in attesa decisione Andrea su D19).
