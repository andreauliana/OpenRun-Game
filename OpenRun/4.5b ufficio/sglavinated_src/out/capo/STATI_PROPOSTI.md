# STATI PROPOSTI — out/capo/ (Turno 1, ora 0 di 12)

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Regole 4.6: scrittura solo nei percorsi assegnati; review indipendente (autore != revisore); prove = comando, exit code, misure; percorsi relativi, mai cd. RICHIESTA.md e SCAMBIO.md non si modificano (sola lettura).

## Stato tappa attiva — Tappa 1, Gate T1 (blocco T1-RUN) — [~]

Tappa: "Il gioco parte, si visualizza il player che si muove, si pilota".
Gate: strada che scorre, auto accelera (A), frena (B), sterza sx/dx. Debug: build pulita, nessun crash in 5 minuti.

Evidenza corrente (20/10 ~14:05): `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi GATE PERCORSO OK, ms/frame medio ~0.18-0.20 (max < 2, limite 20), replay 2x IDENTICO, speed@60=161.79, guida 0 frame fuori strada. Log: `UFFICIO/Officina/Test area/gate_t1.log`; evidenza: `out/developer/ESITO_GATE_T1_20OTT_1405.md`.
Antecedente: Andrea su Mac 19/10 09:39 aveva riportato `GATE T1: FALLITO` (exit 13), giro guidato KO su 5/6 livelli ("percorso poco vario o non guidabile"; solo lv_02_atomico OK). Blocco R1 (tracciati) ha corretto `road.cpp` (raccordo entrata/uscita, deriva solo su curve), `dati/tracciati/tratti.json` (8 tratti), `dati/tracciati/regole.json` e i 6 `livello.json` con campo `tracciato`; gate ora verde su tutte e 6 le piste.

Mancano per chiudere T1-RUN: registrazione review incrociata indipendente su CP-005 e riesecuzione/registrazione ctest (7 test scoperti, criterio CP-005 da confermare). v91 Andrea su runner Mac (SDL2) e misure Pi 400 restano in attesa (D12 DEFAULT A: prima Mac, poi Pi; nessun lavoro Pi dedicato prima dell'ok Andrea).

T1-Q1 DEFAULT: sì — chiudi T1-RUN a [x] se `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, replay 2x IDENTICO, ms/frame <= 20. (Andrea: sì — condizione tecnica soddisfatta, manca registrazione review + ctest.)
T1-Q2 DEFAULT: sì — promuovi P1 a [x]. (Andrea: sì; punto 4 sprite veri ancora aperto, dipende da G1.)

## Decisione registrata di Andrea (19 ott) — frequenza di gioco

60 Hz predefinito, opzione 50 Hz; dopo R1, prima di R2. Tutte le misure in secondi (dt), non frame. Non è una domanda aperta: è già decisione di Andrea (ANDREA_REVIEW.md, voce 19 ott). Destinatari: @developer (autore), @developer-junior (revisore); @capo apre checkpoint dedicato se necessario.

## Deleghe proposte (ruoli 4.6; consegne in out/<ruolo>/)

### 1. @developer — responsabile R1 punti 1-2 (core tracciati) e verifica build/gate
Percorsi scrivibili: `motore_nuovo/src/road/road.cpp`, `motore_nuovo/src/game/*.{cpp,h}`, `motore_nuovo/dati/tracciati/*.json`, `motore_nuovo/dati/livelli/*/livello.json`; consegna in `out/developer/`.

Task completati (da registrare): `road.cpp` raccordo curva con campi `entrata`/`uscita` (default `min(n/3, 60)`), curva costante, correzione deriva solo su segmenti con curva != 0; `dati/tracciati/tratti.json` (8 tratti: rettilineo_corto/lungo, curva_dolce_dx/sx, curva_stretta_dx/sx, esse, tornante, collina_su/giu); `dati/tracciati/regole.json`; campo `tracciato` nei 6 `livello.json` (lv_02_atomico riferimento).
Task a completare: rieseguire `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20) ed eseguire i test CTest (`sh UFFICIO/Officina/TOOL/run_tests.sh`); registrare comando, exit code, misure in `out/developer/`.

Revisore: @developer-junior (peer review incrociata su punti 4-5 di R1: `prova_gate` legge `regole.json`, test espansione tratti, rapporto piste).
Criteri di completamento: gate verde su tutte e 6 le piste; replay 2x IDENTICO; ms/frame <= 20; ctest verde; review incrociata registrata su CP-005.

### 2. @developer-junior — responsabile R1 punti 3-5 (gate/test) e T1-RUN punti 5-7
Percorsi scrivibili: `motore_nuovo/test/prova_gate.cpp`, `motore_nuovo/dati/livelli/*/livello.json`; consegna in `out/developer-junior/`.

Task: `prova_gate` legge `regole.json` al posto delle soglie hardcoded e stampa, per ogni pista, quale TRATTO viola quale regola (nome tratto + segmento iniziale); test che l'espansione di ogni `tracciato` dia numero segmenti = somma lunghezze (6/6); rapporto test per piste ancora poco guidabili; casi `prova_gate` per wrap di `playerZ` oltre `totalLen` e inattivita' 10 s -> show/fumetto visibile in screenshot (T1-RUN punti 5-7).

Revisore: @developer (peer review incrociata su punti 1-3 di R1).
Criteri di completamento: gate percorso verde su tutte e 6 le piste; ctest verde; review incrociata registrata su CP-005.

### 3. @designer (= @grafico) — responsabile G1 (catalogo grafico)
Percorsi scrivibili: `assets_originali/CATALOGO_GRAFICO.json`, `assets_originali/INDICE_ASSET.json/.md`, `MAPPA_LIVELLI_SPONSOR.json`; consegna in `out/designer/`.

Task: catalogo unico (un elemento per risorsa: `id`, `tipo`, `stato` mancante/bozza/in_costruzione/pronto/approvato/scartato, `file`, `raw`, `livelli`); cataloga i 100 PNG fuori elenco (varianti B/C, scenari doppi) come `scartato` o nuovo elemento, senza cancellare file; ogni sprite nuovo entra nel catalogo con il suo stato prima di essere usato; motore e test usano solo `pronto` e `approvato`; verifica temi lv_01_neon..lv_06_desktop, cartelloni sponsor ("Curve Pericolose" ~20% segmenti, "Brinis Udine" 10-15%), HUD variante B con elemento "tempo" residuo in alto a sinistra sotto velocita' (D8-A).

Revisore: @capo (completezza catalogo, provenienza e licenze; non e' peer review tecnica né approvazione di rilascio).
Criteri di completamento: `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; manifest resta bozza fino ad approvazione PNG.

## Stati blocchi RICHIESTA_APERTA proposti (RICHIESTA.md/SCAMBIO.md non toccati)

C [x] · P1 [x] (T1-Q2 sì) · P1b [x] · P2 [x] · P3 [x] · T1-RUN [~] (gate verde 6/6, in attesa review CP-005 + ctest) · R1 [~] (modifiche eseguite, gate verde, review incrociata da registrare) · R2 [-] (dopo R1) · G1 [~] (attivato per T1-Q4) · H [~] (v91 Andrea su Mac + fps/temperatura Pi 400; nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Andrea) · J [ ] · MD0 [-] · T5 [-].

## Domande ad Andrea (sempre con DEFAULT, procedo senza aspettare)

### D13 | Chiusura T1-RUN | capo | [APERTA]
Domanda: confermare passaggio di T1-RUN a [x] non appena registrata la review incrociata indipendente su CP-005 e rieseguito ctest verde, dato che il gate `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` e' gia' verde (20/10 ~14:05, exit 0, 6/6 percorsi OK, replay 2x IDENTICO, ms/frame <= 20)?
Opzioni: A) sì, chiudi T1-RUN [x] dopo review CP-005 + ctest  B) no, attendere ulteriore prova/decisione di Andrea
Raccomandazione: A
DEFAULT usato nel frattempo: sì (T1-Q1 risposta Andrea: sì; gate verde registrato in `STATUS_OFFICINA.md` e `STATO_CORRENTE.md`)
Impatto se cambi idea: T1-RUN resta [~] e si attende nuova prova di Andrea prima della tappa 2.
RISPOSTA DI ANDREA: — (in attesa)

### D14 | Ordine test post-R1 e Pi 400/RetroPie | capo | [APERTA]
Domanda: dopo T1-RUN [x] e CP-005 chiuso, Andrea ri-verifica il runner su Mac (v91, SDL2) prima di ogni lavoro dedicato a Pi 400/RetroPie, come da D12 (risposta A registrata) e ordine test T1-Q5?
Opzioni: A) sì, ri-verifica Mac prima di Pi 400  B) no, si procede direttamente a Pi 400 dopo review
Raccomandazione: A
DEFAULT usato nel frattempo: sì — nessun lavoro dedicato a Pi 400/RetroPie finché Andrea non dà ok su Mac; non rompere la build Linux (core libretro manca `libretro.h`, `BUILD_LIBRETRO=ON` non compila oggi).
Impatto se cambi idea: saltare ri-verifica Mac e passare direttamente a Raspberry Pi 400/RetroPie (serve `libretro.h` in `src/libretro/`).
RISPOSTA DI ANDREA: — (D12 A già registrato; conferma richiesta)

### D15 | Attivazione G1 e delega @designer | capo | [APERTA]
Domanda: confermare attivazione G1 (catalogo grafico) con delega al @designer (= @grafico): catalogare i 100 PNG fuori elenco, `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato`?
Opzioni: A) sì, attiva G1 su @designer  B) no, tieni G1 fuori scope T1
Raccomandazione: A
DEFAULT usato nel frattempo: sì (Andrea T1-Q4: attivare)
Impatto se cambi idea: G1 resta [ ]; sprite veri (P1 punto 4, traffico, personaggi, UFO) restano bloccati.
RISPOSTA DI ANDREA: — (T1-Q4 già risposto: attivare; conferma richiesta)

## Chiusura turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~], R1 [~], G1 [~], H [~]) -> non creo FINITO.
Non sono bloccato -> non scrivo PER_CLAUDE.md.
Prossimo: registrare review incrociata indipendente su CP-005 (@developer-junior rivede @developer punti 1-3; @developer rivede @developer-junior punti 4-5), rieseguire ctest, poi chiudere T1-RUN [x] e avviare G1.
