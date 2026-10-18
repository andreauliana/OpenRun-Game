# Proposte del capo (sostituisci questo file a ogni turno)
Andrea e Claude leggono qui e aggiornano RICHIESTA.md e SCAMBIO.md.

## Stati proposti
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [x] proposto | `sh tools/gate_t1.sh soak` exit 0 (17 ott): out/collaudatore/gate_t1.log "GATE T1: TUTTO OK" — speed@60=161.79 > 0 frame 60, replay 2x IDENTICO su 3 livelli, ms/frame medio 0.36 max 0.651 <= 20, offroad raggiunto (f278/f167/f285), soak 30000 OK; screenshot partenza/curva/fuoristrada per livello in out/collaudatore/lv_0*. Promozione a [x] solo Andrea/Claude dopo v91 Andrea (punto 8, FEEDBACK_ANDREA.md).

P1 | [~] | @sviluppatore: punti 1-3 e 5 fatti (player accel A/Up decrescente vs MAX_SPEED 250, freno B/Down BRAKE 120, coast 0.995, sterzo vs velocita', offroad 0.99, ribaltamento, drawHud; build cmake OK, prova_gate compilato in build_gate/). Gate P1 mancante: 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto) — shell negata 6x `Permission denied: shell`. Prova: build cmake ok + 3 screenshot out/sviluppatore/. Promozione a [x] solo Andrea/Claude.

H | [~] | attende Andrea punto 8 (T1-RUN): provare sglavinated_runner su Mac (SDL2), scrivere v91 in FEEDBACK_ANDREA.md (guida OK o ritocco un parametro alla volta), misurare fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled) — non eseguibile qui (regole 9/21 AGENTS.md). Chiude quando Andrea scrive v91 e riporta le misure.

J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti sullo stesso problema o fine blocco importante; non ancora. PER_CLAUDE.md solo se serve.

M | [~] | @grafico HUD/font — Tappa 3, non indispensabile al Gate T1; nessuna nuova funzione avviata (regola feedback: niente tappe successive).

G1 | [ ] | @grafico catalogo grafico assets_originali/CATALOGO_GRAFICO.json — solo su richiesta Andrea (non richiesta, D4 DEFAULT no); check_catalogo_grafico.py exit 0 e "PNG non catalogati: 0".

## Deleghe del turno
@sviluppatore (motore_nuovo/src/, motore_nuovo/CMakeLists.txt, motore_nuovo/dati/): presidia il codice su disco (player/, road/, render/, game/). Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio), core senza SDL, licenza SPDX MIT su file nuovi. Riprovare shell e produrre 3 screenshot in out/sviluppatore/ con `./build_gate/prova_gate out/sviluppatore/ lv_01_neon/lv_02_atomico/lv_03_foglie smoke` (outDir argv[1], nessun cambio codice) per chiudere P1. Non avviare P2/P3/T5/MD0 (fuori scope tappa 1, feedback Andrea 17 ott). Se un check automatico torna rosso, il passo torna a te (regola 5).

@collaudatore (out/collaudatore/, motore_nuovo/test/): gate T1 gia' verde (`sh tools/gate_t1.sh soak` exit 0, log out/collaudatore/gate_t1.log, screenshot out/collaudatore/lv_0*/). Consegnare VOCE_SCAMBIO con esito e numeri (speed, replay, ms/frame, offroad). Se richiesto da @sviluppatore dopo modifiche: rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh (max 3 rilievi azionabili, sola lettura, out/REVIEW/). Lanciare indice_assets.py e quota_open.py (>= 50%) se non gia' fatti dopo l'ultima generazione asset. Non avviare nuove tappe.

Andrea (punto 8 T1-RUN, out/ e ../ANDREA/): v91 su FEEDBACK_ANDREA.md (runner Mac SDL2, guida OK o da ritoccare un parametro alla volta) + fps/temperatura Pi 400. Questo chiude H e permette la promozione T1-RUN/P1 a [x] (solo Andrea/Claude).

@grafico / @ricercatore: non attivati in questo turno (nessuna richiesta Andrea, G1 non richiesta).

Prossimo: v91 Andrea + 3 screenshot out/sviluppatore/ -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e tappa 2.
