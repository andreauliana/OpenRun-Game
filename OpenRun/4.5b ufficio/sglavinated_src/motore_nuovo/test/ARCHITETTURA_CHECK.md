# ARCHITETTURA_CHECK — output Blocco H (debugger)

Progetto: Outrun Sglavinated Edition v3 (open), motore nuovo (pista C).
Data: 16 ott 2026. Agente: @debugger. Stato: build bloccata (nessun toolchain in questo ambiente, regola 21), check eseguito per ispezione del codice consegnato da @coder (BLOCCO C, motore_nuovo/src/ + CMakeLists.txt, sola lettura).

Riferimenti: motore_nuovo/docs/ARCHITETTURA.md (vincoli duri), motore_nuovo/docs/BUILD.md (comandi e dipendenze), PIANO_TURNI.md (CHECK 2: misura fps se esiste il runner), VISIONE.md.

---

## 1. Esito build

| Elemento | Esito | Nota |
|---|---|---|
| `cmake -S . -B build && cmake --build build` | NON eseguibile qui | nessun cmake, nessun g++/c++/cc, shell negata (regola 21). Non insistere; segnalato in SCAMBIO.md. |
| Ambiente richiesto | cmake + compilatore C++17 + SDL2 (video/audio) + nlohmann/json (se usato) | v. BUILD.md §2 (§4 per i comandi Mac/Pi 400). |
| Comando atteso | `cd motore_nuovo && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j` | poi `./build/sglavinated_runner`; libretro con `-DBUILD_LIBRETRO=ON` (rinominata; v. voce 28/29, match BUILD.md riga 78). |

Gate "compila con cmake": rinviato a un ambiente con toolchain (Pi 400 con build-essential cmake libsdl2-dev, o Mac con brew). Non è un rifiuto del codice, è un limite dell'ambiente.

---

## 2. Difetti di build individuati per ispezione (da passare a @coder, non modificati)

Questi impedirebbero la compilazione o la produzione del runner anche appena disponibile un compilatore:

| # | File:riga | Difetto | Proposta (rep old -> new) |
|---|---|---|---|
| 1 | src/game/game.cpp:1 | `#include "game/game.h>` chiuso con `>` invece di `"` | `#include "game/game.h"` |
| 2 | src/game/game.h:2-12 | Tutti gli `#include "core/...h>` chiusi con `>` | chiudere con `"` (7 righe) |
| 3 | src/runner/main.cpp:1 | `#include "game/game.h>` chiuso con `>` invece di `"` | `#include "game/game.h"` |
| 4 | CMakeLists.txt:43 | `if(SDL2_FOUND)` senza `find_package(SDL2)` prima: il target `sglavinated_runner` viene sempre escluso (nessun errore, nessun eseguibile) | inserire `find_package(SDL2 REQUIRED)` prima dell'if |

Rep proposti (solo indicativi, applica @coder):
- rep('#include "game/game.h>', '#include "game/game.h"') in game.cpp:1 e runner/main.cpp:1
- rep('#include "core/frame_counter.h>', '#include "core/frame_counter.h"') e simili in game.h:2-12
- CMakeLists: aggiungere `find_package(SDL2 REQUIRED)` prima di `if(SDL2_FOUND)`

---

## 3. Determinismo e replay (CONFERMATI)

| Requisito (ARCHITETTURA.md §5) | File | Stato | Evidenza |
|---|---|---|---|
| Tempo = contatore di frame, nessun orologio di sistema | core/frame_counter.h | OK | `FrameClock { u64 frame; void tick() { ++frame; } }`; nessun `std::chrono`/`time()` nel core (grep: `SDL_GetTicks` solo in runner/main.cpp 96,138,142, solo pacing runner) |
| RNG deterministico con seme | core/rng.h | OK | `Rng(u32 seed)`, xorshift32, `state_ = (s == 0) ? 0x9E3779B9u : s` |
| Replay input registrazione/riproduzione | core/replay.h, core/replay.cpp | OK | `Mode { Off, Record, Playback }`, `record(u64)`, `next()`, `startRecord()` con `reserve(4096)`, `startPlayback(std::vector<u64>)` |
| Gioco determinista (dt fisso) | game/game.cpp | OK | `update()`: `dt = 1.0f/50.0f`, `clock_.tick()` a fine frame, `replay_.record(...)`, seme fisso nel runner `Game(20261015u)` |
| Hash frame per test replay | game/game.cpp:92-105 | OK parziale | `frameHash()` FNV-1a su framebuffer + stato; runner logga ogni 300 frame (linea 145), spec prevede ogni 60 frame → allineare a 60 per il test deterministico |
| Stesso input = stesso hash (2 esecuzioni) | — | da misurare | test: stesso file di input due volte -> stesso hash ogni 60 frame (Piano Pi 400, §4) |

---

## 4. Vincoli duri vs codice consegnato (ARCHITETTURA.md §2)

| Vincolo | Valore | Riscontro | Stato |
|---|---|---|---|
| C++17 | CMake | OK | CMakeLists.txt: CXX_STANDARD 17, REQUIRED, EXTENSIONS OFF |
| Render software 320x224 | render/framebuffer.h | OK | `WIDTH = 320`, `HEIGHT = 224`, `std::array<u32, WIDTH*HEIGHT>` |
| 50 Hz (PAL) | game/game.cpp | OK | `dt = 1.0f/50.0f` |
| Audio int16 stereo | audio/mixer.h/cpp, game.h | OK | `AUDIO_FRAME_SAMPLES = 44100/50`, `vector<i16>` stereo interleaved |
| Input RetroPad | input/pad.h | OK | presente (mappatura nel runner) |
| Zero allocazioni nel ciclo di gioco | game/game.cpp | NON OK (rischio) | `spawnTraffic()` fa `sprites_.clear()` + 6 `push_back` ogni frame; `replay_.record` fa `push_back` (può riallocare oltre `reserve(4096)`) |
| Core senza SDL | CMakeLists.txt + grep | OK | target `sglavinated_core` senza SDL; SDL solo runner e libretro |
| Niente asset/tracciati Sega | game/game.cpp:14-26 | OK | tracciato di prova generato da seme, commento "nessun tracciato Sega" |

---

## 5. Piano misura fps su Raspberry Pi 400 (output Blocco H)

Eseguire solo su Pi 400 con toolchain (build Release, nessun debug):

1. Build: `cd motore_nuovo && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)`.
2. Termico prima: `vcgencmd measure_temp`, `vcgencmd get_throttled` (annotare valori base).
3. Test deterministico: preparare un file di input (registro tastiera/joy per N frame); eseguire il runner due volte con lo stesso file (replay in playback); confrontare `frameHash` ogni 60 frame: devono essere identici su entrambe le esecuzioni.
4. Misura frame: harness di test (strumento esterno al core, non inserire orologi nel core) che misuri ms/frame di `update()+render()` su almeno 600 frame consecutivi; report: media, p99, massimo, conteo frame > 20 ms.
5. Termico durante/dopo: `vcgencmd measure_temp`, `vcgencmd get_throttled` a intervalli; nessun throttling, temperatura stabile.
6. Criteri di accettazione (VISIONE.md / BUILD.md §5): 50 fps stabili, frame <= 20 ms, obiettivo <= 12 ms, nessun picco ripetuto, zero allocazioni nel ciclo di gioco (verificare spawnTraffic e replay.record dopo le correzioni di §2).
7. Tutti i numeri (ms/frame, temp, throttled, hash match yes/no) vanno riportati in SCAMBIO.md da @debugger dopo la prima esecuzione su Pi 400.

---

## 6. ARCHITETTURA_CHECK.md

Questo file è il risultato del Blocco H e va conservato in `motore_nuovo/test/ARCHITETTURA_CHECK.md`. Non sostituisce la review di @reviewer (Blocco I) né la misura effettiva su Pi 400.

---
Fatto da @debugger, sola ispezione, nessun file di @coder modificato.
