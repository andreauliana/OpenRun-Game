# STATI PROPOSTI — out/capo/ (Turno 2, ora 3 di 12)

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Regole 4.6: scrittura solo nei percorsi assegnati; review indipendente (autore != revisore); prove = comando, exit code, misure; percorsi relativi, mai cd. RICHIESTA.md e SCAMBIO.md non si modificano (sola lettura).

## Stato tappa attiva — Tappa 1, Gate T1 (blocco T1-RUN) — [~]

Tappa: "Il gioco parte, si visualizza il player che si muove, si pilota".
Gate: strada che scorre, auto accelera (A), frena (B), sterza sx/dx. Debug: build pulita, nessun crash in 5 minuti.

Evidenza (ANDREA_REVIEW.md T1-Q3, 19/10 Mac): `sh tools/gate_t1.sh soak` exit 13, GATE T1 FALLITO.
- Core OK: soak 30000 frame, accelerazione, ribaltamento, sterzo, curve, gara a tappe, screenshot P1; ms/frame medio ~0.19 max 0.94; replay 2x IDENTICO 6/6 livelli, determinismo OK.
- KO: giro guidato FALLITO su 5/6 livelli ("percorso poco vario o non guidabile"); solo lv_02_atomico OK. Riferimento guidabile: lv_02_atomico rettilineo 40%, curva max 0.86.
- D13 (20/10): gate soak dichiarato verde (exit 0, 6/6 percorsi OK, replay 2x IDENTICO, ms/frame <= 20); chiusura T1-RUN attende review CP-005 + ctest + conferma Andrea.

Blocco attivo (priorità Andrea): R1 — tuning tracciati/percorsi per tutte le 6 piste, poi rieseguire gate.

Mancano per chiudere T1-RUN: (1) gate soak verde 6/6 piste dopo R1; (2) ctest verde; (3) checkpoint CP-005 con entrambe le review indipendenti registrate; (4) conferma Andrea D13 (DEFAULT sì). v91 Andrea runner Mac (SDL2) ancora da fare: D12/D14 DEFAULT sì — nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Andrea su Mac.

## Decisione Andrea già presa (non domanda aperta)

Frequenza di gioco 60 Hz predefinito, opzione 50 Hz; tutte le misure in secondi (dt); dopo R1, prima di R2. Destinatari: @developer (autore), @developer-junior (revisore); @capo apre checkpoint dedicato se necessario.

## Deleghe proposte (ruoli 4.6; consegne in out/<ruolo>/)

### 1. @developer — responsabile R1 punti 1-2 (core tracciati) e verifica build/gate
Percorsi scrivibili: `motore_nuovo/src/road/road.cpp`, `motore_nuovo/src/road/road.h`, `motore_nuovo/src/game/*.{cpp,h}`, `motore_nuovo/dati/tracciati/*.json`, `motore_nuovo/dati/livelli/*/livello.json`; consegna in `out/developer/`.
Task: raccordo curva con `entrata`/`uscita` (default `min(n/3, 60)`), curva costante, correzione deriva solo su segmenti con curva != 0; `dati/tracciati/tratti.json` (libreria tratti con nome) e `regole.json`; campo `tracciato` nei 6 `livello.json` (lv_02_atomico riferimento); loader con fallback su `segmenti`.
Task a completare: rieseguire `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20) ed eseguire `sh UFFICIO/Officina/TOOL/run_tests.sh` (ctest 17/17); registrare comando, exit code, misure in `out/developer/`.
Revisore: @developer-junior (peer review incrociata).
Criteri di completamento: gate verde su tutte e 6 le piste; replay 2x IDENTICO; ms/frame <= 20; ctest verde; review incrociata registrata.

### 2. @developer-junior — responsabile R1 punti 3-5 e T1-RUN punti 5-7
Percorsi scrivibili: `motore_nuovo/test/prova_gate.cpp` (edit da sbloccare/chiarire — blocco segnalato Turno 1), `motore_nuovo/dati/livelli/*/livello.json`; consegna in `out/developer-junior/`.
Task: `prova_gate` legge `dati/tracciati/regole.json` al posto delle soglie hardcoded e stampa, per ogni pista, quale TRATTO viola quale regola (nome tratto + segmento iniziale); test espansione tracciati OK 6/6; casi `prova_gate` wrap di `playerZ` oltre `totalLen` e inattività 10 s -> show/fumetto visibile (T1-RUN punti 5-7).
Blocco segnalato: CONTROLLO.md segnala permessi negati per developer-junior (2); edit su `motore_nuovo/test/prova_gate.cpp` da chiarire/sbloccare prima del completamento.
Revisore: @developer (peer review incrociata).
Criteri di completamento: gate percorso verde su tutte e 6 le piste; ctest verde; review incrociata registrata.

### 3. @designer (= @grafico) — responsabile G1 (catalogo grafico)
Percorsi scrivibili: `assets_originali/CATALOGO_GRAFICO.json`, `assets_originali/INDICE_ASSET.json/.md`, `MAPPA_LIVELLI_SPONSOR.json`; consegna in `out/designer/`.
Task: catalogo unico (`id`, `tipo`, `stato` mancante/bozza/in_costruzione/pronto/approvato/scartato, `file`, `raw`, `livelli`); cataloga i 100 PNG fuori elenco (varianti B/C, scenari doppi) come `scartato` o nuovo elemento, senza cancellare file; ogni sprite nuovo entra nel catalogo con il suo stato prima di essere usato; motore e test usano solo `pronto` e `approvato`; verifica temi lv_01_neon..lv_06_desktop, cartelloni sponsor ("Curve Pericolose" ~20% segmenti, "Brinis Udine" 10-15%), HUD variante B con elemento "tempo" residuo in alto a sinistra sotto velocità (D8-A).
Revisore: @capo (completezza catalogo, provenienza e licenze; non è peer review tecnica né approvazione di rilascio).
Criteri di completamento: `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; manifest resta bozza fino ad approvazione PNG.

## Stati blocchi proposti (RICHIESTA.md/SCAMBIO.md non toccati)

C [x] · P1 [~] · P1b [x] · P2 [x] · P3 [x] · M [~] · R1 [~] (priorità: tuning percorsi 6 piste dopo fallimento giro guidato 5/6) · R2 [-] (dopo R1) · G1 [~] (attivato per T1-Q4/D15; DEFAULT sì) · T1-RUN [~] (gate core OK, giro guidato in tuning via R1; in attesa CP-005 + ctest + conferma Andrea D13) · H [~] (v91 Andrea su Mac + fps/temperatura Pi 400; nessun lavoro Pi 400/RetroPie dedicato prima dell'ok Andrea) · J [ ] · N [ ] · MD0 [-] · T5 [-] · Frequenza 60 Hz [~] (decisione Andrea 19 ott; dopo R1, prima di R2).

## Domande ad Andrea (sempre con DEFAULT, procedo senza aspettare)

### D13 | Chiusura T1-RUN | capo | [APERTA]
Domanda: confermare passaggio di T1-RUN a [x] non appena registrata la review incrociata indipendente su CP-005 (entrambe positive) e rieseguito ctest verde, dopo che R1 ha riportato il giro guidato verde su tutte e 6 le piste?
Opzioni: A) sì, chiudi T1-RUN [x] dopo CP-005 + ctest  B) no, attendere ulteriore prova/decisione di Andrea
Raccomandazione: A
DEFAULT usato nel frattempo: sì (T1-Q1 risposta Andrea: sì; gate core verde registrato, giro guidato in tuning via R1).
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
DEFAULT usato nel frattempo: sì (Andrea T1-Q4: attivare).
Impatto se cambi idea: G1 resta [ ]; sprite veri (P1 punto 4, traffico, personaggi, UFO) restano bloccati.
RISPOSTA DI ANDREA: — (T1-Q4 già risposto: attivare; conferma richiesta)

## Chiusura turno

Non tutti i blocchi della tappa sono [x] (R1 [~], G1 [~], T1-RUN [~], H [~], P1 [~], M [~]) -> non creo FINITO.
Non sono bloccato come Capo (scrittura out/capo/STATI_PROPOSTI.md consentita) -> non scrivo PER_CLAUDE.md.
Blocchi operativi residui: (a) @developer esegue tuning R1 e registra gate soak + ctest in out/developer/; (b) @developer-junior esegue prove prova_gate e review incrociata (edit da sbloccare); (c) @capo apre CP-005 dopo consegna R1 e registra entrambe le review indipendenti; poi chiudere T1-RUN [x] dopo conferma Andrea D13 e avviare G1.