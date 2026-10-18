# Registro revisioni — OpenRun 4.6

Registro append-only degli eventi checkpoint. Il dettaglio e le prove sono nei dossier collegati; `STATO_CORRENTE.md` resta il riepilogo aggiornato da leggere per primo.

| Data | ID | Evento | Stato risultante | Riferimento |
|---|---|---|---|---|
| 2026-10-18 | CP-001 | Apertura e registrazione della baseline 4.6 | APERTO | [Dossier CP-001](./CP-001-baseline-openrun-46.md) |
| 2026-10-18 | CP-001 | Chiusura come riferimento documentale; nessuna approvazione di release | CHIUSO (baseline) | [Dossier CP-001](./CP-001-baseline-openrun-46.md) |
| 2026-10-18 | CP-002 | Apertura inventario grafico, verifica strumenti e handoff unico al Capo | APERTO — review indipendente da assegnare | [Dossier CP-002](./CP-002-inventario-grafico.md) |
| 2026-10-18 | CP-003 | Apertura handoff Officina, aggiornamento tool CMake/test e permessi | APERTO — review indipendente e test CTest da completare | [Dossier CP-003](./CP-003-status-officina-e-test.md) |

## Regole di aggiornamento

- Aggiungere una nuova riga in fondo per ogni apertura, cambio stato, blocco, sblocco o chiusura; non modificare né rimuovere eventi precedenti.
- Per un nuovo dossier usare il prossimo ID libero. Per una correzione successiva registrare un nuovo evento nello stesso dossier.
- Aggiornare il registro solo dopo aver annotato l'evento nel dossier e sincronizzato `STATO_CORRENTE.md`.
