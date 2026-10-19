# STATUS OFFICINA — OpenRun 4.6

Aggiornato: 2026-10-19 09:01 (ora locale). Questo è l'unico handoff operativo dell'Officina per il Capo; i dossier di checkpoint devono riassumere le prove pertinenti e collegare i log, senza copiarli.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | [VOCE_SCAMBIO_1171.md](../../out/builder-developer/VOCE_SCAMBIO_1171.md), 2026-10-19 08:56 (builder-developer) |
| Gate registrato | `GATE T1: TUTTO OK` nel [log Officina](./Test%20area/gate_t1.log), aggiornato alle 22:56 da `sh tools/gate_t1.sh` (smoke) e `sh tools/gate_t1.sh soak`, exit 0 dichiarato per entrambi; il log non registra da solo l'exit code del processo |
| Ultimo exit code dichiarato | 0 per `sh tools/gate_t1.sh` (smoke) e `sh tools/gate_t1.sh soak` alle 22:56; 6 livelli + soak 30000 frame lv_01_neon, replay identico, accelerazione, ribaltamento/ripartenza, sterzo e continuità curve OK; T1-RUN [x] per DEFAULT T1-Q1. Nessun riesecuzione in questo turno (sessione senza esecuzione comandi); ultimo esito noto invariato. |
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

## Report turno 22:52 — developer-builder-junior

- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; G1 @grafico; P1 @sviluppatore). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-18_2252.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:56 — builder-developer

- Eseguiti `sh tools/gate_t1.sh` (smoke) e `sh tools/gate_t1.sh soak`: exit 0 entrambi, `GATE T1: TUTTO OK` in `UFFICIO/Officina/Test area/gate_t1.log`.
- 6 livelli (lv_01_neon..lv_06_desktop): prova_gate + percorso GATE PERCORSO OK, guida 0 frame fuori strada, giro guidato completato su tutti.
- Soak lv_01_neon 30000 frame: ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO. Smoke: ms/frame medio 0.177-0.184 max 0.263-0.366.
- speed@60=161.79, speed@1=9.90; accelerazione, ribaltamento/ripartenza, sterzo, continuita' curve OK; gara 4 bivi OK (arrivo, posizione, incidenti, replay IDENTICO); screenshot P1 OK (curva, sponsor, personaggio+fumetto).
- T1-RUN chiuso [x] per DEFAULT T1-Q1 (ANDREA_REVIEW: soak exit 0, replay 2x IDENTICO, ms/frame <= 20; risposta Andrea non registrata).
- Zero scritture in motore_nuovo/ e tools/; nessun test del Junior modificato.
- Residuo: v91 Andrea su sglavinated_runner (Mac SDL2) e fps/temperatura Pi 400 (T1-Q3/T1-Q8); G1 [ ] fuori scope T1.
- Consegne: bacheche/builder-developer/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1160.md; questo status aggiornato.

## Report turno 06:53 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer (fuori scope T1 per DEFAULT T1-Q4); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8).
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1.
- Consegne scritte: bacheche/builder-developer/diario.md (1 riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1162.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Sintesi dei report precedenti pertinenti

- 22:52 — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 06:53 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma dello stato 22:56 (exit 0, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, replay 2x IDENTICO; T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti).
- 22:56 — builder-developer: gate smoke e soak exit 0, GATE T1 TUTTO OK, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak (limite 20), replay 2x IDENTICO; T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti.
- 22:33 — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 22:15 — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 21:56 — developer-builder-junior: lettura fonti completata, nessuna modifica di codice, consegne scritte; gate T1 di riferimento exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20.
- 20:25 — STATUS_OFFICINA: CP-003 aperto, review indipendente da assegnare; ultimo report sviluppatore 1155 (13:49), gate exit 0, tre livelli, replay identico, speed@60=161.79, max 0.393 ms/frame; CTest sette test scoperti non rieseguiti; runner compila senza SDL2_image.
- 13:49 — report 1155: gate exit 0 sui tre livelli, media 0.19-0.20 ms/frame, massimo 0.393 ms/frame, replay doppio identico, off-road raggiunto, accelerazione/ribaltamento/sterzo/curve verificati.
- 13:00 — report 1154: diradamento riferimenti cartelloni nei dati livello (Neon 444->56, Atomico 418->53, Foglie 481->61), gate positivo, massimo 0.277 ms/frame.
- 21:15 — SCAMBIO_RECENTE checkpoint 1 review: TUTTO OK, quota open 99.5%, blocchi fatti 19 / da fare 7; 3 screenshot in out/sviluppatore/screenshot_p1/ senza licenza (fuori competenza del ruolo Junior).
- 07:20 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-builder-junior/); ultimo esito gate T1 invariato (exit 0 da @collaudatore, 22:56).
- 07:25 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto (ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico); nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-builder-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti.
- 08:14 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-builder-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, smoke medio 0.177-0.184 max 0.263-0.366 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti.
- 07:04 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1; zero scritture in motore_nuovo/ e tools/.
- 07:14 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1 (P1 punto 4, G1 [ ] fuori scope T1); zero scritture in motore_nuovo/ e tools/.
- 07:23 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1 (P1 punto 4, G1 [ ] fuori scope T1); zero scritture in motore_nuovo/ e tools/.
- 07:18 (19/10) — developer-builder-junior: nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0718.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:20 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0720.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:25 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0725.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:32 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0732.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:47 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0747.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:50 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri player/traffico/show) e G1 [ ] sono di Designer (DEFAULT T1-Q4: non attivare); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (ANDREA_REVIEW T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, smoke medio 0.177-0.184 max 0.263-0.366 (limite 20), replay 2x IDENTICO, speed@60=161.79, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. Consegne scritte: bacheche/builder-developer/diario.md (1 riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1166.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.
- 08:17 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer (DEFAULT T1-Q4: non attivare); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (ANDREA_REVIEW T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì). Consegne scritte: bacheche/builder-developer/diario.md (1 riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1168.md e out/builder-developer/RISULTATI_1910_0817.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.
- 08:14 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-builder-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 08:21 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0821.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 08:35 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1, DEFAULT T1-Q4); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì). Consegne scritte: bacheche/builder-developer/diario.md (1 riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1169.md e RISULTATI_1910_0835.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 08:42 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea (prova screenshot differita, shell negata).
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/builder-developer/diario.md (1 riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1170.md e RISULTATI_1910_0842.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 08:56 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/builder-developer/diario.md (una riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1171.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 09:01 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0901.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Report turno 09:06 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/builder-developer/diario.md (una riga), bacheche/builder-developer/RIASSUNTO.md (<=12 righe), out/builder-developer/VOCE_SCAMBIO_1172.md e RISULTATI_1910_0906.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 09:17 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea; J [ ] escalation; MD0/T5 [-] sospesi). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-builder-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-builder-junior/REPORT_TURNO_2026-10-19_0917.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Sintesi dei report precedenti pertinenti

- 09:17 (19/10) — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne di fine turno scritte; gate T1 riferimento invariato (exit 0 da @collaudatore, 22:56).
- 09:06 (19/10) — builder-developer: nessun blocco eseguibile (P1 punto 4 sprite veri e G1 [ ] di Designer/@grafico); nessuna modifica di codice né gate; ultimo esito noto exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1.
- 09:01 (19/10) — developer-builder-junior: turno senza blocchi assegnati, nessuna modifica di codice, consegne scritte; gate T1 riferimento invariato (exit 0 da @collaudatore).
- 22:56 (18/10) — builder-developer: gate smoke e soak exit 0, GATE T1 TUTTO OK, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak (limite 20), replay 2x IDENTICO; T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti.

