# P1b + P1 punti 1-3 — esito @developer (17 ott 2026)

## Comandi eseguiti
Nessun comando shell: shell negata in questa sessione ("Permission denied: shell", regola 21).
Solo modifiche file (tool read/edit) su `motore_nuovo/src/`.

## P1b (punto 1) — verificato gia' presente, nessuna modifica
- `trafficoAttivo_ = false` → `src/game/game.h:92` OK
- collisioni solo se `trafficoAttivo_` → `src/game/game.cpp` update OK
- `offroad` calcolato PRIMA del clamp X_LIMIT → `src/player/player.cpp` update OK
- guardia `if (sponsorSpriteIdx_.empty()) return false;` → `src/game/game.cpp` leggiCartelloniDaFile OK
- niente `radioM.path = "shots/..."` → `src/game/game.cpp` init (radioM senza path, loadRadio ritorna 0) OK

## P1 punti 1-2 (input) — verificato gia' presente, nessuna modifica
- `src/player/player.cpp` input(): accel `Pad::Up | Pad::A`, freno `Pad::Down | Pad::B` OK

## P1 punto 3 (curve da dati) — AGGIUNTO
- `src/game/game.h`: dichiarato `leggiSegmentiDaFile`.
- `src/game/game.cpp`: `init` legge campo `segmenti` ({curva, collina, lunghezza}) da
  `motore_nuovo/dati/livelli/<id>/livello.json` via `leggiSegmentiDaFile`; pattern ripetuto
  a >= 2*DRAW_DIST segmenti; fallback hardcoded se file/campo mancante/illeggibile.
- `src/road/road.h`: `struct SegmentDesc` + overload `build(const std::vector<SegmentDesc>&, u32)`.
- `src/road/road.cpp`: implementato build con lunghezza variabile; proiezione: startIdx per
  lunghezze cumulative + z con wrap (tracciato circolare).
- `src/road/road_segment.h`: campo `length`.
- Determinismo: stesso file = stesso tracciato; fallback hardcoded deterministico (no RNG).
- SPDX MIT su tutti i file toccati (riga 1, gia' presente).

## Build (punto 4) — NON eseguita
Shell negata (regola 21). `cmake --build` impossibile in questa sessione.

## Gate P1b (punto 5) — NON eseguito
Shell negata: impossibile provare headless/runner o salvare screenshot in `out/coder/`.

## Coda aperta
Build + gate P1b da eseguire dove shell e' consentita (voce 76: `clang++` in allowlist per
@debugger; `cmake --build` su Pi 400 o Mac con toolchain, regola 9/21).
Voce in SCAMBIO.md a cura di @capo (regola 15: @developer scrive solo src/, CMakeLists.txt, out/coder/).
