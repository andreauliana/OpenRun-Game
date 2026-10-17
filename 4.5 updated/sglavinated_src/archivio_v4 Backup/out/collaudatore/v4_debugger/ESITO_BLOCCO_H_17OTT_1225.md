# Esito Blocco H — @debugger | 17/10/2026 12:25

## Stato
Blocco H [~] — invariato. Build cmake, replay deterministico, ms/frame, fps, temperatura NON eseguibili in questo ambiente (shell negata, regola 21). Rimandati a Pi 400 con toolchain Andrea (D9 DEFAULT: solo `cmake --build`).

## Prova eseguita (statica, sola lettura)
- Shell `clang++ -std=c++17 -fsyntax-only` e `cmake`: permission denied (regola 21, non si aggira).
- Lettura file sorgente per verifica rilievi reviewer voce 1113 (ancora [APERTA]):
  1. `motore_nuovo/CMakeLists.txt:1` — `# SPDX-License-Identifier: MIT` presente. OK.
  2. `motore_nuovo/CMakeLists.txt:17` — `src/core/json_mini.h` presente in `sglavinated_core`. OK.
  3. `motore_nuovo/src/road/road.cpp:87-88` — wrap `playerX` a `+/-ROAD_HALF_WORLD` (900), compatibile con clamp. OK.
  4. `motore_nuovo/src/render/renderer.cpp:115-118` — `std::lower_bound` su `projs_` ordinato per `worldZ`. OK.
  -> Tutti i 3 rilievi voce 1113 già sanati da @coder su disco. Nessun patch da applicare (regola 15: debugger non modifica codice di prodotto).

## Rilievo residuo
- `motore_nuovo/docs/BUILD.md:29` e comandi `:70`, `:99` elencano `nlohmann/json` come dipendenza, ma il codice usa il parser interno `core/json_mini.h` (`game.cpp:3`) e `CMakeLists.txt` non richiama nlohmann. Correzione proposta (diff, da applicare a @architetto, motore_nuovo/docs/):
  - rimuovere `nlohmann-json` da `brew install` riga 70 e `libnlohmann-json-dev` da `apt install` riga 99;
  - rimuovere riga JSON (`| JSON | nlohmann/json, un solo header | idem | MIT | [C] |`) dalla tabella riga 29.

## Artefatti
- `motore_nuovo/test/prova_gate.cpp`: presente (145 righe), core-only, nessun SDL, seed 20261015u, input deterministico (Pad::A accelera, Pad::B frena, Left/Right sterzo), loop 500 frame a 50 Hz, 3 screenshot PPM ai frame 1/100/300, CSV log con frame/velocita/playerX/playerZ/offroad/frameHash, frameHash ogni 60 frame. Target CMake `prova_gate` già definito (righe 109-113, attivo se il file esiste).
- `out/debugger/ESITO_BLOCCO_H_17OTT_1225.md`: questo file.
- `bacheche/debugger/diario.md`: aggiornato (5 righe).
- `bacheche/debugger/RIASSUNTO.md`: riscritto (max 12 righe).
- `SCAMBIO.md`: voce 1135 aggiunta in fondo.

## Prossimo passo
Su Pi 400 con toolchain Andrea (regole 9/21):
1. `cmake --build build --target prova_gate` (D9 DEFAULT)
2. Replay 2x stesso input -> frameHash/60 identico
3. ms/frame <= 20 ms (obiettivo <= 12), fps 50 stabili, temperatura
4. test prova_gate 10s headless + 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione
H passa a [x] solo dopo misure reali su Pi 400.
