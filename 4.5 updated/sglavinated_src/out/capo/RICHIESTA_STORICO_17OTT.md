# RICHIESTA.md — storico spostato il 17 ott 2026 (da Claude)

Perche': le celle Stato e i RIEPILOGO TURNO di RICHIESTA.md erano diventati paragrafi lunghi, riletti dal capo a ogni turno
(RICHIESTA_APERTA.md li ripete tutti perche' non sono [x]). In RICHIESTA.md ora c'e' una riga per cella e un solo
`## RIEPILOGO CORRENTE`. Qui sotto il testo originale, IDENTICO, per non perdere nulla (regola 6).

## 1. Celle Stato originali (prima della compattazione)

### BLOCCO C
| Stato | [x] | (verifica statica sintassi OK: 30 file nucleo, 30/30 SPDX MIT, 0 include malformati, core senza SDL — voci SCAMBIO 424/60. Build cmake/fps reali non eseguibili qui (regola 21) -> rimandati a Pi 400, blocco H.)

### BLOCCO D
| Stato | [x] | (confermato da @capo: @traduttore ha consegnato SPEC_GAMEPLAY.md + modulo gameplay in src/gameplay/ (show.cpp/h, escalation, frasi, personaggi, ufo), dati/frasi.json 156 fumetti già consegnati con CONS; dati/show_logica.md; ogni file con licenza dichiarata, voci SCAMBIO 23 e BAR traduttore)

### BLOCCO G
| Stato | [x] | (consegna su disco 16 ott 2026: out/testi/RIEPILOGO_CONSEGNA_G.md, motore_nuovo/dati/testi/REVISIONE_FRASI.md, dati/frasi.json versione 2 + personaggi/gianni e gesu/fumetti.json; 156 frasi verificate, 0 violazioni, 16 righe corrette; check_catalogo/quota_open non rieseguibili qui (regola 21), gia' fatti 91.8% > 50%; Stato aggiornato da @capo)

### BLOCCO H
| Stato | [~] | (@debugger, voce SCAMBIO 22 + voci 35/36 pubblicate da @capo: verifica statica OK — SPDX MIT 25/25 src, frameHash ogni 60, core senza SDL/orologio, 0 riferimenti Sega; build cmake, replay 2x stesso input, ms/frame/fps/temperatura NON eseguibili qui (regola 21) -> rimandate a Pi 400 con toolchain, Andrea; piano motore_nuovo/test/ARCHITETTURA_CHECK.md. CHECKPOINT Claude CLI 10:14: riportare a [~] finché non passa `clang++ -std=c++17 -fsyntax-only` sul nucleo, vedi SCAMBIO voce successiva. **[17 ott 14:00 Claude, build reale in cloud Linux g++/cmake]** `cmake --build build --target prova_gate` PASSA (dopo 2 correzioni vere: game.h `using gameplay::ShowLayout`, prova_gate.cpp include+using). lv_01/02/03: speed@60=33.8, replay 2x frameHash IDENTICO, 0,05 ms/frame, soak 30000 frame OK, ASan+UBSan puliti, 3 screenshot in out/debugger/. RESTA [~]: fps/temperatura su Pi 400 (Andrea) e parere di Andrea sulla guida (v90 NON OK). Dettagli: out/capo/PER_PROSSIMO_RUN.md, BUILD.md sez.5.)

### BLOCCO M
| Stato | [~] | (consegnato T2, SCAMBIO voce 15; variante A default, scelta Andrea in attesa; manifest tornati "bozza" dopo PNG B/N, voce 62/63, D8 elemento "tempo" da aggiungere a hud.json)

### BLOCCO P1
| Stato | [~] | (16 ott 17:55, Claude: punti 1-2 fatti; punto 3 segmenti da livello.json fatto da @coder voce 78; punto 5 wiring show fatto da @coder voce 87; punto 4 sprite veri in attesa di Andrea ANDREA/sprite_ritoccati/. Gate build cmake + 3 screenshot da fare sul Mac/Pi 400, regola 21). **[17 ott 14:00 Claude, build reale in cloud Linux g++/cmake]** Build OK, 3 screenshot strada (partenza/curva/fuoristrada) in out/debugger/. RESTA [~]: gli screenshot mostrano ancora rettangoli segnaposto al posto di sprite/cartelloni, nessun traffico, nessun personaggio/fumetto: vedi BLOCCO T1-RUN. |

### BLOCCO P1b
| Stato | [x] | (tutte le modifiche codice fatte da @coder voce 86/87: traffico disattivo trafficoAttivo_=false, offroad prima del clamp X_LIMIT, OFFROAD_LIMIT=700, guardia sponsorSpriteIdx_, niente shots/pcmr_M.bin -> playlist.json. Gate: prova headless/runner 10s sterzo sx/dx, auto visibile, nessun crash — NON eseguibile qui, regola 21; da fare su Pi 400/Mac con toolchain Andrea). **[17 ott 14:00 Claude, build reale in cloud Linux g++/cmake]** Gate headless PASSATO in cloud: sterzo sx (X min -165) e dx, auto visibile, offroad raggiunto al frame 476, 30000 frame senza crash. |

## 2. Sezioni RIEPILOGO / AGGIORNAMENTO TURNO originali

## RIEPILOGO TURNO 1 (ora 0 di 12) — @capo | 17 ott 2026
Tappa attiva: 1 (Gate: strada che scorre, auto accelera A, frena B, sterza sx/dx, 5 min senza crash). FEEDBACK_ANDREA.md: priorita al primo Gate C++ giocabile, squadra coder+debugger, nessun agente aggiunto se non indispensabile al Gate, niente tappe future.

Stati aggiornati:
- C [~] -> [x] (sintassi nucleo OK, build cmake/fps su Pi 400 in blocco H)
- H [~] invariato (build/fps/replay solo su Pi 400, regola 21)
- P1 [~] invariato (punti 1-2 fatti da Claude voce 66, punti 3-5 dipendono da P2/P3)
- P1b [ ] -> [~] (@coder: disattiva traffico segnaposto + collisioni, fixa clamp playerX)
- P2 [ ] invariato (non avviato: coder+debugger solo per il Gate)
- P3 [ ] invariato (non avviato: coder+debugger solo per il Gate)
- M [x] -> [~] (manifest tornati "bozza", D8 elemento "tempo" da aggiungere a hud.json)

Assegnati: P1b -> @coder; verifica sintassi nucleo -> @debugger.
D9 aperta in DECISIONI_ANDREA.md (build: solo cmake --build quando clang++ negato).

## RIEPILOGO TURNO 3 (ora 4 di 12) — @capo | 17 ott 2026
Tappa attiva: 1 (Gate: strada che scorre, accelera A, frena B, sterza sx/dx, 5 min senza crash). FEEDBACK_ANDREA: priorita al primo Gate C++ giocabile, squadra coder+debugger, niente tappe future; grafica provvisoria finché ANDREA/sprite_ritoccati/ vuoto.
Stati confermati: C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [x], M [~], J [ ], N [ ], MD0 [-], T5 [-].
Esiti: P1b codice su disco (3 rilievi reviewer sanati: guardia sponsorSpriteIdx_, niente path shots/, offroad prima del clamp, traffico disattivo e collisioni guardate - voce 87 SCAMBIO); P1 punti 1-2 OK (Pad::A accelera, Pad::B frena), punto 3 aggiunto (Game::init legge segmenti da livello.json), punto 5 wiring show fatto (voce 87), punto 4 sprite veri in attesa di Andrea. Build cmake e gate test NON eseguibili qui (regola 21): rimandati a Pi 400/Mac con toolchain (D9 DEFAULT: solo cmake --build). H: sintassi OK 34/34 file; build/fps/replay su Pi 400 (Andrea).
Deleghe chiuse: voce 86/87 (@coder P1b + 3 rilievi reviewer CHECKPOINT 2) [CHIUSA], voce 72 (@livelli P2) chiusa voce 75, voce 73 (@traduttore P3) chiusa voce 77, voce 74 (@debugger sintassi) chiusa voce 76.
D9 [APERTA]: DEFAULT solo cmake --build; Andrea non ha risposto, si prosegue col default.
Niente nuovi manifest/PNG/documenti finché P1 non ha 3 screenshot in out/coder/ (voce 70/77). check_catalogo OK; quota_open 100% > 50%. Nessun PER_CLAUDE.md (nessun doppio fallimento). FINITO non creabile (blocchi T1 non tutti [x]).

## RIEPILOGO TURNO 4 (ora 6 di 12) — @capo | 17 ott 2026
Tappa attiva: 1 (Gate: strada che scorre, accelera A, frena B, sterza sx/dx, 5 min senza crash). FEEDBACK_ANDREA: priorita al primo Gate C++ giocabile, squadra coder+debugger, niente tappe future; grafica provvisoria finché ANDREA/sprite_ritoccati/ vuoto.
Stati confermati: C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [x], M [~], J [ ], N [ ], MD0 [-], T5 [-].
Esiti: nulla di nuovo eseguibile in questo ambiente (regola 21: shell/compilatore negati; build e test reali su Pi 400 con toolchain Andrea; D9 DEFAULT solo cmake --build). P1b codice su disco, 3 rilievi reviewer sanati (voce 87 SCAMBIO); P1 punti 1-2-3 OK (Pad::A/B accelera/frena, SDL2_image, Game::init legge segmenti da livello.json), punti 4-5 in attesa (sprite veri = Andrea; show wiring fatto voce 87, non indispensabile al Gate per reviewer); H sintassi OK 34/34 file (voce 76/78), build cmake/replay/fps/temperatura in attesa Pi 400.
Deleghe voce 79 SCAMBIO.md: P1b verifica gate -> @coder (Pi 400/Mac); H test accettazione -> @debugger (Pi 400, Andrea).
Nessuna assemblea aperta (ASSEMBLEE_APERTE.md vuoto); BACHECA_RECENTE.md senza messaggi operativi (ignorati messaggi informali). DECISIONI_ANDREA.md D9 [CHIUSA] (Andrea ha risposto: DEFAULT solo cmake --build). Nessun PER_CLAUDE.md. FINITO non creabile (blocchi T1 non tutti [x]).

## RIEPILOGO TURNO 1 (ora 0 di 12) — @capo | 17 ott 2026 (snapshot storico, superato dagli aggiornamenti sotto)
Questo riepilogo registra la situazione iniziale del turno e non va usato come stato o delega corrente. La durata d'accettazione di 5 minuti qui riportata e' superata: fa fede la voce 93 di `SCAMBIO.md`, che richiede 10 minuti senza crash. Anche il vecchio rinvio a "reviewer voce 104" e' superato; i rilievi e il loro esito sono ricostruiti nelle voci 1135-1136 di `SCAMBIO.md`.
Tappa attiva: 1 (Gate FEEDBACK_ANDREA v90: strada che scorre con strisce di corsia che scorrono, accelera A con curva verso v.max, freno B/Down, rallentamento a gas rilasciato, sterzo dipendente dalla velocita', spinta esterno in curva, rallentamento fuori strada OFFROAD_LIMIT=700<X_LIMIT=900; prova headless target prova_gate 10s + 3 screenshot out/debugger/ partenza/curva/fuori strada + log velocita e posizione).
FEEDBACK_ANDREA v90: NON OK tappa 1 — manca engine di guida completo e proiezione strada corretta. CLAUDE voce 91: 4 bug in road.cpp/renderer.cpp (scala proiettiva fuori misura, strada ignora playerX, ordine disegno invertito, sprite con formula diversa). I rilievi successivi del reviewer sono riepilogati nella voce 1135 di `SCAMBIO.md`.
Stati: C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [~], M [~], J [ ], N [ ], MD0 [-], T5 [-].
Deleghe Turn 1:
- @coder (P1 [~], priorita' massima): (1) road.cpp project() — nuova proiezione pseudo-3D unica (depth=1/tan(FOV/2), camX=playerX*ROAD_WIDTH, s=depth/z, sx=W/2+s*(worldX-camX)*W/2, sy=H/2-s*(worldY-camY)*H/2, curve accumulate dx+=curve); (2) renderer.cpp drawRoad() — disegna segmenti dal lontano al vicino; (3) renderer.cpp drawSprites() — stessa proiezione della strada (base in sy del segmento, sx stessa scala); (4) road.cpp — rimuovere Rng rng(seed;) righe 18 e 57; (5) BUILD.md — rimuovere nlohmann/json da tabella riga 29 e comandi righe 70, 99; (6) CMakeLists.txt — aggiungere src/core/json_mini.h alla lista sglavinated_core. Solo motore_nuovo/src/ + CMakeLists.txt + motore_nuovo/docs/BUILD.md, SPDX MIT, zero alloc ciclo, niente asset Sega, determinismo preservato. Build cmake (D9 DEFAULT: solo cmake --build) e test su Pi 400/Mac, toolchain Andrea (regola 21) -> @debugger H.
- @debugger (H [~]): su Pi 400/Mac con toolchain Andrea (D9: solo `cmake --build`) — configurare e compilare core, `prova_gate` e runner; eseguire smoke test headless 10s previsto da FEEDBACK_ANDREA v90 e prova d'accettazione di 10 minuti senza crash (durata indicata nella voce 93 di `SCAMBIO.md`), replay 2x con stesso input e hash/60 identici, frame <=20ms (obiettivo <=12), 50 fps stabili, temperatura, 3 screenshot distinti e leggibili (partenza, curva, fuori strada) e log velocita'/posizione. Consultare `motore_nuovo/test/SUPPORTO_GATE_GAMEPLAY_T1.md` per i controlli di compilazione, coordinate/sprite e validazione; non dichiarare superato cio' che non e' stato eseguito. Non eseguibile qui (regola 21).
D9 [CHIUSA] (Andrea: cmake --build). check_catalogo OK; quota_open > 50%. Nessun PER_CLAUDE.md. FINITO non creabile (blocchi T1 non tutti [x]).

## AGGIORNAMENTO TURNO 1 (ora 0 di 12) — @capo | 17 ott 2026 (stato operativo aggiornato)
Tappa attiva: 1. Il test headless di 10s e' solo smoke test; la prova d'accettazione senza crash dura 10 minuti, come specificato nella voce 93 di `SCAMBIO.md`. Voci 93 e 1135 chiuse per la sola implementazione [CHIUSA] (SCAMBIO 1136): questo non chiude il gate. Il supporto statico in `motore_nuovo/test/SUPPORTO_GATE_GAMEPLAY_T1.md` elenca ancora interventi/verifiche da completare su renderer e `prova_gate` (include, coordinate/segmenti, scritture/screenshot e copertura casi gameplay); non considerarli risolti senza correzione e verifica. `BUILD.md` mantiene nlohmann/json, assegnato ad @architetto (voce 1137, non bloccante). Build, smoke test, accettazione, replay/performance e screenshot su Pi 400/Mac non sono attestati in questo ambiente (regola 21): @debugger, completare la voce 1138 e registrare separatamente ogni esito, senza dichiarare superate prove non eseguite. Stati correnti riportati da SCAMBIO 1136: C [x], P1 [~], P1b [~], H [~], P2 [x], P3 [x], M [~], J [ ], N [ ], MD0 [-], T5 [-]. Nessuna assemblea aperta (ASSEMBLEE_APERTE.md vuoto); BACHECA_RECENTE senza messaggi operativi. Nessun PER_CLAUDE.md. FINITO non creabile (blocchi T1 non tutti [x]).
