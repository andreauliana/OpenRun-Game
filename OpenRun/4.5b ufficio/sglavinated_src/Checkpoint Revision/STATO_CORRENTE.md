# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-10-18. Fonte dettagliata: [CP-001 — baseline](./CP-001-baseline-openrun-46.md).

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-003](./CP-003-status-officina-e-test.md) — status Officina, tool e permessi di test, aperto |
| Stato attività | CP-003 attende review indipendente e test CTest; CP-002 inventario grafico resta aperto in attesa di revisore |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `tools/`; nessuno spostamento fisico completato |
| Ruoli | Capo, Designer, Builder/Developer, Developer/Builder Junior |
| Review | Peer review tra i due sviluppatori; autore distinto dal revisore |
| Build/test | Consultare [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) per il report tecnico corrente; ultimo gate registrato: 3 livelli OK, replay identico, accelerazione/ribaltamento/sterzo/curve OK |
| Limite della prova | Eseguito su macOS; prestazioni e compatibilità su Raspberry Pi 400 non verificate da questo gate |
| Decisioni di Andrea | Non richiesta per la sola registrazione della baseline; nessuna approvazione di release registrata |

## Handoff grafico

Leggere [STATUS_GRAFICO_PER_CAPO.md](../out/designer/STATUS_GRAFICO_PER_CAPO.md) come unico riepilogo aggiornato del Designer. Inventario dettagliato: [CATALOGO_RISORSE.md](../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md). I testi dei fumetti risultano completi; mancano asset grafici approvati per personaggi/concorrenti e le richieste sei percorsi/tre finali animati non sono presenti.

## Handoff Officina

Leggere [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) per l'ultimo report di sviluppo, build, test e blocchi. Il checkpoint attivo è [CP-003](./CP-003-status-officina-e-test.md). Nel checkpoint riportare soltanto esito, misure e riferimenti ai log pertinenti; lo status non sostituisce la peer review.

## Prossime azioni

1. Il Capo assegna revisori indipendenti per CP-002 e CP-003; per CP-003 attiva inoltre il progetto CMake in VS Code e richiede l'esecuzione del test interno.
2. I revisori registrano esito e prove nei rispettivi checkpoint; in caso di rilievi, il Capo mantiene aperto il checkpoint e richiede correzione e nuova review indipendente.
3. Il Capo aggiorna questa scheda e il registro quando cambia lo stato o si apre/chiude un checkpoint.

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
