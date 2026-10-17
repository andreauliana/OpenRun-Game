# Riepilogo @coder — Turno 1, 17/10/2026 13:15

## Blocci su cui ho lavorato
- P1 [~] — PRIORITÀ MASSIMA (Andrea 16 ott): primo livello giocabile
- P1b [~] — niente collisioni fantasma

## Modifiche apportate (FEEDBACK_ANDREA v90 — Tappa 1 NON OK)

### motore_nuovo/src/render/renderer.cpp
1. **drawSegmentTrapezoid()**: strisce di corsia centrali ora SCORRONO verso il giocatore. Fase = fmod(playerZ, SEG_LEN)/SEG_LEN, pitch = 0.5 segmento. Dà il senso di velocità visibile sulla strada (FEEDBACK: "strisce di corsia che scorrono, così la velocità si vede").
2. **drawPlayerCar()**: auto CENTRATA a cx=160 con minimo accenno sterzo (offsetX * 0.013f, max ~12 px). Prima si spostava di offsetX * 0.15f (fino a 135 px) mentre la strada si spostava con camX = playerX: l'auto finiva fuori carreggiata sterzando (FEEDBACK: "l'auto non si capisce bene cosa sta facendo").

### motore_nuovo/src/render/renderer.h
3. Firma di drawSegmentTrapezoid aggiornata: aggiunto parametro `f32 scrollPhase = 0.0f`.

### Verificato su disco (nessuna modifica necessaria)
- player.cpp/player.h: dinamica guida OutRun completa — accelerazione asintotica MAX*(1-e^(-t)), freno Pad::B/Down, attrito 0.98f (gas rilasciato), sterzo proporzionale a velocità, spinta esterno curva (game.cpp:279), offroad calcolato PRIMA del clamp (OFFROAD_LIMIT=700 < X_LIMIT=900), rallentamento OFFROAD_SLOW 0.85.
- road.cpp/road.h: proiezione pseudo-3D unica (depth = 1/tan(FOV/2), camX = playerX, s = depth/z, dx += curve accumulate), wrap playerX a ±ROAD_HALF_WORLD, Rng rimosso.
- renderer.cpp drawRoad(): disegna segmenti dal lontano al vicino (painter). drawSprites(): stessa proiezione della strada + lower_bound O(log DRAW_DIST).
- game.cpp: segmenti da livello.json, traffico segnaposto disattivo (trafficoAttivo_ = false), frameHash con stato posizionale + RNG.

## Blocker
- Build cmake, gate prova_gate (10s headless, 3 screenshot, log velocità/posizione), replay deterministico, ms/frame, fps: NON eseguibili qui (regola 21 — shell/compilatore negati). Da eseguire su Pi 400/Mac con toolchain Andrea (D9 DEFAULT: solo `cmake --build`). Assegnato a @debugger (blocco H, voce 1138).
- SCAMBIO.md: scrittura negata dallo strumento; voce 1140 documentata qui, in diario.md e in RIASSUNTO.md.

## Stati invariati
C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [~], M [~], J [ ], N [ ], MD0 [-], T5 [-].

## File toccati
- motore_nuovo/src/render/renderer.h (SPDX MIT)
- motore_nuovo/src/render/renderer.cpp (SPDX MIT)
Nessun file cancellato. Nessun tocco di `../`.
