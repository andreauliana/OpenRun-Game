# Piano a turni (controllo interno ogni 2 ore, revisione umana dopo 6-8 ore)
Il ciclo calcola il turno dal tempo trascorso. `TAPPE.md` e il Gate della tappa attiva prevalgono sulla sequenza oraria qui sotto: i turni ordinano solo lavoro compatibile e non autorizzano ad avviare tappe future. Finche' e' attiva la tappa 1, la priorita' e' il primo test C++ giocabile; ricerca, dialoghi, grafica e livelli non necessari al Gate restano in attesa. La squadra predefinita e' `sviluppatore collaudatore`; si amplia solo se il Gate richiede supporto. Ogni 2 ore esegue controlli automatici; @collaudatore fa un solo passaggio sui file modificati (massimo 3 rilievi azionabili) e la chiamata viene saltata se non ci sono cambi pertinenti. Non si fa review a ogni consegna. Claude CLI resta disattivato (`CHK_CLAUDE_OGNI=0`). @capo applica i rilievi al checkpoint. Nessuno aspetta Andrea (decisioni in DECISIONI_ANDREA.md, con default).

| Turno | Ore | Priorita' per agente | Uscita attesa |
|---|---|---|---|
| T1 | 0-2 | @collaudatore: B0 (FONTI.md), poi B · @sviluppatore: CONS (fumetti/tabelle in JSON, logica show in .md) · @ricercatore: E (font, texture, suoni liberi) | docs/FONTI.md, FEDELTA.md, ARCHITETTURA, dati estratti, tabelle ricerca |
| CHECK 1 | 2h | controlli automatici + un passaggio del collaudatore solo sui file cambiati | massimo 3 rilievi azionabili |
| T2 | 2-4 | @sviluppatore: C (nucleo + runner, strada a segmenti) · @sviluppatore: K (3 livelli) · @grafico: M (varianti font/menu/titolo) · @grafico: G se serve | codice che compila, livelli JSON, varianti UI |
| CHECK 2 | 4h | controlli automatici + collaudatore solo se ci sono altri cambi | massimo 3 rilievi azionabili |
| T3 | 4-6 | @sviluppatore: replay deterministico, personaggi/fx (razzo, sollevamento) come moduli · @grafico: L (varianti scenari) · @grafico: F (varianti sagome) · @collaudatore: H (build, fps, replay) | prototipo con 1 livello, varianti da scegliere |
| CHECK 3 | 6h | controlli automatici + collaudatore solo se ci sono altri cambi | riepilogo breve |
| T4 | 6-8 | rifinitura: applicare i rilievi, @collaudatore esegue i test di accettazione, aggiornare CATALOGO e CREDITI, quota open · preparare il blocco N solo se C gira | stato pulito per la revisione umana |
| PACCHETTO | 8h (poi si prosegue fino a 12h al massimo) | pacchetto per Claude app (PACCHETTO_PER_CLAUDE_APP.md) + RIEPILOGO_MATTINA.md | Andrea + Claude app Pro decidono |

Gerarchia: agenti OpenCode lavorano; @collaudatore fa un solo controllo leggero ai checkpoint; **Claude app Pro** interviene a mano dopo le 6-8 ore leggendo il pacchetto. Se un turno non finisce nel suo tempo, il lavoro prosegue nel successivo; nessun blocco viene saltato senza una riga in SCAMBIO.md.

Cambi: ogni agente puo' chiedere un passaggio ad altro agente o un cambio modello (AGENTS.md regola 13); il ciclo applica i cambi modello a ogni checkpoint.

Pause e assemblee: regola 14 di AGENTS.md (max 3 cicli di fila per agente; BACHECA_PUBBLICA.md viene letta a ogni turno, ASSEMBLEA.md si usa per decisioni trasversali ed e' moderata da @capo).

Bacheca pubblica: facoltativa, massimo 3 righe per messaggio; il ciclo passa al capo solo le ultime 24 righe e non richiede risposte ai messaggi informali.
