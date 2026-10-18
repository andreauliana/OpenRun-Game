# STATUS OFFICINA — OpenRun 4.6

Aggiornato: 2026-10-18 15:51 (ora locale). Questo è l'unico handoff operativo dell'Officina per il Capo; i dossier di checkpoint devono riassumere le prove pertinenti e collegare i log, senza copiarli.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | [VOCE_SCAMBIO_1155.md](../../out/sviluppatore/VOCE_SCAMBIO_1155.md), 2026-10-18 13:49 |
| Gate registrato | Esito completo `GATE T1: TUTTO OK` nel [log Officina](./Test%20area/gate_t1.log), file aggiornato alle 15:29:30; il log non registra da solo l'exit code del processo |
| Ultimo exit code dichiarato | 0 nel report 1155 per `sh tools/gate_t1.sh`; tre livelli, replay identico, accelerazione, crash/ripartenza, sterzo e continuità curve OK |
| Build | Configurazione Release in `UFFICIO/Officina/build/`; CMake cache e output compilati presenti |
| CTest | Sette test registrati nel build esistente. In questa verifica sono stati elencati ma non rieseguiti |
| Runner SDL | SDL2 presente nella cache CMake; `SDL2_image` non trovata, quindi il caricamento PNG nel runner è disabilitato |
| Review/release | Nessuna approvazione di release dedotta dal gate; assegnare peer review indipendente per i checkpoint tecnici |

## Sintesi dei report precedenti

- **13:49 — report 1155:** gate con exit 0 sui tre livelli. `speed@60=161.79`, `speed@1=9.90`, media 0.19–0.20 ms/frame, massimo 0.393 ms/frame; replay doppio identico, off-road raggiunto ai frame 278/167/285, accelerazione, ribaltamento, sterzo e curve continui verificati.
- **13:00 — report 1154:** diradati i riferimenti ai cartelloni nei dati livello senza cambiare gli sponsor approvati: Neon 444→56, Atomico 418→53, Foglie 481→61. Gate di nuovo positivo, massimo misurato 0.277 ms/frame.
- **Prima dei report 1154–1155:** i diari documentano rifiuti della shell per esecuzione diretta del binario, copia/scrittura in percorsi di output non autorizzati e, in un tentativo, timeout. Il flusso aggiornato usa i wrapper autorizzati e deposita gli artefatti in Officina; il gate successivo ha completato.

Fonti: [diario recente](../../bacheche/sviluppatore/DIARIO_RECENTE.md), [sintesi ruolo sviluppatore](../../bacheche/sviluppatore/RIASSUNTO.md), report [1154](../../out/sviluppatore/VOCE_SCAMBIO_1154.md) e [1155](../../out/sviluppatore/VOCE_SCAMBIO_1155.md). I log completi restano nei percorsi di origine.

## Toolchain, build e permessi

La configurazione attiva è [CMakeLists.txt](../../motore_nuovo/CMakeLists.txt); i sorgenti restano in `motore_nuovo/`, gli script in `tools/` e gli output in `UFFICIO/Officina/`.

- `sglavinated_core`: libreria C++17 portabile e priva di dipendenze SDL.
- `sglavinated_runner`: runner SDL2, creato quando SDL2 è disponibile.
- `sglavinated_libretro`: modulo stub opzionale (`BUILD_LIBRETRO=ON`).
- `prova_gate`: eseguibile di test senza SDL; abilita sette test CTest se è presente `motore_nuovo/test/prova_gate.cpp`.
- `sh tools/run_tests.sh`: percorso standard CMake/CTest in Release, ora riusa `UFFICIO/Officina/build/`.
- `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak`: prove integrate e soak; il wrapper gestisce la build e scrive i log in `Test area/`.
- `sh tools/check_sdl_init.sh`: controllo del runner SDL compilato.

Le allowlist di Builder/Developer e Developer/Builder Junior consentono i wrapper di test e la scrittura di questo solo status, non la modifica manuale degli output generati. Il Capo ha accesso in lettura allo status; il Designer non scrive nell'Officina.

## Verifica di questa revisione

- `ctest --test-dir UFFICIO/Officina/build -N`: sette test scoperti; non eseguiti da questo comando.
- CMake Tools in VS Code non ha un progetto CMake attivo selezionato e non è riuscito a configurarlo; il pannello diagnostica non mostra errori. Non conteggiare questa operazione come build o test passati.
- Prossimo test eseguibile dal ruolo di sviluppo: `sh tools/run_tests.sh`; riportare comando, exit code e risultati CTest in questo status, mantenendo il log originale in Officina.
