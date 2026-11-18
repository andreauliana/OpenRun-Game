# FOGLIO RICHIESTA — OpenRun v4.7

Stato: [ ] da fare · [~] in corso · [x] fatto · [!] bloccato (scrivi una riga in `PER_CLAUDE.md`) · [-] sospeso (non iniziare)

Questo foglio lo modificano SOLO Andrea e Claude (il ciclo lo legge per scegliere gli agenti). @capo propone gli stati in `UFFICIO/out/capo/STATI_PROPOSTI.md`.
Ruoli: @capo, @developer, @developer-junior, @grafico (AGENTS.md). I nomi vecchi (@sviluppatore, @collaudatore, @builder-developer, @designer) li traducono gli script.
Blocchi chiusi o sospesi fino al 18/11 (T1-RUN, R1, R2, F60, FORK1, DECO1, PIST1, RIV, AUTO1, ENG1, KO1, FIN, PERC1, MIG1; sospesi H, MD0, T5, N, M): `UFFICIO/archivio_per_claude/RICHIESTA_BLOCCHI_CHIUSI_FINO_18NOV.md` (non leggerlo, non riaprirli).
Nuovo blocco: `## BLOCCO <NOME> — @ruolo ...`, righe `| Compito |`, `| Vincoli |`, `| Gate |`, `| Stato | [ ] |`.

<!-- ===== INIZIO AREA V47: lavoro continuo verso la v4.7, dopo i propri passi MIG1 ===== -->
## BLOCCO V47 — avanti verso la v4.7 (continuo) @developer @developer-junior @grafico @capo

**Come si lavora (Andrea 08/11 notte):** non si aspetta l'ok di Andrea. Finiti i tuoi passi MIG1, vai avanti qui, un passo per turno, sempre dentro il raggio d'azione di ANDREA_REVIEW (base legacy e vocabolario non si toccano: si AGGIUNGE). Andrea guarda le parti grafiche quando puo'; il suo parere arriva come blocco nuovo, non ferma questo. Stesse prove di MIG1 (ctest verde, gate exit 0, replay identico, Pi 400 <= 12 ms).

### @developer
- **V1 Guida piu' interessante (+20%), solo aggiunte:** scia dietro un'auto (a meno di ~1500 davanti, stessa corsia, per 1 s: piccolo guadagno di velocita'), sorpasso al pelo (un'auto passata a meno di ~350 di lato: breve suono e +0.5 s di tempo). Niente cambi a sterzo, curve, derapata, incidenti. Una voce nuova per azione in `ULTIMO_TURNO.md` (Claude la mette nel vocabolario).
- **V2 Pausa:** FATTA da Claude 08/11 notte (Start o Invio). Non rifarla; passa al punto dopo.
- **V3 Fluidita':** misura ms/frame su tutti e 6 i livelli (gate `percorso`), elenca i 3 punti piu' lenti e alleggeriscili senza cambiare la resa.
- **V4 Risultati -> gioco:** passaggio piu' morbido (dissolvenza di 0.5 s) fra risultati, menu e partenza.
- **V5 Testi:** tutto quello che si vede a schermo in inglese semplice (fasi, risultati, frasi).

### @developer-junior
- **VJ1** Dopo ogni passo V: prova dedicata col gate esistente e una riga nel tuo `REVIEW_V47.md` (riscritto).
- **VJ2** Ponte e riva: screenshot di spiagge, gabbiani e barche in lv_01, lv_04, lv_06; se in qualche punto sono troppo fitti scrivilo con livello e posizione (Andrea decide).

### @grafico
- **VG1** Dopo G1: quadro di cosa manca (`OGGETTI_LIVELLI.md`, gruppo B) e proposte con anteprima in `UFFICIO/Officina/Test area/grafico/`; mai ritoccare i PNG che Andrea ridisegna (auto, logo, sfondi, oggetti dei livelli).

**Revisione Claude 09/11 pomeriggio:** V1, V3, V4 tenuti con due correzioni. V1: il +0.5 s del sorpasso al pelo scattava a ogni sorpasso (bastava avere l'auto davanti in corsia); ora conta la distanza di lato nel momento del sorpasso. V4: la fase vecchia spariva subito (schermo vuoto) e dal menu alla gara la dissolvenza non si vedeva; ora la scena vecchia si scurisce e quella nuova schiarisce, anche la strada. Non rifare questi punti. Prossimi: @developer V5 (resto dei testi), @grafico V5 frasi.json.

**Revisione Claude 10/11 mattina:** V1-V5 fatti e tenuti (frasi.json in inglese, it1/it2 conservati; splash "OPEN RUN"). VJ1 su V5 fatto. **VJ2 fatto**: gli screenshot di riva li guarda Andrea quando puo', non si aspetta. Tolti i due script di traduzione usa-e-getta (`traduci_frasi_en.py`, `v5_frasi_inglese.py`): il lavoro e' fatto, non ricrearli. Non rifare V1-V5. ctest 27/27 verde sul main del 10/11.

**Perche' stamattina vi siete fermati:** i passi della lista erano finiti alle 08:30. Da qui in poi la lista ha passi **"sempre disponibili"** (segnati ∞): quando i tuoi passi numerati sono [x], fai un passo ∞, uno per turno, su un livello o un file diverso dal turno prima. **"Nessun blocco azionabile" non e' piu' un esito valido** finche' esiste un tuo passo ∞.

### @developer — seconda lista
- **V6 Core libretro (RetroPie).** `cmake -DBUILD_LIBRETRO=ON` compila (Claude 10/11, Linux). Confronta `src/libretro/sglavinated_libretro.cpp` col runner SDL2 (`src/runner/`): partenza con griglia e 3-2-1, pausa con Start, 50/60 Hz da `config.json`, musica .xm e reazioni, nome giocatore salvato, passaggio menu -> gara -> risultati. Correggi nel file libretro solo cio' che manca; elenco in `ULTIMO_TURNO.md`.
- **V7 Niente allocazioni nel ciclo di gioco.** Cerca dentro `Game::update`, `Game::render` e nel renderer `new`, `push_back` senza `reserve`, `std::string` costruite a ogni frame, `resize`. Spostale fuori dal ciclo (prealloca all'avvio del livello). Misura ms/frame (gate `percorso`) prima e dopo; resa e replay IDENTICI.
- **V8 Caricamento.** Misura in ms quanto ci mette a caricare ogni livello e il menu (log nel gate). Se uno supera 1 s sul Mac, alleggerisci (carica una volta sola, niente letture doppie dei file). Non cambiare i dati.
- **∞ V9 Fluidita' a giro:** un livello per turno (lv_01 -> lv_06, poi da capo): gate `percorso`, guarda il frame piu' lento; se supera 8 ms sul Mac trova la funzione e alleggeriscila senza cambiare la resa. Se e' gia' sotto, scrivilo e passa al livello dopo.

### @developer-junior — seconda lista
- **VJ1** resta: dopo ogni passo V del @developer, prova col gate esistente e una riga in `REVIEW_V47.md`.
- **VJ3 50 e 60 Hz:** `prova_gate ... <livello> frequenza` su tutti e 6 i livelli (uno o due per turno). Tempi in secondi uguali alle due frequenze; esiti in `REVIEW_V47.md`.
- **VJ4 Soak:** `prova_gate ... <livello> soak` su tutti e 6 i livelli, uno per turno: replay, ms/frame massimo, eventuali crash.
- **∞ VJ5 Review del codice:** leggi l'ultimo cambio del @developer in `src/` e scrivi in `REVIEW_V47.md` un possibile errore o una semplificazione, con file e riga. Prima di ogni prova ricompila (`run_tests.sh`): mai usare un `prova_gate` vecchio.

### @grafico — seconda lista
- **VG2 [x] CHIUSO (Claude 11/11): frasi.json va bene cosi', non riscriverla piu'.** Era: rileggi le 156 frasi di `motore_nuovo/dati/frasi.json` (`linea1`/`linea2`) e sistema quelle che in inglese suonano strane o restano a meta'. Max 16 caratteri per riga, maiuscole, stesso id e personaggio; `it1`/`it2` non si toccano. Venti frasi per turno.
- **∞ VG3 PNG di Andrea convertiti:** Andrea sta ridisegnando gli oggetti. A ogni turno confronta le date dei PNG in `GRAFICA/scenari/` con i loro `.sglraw`: se un PNG e' piu' nuovo, convertilo con lo script del catalogo (per `comuni/oggetti`: `converti_oggetti_raw.py`) senza ritoccare il disegno, e scrivi quali file. Se e' tutto allineato, scrivilo in una riga e fermati.

| Chi tocca adesso | @developer V9 ∞ · @developer-junior VJ4 poi VJ5 ∞ · @grafico VG3 ∞ (VG2 chiuso) |
|---|---|
| Stato | [~] | (Claude 10/11: seconda lista, lavoro continuo, nessuna attesa) |

<!-- ===== FINE AREA V47 ===== -->

---

## RIEPILOGO CORRENTE — tenuto da Andrea/Claude, un solo blocco
Stato (18/11): unico blocco aperto V47 [~], lavoro continuo con passi sempre disponibili (∞). Base legacy 08/11 e milestone "legacy 18/11" (guida, auto, voce dello sterzo) bloccate: si AGGIUNGE, non si ritocca (ANDREA_REVIEW.md). Prova sul Pi 400 (blocco H) sospesa finche' Andrea non la fa.
Tocca a (Claude 18/11): @developer V9 ∞ · @developer-junior VJ4 poi VJ5 ∞ · @grafico VG3 ∞ · @capo una riga di registro breve per turno. "Nessun blocco azionabile" non e' un esito valido finche' esiste un tuo passo ∞. Lo storico dei vecchi "Tocca a" e' in `UFFICIO/archivio_per_claude/RICHIESTA_TOCCA_A_FINO_08NOV.md` (non leggerlo).
