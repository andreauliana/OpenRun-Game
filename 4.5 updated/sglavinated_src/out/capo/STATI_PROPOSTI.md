# Proposte del capo (sostituisci questo file a ogni turno)
Andrea e Claude leggono qui e aggiornano RICHIESTA.md e SCAMBIO.md.

## Stati proposti
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [~] | @sviluppatore punti 1-4 codice su disco (player.cpp dinamica OutRun: accel A/Up, freno B/Down, offroad OFFROAD_LIMIT=700<X_LIMIT=900, sterzo vs velocita', attrito COAST; road.cpp proiezione colline/curva corretta, camera segue auto; renderer.cpp sprite veri zRel modulo totalLen, disegno lontano->vicino, strisce di corsia; 21 .sglraw sponsor in motore_nuovo/dati/sprite_raw/sponsor/; prova_gate.cpp casi smoke/soak/avvolgimento/inattivita/sterzo/accelerazione/curve/crash con caricaSponsor prima di init). @collaudatore punti 4b (setSponsorSprites prima di init, gia' presente), 5 (casi avvolgimento playerZ oltre totalLen_ e inattivita 10s -> show/fumetto, gia' presenti), 6 (indice_assets.py + quota_open.py >= 50%), 7 (review renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh). Prova: `sh tools/gate_t1.sh soak` exit 0, replay 2x IDENTICO, out/collaudatore/gate_t1.log (speed>0 frame 60, ms/frame <= 20, offroad raggiungibile), 3 screenshot out/collaudatore/<livello>/ (partenza, curva, fuori strada) + log velocita/posizione. Gate T1 si chiude con exit 0 del gate + Andrea v91 (FEEDBACK_ANDREA.md) su runner Mac e fps/temp Pi 400.

P1 | [~] | @sviluppatore punto 4 (sprite veri nel gate): 21 .sglraw consegnati + caricaSponsor in prova_gate.cpp + setSponsorSprites in Game. Punti 1-3 e 5 gia' fatti su disco. Prova: build cmake ok + gate smoke exit 0 (da @collaudatore). Promuove a [x] solo Andrea/Claude dopo gate verde.

H | [~] | attende Andrea punto 8 (v91 su FEEDBACK_ANDREA.md, runner SDL2 su Mac, fps/temperatura Pi 400 vcgencmd). Chiude quando Andrea scrive v91 e riporta misure.

J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti sullo stesso problema o fine blocco importante; non ancora.

M | [~] | @grafico HUD/font — Tappa 3, non indispensabile al Gate T1; bloccato fino a screenshot out/coder/ (voce 70/78); nessuna nuova funzione avviata.

G1 | [ ] | @grafico catalogo grafico assets_originali/CATALOGO_GRAFICO.json — su richiesta di Andrea (non ancora richiesta); check_catalogo_grafico.py exit 0 e "PNG non catalogati: 0".

## Deleghe del turno
@sviluppatore: T1-RUN/P1 — presidia il codice su disco (motore_nuovo/src/player/, motore_nuovo/src/road/, motore_nuovo/src/render/, motore_nuovo/src/game/, motore_nuovo/test/prova_gate.cpp, motore_nuovo/CMakeLists.txt). Zero alloc nel ciclo di gioco, determinismo (frame, RNG seed, nessun orologio), core senza SDL, licenza SPDX MIT. Se il gate (eseguito da @collaudatore) torna rosso, il passo torna a te con il log (regola 5). Non avviare P2/P3/T5 (fuori scope tappa 1).

@collaudatore: T1-RUN punti 4b/5/6/7 — (4b) conferma setSponsorSprites chiamato prima di init() in prova_gate.cpp; (5) conferma casi avvolgimento playerZ oltre totalLen_ e inattivita 10s -> show/fumetto visibile in screenshot; esegui `sh tools/gate_t1.sh soak` (un solo comando, senza pipe/2>&1/; ) dalla radice sglavinated_src; report con exit code e numeri da out/collaudatore/gate_t1.log (speed>0 frame 60, replay 2x IDENTICO, ms/frame <= 20, offroad raggiungibile); salva 3 screenshot out/collaudatore/<livello>/ (partenza, curva, fuori strada) + log velocita e posizione; (6) lancia python3 motore_nuovo/strumenti_py/indice_assets.py e quota_open.py (>= 50%); (7) dopo modifiche @sviluppatore, rivedi SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh (max 3 rilievi azionabili, sola lettura, out/REVIEW/). Dopo ogni modifica coder: `sh tools/gate_t1.sh soak`.

Andrea (punto 8 T1-RUN): provare sglavinated_runner sul Mac (SDL2), scrivere v91 in FEEDBACK_ANDREA.md (guida OK o da ritoccare un parametro alla volta), misurare fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled) — non eseguibile qui (regole 9/21). Questo chiude H e permette di promuovere T1-RUN/P1.

@grafico / @ricercatore: non attivati in questo turno (nessuna richiesta Andrea, G1 non richiesta).

Prossimo: @collaudatore esegue gate soak -> exit 0 -> v91 Andrea -> T1-RUN/P1/H a [x] (solo Andrea/Claude).
