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

| Chi tocca adesso | sostituito dal BLOCCO V48 (27/11) |
|---|---|
| Stato | [-] | (Claude 27/11: V9, VJ5 e VG3 ∞ sostituiti da V48. Il lavoro fatto fino al 27/11 resta tutto, non si rifà e non si annulla) |

<!-- ===== FINE AREA V47 ===== -->

## BLOCCO V48 — Pi 400, RetroPie e 60 fps @developer @developer-junior @grafico @capo

**Andrea 27/11:** si tiene tutto il lavoro degli agenti fino a stamattina (ctest 27/27). Adesso conta che il gioco giri bene sul **Raspberry Pi 400 e su RetroPie**, a **60 fps costanti**, e che l'esperienza di gioco migliori. Basta ritocchi invisibili sul Mac. Un passo per turno, dentro ANDREA_REVIEW (legacy e auto del giocatore non si toccano: `drawPlayerCar` resta bloccata, D20).

**60 Hz (Andrea 27/11):** il gioco si fa, si prova e si misura **solo a 60 Hz** (Mac, HDMI, Raspberry Pi 400, RetroPie). Niente piu' lavoro o prove a 50 Hz: non lanciare `frequenza` o `frequenza_confronto`, VJ3 e' chiuso, la differenza 2,40% su lv_05_spray non conta. Il codice dell'opzione 50 Hz resta com'e' (non si toglie e non si ritocca).

**Riferimento di guida:** chi tocca guida, rivali, urti o tempi controlla la sezione "Riferimento di guida" in fondo a `REGOLE_AUTO_GIOCATORE.md`. Si ottimizza la velocita' del codice, non la sensazione di guida.

**Paletti a bordo strada: chiusi.** Oggi non ci sono al bivio, sul ponte, in galleria e agli incroci, e vanno bene cosi'. Non si ritoccano piu'. La prova `pali` e' tarata su lv_01: su lv_06 esce 27 per la soglia, non per un errore del gioco; non usarla sugli altri livelli.

### @developer
- **W1 Pad SNES e PS1 su RetroPie (subito).** Andrea 27/11: priorita' ai pad comuni (cloni SNES USB, PlayStation 1/2 via adattatore), non a volanti e stick analogici (lo stick resta com'e', nessun lavoro). Stessi tasti nel runner (`src/runner/main.cpp`, usato da Ports) e nel core (`src/libretro/sglavinated_libretro.cpp`): croce = sterzo e menu; gas = B del SNES / Croce PS1 (+ R2); freno = A del SNES / Cerchio PS1 (+ L2, + Giu'); Start = pausa; Select+Start = esci. Oggi nel runner i grilletti L2/R2 dei pad riconosciuti da SDL non sono mappati: aggiungili. Scrivi la tabella dei tasti in `pi/LEGGIMI.md`. **Fine:** runner e core con la stessa mappa; `ctest -j1` 27/27; una riga in `ULTIMO_TURNO.md`.
- **W2 Pi 400 sotto i 12 ms.** Usa `misura_pi` (PR #127: `misura_pi <out> <secondi> [livello|gara]`) invece del gate `percorso`. Prendi il livello con p99 più alto e alleggerisci **una** funzione pesante per turno (render della strada, oggetti, CRT), mai l'auto del giocatore. Quando Andrea porta `misure_pi400/<data>/riepilogo.txt` dal Pi, si lavora su quei numeri. Resa e replay IDENTICI. **Fine:** p99 <= 12 ms su tutti i livelli alla misura di Andrea sul Pi; finché non c'è, p99 sul Mac senza peggiorare.
- **W4 Caricamento leggero.** Misura avvio, menu e caricamento di ogni livello (PNG/raw, musica .xm, tracciati) con i log CARICAMENTO del gate. Se un caricamento supera 1 s sul Mac (sul Pi conta 4-5 volte di piu'), togli letture doppie o conversioni ripetute. Dati e resa identici.
- **W3 60 fps costanti.** In `soak` su tutti i 6 livelli nessun frame sopra 16,7 ms (un frame perso a 60 Hz). Se succede: livello, frame, funzione, correzione.

### @developer-junior
- **WJ1 Misura di riferimento (subito).** `misura_pi` sui 6 livelli + `gara` sul Mac. Una riga per livello in `REVIEW_V48.md`: media, p95, p99, max ms/frame (update/render/CRT). Nessun codice toccato.
- **WJ2 Review di W1-W4** (al posto di VJ5): dopo ogni passo del @developer, ricompila (`run_tests.sh`), prova dedicata e una riga in `REVIEW_V48.md`. Per W1: confronta tasto per tasto la mappa del runner con quella del core.
- **WJ3 RetroPie senza Pi.** Compila il core (`-DBUILD_LIBRETRO=ON`) e controlla che `pi/compila_e_installa.sh` e `pi/LEGGIMI.md` usino percorsi e target che esistono davvero. Solo elenco in `REVIEW_V48.md`, non modificare gli script `pi/`.

### @grafico
- **WG1 Un solo controllo.** Unisci `vg3_date_png_raw.py`, `vg3_png_non_catalogati.py` e `vg3_raw_senza_catalogo.py` in **un** script con gli stessi controlli ed exit code; cancella i tre vecchi e i `_tmp_*.py`. **Fine:** un solo script vg3 in `Dipartimento Grafico/TOOLS/`, exit 0 sul main.
- **WG2 Poi fermo.** Converti solo quando Andrea cambia un PNG. Se non c'è nulla: una riga nel diario e ti fermi. Gli script non si riscrivono più.

| Chi tocca adesso | @developer W1 poi W2/W3/W4 · @developer-junior WJ1 poi WJ2, WJ3 · @grafico WG1 poi WG2 |
|---|---|
| Stato | [~] | (Claude 27/11, deciso con Andrea) |

---

## RIEPILOGO CORRENTE — tenuto da Andrea/Claude, un solo blocco
Stato (27/11): unico blocco aperto **V48** [~] (Pi 400, RetroPie, 60 fps). V47 [-] sostituito, il suo lavoro resta. Base legacy 08/11 e milestone "legacy 18/11" (guida, auto, voce dello sterzo) bloccate: si AGGIUNGE, non si ritocca (ANDREA_REVIEW.md). Prova sul Pi 400 (blocco H) sospesa finche' Andrea non la fa.
Tocca a (Claude 27/11): @developer W1 · @developer-junior WJ1 · @grafico WG1 · @capo una riga di registro breve per turno. Lo storico dei vecchi "Tocca a" e' in `UFFICIO/archivio_per_claude/RICHIESTA_TOCCA_A_FINO_08NOV.md` (non leggerlo).
