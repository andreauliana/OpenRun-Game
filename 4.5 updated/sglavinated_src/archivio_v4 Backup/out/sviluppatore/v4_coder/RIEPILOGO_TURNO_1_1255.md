# Riepilogo turno @coder — 17/10/2026 12:55

## Blocchi miei (Stato [~])
- P1 [~] (PRIORITA' MASSIMA): codice completo su motore_nuovo/src/ + CMakeLists.txt. Gate: build cmake + 3 screenshot out/coder/ — non eseguibile qui (regola 21).
- P1b [~]: traffico segnaposto disattivo (trafficoAttivo_=false), offroad prima del clamp X_LIMIT, OFFROAD_LIMIT=700, guardia sponsorSpriteIdx_ vuota, clamp playerX fixato. Codice su disco. Gate: prova runner 10s sterzo sx/dx — non eseguibile qui (regola 21).

## Voce 1135 (3 rilievi reviewer) — tutte e 3 applicate su disco
1. CMakeLists.txt: SPDX header riga 1 + json_mini.h riga 17.
2. road.cpp: wrap playerX +/-ROAD_HALF_WORLD=900; Rng rimosso.
3. renderer.cpp: drawSprites lower_bound su worldZ; stessa proiezione strada.

## Dinamica guida (FEEDBACK_ANDREA v90) — implementata
- Accelerazione asintotica verso MAX_SPEED (Pad::A); freno (Pad::B); attrito gas rilasciato (speed*=0.98f).
- Sterzo dipendente dalla velocita'; spinta esterno in curva; rallentamento fuori strada (OFFROAD_LIMIT=700 < X_LIMIT=900).
- Auto sempre visibile (cx = 160 + x*0.15f, x clampato a +/-900 -> cx in [25,295] su 320px).

## Bloccante ambientale
- Shell/compilatore negati (regola 21): build cmake, prova_gate, fps/replay su Pi 400/Mac con toolchain Andrea (@debugger H).
- BUILD.md nlohmann/json: fuori perimetro @coder, segnalato a @architetto (voce 1137, non bloccante).

## Stati
C [x], H [~], P1 [~], P1b [~], P2 [x], P3 [x], M [~], J [ ], N [ ], MD0 [-], T5 [-].
