# CP-003 — Status Officina, tool e permessi di test

| Campo | Valore |
|---|---|
| Stato | APERTO — in attesa di review indipendente e prova CTest |
| Aperto / ultima attività | 2026-10-18 / 2026-10-18 |
| Ambito e criteri di completamento | Centralizzare l'handoff tecnico per il Capo; verificare target CMake, wrapper di test, output Officina e allowlist; ottenere una peer review e una prova CTest valida |
| Responsabile | Aggiornamento operativo 4.6 (Copilot) |
| Revisore indipendente | Da assegnare dal Capo; deve essere diverso dall'autore |
| Percorsi autorizzati | `UFFICIO/Officina/`, `.opencode/agents/`, `AGENTS.md`, `tools/`, `Checkpoint Revision/` |
| Decisioni richieste ad Andrea | Nessuna per documentare lo status; nessuna approvazione di release implicata |
| Handoff Officina | [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) |

## Sintesi

- Creato uno status unico dell'Officina per il Capo, con il report sviluppatore 1155, sintesi dei report precedenti, log sorgente, toolchain e blocchi.
- Documentati i target `sglavinated_core`, `sglavinated_runner`, `sglavinated_libretro` opzionale e `prova_gate`; l'area CMake di test registra sette test.
- Aggiunte allowlist per `tools/run_tests.sh`, `tools/check_sdl_init.sh` e aggiornamento dello status tecnico ai due ruoli di sviluppo; il Capo ha accesso in lettura.
- Spostata la build predefinita del wrapper CMake/CTest in `UFFICIO/Officina/build/` e corretto il percorso predefinito del runner SDL.
- La revisione non è ancora validata da un altro agente; il test CTest non è stato eseguito in questa sessione.

## Modifiche

| Percorso/file | Scopo | Autore |
|---|---|---|
| `UFFICIO/Officina/STATUS_OFFICINA.md` | Handoff tecnico unico e sintesi dei report/log | Copilot |
| `.opencode/agents/{builder-developer,developer-builder-junior,capo}.md` | Permessi e procedura di lettura/aggiornamento status | Copilot |
| `tools/run_tests.sh`, `tools/check_sdl_init.sh` | Usare build e runner predefiniti nell'area Officina | Copilot |
| `UFFICIO/Officina/*`, `AGENTS.md`, `Checkpoint Revision/*` | Documentare tool, output, status e procedura di checkpoint | Copilot |

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 2026-10-18 | `ctest --test-dir UFFICIO/Officina/build -N` | Elenco completato; nessun test eseguito | 7 test scoperti | Build CMake esistente |
| 2026-10-18 15:29 | Gate registrato in `UFFICIO/Officina/Test area/gate_t1.log` | `GATE T1: TUTTO OK`; l'exit code non è registrato nel log | 3 livelli; replay, accelerazione, crash, sterzo e curve OK | [Log gate](../UFFICIO/Officina/Test%20area/gate_t1.log) |
| 2026-10-18 13:49 | `sh tools/gate_t1.sh` (report 1155) | Exit 0 | Media 0,19–0,20 ms/frame; max 0,393 ms/frame | [Report 1155](../out/sviluppatore/VOCE_SCAMBIO_1155.md) |
| 2026-10-18 | CMake Tools: richiesta Run CTest | Non eseguito: l'estensione non ha un progetto CMake attivo configurato; diagnostica vuota | Nessun risultato test conteggiato | Stato verificato durante il task |
| 2026-10-18 | YAML agenti, link locali, `sh -n` sui wrapper, `git diff --check` | OK | 10 documenti con link validi; sintassi script/config valida | Verifiche locali, non peer review |

Non copiare output estesi: i log restano nella posizione operativa e sono collegati dallo status. La discovery CTest e la validazione documentale non sostituiscono l'esecuzione dei test o la review indipendente.

## Review indipendente

- Revisore: da assegnare dal Capo.
- File/diff esaminati:
- Esito: IN ATTESA.
- Verificare semantica delle allowlist, destinazioni di output dei wrapper, configurazione CMake e riferimenti dai due status.
- Prova richiesta: eseguire il flusso CMake/CTest con il progetto correttamente selezionato e registrare comando, exit code ed esito misurabile.
- Nuova review dopo correzione:

## Decisione di Andrea

- Stato: NON RICHIESTA per la sola documentazione operativa; nessuna approvazione di release.

## Eventi

- 2026-10-18 — aperto dopo l'aggiornamento dello status Officina, dei wrapper CMake/test e delle allowlist; revisore e prova CTest restano da assegnare/eseguire.
