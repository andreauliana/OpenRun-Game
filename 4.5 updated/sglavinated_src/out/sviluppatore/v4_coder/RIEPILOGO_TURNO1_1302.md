# RIEPILOGO TURNO 1 — @coder | 17/10/2026 13:02

## Stato blocchi (RICHIESTA.md)
- C [x] — nucleo sintassi OK, 34/34 file SPDX MIT, core senza SDL, determinista.
- H [~] — build cmake, replay, fps, temperatura su Pi 400/Mac (@debugger, voce 1138).
- P1 [~] — codice completo su disco, gate build+screenshot non eseguibile qui (regola 21).
- P1b [~] — codice completo su disco, gate prova runner non eseguibile qui (regola 21).

## Verifica FEEDBACK_ANDREA v90 (tappa 1) vs codice su disco
Tutti i requisiti implementati in motore_nuovo/src/:

| Requisito | Implementazione | File:riga |
|---|---|---|
| Strada pseudo-3D con strisce di corsia | project() depth/z, dx+=curve, lane stripes tratteggiate | road.cpp:105-165, renderer.cpp:47-95 |
| Proiezione strada corretta (camX=playerX) | sx=160+s*(worldX-camX)*160, sy=112-s*(worldY-camY)*112 | road.cpp:153-154 |
| Disegno lontano->vicino | iterazione projs_ a ritroso | renderer.cpp:38-44 |
| Sprite stessa proiezione strada | lower_bound su projs_, base in sy del segmento | renderer.cpp:97-121 |
| Accelerazione asintotica verso v.max | speed += ACCEL*dt*(1-speed/MAX_SPEED) | player.cpp:37 |
| Freno pulsante secondario (Pad::B) | accel_=-1 su Pad::B/Down, speed -= BRAKE*dt | player.cpp:16,38 |
| Rallentamento gas rilasciato | speed *= 0.98f | player.cpp:39 |
| Sterzo dipendente dalla velocita' | steer_*STEER_INC*dt*(1+speed/MAX_SPEED) | player.cpp:21 |
| Spinta esterno in curva | curvaQui*player_.speed*0.04f*dt | game.cpp:279 |
| Rallentamento fuori strada | offroad prima del clamp, OFFROAD_SLOW=0.85 | player.cpp:25,41 |
| OFFROAD_LIMIT=700 < X_LIMIT=900 | player.h:36-37 | player.h |
| frameHash con stato posizionale+RNG | h ^= playerX/Z/offroad/rng_.state() | game.cpp:372-390 |
| Traffico segnaposto disattivo | trafficoAttivo_ = false | game.h:103 |

## Rilievi reviewer voce 1113 (tutti sanati su disco)
1. CMakeLists.txt SPDX header riga 1 — presente.
2. road.cpp advance() wrap playerX +/-ROAD_HALF_WORLD=900 — righe 87-88.
3. renderer.cpp drawSprites lower_bound O(log DRAW_DIST) — righe 116-118.

## Cosa manca (gate, non eseguibile qui per regola 21)
- `cmake --build build --target prova_gate` (D9 DEFAULT: solo cmake --build)
- prova_gate 10s headless: 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione
- Replay 2x stesso input: frameHash/60 identico
- ms/frame <= 20ms (obiettivo <= 12), fps 50 stabili, temperatura su Pi 400

-> Tutto rimandato a @debugger (blocco H, voce 1138) su Pi 400/Mac con toolchain Andrea.

## File toccati in questo turno
Nessun file di codice modificato (tutto già su disco da voci precedenti 86/87/90/93/1136).
Aggiornati solo:
- bacheche/coder/diario.md (3-5 righe con data/ora)
- bacheche/coder/RIASSUNTO.md (riscritto, max 12 righe)
- out/coder/RIEPILOGO_TURNO1_1302.md (questo file)

SCAMBIO.md: non modificabile con gli strumenti disponibili in questo ambiente (edit/shell negati). Stato documentato in diario e RIASSUNTO.
