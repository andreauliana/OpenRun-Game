# STATUS OFFICINA — OpenRun 4.6

Aggiornato: 2026-10-18 22:33 (ora locale). Questo è l'unico handoff operativo dell'Officina per il Capo; i dossier di checkpoint devono riassumere le prove pertinenti e collegare i log, senza copiarli.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | [VOCE_SCAMBIO_1155.md](../../out/sviluppatore/VOCE_SCAMBIO_1155.md), 2026-10-18 13:49 |
| Gate registrato | Esito completo `GATE T1: TUTTO OK` nel [log Officina](./Test%20area/gate_t1.log), file aggiornato alle 15:29:30; il log non registra da solo l'exit code del processo |
| Ultimo exit code dichiarato | 0 nel report 1155 per `sh tools/gate_t1.sh`; tre livelli, replay identico, accelerazione, crash/ripartenza, sterzo e continuità curve OK |
| Build | Configurazione Release in `UFFICIO/Officina/build/`; CMake cache e output compilati presenti |
| CTest | Sette test registrati nel build esistente. In questa verifica sono stati elencati ma non rieseguiti |
| Runner SDL | SDL2 presente; il runner compila. CMake cerca SDL2_image via config package o pkg-config; non essendo installata/visibile qui, il caricamento PNG resta disabilitato |
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
- SDL2_image è documentata come dipendenza opzionale del solo runner nella [scheda TOOL](./TOOL/README.md) e nella guida [BUILD.md](../../motore_nuovo/docs/BUILD.md), con istruzioni Homebrew e Raspberry Pi OS/RetroPie.

Le allowlist di Builder/Developer e Developer/Builder Junior consentono i wrapper di test e la scrittura di questo solo status, non la modifica manuale degli output generati. Il Capo ha accesso in lettura allo status; il Designer non scrive nell'Officina.

## Verifica di questa revisione

- `cmake -S "4.5b ufficio/sglavinated_src/motore_nuovo" -B "4.5b ufficio/sglavinated_src/UFFICIO/Officina/build" -DCMAKE_BUILD_TYPE=Release`: configurazione completata; ricerca SDL2_image via config package/pkg-config, non trovata in questo ambiente.
- `cmake --build "4.5b ufficio/sglavinated_src/UFFICIO/Officina/build" --target sglavinated_runner`: exit 0; runner compilato con il caricamento PNG disabilitato.
- `brew install sdl2_image`: non completato. Homebrew si è fermato durante il link dell'upgrade di `libtool` per conflitti con symlink preesistenti (`/usr/local/include/libltdl` e `/usr/local/share/libtool`); la pulizia ha inoltre segnalato un diniego su `/usr/local/lib/docker/cli-plugins`. Nessun link è stato sovrascritto o rimosso.
- `ctest --test-dir UFFICIO/Officina/build -N`: sette test scoperti; non eseguiti da questo comando.
- CMake Tools in VS Code non ha un progetto CMake attivo selezionato e non è riuscito a configurarlo; il pannello diagnostica non mostra errori. Non conteggiare questa operazione come build o test passati.
- Prossimo test eseguibile dal ruolo di sviluppo: `sh tools/run_tests.sh`; riportare comando, exit code e risultati CTest in questo status, mantenendo il log originale in Officina.

## Report turno 21:56 — developer-builder-junior

- Lettura fonti obbligatoria completata: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATUS_OFFICINA.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test eseguito, nessuna build avviata in questo turno.
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-18_2156.md.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:15 — developer-builder-junior

- Lettura fonti obbligatoria completata (ordine richiesto): RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-18_2215.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:33 — developer-builder-junior

- Lettura fonti obbligatoria completata (ordine richiesto): RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-18_2233.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Sintesi dei report precedenti pertinenti

- 22:33 — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 22:15 — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 21:56 — developer-builder-junior: lettura fonti completata, nessuna modifica di codice, consegne scritte; gate T1 di riferimento exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20.
- 20:25 — STATUS_OFFICINA: CP-003 aperto, review indipendente da assegnare; ultimo report sviluppatore 1155 (13:49), gate exit 0, tre livelli, replay identico, speed@60=161.79, max 0.393 ms/frame; CTest sette test scoperti non rieseguiti; runner compila senza SDL2_image.
- 13:49 — report 1155: gate exit 0 sui tre livelli, media 0.19-0.20 ms/frame, massimo 0.393 ms/frame, replay doppio identico, off-road raggiunto, accelerazione/ribaltamento/sterzo/curve verificati.
- 13:00 — report 1154: diradamento riferimenti cartelloni nei dati livello (Neon 444->56, Atomico 418->53, Foglie 481->61), gate positivo, massimo 0.277 ms/frame.
- 21:15 — SCAMBIO_RECENTE checkpoint 1 review: TUTTO OK, quota open 99.5%, blocchi fatti 19 / da fare 7; 3 screenshot in out/sviluppatore/screenshot_p1/ senza licenza (fuori competenza del ruolo Junior).
