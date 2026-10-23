# Registro revisioni — OpenRun 4.6

Registro append-only degli eventi checkpoint. Il dettaglio e le prove sono nei dossier collegati; `STATO_CORRENTE.md` resta il riepilogo aggiornato da leggere per primo.

| Data | ID | Evento | Stato risultante | Riferimento |
|---|---|---|---|---|
| 2026-10-18 | CP-001 | Apertura e registrazione della baseline 4.6 | APERTO | [Dossier CP-001](./CP-001-baseline-openrun-46.md) |
| 2026-10-18 | CP-001 | Chiusura come riferimento documentale; nessuna approvazione di release | CHIUSO (baseline) | [Dossier CP-001](./CP-001-baseline-openrun-46.md) |
| 2026-10-18 | CP-002 | Apertura inventario grafico, verifica strumenti e handoff unico al Capo | APERTO — review indipendente da assegnare | [Dossier CP-002](./CP-002-inventario-grafico.md) |
| 2026-10-18 | CP-003 | Apertura handoff Officina, aggiornamento tool CMake/test e permessi | APERTO — review indipendente e test CTest da completare | [Dossier CP-003](./CP-003-status-officina-e-test.md) |
| 2026-10-18 | CP-004 | Consolidamento fonte Andrea e classificazione archivio v4 | APERTO — review indipendente da assegnare | [Dossier CP-004](./CP-004-review-e-archivio-v4.md) |
| 2026-10-19 | CP-005 | Apertura blocco R1 tracciati modificabili per le 6 piste (gate percorso 5/6 fallito) | APERTO — review indipendente da assegnare | [Dossier CP-005](./CP-005-tracciati-percorsi.md) |
| 2026-10-22 | CP-005 | Chiusura formale del blocco R1 tracciati per le 6 piste: review incrociata indipendente completa (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA) e ctest 17/17 verde (exit 0); gate soak verde già registrato (6/6 piste, replay 2x IDENTICO, ms/frame <= 20) | CHIUSO | [Dossier CP-005](./CP-005-tracciati-percorsi.md) |
| 2026-10-23 | — | T1-RUN: gate verde (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10), CP-005 chiusa con review incrociata indipendente positiva, prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok); T1-RUN proposto [x] (DEFAULT D19=A), decisione Andrea in attesa (PRONTO PER ANDREA, regola 8) | APERTO (in attesa decisione Andrea) | STATO_CORRENTE.md, out/capo/STATI_PROPOSTI.md, ANDREA_REVIEW.md (D19 [APERTA]) |
| 2026-10-23 | CP-006 | Chiusura formale T1-RUN (Tappa 1, Gate T1): gate verde registrato (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20), CP-005 review incrociata indipendente positiva, prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok); DEFAULT A applicato su D19 [APERTA] (risposta mancante), si procede senza aspettare | PRONTO PER ANDREA | [Dossier CP-006](./CP-006-t1-run-chiusura.md) |

## Regole di aggiornamento

- Aggiungere una nuova riga in fondo per ogni apertura, cambio stato, blocco, sblocco o chiusura; non modificare né rimuovere eventi precedenti.
- Per un nuovo dossier usare il prossimo ID libero. Per una correzione successiva registrare un nuovo evento nello stesso dossier.
- Aggiornare il registro solo dopo aver annotato l'evento nel dossier e sincronizzato `STATO_CORRENTE.md`.
