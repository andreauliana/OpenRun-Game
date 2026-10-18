# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-10-18. Fonte dettagliata: [CP-001 — baseline](./CP-001-baseline-openrun-46.md).

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-001](./CP-001-baseline-openrun-46.md) — baseline iniziale, chiuso |
| Stato attività | Nessun checkpoint di lavoro aperto |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `tools/`; nessuno spostamento fisico completato |
| Ruoli | Capo, Designer, Builder/Developer, Developer/Builder Junior |
| Review | Peer review tra i due sviluppatori; autore distinto dal revisore |
| Build/test | Gate locale più recente: `sh tools/gate_t1.sh`, exit 0; 3 livelli OK, replay identico, accelerazione/ribaltamento/sterzo/curve OK |
| Limite della prova | Eseguito su macOS; prestazioni e compatibilità su Raspberry Pi 400 non verificate da questo gate |
| Decisioni di Andrea | Non richiesta per la sola registrazione della baseline; nessuna approvazione di release registrata |

## Prossime azioni

1. Prima del prossimo task sostanziale, il Capo apre un nuovo checkpoint e assegna responsabile, revisore, percorsi e criteri.
2. Il revisore registra esito e prove nel checkpoint assegnato; in caso di rilievi, il Capo mantiene il checkpoint aperto e richiede correzione e nuova review indipendente.
3. Il Capo aggiorna questa scheda e il registro quando cambia lo stato o si apre/chiude un checkpoint.

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
