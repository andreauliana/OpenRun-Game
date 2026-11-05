# Esito verifica statica prova_gate.cpp — @debugger — 17 ott 2026 12:58

## Stato
Blocco H [~] — build/replay/fps/temperatura NON eseguibili qui (regola 21: shell negata). Da eseguire su Pi 400 con toolchain Andrea (voce SCAMBIO 1138).

## File verificato
- `motore_nuovo/test/prova_gate.cpp` (145 righe, SPDX MIT riga 1)
- Target CMake: righe 109-115 di `motore_nuovo/CMakeLists.txt` (OK, si attiva se prova_gate.cpp esiste)

## Bug trovati (verifica statica, diff proposta non applicata)

### Bug 1 — Include path errato (build fallirebbe)
`prova_gate.cpp:9`: `#include "motore_nuovo/src/game/game.h"`
Con `target_include_directories(prova_gate PRIVATE src)` (CMakeLists:113), il percorso corretto è `"game/game.h"`. Il percorso assoluto relativo alla cartella del progetto risolverebbe in `src/motore_nuovo/src/game/game.h` (inesistente).
**Diff**: `rep("#include \"motore_nuovo/src/game/game.h\"", "#include \"game/game.h\"")`

### Bug 2 — Screenshot offroad mai salvato in-loop
`prova_gate.cpp:54`: `sc_frames[2] = -1`. Condizione loop `frame == sc_frames[screenshot_idx]` con `screenshot_idx=2` diventa `frame == -1`, mai vero per frame >= 0.
Quando offroad viene rilevato, `sc_frames[2]` non viene aggiornato a `frame`. Screenshot 2 non viene mai salvato durante il loop.
**Diff**: dopo `first_offroad_frame = frame;` aggiungere `sc_frames[2] = first_offroad_frame;`

### Bug 3 — Exit code sempre 0
`prova_gate.cpp:139-148`: `exit_ok` inizializzato a `true` e mai modificato. Il commento dice "0 se speed > 0 dopo i primi 60 frame" ma il controllo non è implementato.
**Diff**: dopo il loop, leggere `state().playerSpeed` al frame 60 e impostare `exit_ok = (speed > 0)`.

## Sintassi nucleo (già verificata OK)
34/34 file, `clang++ -std=c++17 -fsyntax-only -Isrc`, exit 0, 0 errori, 5 warning non bloccanti (voci 76/78/80).

## Gate da eseguire su Pi 400 (Andrea)
1. `cmake --build build --target prova_gate` (D9 DEFAULT)
2. Replay 2x stesso input: frameHash/60 identico
3. ms/frame <= 20ms (obiettivo <= 12ms), fps 50 stabili
4. prova_gate 10s headless + 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione
