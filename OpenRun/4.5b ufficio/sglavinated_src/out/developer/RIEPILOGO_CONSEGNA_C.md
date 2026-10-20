# out/coder — consegna BLOCCO C (@developer)

Stato: [x] (consegnato T2, SCAMBIO.md voce 17; rifinitura round 1 documentata in bacheche/coder/diario.md).
Data: 16 ott 2026. Agente: @developer. Cartelle: motore_nuovo/src/, motore_nuovo/CMakeLists.txt.

## Contenuto
- core/: types.h, frame_counter.h (contatore frame = unico orologio), rng.h (xorshift32 con seme), state.h, replay.h/cpp (registrazione/riproduzione input per test replay)
- road/: road_segment.h, road.h/cpp (strada a segmenti con curve e salite, proiezione su 320x224)
- render/: framebuffer.h/cpp (framebuffer software 320x224 a 32 bit), palette.h/cpp (palette 256 colori costruita a mano), sprite.h, renderer.h/cpp (disegno strada a trapezzi, sprite, auto del giocatore)
- input/pad.h (input logico tipo RetroPad), audio/mixer.h/cpp (mixer int16 stereo, suono motore sintetico)
- player/player.h/cpp (auto del giocatore: sterzo, accelerazione, offroad, marcia automatica)
- game/game.h/cpp (ciclo di gioco deterministico update/render, frameHash per i test di replay)
- runner/main.cpp (runner SDL2: finestra, input, audio, fixed timestep 50 fps; solo test desktop)
- libretro/sglavinated_libretro.cpp (stub documentato retro_init/load_game/run, target OFF di default; core non dipende da SDL)

## Vincoli rispettati
C++17, nucleo indipendente dalla piattaforma (zero SDL nel core), tempo = contatore di frame, RNG con seme fisso, nessun orologio di sistema nel core, nessun asset Sega, nessun codice di terzi (tecniche di riferimento Lou's Pseudo 3d Page e Jake Gordon JavaScript Racer solo come idee citate nei commenti), hash del frame esposto per i test di replay, target 50 fps (frame <= 20 ms, obiettivo <= 12 ms su Pi 400).

## Caveat
cmake/build e misura fps non eseguibili in questo ambiente (nessun compilatore, regola 21: non si insiste) -> rimandate a @debugger (blocco H) e alla Pi 400 con toolchain.

## Rifinitura round 1
Corretto errore di battitura negli include (`#include "...h>` -> `#include "...h"`) in 9 file di motore_nuovo/src/ cosicche' il core compili con cmake non appena la toolchain e' disponibile.

## Rifinitura round 2 (16 ott 2026, turn@developer)
- CMakeLists.txt: aggiunti src/gameplay/escalation.h, frasi.h/cpp, personaggi.h, show.h/cpp, ufo.h a sglavinated_core (interfaccia verso il nucleo, blocco D, scritto da @traduttore in src/gameplay/); find_package(SDL2 QUIET): il core si compila anche senza SDL2, il runner solo se SDL2 e' presente (match BUILD.md riga 78).
- game.h/game.cpp: ShowManager membro di Game; update() passa StatoGameplayInput (frame, stage=0, inGara=true, inputAttivo, offRoad), chiama crash() sul fronte di salita della collisione giocatore-traffico (placeholder deterministico), espone showDati()/showAttivo()/livelloEscalation()/ufoDati() per renderer/HUD/fx; showManager_.reset(seed_) in init() (stesso seed = stessi show).
- Pulizia: rimossi midCurve/midHill non usati (road.cpp) e #include <cstring> (game.cpp); renderer.cpp riutilizza projs_ membro (zero riallocazioni per frame dopo il primo).
- Verifica statica: 32/32 file src con SPDX MIT riga 1; include "..." OK (0 chiusi con `>`); ordine membri/inizializzatori OK; frameHash ogni 60 frame OK; core senza SDL e senza orologio di sistema; niente asset/tracciato/musica/codice Sega (grep solo commenti negativi).
- Gate "compila con cmake", misura fps e replay deterministico NON eseguibili qui (regola 21: nessun cmake/compilatore, shell negata, non si insiste) -> rimandati a @debugger (blocco H) e Pi 400 con toolchain. BLOCCO C resta [x] con caveat build; aggiornamento stato RICHIESTA.md spetta a @capo (scrittura .md di coordinamento negata a @developer: esito lasciato in out/coder/ e bacheche/coder/diario.md).
