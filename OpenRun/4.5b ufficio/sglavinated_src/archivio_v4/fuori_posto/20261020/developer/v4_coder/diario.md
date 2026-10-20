### 17/10 12:55 — Turno 1 (ciclo corrente)
- Verificato su disco: voce 1135 (3 rilievi reviewer) tutte e 3 applicate — CMakeLists SPDX header riga 1 + json_mini.h riga 17; road.cpp wrap playerX +/-ROAD_HALF_WORLD=900 + Rng rimosso; renderer.cpp drawSprites lower_bound O(log DRAW_DIST) + stessa proiezione strada.
- P1 [~] e P1b [~]: codice completo su disco (dinamica OutRun, proiezione pseudo-3D, cartelloni da livello.json, show wiring, frameHash stato posizionale). Gate build cmake + prova runner + 3 screenshot out/coder/ non eseguibili qui (regola 21, shell/compilatore negati) -> rimandati a Pi 400/Mac (@debugger H).
- BUILD.md nlohmann/json: fuori perimetro @developer (regola 15), segnalato a @architetto (voce 1137, non bloccante).
- Stati invariati: C [x], H [~], P1 [~], P1b [~]. Voce 1135 [CHIUSA] per la parte @developer.

### 17/10 13:02 — Turno 1 (ciclo corrente)
- Verificato FEEDBACK_ANDREA v90 vs codice su disco: tutti i 6 requisiti tappa 1 implementati (accelerazione asintotica player.cpp:37, freno Pad::B player.cpp:16, attrito player.cpp:39, sterzo vs velocita' player.cpp:21, spinta esterno curva game.cpp:279, rallentamento offroad player.cpp:25/41 con OFFROAD_LIMIT=700<X_LIMIT=900).
- Verificato proiezione strada: road.cpp project() depth/z con dx+=curve accumulate; renderer.cpp drawRoad lontano->vicino + drawSprites stessa proiezione + lower_bound. Tutti i 3 rilievi reviewer voce 1113 sanati su disco.
- SCAMBIO.md: edit negato (strumento non consente scrittura su file di coordinamento); stato documentato in diario e RIASSUNTO.
- Stati invariati: C [x], H [~], P1 [~], P1b [~]. Gate cmake + prova_gate + replay + fps + screenshot in attesa su Pi 400/Mac (@debugger voce 1138).

### 17/10 13:15 — Turno 1 (ciclo corrente)
- Applicato FEEDBACK_ANDREA v90 (Tappa 1 NON OK): renderer.cpp drawSegmentTrapezoid() con strisce di corsia che scorrono (fase = fmod(playerZ, SEG_LEN)/SEG_LEN, pitch 0.5 segmento) per senso di velocita' visibile; drawPlayerCar() con auto CENTRATA (cx=160, offsetX*0.013f max ~12px invece di 0.15f fino a 135px che portava l'auto fuori strada sterzando); renderer.h firma drawSegmentTrapezoid aggiornata (parametro scrollPhase).
- Dinamica guida OutRun giA' su disco e verificata: accelerazione asintotica MAX*(1-e^(-t)), freno Pad::B/Down, attrito 0.98f, sterzo vs velocita', spinta esterno curva (game.cpp:279), offroad prima del clamp (OFFROAD_LIMIT=700 < X_LIMIT=900), rallentamento OFFROAD_SLOW 0.85.
- Proiezioni strada e sprite gia' unificate (voci 93/1135 chiuse su disco): road.cpp project() unica, drawRoad lontano->vicino, drawSprites stessa proiezione + lower_bound.
- SCAMBIO.md edit negato (strumento): voce 1140 documentata solo qui e in RIASSUNTO. Build cmake + gate NON eseguibili qui (regola 21) -> Pi 400/Mac (@developer/@developer-junior H).
- Stati invariati: C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [~], M [~].
[17/10 14:20] T1-RUN | [ ] | (nota di Claude) build OK con `sh tools/gate_t1.sh` (le righe sopra su "shell negata" e su fase/scrollPhase delle strisce sono superate: renderer a bande per indice segmento) | prossimo: guida offroad, colline, wrap sprite, sprite veri nel gate
[17/10 16:20] T1-RUN | [~] | sh tools/gate_t1.sh exit 0, GATE T1 TUTTO OK, 3 livelli, replay 2x IDENTICO, speed@60=33.76, ms/frame medio 0,072 max 0,199 (limite 20), offroad raggiunto f.476, avvolgimento e inattivita OK | prossimo: sprite veri nel gate (punto 4); Andrea/Claude incollano out/coder/VOCE_SCAMBIO_1146.md
