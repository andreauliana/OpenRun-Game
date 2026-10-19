# CP-004 — Fonte Andrea e riconciliazione archivio v4

| Campo | Valore |
|---|---|
| Stato | APERTO — in attesa di review indipendente |
| Aperto / ultima attività | 2026-10-18 / 2026-10-18 |
| Ambito e criteri di completamento | Consolidare feedback e decisioni di Andrea in una fonte corrente; riallineare i riferimenti attivi; classificare l'archivio v4 senza perdere materiale storico |
| Responsabile | Aggiornamento operativo 4.6 (Copilot) |
| Revisore indipendente | Da assegnare dal Capo; deve essere diverso dall'autore |
| Percorsi autorizzati | `ANDREA_REVIEW.md`, `DECISIONI_ANDREA.md`, `FEEDBACK_ANDREA.md`, `AGENTS.md`, `COSA_FA_L_UMANO.md`, `DA_FARE_QUANDO_SONO_DAVANTI.md`, `bacheche/capo/`, `out/capo/`, strumenti in `../strumenti/`, documentazione gameplay e `Checkpoint Revision/` |
| Decisioni richieste ad Andrea | Nessuna per consolidamento documentale; non implica approvazione del gate o della release |
| Review collegata | Dossier corrente per la verifica dei riferimenti e della classificazione |

## Sintesi

- `ANDREA_REVIEW.md` è la fonte canonica 4.6 per feedback e decisioni. I contenuti sostanziali della precedente decisione D1-D11 e del feedback v90 sono conservati; le richieste T1 che collidono con D1-D4 sono rinominate T1-Q1–T1-Q4.
- `FEEDBACK_ANDREA.md` non contiene piu' decisioni autonome di Andrea. Manuale, promemoria, handoff del @capo e documentazione delle fonti ora indirizzano alla fonte canonica.
- D10 e D11 hanno risposte esplicite di Andrea. La risposta non chiude il checkpoint né sostituisce la prova tecnica: il Capo deve verificare il risultato nel dossier pertinente.
- `archivio_v4/` contiene 126 file tra regole, profili di agenti, strumenti, bacheche, report, bozze e prove storiche. È congelato e non è stato cancellato o modificato.
- I principi durevoli già pertinenti a 4.6 sono verificati nel progetto corrente: offroad calcolato prima del clamp in `player.cpp`, passaggio `ShowManager`→`DisegnoShow`→renderer in `game.cpp`, hash di frame/stato in `Game::frameHash()`. La guida `DISEGNO_SHOW.md` e il manifest gameplay sono stati aggiornati per riflettere l'implementazione corrente e le parti visive ancora mancanti.

## Classificazione archivio v4

| Materiale | Valutazione | Azione |
|---|---|---|
| `AGENTS_v4.md`, `agenti_v4/`, `PIANO_TURNI_v4.md` | Regole, ruoli e assegnazioni della squadra v4/v4.5; superati dai ruoli e dalle regole 4.6 | Conservare come storia; le regole correnti sono in `AGENTS.md` e `.opencode/agents/` |
| `BUILD_v4.md`, `V85C_NOTE.md`, note Mega Drive in `README_v4.md` | Procedura ROM/68000 e toolchain non applicabili al motore C++ corrente; il progetto vieta materiale Sega | Non importare né eseguire; usare i gate CMake documentati in `AGENTS.md` |
| `TODO_OPENCODE.md`, `PACCHETTO_PER_CLAUDE_APP.md` | Task e stati di una tappa/versione precedente, con decisioni e ruoli superati | Non riaprire come task 4.6 senza nuova assegnazione e prova |
| `out/`, `bacheche/` | Consegnati, report, log, diari e bozze storici | Conservare come provenienza; non considerarli status o prova correnti |
| `REA_AGENTI.md`, `REA_METODO.md`, `REA_USO_PROGETTO.md` | Istruzioni d'uso REA, non dipendenze del runtime né regole operative attuali | Conservare; usare solo su richiesta esplicita di reverse engineering |
| Dettagli durevoli su determinismo, clamp, show e frame hash | Utili solo se riconfermati sul codice corrente | Già incorporati/verificati; nessuna copia duplicata richiesta |

L'indice strumenti esclude `archivio_v4/` dalla scansione; l'archivio non è input di build o fonte di asset di produzione. Non rimuovere file storici: l'eventuale eliminazione richiede una decisione e una verifica separata delle dipendenze.

## Modifiche

| Percorso/file | Scopo | Autore |
|---|---|---|
| `ANDREA_REVIEW.md` | Fonte unica; conflitti ID risolti e feedback v90 importato | Copilot |
| `DECISIONI_ANDREA.md`, `FEEDBACK_ANDREA.md` | Rimandi compatibili alla fonte canonica | Copilot |
| `../strumenti/ciclo.sh`, `../strumenti/pannello.py`, `../strumenti/recap.sh` | Lettura della fonte canonica al posto dei nomi legacy | Copilot |
| `AGENTS.md`, `motore_nuovo/src/gameplay/DISEGNO_SHOW.md`, `motore_nuovo/src/gameplay/manifest.json` | Regole di fonte/archivio e stato corrente del wiring gameplay | Copilot |
| `RICHIESTA.md`, `PACCHETTO_PER_CLAUDE_APP.md`, documenti/cataloghi collegati | Riferimenti attivi alla fonte canonica | Copilot |
| `COSA_FA_L_UMANO.md`, `DA_FARE_QUANDO_SONO_DAVANTI.md`, `bacheche/capo/`, `out/capo/`, `motore_nuovo/docs/FONTI.md` | Handoff attivi riallineati; registri storici lasciati invariati | Copilot |

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 2026-10-18 | Inventario `archivio_v4/` | Completato | 126 file; nessun file rimosso | Inventario della cartella |
| 2026-10-18 | Ispezione codice gameplay | Completato | offroad prima del clamp; show aggiornato e consumato dal renderer; `frameHash()` presente | `motore_nuovo/src/player/player.cpp`, `motore_nuovo/src/game/game.cpp` |
| 2026-10-18 | Audit riferimenti attivi e `git diff --check` | Completato | Handoff correnti usano `ANDREA_REVIEW.md`; alias mantenuti, riferimenti storici preservati | Diff locale |

## Review indipendente

- Revisore: da assegnare dal Capo.
- File/diff esaminati:
- Esito: IN ATTESA.
- Verificare che non siano rimaste istruzioni attive a scrivere nei file alias; controllare i riferimenti del ciclo/pannello e la coerenza della classificazione dell'archivio.
- I riferimenti rimasti nei registri/diari storici descrivono il flusso v4.5 dell'epoca e non sono istruzioni attive; non riscriverli.
- Verificare in particolare che il ciclo v4.5 non venga presentato come automazione 4.6: la migrazione completa dei ruoli e dei registri legacy non rientra in questo dossier.
- Nuova review dopo correzione:

## Decisione di Andrea

- Stato: NON RICHIESTA per il consolidamento documentale; nessuna approvazione di release o del gate implicata.

## Eventi

- 2026-10-18 — aperto dopo il consolidamento della fonte Andrea e la classificazione dell'archivio v4; in attesa di review indipendente.
- 2026-10-18 — aggiornati i riferimenti nei manuali e negli handoff correnti; i due file legacy restano solo per compatibilità e i log storici sono stati preservati.
