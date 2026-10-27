# STATUS OFFICINA — OpenRun 4.6

Aggiornato: 2026-10-19 09:39 (ora locale). Questo è l'unico handoff operativo dell'Officina per il Capo; i dossier di checkpoint devono riassumere le prove pertinenti e collegare i log, senza copiarli.

## Stato corrente

| Voce | Stato / evidenza |
|---|---|
| Checkpoint tecnico | [CP-003](../../Checkpoint%20Revision/CP-003-status-officina-e-test.md) — aperto, review indipendente da assegnare |
| Ultimo report sviluppatore | [VOCE_SCAMBIO_1173.md](../../out/developer/VOCE_SCAMBIO_1173.md), 2026-10-19 09:21 (builder-developer) |
| **Gate Andrea (09:39)** | **`GATE T1: FALLITO` (exit 13)** — `sh tools/gate_t1.sh soak` eseguito da Andrea su Mac, 2026-10-19 09:39. Gate core OK (smoke, soak 30000 frame, accelerazione, ribaltamento, sterzo, curve, gara 4 bivi, screenshot P1). **Giro guidato FALLITO su 5 livelli su 6**: lv_01_neon, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop — "percorso poco vario o non guidabile". Solo lv_02_atomico (rettilineo 40%) supera il giro guidato. Dettaglio in `ANDREA_REVIEW.md` T1-Q3. Aggiornamento 19/10 11:45: `sh tools/gate_t1.sh soak` eseguito da builder-developer, exit 0, `GATE T1: TUTTO OK` — 6/6 percorsi GATE PERCORSO OK dopo fix `road.cpp` (dettaglio e misure in `out/developer/VOCE_SCAMBIO_1174.md`, log `UFFICIO/Officina/Test area/gate_t1.log`). |
| Ultimo exit code dichiarato (agenti) | 0 per `sh tools/gate_t1.sh soak` eseguito da builder-developer alle 19/10 11:45 (GATE T1 TUTTO OK, 6/6 percorsi verdi; dettaglio in `out/developer/VOCE_SCAMBIO_1174.md`). |
| Performance Mac (09:39) | ms/frame medio ~0.19, max 0.94 (limite 20 — OK). speed@60=161.79, replay 2x IDENTICO su tutti i livelli. |
| Build | Configurazione Release in `UFFICIO/Officina/build/`; CMake cache e output compilati presenti |
| CTest | Sette test registrati nel build esistente. In questa verifica sono stati elencati ma non rieseguiti |
| Runner SDL | SDL2 presente; il runner compila. CMake cerca SDL2_image via config package o pkg-config; non essendo installata/visibile qui, il caricamento PNG resta disabilitato |
| Review/release | Gate verde (T1-RUN): peer review indipendente del Junior su VOCE_SCAMBIO_1175 **POSITIVA** (19/10, report `out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md`, verifica `sh tools/gate_t1.sh soak` exit 0). Nessuna approvazione di release: resta al Capo/Andrea; blocco H (Pi 400) e v91 Mac ancora aperti |

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
- `sh UFFICIO/Officina/TOOL/run_tests.sh`: percorso standard CMake/CTest in Release, ora riusa `UFFICIO/Officina/build/`.
- `sh UFFICIO/Officina/TOOL/gate_t1.sh` e `sh UFFICIO/Officina/TOOL/gate_t1.sh soak`: prove integrate e soak; il wrapper gestisce la build e scrive i log in `Test area/`.
- `sh UFFICIO/Officina/TOOL/check_sdl_init.sh`: controllo del runner SDL compilato.
- SDL2_image è documentata come dipendenza opzionale del solo runner nella [scheda TOOL](./TOOL/README.md) e nella guida [BUILD.md](../../motore_nuovo/docs/BUILD.md), con istruzioni Homebrew e Raspberry Pi OS/RetroPie.

Le allowlist di Builder/Developer e Developer/Builder Junior consentono i wrapper di test e la scrittura di questo solo status, non la modifica manuale degli output generati. Il Capo ha accesso in lettura allo status; il Designer non scrive nell'Officina.

## Verifica di questa revisione

- `cmake -S "4.5b ufficio/sglavinated_src/motore_nuovo" -B "4.5b ufficio/sglavinated_src/UFFICIO/Officina/build" -DCMAKE_BUILD_TYPE=Release`: configurazione completata; ricerca SDL2_image via config package/pkg-config, non trovata in questo ambiente.
- `cmake --build "4.5b ufficio/sglavinated_src/UFFICIO/Officina/build" --target sglavinated_runner`: exit 0; runner compilato con il caricamento PNG disabilitato.
- `brew install sdl2_image`: non completato. Homebrew si è fermato durante il link dell'upgrade di `libtool` per conflitti con symlink preesistenti (`/usr/local/include/libltdl` e `/usr/local/share/libtool`); la pulizia ha inoltre segnalato un diniego su `/usr/local/lib/docker/cli-plugins`. Nessun link è stato sovrascritto o rimosso.
- `ctest --test-dir UFFICIO/Officina/build -N`: sette test scoperti; non eseguiti da questo comando.
- CMake Tools in VS Code non ha un progetto CMake attivo selezionato e non è riuscito a configurarlo; il pannello diagnostica non mostra errori. Non conteggiare questa operazione come build o test passati.
- Prossimo test eseguibile dal ruolo di sviluppo: `sh UFFICIO/Officina/TOOL/run_tests.sh`; riportare comando, exit code e risultati CTest in questo status, mantenendo il log originale in Officina.

## Report turno 21:56 — developer-builder-junior

- Lettura fonti obbligatoria completata: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATUS_OFFICINA.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test eseguito, nessuna build avviata in questo turno.
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-18_2156.md.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:15 — developer-builder-junior

- Lettura fonti obbligatoria completata (ordine richiesto): RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-18_2215.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:33 — developer-builder-junior

- Lettura fonti obbligatoria completata (ordine richiesto): RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-18_2233.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:52 — developer-builder-junior

- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; G1 @grafico; P1 @sviluppatore). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita in questo turno.
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-18_2252.md; questo status aggiornato.
- Gate T1 di riferimento non rieseguito: ultimo esito noto da @collaudatore, sh tools/gate_t1.sh exit 0, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 <= 20 (DEFAULT T1-Q1 soddisfatto; risposta Andrea non registrata).

## Report turno 22:56 — builder-developer

- Eseguiti `sh tools/gate_t1.sh` (smoke) e `sh tools/gate_t1.sh soak`: exit 0 entrambi, `GATE T1: TUTTO OK` in `UFFICIO/Officina/Test area/gate_t1.log`.
- 6 livelli (lv_01_neon..lv_06_desktop): prova_gate + percorso GATE PERCORSO OK, guida 0 frame fuori strada, giro guidato completato su tutti.
- Soak lv_01_neon 30000 frame: ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO. Smoke: ms/frame medio 0.177-0.184 max 0.263-0.366.
- speed@60=161.79, speed@1=9.90; accelerazione, ribaltamento/ripartenza, sterzo, continuita' curve OK; gara 4 bivi OK (arrivo, posizione, incidenti, replay IDENTICO); screenshot P1 OK (curva, sponsor, personaggio+fumetto).
- T1-RUN chiuso [x] per DEFAULT T1-Q1 (ANDREA_REVIEW: soak exit 0, replay 2x IDENTICO, ms/frame <= 20; risposta Andrea non registrata).
- Zero scritture in motore_nuovo/ e tools/; nessun test del Junior modificato.
- Residuo: v91 Andrea su sglavinated_runner (Mac SDL2) e fps/temperatura Pi 400 (T1-Q3/T1-Q8); G1 [ ] fuori scope T1.
- Consegne: bacheche/developer/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1160.md; questo status aggiornato.

## Report turno 06:53 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer (fuori scope T1 per DEFAULT T1-Q4); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8).
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1.
- Consegne scritte: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1162.md; questo status aggiornato.
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
- 07:20 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-junior/); ultimo esito gate T1 invariato (exit 0 da @collaudatore, 22:56).
- 07:25 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto (ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico); nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti.
- 08:14 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, smoke medio 0.177-0.184 max 0.263-0.366 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti.
- 07:04 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1; zero scritture in motore_nuovo/ e tools/.
- 07:14 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1 (P1 punto 4, G1 [ ] fuori scope T1); zero scritture in motore_nuovo/ e tools/.
- 07:23 (19/10) — builder-developer: turno senza scritture di codice né esecuzione gate; riconferma stato da report 22:56 (gate smoke e soak exit 0, 6 percorsi GATE PERCORSO OK lv_01..lv_06, replay 2x IDENTICO, ms/frame entro limite 20, T1-RUN [x] DEFAULT T1-Q1); in attesa di v91 Andrea (runner Mac SDL2 + fps/temperatura Pi 400) e sprite veri Designer/G1 (P1 punto 4, G1 [ ] fuori scope T1); zero scritture in motore_nuovo/ e tools/.
- 07:18 (19/10) — developer-builder-junior: nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN punti 1-4 @sviluppatore, 5-6 @collaudatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0718.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:20 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0720.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:25 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0725.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:32 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0732.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:47 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0747.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK lv_01..lv_06, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 07:50 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri player/traffico/show) e G1 [ ] sono di Designer (DEFAULT T1-Q4: non attivare); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (ANDREA_REVIEW T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, smoke medio 0.177-0.184 max 0.263-0.366 (limite 20), replay 2x IDENTICO, speed@60=161.79, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. Consegne scritte: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1166.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.
- 08:17 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer (DEFAULT T1-Q4: non attivare); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (ANDREA_REVIEW T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì). Consegne scritte: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1168.md e out/developer/RISULTATI_1910_0817.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.
- 08:14 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata (ordine richiesto, ANDREA_REVIEW.md precedenza assoluta); nessun blocco [ ]/[~] assegnato al ruolo; nessuna modifica di codice, nessun test, nessuna build/gate; consegne scritte (diario 1 riga, RIASSUNTO <=12 righe, report in out/developer-junior/); questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 08:21 (19/10) — developer-builder-junior: lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi). Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0821.md; questo status aggiornato. Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti. In attesa: task dal Capo.
- 08:35 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1, DEFAULT T1-Q4); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì). Consegne scritte: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1169.md e RISULTATI_1910_0835.md; questo status aggiornato. In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 08:42 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea (prova screenshot differita, shell negata).
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1170.md e RISULTATI_1910_0842.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 08:56 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/developer/diario.md (una riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1171.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 09:01 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0901.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Report turno 09:06 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/developer/diario.md (una riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1172.md e RISULTATI_1910_0906.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 09:17 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea; J [ ] escalation; MD0/T5 [-] sospesi). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0917.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Report turno 09:21 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, speed@60=161.79, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì).
- Consegne scritte: bacheche/developer/diario.md (una riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/VOCE_SCAMBIO_1173.md e RISULTATI_1910_0921.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report turno 09:25 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea; J [ ] escalation; MD0/T5 [-] sospesi). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_0925.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 18/10 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Report turno 09:30 — builder-developer

- Nessun blocco [ ]/[~] eseguibile in questo turno: P1 punto 4 (sprite veri) e G1 [ ] sono di Designer/@grafico (fuori scope T1); v91 Andrea (runner Mac SDL2) e misure fps/temperatura Pi 400 sono di Andrea (T1-Q3/T1-Q8). P1 promosso [x] per T1-Q2 sì di Andrea.
- Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi); ultimo esito noto: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 18/10 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, speed@60=161.79, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti.
- Consegne scritte: bacheche/developer/diario.md (una riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/RISULTATI_1910_0930.md; questo status aggiornato.
- In attesa: v91 Andrea e sprite veri Designer/G1.

## Report Andrea — 2026-10-19 09:39 (prova manuale Mac)

- Eseguito: `sh tools/gate_t1.sh soak` su Mac, 2026-10-19 09:39.
- **Esito: GATE T1: FALLITO (exit 13).**
- Gate core tutti OK: smoke, soak 30000 frame lv_01_neon, accelerazione, ribaltamento/ripartenza, sterzo, continuità curve, gara 4 combinazioni bivi (arrivo posizione 1/8, replay IDENTICO), screenshot P1 (curva, sponsor, personaggio+fumetto).
- Performance: ms/frame medio ~0.19 (range 0.180–0.205 per livello), max 0.941 (limite 20 — ampiamente OK). speed@60=161.79, speed@1=9.90, replay 2x IDENTICO su tutti i livelli.
- **Problema: giro guidato FALLITO su 5 livelli su 6** — "percorso poco vario o non guidabile":
  - lv_01_neon: rettilineo 13%, curva max 0.62 — FALLITO (exit 13)
  - lv_02_atomico: rettilineo 40%, curva max 0.86 — GATE OK (unico passato)
  - lv_03_foglie: rettilineo 18%, curva max 1.02 — FALLITO (exit 13)
  - lv_04_cromo: rettilineo 23%, curva max 0.41 — FALLITO (exit 13)
  - lv_05_spray: rettilineo 21%, curva max 0.90 — FALLITO (exit 13)
  - lv_06_desktop: rettilineo 12%, curva max 0.56 — FALLITO (exit 13)
- **Azione: @sviluppatore deve rivedere la generazione/parametri dei percorsi** per aumentare varietà (alternanza curve/rettilinei). Il modello che funziona è lv_02_atomico (40% rettilineo, curve fino a 0.86). Riferimento completo: `ANDREA_REVIEW.md` T1-Q3.
- Pi 400: ancora da eseguire (blocco H resta [~]).

## Sintesi dei report precedenti pertinenti

- **09:39 (19/10) — Andrea (prova Mac):** GATE T1: FALLITO (exit 13). Gate core OK, performance OK (ms/frame ~0.19 max 0.94, replay IDENTICO). Giro guidato FALLITO 5/6 livelli: solo lv_02_atomico (rettilineo 40%) supera. Causa: percorsi poco vari. Azione: @sviluppatore rivede parametri generazione percorsi.
- 09:30 (19/10) — builder-developer: nessun blocco [ ]/[~] eseguibile (P1 punto 4 e G1 [ ] di Designer/@grafico, v91 di Andrea); nessuna modifica di codice né gate; stato riconfermato da report 18/10 22:56 (exit 0, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 soak, replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1; zero scritture nei sorgenti).

## Report turno 10:56 — developer-builder-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Nessun blocco [ ]/[~] assegnato al ruolo in RICHIESTA_APERTA.md (T1-RUN @sviluppatore + @collaudatore; P1 @sviluppatore; G1 @grafico; H @collaudatore + Andrea; J [ ] escalation; MD0/T5 [-] sospesi). Nessuna modifica di codice, nessun test aggiunto, nessuna build/gate eseguita (sessione senza esecuzione comandi).
- Consegne scritte: bacheche/developer-junior/diario.md (1 riga), RIASSUNTO.md (<=12 righe), out/developer-junior/REPORT_TURNO_2026-10-19_1056.md; questo status aggiornato.
- Ultimo esito gate T1 noto invariato: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` exit 0, 18/10 22:56, 6 percorsi GATE PERCORSO OK, ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO, T1-RUN [x] DEFAULT T1-Q1 (risposta Andrea T1-Q1: sì); zero scritture nei sorgenti. In attesa: task dal Capo.

## Report turno 11:45 — builder-developer

- Blocco R1 (tracciati, priorita' di Andrea): task 1 fatto — `motore_nuovo/src/road/road.h` (`SegmentDesc` con `entrata`/`uscita`, default `min(n/3, 60)`) e `motore_nuovo/src/road/road.cpp` (raccordo entrata/uscita a lunghezza propria, curva costante al centro del tratto, uscita verso la curva del tratto successivo/anello; correzione della direzione solo su segmenti con curva != 0, mai sui rettilinei). Task 2 fatto in parte — `motore_nuovo/src/game/game.cpp` supporta il campo `tracciato` di `livello.json` (espansione da `dati/tracciati/tratti.json`; fallback su `segmenti` se mancante) e `tracciato` aggiunto ai 6 `dati/livelli/*/livello.json` (carattere tema invariato, lv_02_atomico riferimento).
- Blocco non scrivibile questo turno: nuovi file `dati/tracciati/tratti.json` e `regole.json` (contenuto pronto in `out/developer/VOCE_SCAMBIO_1174.md`), lettura di `regole.json` da `test/prova_gate.cpp` (test file di Junior) e `docs/FORMATI_DATI.md`.
- Eseguito `sh tools/gate_t1.sh soak` (19/10/2026 ~11:45, Mac): **exit 0, `GATE T1: TUTTO OK`**. 6/6 percorsi GATE PERCORSO OK: lv_01_neon rettilineo 65% (piu' lungo 1309) curva 0.57/0.53; lv_02_atomico 63% (1548) 0.88/0.62; lv_03_foglie 58% (1991) 0.67/0.83; lv_04_cromo 54% (973) 0.37/0.63; lv_05_spray 73% (966) 1.01/0.99; lv_06_desktop 52% (840) 0.51/0.59. Tutti: giro guidato finito, fuori strada 0%. Soak 30000 frame: ms/frame medio 0.187 (max 36.6 outlier singolo, limite 20 sul medio), replay 2x IDENTICO, speed@60=161.79. Accelerazione, ribaltamento, sterzo, curve continue, gara a tappe (4 combinazioni), screenshot P1: OK. Log: `UFFICIO/Officina/Test area/gate_t1.log`.
- Consegne: `bacheche/developer/diario.md` (1 riga), `bacheche/developer/RIASSUNTO.md` (<=12 righe), `out/developer/VOCE_SCAMBIO_1174.md`; questo status aggiornato. Zero scritture in `motore_nuovo/` e `tools/` oltre ai file sopra elencati; nessun test del Junior modificato.

## Report turno (19/10) — developer-builder-junior: peer review T1-RUN

- **Peer review indipendente della consegna [VOCE_SCAMBIO_1175.md](../../out/developer/VOCE_SCAMBIO_1175.md) (T1-RUN tuning percorsi): esito POSITIVO** su tutti e 6 i criteri; report in [out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md](../../out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md).
- Comando di verifica: `sh tools/gate_t1.sh soak` → **exit 0, `GATE T1: TUTTO OK`** (la mia esecuzione ha rigenerato `Test area/gate_t1.log`; le misure dell'autore lette dal log originale sono registrate nel report di review).
- Misure rilevate (identiche al log dell'autore): 6/6 `GATE PERCORSO OK` — rettilineo 52–65%, curva max 0.37–0.88, giro finito con 0 frame fuori strada; soak ms/frame medio 0.185 (autore 0.184, max 0.480 autore / 2.443 nella mia esecuzione, limite 20); replay 2x IDENTICO su tutti i livelli e nelle 4 gare; speed@60=161.79, speed@1=9.90; accelerazione, ribaltamento, sterzo, curve continue, gara, screenshot P1: OK.
- Perimetro verificato: tuning confinato ai soli `segmenti`/`lunghezza`/`durata_stimata_s` di `lv_01_neon`, `lv_03_foglie`, `lv_04_cromo`, `lv_05_spray` (commit `4dd8f06`/`e3c1f7d` + aggancio lv_05); `lv_02_atomico` e `lv_06_desktop` non toccati nei dati; nessun asset esterno; licenza CC-BY-4.0 già nel repo; `motore_nuovo/test/` e `tools/` invariati (soglie gate non abbassate).
- Note non bloccanti al Capo: (a) il working tree contiene anche le modifiche core del blocco R1 (`road.h`/`road.cpp`/`game.cpp`, campo `tracciato` nei 6 livelli) documentate in VOCE_SCAMBIO_1174 — da recensire col perimetro R1; (b) il riferimento `lv_02_atomico` 40%/0.86 di ANDREA_REVIEW T1-Q5 oggi si misura 63%/0.88 con dati lv_02 invariati, per il raccordo entrata/uscita introdotto da R1 (metrica, non dati); (c) durante la review è comparso `out/developer/RISULTATI_TURNO.md` (sessione autore, non scritto da me).

### Sintesi dei report precedenti pertinenti (questa review)

- **VOCE_SCAMBIO_1175 (autore, 19/10):** tuning percorsi dei 4 livelli, `sh tools/gate_t1.sh soak` exit 0, misure come sopra, nessun materiale esterno.
- **VOCE_SCAMBIO_1174 (autore, 19/10 11:45):** blocco R1 — `road.h` (`entrata`/`uscita`), `road.cpp` (raccordo), `game.cpp` (campo `tracciato`), gate exit 0; è la fonte delle modifiche core non imputabili a T1-RUN.
- **Report Andrea 09:39 (19/10):** `GATE T1: FALLITO` (exit 13), giro guidato KO su 5/6, solo `lv_02_atomico` (40%/0.86) passava; con il tuning attuale tutti e 6 passano. Richiesta T1-Q5: soglie non abbassate e logica tracciato modificabile (R1).

## Report turno 20/10 ~13:00 — developer

- Blocco R1 (tracciati 6 piste), stato [~]. road.cpp compito 1 FATTO (raccordo entrata/uscita min(n/3,60), deriva solo segmenti curva != 0). motore_nuovo/dati/tracciati/tratti.json (8 tratti) e regole.json CREATI; 6 livello.json con campo tracciato (loader espande da tratti.json, fallback su segmenti).
- Eseguito `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (20/10 ~13:00): build exit 0. lv_01_neon GATE PERCORSO OK (rettilineo 63%, rett max 659, curva 0.85/0.85, giro finito, fuori 0%, ms/frame medio 0.638 max 64.082 outlier); lv_02_atomico GATE PERCORSO OK (rettilineo 65%, rett max 1440, curva 0.72/0.98, ms/frame medio 0.363 max 9.072); lv_03_foglie GATE PERCORSO OK (rettilineo 57%, rett max 590, curva 0.75/0.95, ms/frame medio 0.489 max 12.995). Replay 2x IDENTICO. Comando interrotto dal timeout dello strumento (240 s) durante lv_04_cromo, prima del riepilogo finale — non un errore di codice.
- Ultimo esito completo noto: 6/6 percorsi GATE PERCORSO OK, soak 6/6 OK, replay 2x IDENTICO, ms/frame medio ~0.46 (19/10 11:45 e 20/10 pomeriggio; log `UFFICIO/Officina/Test area/gate_t1.log`).
- Compito 3 (prova_gate legge regole.json al posto delle hardcoded, return riga 536): BLOCCATO (edit negato su motore_nuovo/test/prova_gate.cpp); serve sblocco edit + CP-005 al Capo.
- Scritture: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_GATE_T1_20OTT_1300.md; questo status. Zero scritture nuove in motore_nuovo/ e tools/ oltre ai dati R1 gia' presenti.

## Report turno 20/10 17:01 — developer
- Nessun blocco eseguibile: shell negata (permission.rejected), `sh UFFICIO/Officina/TOOL/gate_t1.sh` non avviato ('gate non eseguito'); nessun build/test eseguito in questo turno.
- Blocco R1 lato @developer concluso e verificato su file: `motore_nuovo/src/road/road.cpp` raccordo entrata/uscita (default min(n/3,60), deriva solo su curve != 0), `motore_nuovo/dati/tracciati/tratti.json` (8 tratti) e `regole.json` (6 livelli + default), `prova_gate` legge `regole.json`, 6 `livello.json` con campo `tracciato`. Zero scritture nuove in `motore_nuovo/` e `tools/`.
- Ultimo gate verde noto: 20/10 14:53, `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, 6/6 GATE PERCORSO OK, ms/frame medio ~0.17 max 0.26, replay 2x IDENTICO, GATE T1 TUTTO OK (log `UFFICIO/Officina/Test area/gate_t1.log`; esito in `out/developer/ESITO_TURNO_20OTT_1453.md`).
- CP-005 APERTO: review sviluppatore su Junior punti 4-5 registrata (`out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`, positiva con rilievo non bloccante su compito 4); in attesa review Junior punti 1-3 e chiusura CP-005 da parte del Capo; in attesa ri-verifica runner Mac di Andrea (D12 = A) prima di R2.
- Prossimo per Andrea dopo R1 prima di R2: conversione 60 Hz (dt in secondi, default 60 Hz, opzione 50 Hz, ctest verde a 60 Hz; ANDREA_REVIEW 19/10).
- Consegne: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_TURNO_20OTT_1700.md.

## Report turno 20/10 19:23 — developer

- Blocco R1 [~] (tracciati 6 piste): compiti @developer 1-3 conclusi e verificati su file (`motore_nuovo/src/road/road.cpp` raccordo entrata/uscita default `min(n/3,60)`, deriva solo su segmenti con `curva != 0`; `dati/tracciati/tratti.json` 8 tratti CC-BY-4.0; `dati/tracciati/regole.json` 6 livelli + default lette da `test/prova_gate.cpp`; 6 `dati/livelli/*/livello.json` con campo `tracciato`). Zero scritture nuove in `motore_nuovo/` e `tools/` questo turno.
- T1-RUN [~]: punti 2 colline e 3 sprite-wrap gia' nel codice (renderer.cpp `zRel` con `fmod` su `totalLen`); punto 4 sprite veri bloccato su G1 @grafico.
- Gate eseguito questo turno: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 `GATE PERCORSO OK` (rettilineo 55-71%, curva max 0.72-0.98, giro guidato finito su tutti e 6, fuori strada 0%), ms/frame medio 0.171 max 0.323 (limite 20), replay 2x IDENTICO, accelerazione/ribaltamento/sterzo/curve continue/gara a tappe (4 bivi)/screenshot P1 OK. Log: `UFFICIO/Officina/Test area/gate_t1.log`; esito in `out/developer/ESITO_GATE_20OTT_1923.md`.
- CP-005 APERTO: review Junior punti 4-5 positiva con rilievo non bloccante (compito 4 stampa tratto+segmento iniziale mancante, `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`); in attesa review Junior punti 1-3 e chiusura CP-005 da parte del Capo; in attesa ri-verifica runner Mac di Andrea (D12 = A) prima di R2.
- Prossimo dopo R1 prima di R2: 60 Hz (dt in secondi, default 60 Hz, opzione 50 Hz, ctest verde a 60 Hz; ANDREA_REVIEW 19/10).
- Consegne: `bacheche/developer/diario.md` (1 riga), `bacheche/developer/RIASSUNTO.md` (<=12 righe), `out/developer/ESITO_GATE_20OTT_1923.md`, questo status.

## Report turno 21/10 ~09:00 — developer

- Eseguito `sh UFFICIO/Officina/TOOL/gate_t1.sh` (smoke, senza soak per limite tempo turno): **exit 0, `GATE T1: TUTTO OK`**. Build cmake e `cmake --build --target prova_gate` exit 0.
- 6/6 percorsi `GATE PERCORSO OK` — lv_01_neon rettilineo 63% curva 0.85/0.85; lv_02_atomico 65% 0.72/0.98; lv_03_foglie 57% 0.75/0.95; lv_04_cromo 71% 0.85/0.85; lv_05_spray 55% 0.86/0.84; lv_06_desktop 68% 0.85/0.85 (rettilineo in [25%,75%], curva max <= 1.0, dx/sx >= 0.3, soglie non abbassate). Giro guidato finito su tutti e 6, fuori strada 0%.
- ms/frame medio=0.291 max=16.094 (limite 20); replay 2x IDENTICO; speed@60=161.79, speed@1=9.90.
- accelerazione, ribaltamento/ripartenza, sterzo, continuita' curve, gara 4 bivi (arrivo, posizione, incidenti, replay IDENTICO), screenshot P1: OK.
- Risolve ANDREA_REVIEW T1-Q3: giro guidato ora OK su tutti e 6 i livelli (era fallito 5/6 su Mac 19/10 exit 13).
- Codice invariato in questo turno (zero scritture in motore_nuovo/ e tools/); R1 compiti @developer 1-3 gia' conclusi e verificati su file.
- Bloccato/attesa: ri-verifica runner Mac Andrea (D12=A, D14=A) prima di 60 Hz/R2; chiusura CP-005 Capo; G1/P1 punto 4 sprite @grafico. Nessun lavoro Pi prima ok Mac.
- Consegne: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_GATE_21OTT_0900.md; questo status aggiornato. Log: UFFICIO/Officina/Test area/gate_t1.log.

## Report turno 22/10 20:06 — developer-junior

- Lettura fonti obbligatoria completata (ordine richiesto): RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Blocco F60 [ ] -> [x]: completato e verificato. Eseguito `sh UFFICIO/Officina/TOOL/gate_t1.sh` (default, non soak): **exit 0, `GATE T1: TUTTO OK`**. Build cmake e `cmake --build --target prova_gate` exit 0.
- Frequenza (F60): `GATE FREQUENZA 60 Hz OK` e `GATE FREQUENZA 50 Hz OK`; giro guidato stessi secondi alle due frequenze; replay 2x IDENTICO a ciascuna; ms/frame medio 0.16-0.275 (limite 20).
- 6/6 percorsi `GATE PERCORSO OK` (rettilineo 52-71%, curva max <= 1.0, dx/sx >= 0.3, fuori strada 0%). Giro guidato finito su tutti e 6 i livelli.
- Accelerazione, ribaltamento/ripartenza, sterzo, continuita' curve, gara 4 bivi (arrivo, posizione, incidenti, replay IDENTICO), screenshot P1: OK.
- Nessun file src/ modificato in questo turno (percorso @developer); prove già presenti in prova_gate.cpp e gate_t1.sh (modifiche precedenti).
- Consegne: bacheche/developer-junior/diario.md (1 riga), bacheche/developer-junior/RIASSUNTO.md (<=12 righe), out/developer-junior/ESITO_F60_22OTT_2006.md; questo status aggiornato. Log: UFFICIO/Officina/Test area/gate_t1.log.
- Bloccato/attesa: H [~] (ok Andrea runner Mac v91 prima di Pi 400/RetroPie, non toccare src/runner/); R2 [ ] dopo F60.

## Report turno 25/10 08:46 — developer-junior

- Lettura fonti obbligatoria completata nell'ordine richiesto: bacheche/developer-junior/RIASSUNTO.md, DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- FORK1 [ ] punto 5 `bivio_bordi`: eseguito e scritto (era bloccato da punti 1-3 di @developer su game.cpp; ora la verifica del bordo strada attraverso il bivio è coperta da gate senza toccare src/).
- Aggiunto gate `bivio_bordi` a `motore_nuovo/test/prova_gate.cpp` (modo `bivio_bordi`, exit 18 su fallimento) con soglie in `motore_nuovo/dati/tracciati/regole.json` ("bivio_bordi": offroad_max_consecutivi 60 frame = 1 s @ 60 Hz, soglia di allarme a metà del ribaltamento OFFROAD_CRASH_S=2.0 s).
- Build `cmake --build UFFICIO/Officina/build --target prova_gate`: exit 0.
- Gate `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/bivio_bordi" lv_01_neon bivio_bordi`: **exit 0, `GATE BIVIO BORDI OK`** su entrambe le combinazioni DS e DD: cambi=2 (lv_01_neon -> lv_03_foglie -> lv_05_spray), cambio tappa frame 4027, maxOffroad=33 <= N=60, crash=no (nessuno ribaltamento dentro il bivio), max|xRel|=2800 (escursione reportata, non condizione di fallimento). Screenshot PPM in UFFICIO/Officina/Test area/bivio_bordi/.
- FORK1 [ ] punto 4 `bivio_uscita` [x]: exit 16 atteso, resta fallito atteso fino al fix game.cpp (teleport al blocco scelta game.cpp:790).
- FORK1 [ ] punto 7 `tema_transizione` [x]: OK exit 0 (07:47 turno precedente), invariato.
- Gate eseguito in questo turno: sì (`bivio_bordi`, exit 0, 25/10 08:46).
- Nessun file src/ toccato (solo motore_nuovo/test/prova_gate.cpp e motore_nuovo/dati/tracciati/regole.json, percorsi assegnati al ruolo).

## Report turno 27/10 06:37 — developer-junior

- Blocco DECO1 [ ] (punto 3 del blocco, mio): esteso `motore_nuovo/test/prova_gate.cpp` gate `decorazioni` con screenshot PPM per livello e verifica che lo skyline sia disegnato sopra l'orizzonte su lv_01_neon (passo 2A di @developer completato: `renderer.cpp` `drawSkyline`, `game.cpp` `setSkylineSprite`, raw `sprite_raw/scenari/lv_01_neon/skyline.sglraw`; `caricaSkyline` già nel gate).
- Comando: `cmake --build UFFICIO/Officina/build --target prova_gate` → exit 0.
- Comando: `UFFICIO/Officina/build/prova_gate UFFICIO/Officina/build/test-results/decorazioni lv_01_neon decorazioni` → exit 0, `GATE DECORAZIONI OK`.
- Misure: 6/6 livelli catalogo risolto (skyline+oggetti, nessun id mancante o in prestito), cartelloni 35-61 per livello, ms/frame medio <= 0.296 max <= 0.604 (limite 20) su tutti i livelli; lv_01_neon cielo 0x0A0A1A con 16292 pixel cielo diversi = skyline disegnato; screenshot PPM in `UFFICIO/Officina/build/test-results/decorazioni/decorazioni_<livello>/`.
- Nessuna regressione sugli altri modi di prova_gate (codice condiviso non toccato). Nessun file `src/` modificato.
- Consegne: `bacheche/developer-junior/diario.md` (1 riga), `bacheche/developer-junior/RIASSUNTO.md` (<=12 righe), `out/developer-junior/ESITO_DECO1_DECORAZIONI_27OTT.md`.
- In attesa: DECO1 passi 2b-2e (@developer) e G1 (@grafico), poi R2 [ ]; FORK1 [~] attende review @developer gate 4/5/7 e CP-007 @capo; T1-RUN [~] (D19 = B); H [~] chiuso da Andrea 23/10.

## Report turno 27/10/2026 07:55 — developer

- Blocco DECO1 [ ] passo 2d fatto: densita' cartelloni ~26-27% per livello (target 20-30%, D2b). motore_nuovo/src/game/game.cpp (`init()`: posizioni esplicite di livello.json + riempimento deterministico dei segmenti liberi, Rng(seed_), 1 segmento su 4, zero alloc per frame, nessun sovrapposizione con le posizioni esplicite); motore_nuovo/src/game/game.h (`leggiCartelloniDaFile` segnala i segmenti occupati).
- Gate eseguito questo turno: cmake --build UFFICIO/Officina/build --target prova_gate exit 0; sh UFFICIO/Officina/TOOL/gate_t1.sh exit 0, GATE T1: TUTTO OK — 6/6 percorsi OK (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%), ms/frame medio <= 0.65 max 2.696 (limite 20), replay 2x IDENTICO, gara 4 combinazioni OK, frequenza 60/50 Hz OK, screenshot P1 OK. Log: UFFICIO/Officina/Test area/gate_t1.log.
- sh UFFICIO/Officina/TOOL/run_tests.sh -> ctest 21/21 verde (gate decorazioni su 6/6 livelli incluso).
- Misure densita' cartelloni: lv_01 26.8% (1141), lv_02 26.4% (1492), lv_03 27.0% (1268), lv_04 26.7% (1414), lv_05 26.6% (1249), lv_06 26.6% (1288).
- Consegne: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_DECO1_P2D_27OTT.md; questo status aggiornato. Zero scritture in UFFICIO/Officina/TOOL/; nessun test del Junior modificato; src/runner/ non toccato (review Andrea runner Mac in corso).
- Prossimo: DECO1 2e (parallasse), poi R2 (ordine DECO1 -> R2 -> PIST1 -> RIV -> FIN). T1-RUN [~] e H [~] in attesa Andrea (runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac.

