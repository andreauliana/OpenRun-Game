# RIEPILOGO @developer — Turno 1, 17/10/2026 12:32

Blocco: voce 93 (P1, priorita' massima) + rilievi reviewer CHECKPOINT 1 voce 1114.
Perimetro: motore_nuovo/src/ + motore_nuovo/CMakeLists.txt (regola 15). SPDX MIT. Zero alloc ciclo. Determinismo preservato.

Esito verifiche (lettura file, nessuna modifica shell possibile):
- road.cpp project() — proiezione pseudo-3D unica (depth=1/tan(FOV/2), camX=playerX, s=depth/z, sx=160+s*(x-camX)*160, sy=112-s*(y-camY)*112, curva dx+=curve accumulata). OK.
- road.cpp advance() — wrap playerX a +/-ROAD_HALF_WORLD=900; righe Rng rng(seed) rimosse. OK.
- renderer.cpp drawRoad() — segmenti disegnati lontano->vicino (painter, vicino copre lontano). OK.
- renderer.cpp drawSprites() — stessa proiezione della strada, base sy da segmento proiettato (worldZ>=sprite.worldZ), std::lower_bound O(log DRAW_DIST). OK.
- CMakeLists.txt — header SPDX MIT riga 1; src/core/json_mini.h nella lista sglavinated_core (riga 17). OK.
- player.cpp/h — Pad::A accelera, Pad::B frena; OFFROAD_LIMIT=700<X_LIMIT=900; offroad prima del clamp; accel asintotica a MAX_SPEED; sterzo proporzionale a velocita'. OK.

Non toccato: motore_nuovo/docs/BUILD.md (nlohmann/json righe 29/70/99) — fuori perimetro @developer/@developer-junior (regola 15), segnalato a @capo.

Non eseguibile qui (regola 21, shell negata): clang++ -fsyntax-only, build cmake, target prova_gate, replay 2x, ms/frame, fps -> @developer/@developer-junior su Pi 400/Mac con toolchain Andrea (D9 DEFAULT: solo cmake --build).

SCAMBIO.md: scrittura negata (permesso); esito in bacheche/coder/diario.md e RIASSUNTO.md.

Stati invariati: C [x], H [~], P1 [~], P1b [~]. P1 punto 4 (sprite veri) in attesa di ANDREA/sprite_ritoccati/; punto 5 (show) non indispensabile al Gate (P3 in attesa).
