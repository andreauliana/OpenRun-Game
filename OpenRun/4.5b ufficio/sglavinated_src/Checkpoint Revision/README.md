# Checkpoint Revision

Area protetta per checkpoint e revisioni combinate destinate ad Andrea. Scrittura esclusiva del Capo; gli altri ruoli consegnano le proprie review nei percorsi assegnati.

## Avvio rapido

1. Leggere [STATO_CORRENTE.md](./STATO_CORRENTE.md): riepilogo autorevole, breve e aggiornato.
2. Consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md) per trovare l'ultimo checkpoint e gli eventi recenti.
3. Aprire il checkpoint indicato come attivo. Leggere gli allegati solo se servono per il task.
4. Per una nuova revisione, copiare la struttura di [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md) in un nuovo file `CP-NNN-argomento.md`.

## Regole del registro

- Il Capo è l'unico ruolo che scrive qui. Gli altri ruoli consegnano review e prove nei percorsi loro assegnati.
- `STATO_CORRENTE.md` è una scheda compatta: si aggiorna a ogni apertura/chiusura di checkpoint o cambiamento rilevante; non conserva la cronologia.
- `REGISTRO_REVISIONI.md` è append-only: aggiungere una riga per ogni apertura, cambio stato o chiusura, senza riscrivere le voci precedenti.
- Ogni `CP-NNN-*.md` è il dossier della revisione: decisioni, responsabili/revisori, prove, rilievi, esiti e collegamenti alle fonti.
- Usare ID progressivi univoci `CP-001`, `CP-002`, ... per i dossier; gli eventi dello stesso dossier riutilizzano il suo ID. Aggiornare stato, ultima attività e riferimenti incrociati nella stessa operazione.
- Le prove restano nei log originali. Nel checkpoint registrare comando, exit code, misure essenziali e link relativo al log; non copiare log estesi.
- Gli stati ammessi sono `APERTO`, `IN REVISIONE`, `BLOCCATO`, `PRONTO PER ANDREA`, `CHIUSO`. Un checkpoint può diventare `PRONTO PER ANDREA` solo dopo tutte le review indipendenti positive e le prove richieste. `CHIUSO` richiede che l'esito/decisione di Andrea, oppure la motivazione per cui non è richiesta, sia registrato.
- Un esito positivo non equivale ad approvazione di una decisione riservata ad Andrea. Se manca una review autorizzata o una prova, mantenere `APERTO` o `BLOCCATO`.
- Non cancellare o riscrivere la storia: correggere un dato aggiungendo una nota datata nel checkpoint e aggiornando il riepilogo corrente.

Il checkpoint iniziale [CP-001](./CP-001-baseline-openrun-46.md) registra la baseline di avvio 4.6. Non è un'approvazione di release.
