# Proposte del capo (sostituisci questo file a ogni turno)
Andrea e Claude leggono qui e aggiornano RICHIESTA.md e SCAMBIO.md.

## Stati proposti
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [~] in corso, [x] proposto (condizionato v91 Andrea) | `sh tools/gate_t1.sh soak` exit 0, out/collaudatore/gate_t1.log "GATE T1: TUTTO OK": speed@60=161.79 > 0 frame 60, replay 2x IDENTICO 3 livelli, ms/frame medio 0.36 max 0.651 <= 20, offroad raggiunto f278/f167/f285, soak 30000 OK; screenshot partenza/curva/fuoristrada per livello in out/collaudatore/lv_0*. Promozione [x] solo Andrea/Claude dopo v91 Andrea (punto 8 ANDREA_REVIEW.md, D1 DEFAULT sì).

P1 | [~] | @sviluppatore punti 1-3 e 5 fatti (Pad::A accelera, B/Down freno, coast 0.995, sterzo vs velocita', segmenti da livello.json, build cmake OK, prova_gate compilato). Mancante: 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto); shell `Permission denied: shell` 6x (anche `date`), binary build_gate/prova_gate pronto. Promozione [x] solo Andrea/Claude (D2 DEFAULT sì, bloccato shell).

H | [~] | attende Andrea punto 8 T1-RUN: provare sglavinated_runner su Mac (SDL2), scrivere v91 in ANDREA_REVIEW.md (guida OK o ritocco un parametro alla volta), fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled) — non eseguibile qui (regole 9/21). Chiude quando Andrea scrive v91 + misure (D3 DEFAULT sì).

J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti sullo stesso problema o fine blocco importante; non serve. PER_CLAUDE.md solo se richiesto.

M | [~] | HUD/font (Tappa 3) — non avviato, feedback Andrea 17 ott: niente tappe successive.

G1 | [ ] | catalogo grafico @grafico (assets_originali/CATALOGO_GRAFICO.json) — solo su richiesta Andrea (D4 DEFAULT no, fuori scope Tappa 1); check_catalogo_grafico.py exit 0 e "PNG non catalogati: 0".

MD0 | [-] | sospeso (scelta Andrea 17 ott: prima il C++).

T5 | [-] | tappa 5, non avviata.

## Deleghe del turno

@sviluppatore (motore_nuovo/src/, motore_nuovo/CMakeLists.txt, motore_nuovo/dati/livelli/): presidia il codice su disco (player/, road/, render/, game/). Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, licenza SPDX MIT su file nuovi. Non avviare P2/P3/T5/MD0 (fuori scope tappa 1, feedback Andrea 17 ott). Se un check automatico torna rosso, il passo torna a te (regola 5). Nota: shell negata (`Permission denied: shell`); comandi permessi `sh tools/gate_t1.sh` [soak] e `cmake --build build --target prova_gate` (D9); scrittura file via write tool. Riprovare `sh tools/gate_t1.sh` per rigenerare il gate; i 3 screenshot P1 restano bloccati finche' la shell non e' sbloccata.

@collaudatore (out/collaudatore/, motore_nuovo/test/): gate T1 gia' verde (`sh tools/gate_t1.sh soak` exit 0, log out/collaudatore/gate_t1.log, screenshot out/collaudatore/lv_0*/). Consegnare VOCE_SCAMBIO con esito e numeri (speed, replay 2x, ms/frame, offroad). Se richiesto da @sviluppatore dopo modifiche: rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh (max 3 rilievi azionabili, sola lettura, out/REVIEW/). Lanciare indice_assets.py e quota_open.py (>= 50%) se non fatti dopo l'ultima generazione asset. Non avviare nuove tappe.

Andrea (punto 8 T1-RUN): v91 su ANDREA_REVIEW.md (runner Mac SDL2, guida OK o ritocco un parametro alla volta) + fps/temperatura Pi 400. Rispondere D1-D4 in ANDREA_REVIEW.md (DEFAULT: D1 sì, D2 sì, D3 sì, D4 no). Questo chiude H e permette le promozioni T1-RUN/P1 a [x] (solo Andrea/Claude).

@grafico / @ricercatore: non attivati in questo turno (nessuna richiesta Andrea, G1 non richiesta, D4 DEFAULT no).

Prossimo: v91 Andrea + risposte D1-D4 -> 3 screenshot P1 in out/sviluppatore/ -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e tappa 2. Nessun PER_CLAUDE.md, nessun FINITO (P1 e H non [x]).
