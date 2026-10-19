# Nota di Andrea per sviluppatori — frequenza di gioco (19 ott 2026)

Decisione di Andrea (registrata da Claude). Vale come voce di ANDREA_REVIEW: precedenza sulle altre fonti.
Destinatari: @builder-developer (autore), @developer-builder-junior (revisore). @capo apre un checkpoint dedicato.
Ordine: dopo R1, prima di R2.

## Decisione
1. **Tutte le misure del gioco in secondi, non in frame.** Tempo limite, durata show (oggi 260 frame), soglia di inattivita' (500 frame), timer di checkpoint, animazioni, accelerazioni e attriti devono usare `dt` in secondi.
2. **Frequenza logica predefinita: 60 Hz.** Oggi e' 50 Hz (`src/game/game.cpp:678` `dt = 1/50`, `game.cpp:831` `/50`, `UNITA_PER_KM` in `game.cpp:17`, `src/libretro/sglavinated_libretro.cpp:37` `fps = 50` e `:78` `RETRO_REGION_PAL`).
3. **Opzione 50 Hz** in configurazione (per TV a tubo PAL o RetroPie impostato in PAL), es. `dati/config.json` `"frequenza_hz": 60` oppure opzione del runner `--hz 50`. Il core libretro dichiara la frequenza scelta (60 -> NTSC, 50 -> PAL).

## Perche'
Mac, Pi 400 (solo HDMI), monitor e TV moderni, anche in Europa, lavorano a 60 Hz; anche Windows XP gira su monitor da 60 Hz in su. A 50 Hz logici su uno schermo a 60 il runner ripete un'immagine ogni 6: la strada che scorre fa un piccolo scatto continuo. Una possibile versione per Windows XP (dopo Mac e Pi) non cambia questa scelta.

## Gate
- `ctest` verde a 60 Hz e a 50 Hz; il giro guidato dura gli stessi secondi (+/- 1%) con le due frequenze.
- Replay 2x IDENTICO a ciascuna frequenza (i replay registrati a una frequenza si riproducono a quella).
- Il runner su Mac con vsync a 60 Hz: nessun frame ripetuto in 60 s di guida (contatore nel log del runner).

---

# Parte 2 — SDL, compilazione e compatibilita' multipiattaforma (Andrea, 19 ott)

Andrea: punto importante. Destinatari come sopra; puo' andare nello stesso checkpoint o in uno a parte.

## Stato verificato (main 162dc5d)
- Il core (`sglavinated_core`) e' C++17 puro, senza SDL e senza `std::filesystem`: bene, resta cosi'.
- Solo il runner usa SDL2 (+ SDL2_image opzionale per i PNG sponsor). Funzioni usate: video/renderer, texture streaming, `SDL_QueueAudio`, GameController/Joystick, `SDL_RenderSetIntegerScale`. **Versione minima reale: SDL 2.0.5** (per `SDL_RenderSetIntegerScale`).
- Il core libretro (per RetroPie) non usa SDL: dipende solo da `libretro.h`, che oggi manca, quindi `BUILD_LIBRETRO=ON` non compila.

## Da fare
1. **Include portabile:** in `src/runner/main.cpp` usare `#include <SDL.h>` (con il target `SDL2::SDL2`) invece di `<SDL2/SDL.h>`, che non funziona con alcune installazioni Windows e con il framework macOS.
2. **SDL2main solo se esiste:** in `CMakeLists.txt` collegare `SDL2::SDL2main` solo `if(TARGET SDL2::SDL2main)`; su alcune distribuzioni Linux il target non c'e' e CMake si ferma.
3. **Versione minima dichiarata:** `find_package(SDL2 2.0.5)` e un messaggio chiaro se manca. Scrivere in `docs/SVILUPPATORI.md` come installarla: macOS `brew install sdl2 sdl2_image`; Raspberry Pi OS / Debian `sudo apt install libsdl2-dev libsdl2-image-dev`; Windows con vcpkg o i pacchetti MinGW ufficiali di SDL.
4. **`libretro.h`:** aggiungere l'header ufficiale (licenza MIT) in `src/libretro/` cosi' il core RetroPie compila su Linux/Pi; la frequenza dichiarata segue la Parte 1.
5. **Avvio del runner in RetroPie (Andrea, 19 ott):** il runner deve partire dal menu di RetroPie/EmulationStation come voce del sistema **Ports**, senza desktop. Da preparare:
   - lo script `pi/retropie/OpenRun.sh` (da copiare in `~/RetroPie/roms/ports/`), che entra nella cartella del gioco, imposta `SDL_VIDEODRIVER=kmsdrm` e `SDL_AUDIODRIVER=alsa` e lancia `sglavinated_runner` a schermo intero;
   - l'uscita pulita con Select+Start (pad) ed Esc (tastiera), che torna a EmulationStation;
   - il pad configurato in RetroPie funzionante anche nel gioco (SDL GameController);
   - le istruzioni passo passo nella guida Pi (`guide/BUILD_E_AVVIO_RASPBERRY_PI.md` del progetto e `docs/SVILUPPATORI.md`): compilare sul Pi, copiare lo script, riavviare EmulationStation.
   Il core libretro (punto 4) resta la seconda strada, per giocarlo come "sistema" di RetroArch.
6. **Windows XP (piu' avanti, dopo Mac e Pi):** verificare l'ultima versione di SDL2 che supporta ancora XP e un compilatore C++17 che produca eseguibili per XP. Nel runner `std::filesystem` puo' essere un problema su XP e sui macOS vecchi (serve 10.15+): sostituirlo con `SDL_GetBasePath` + funzioni C standard, dato che il runner usa gia' SDL. Non bloccare il lavoro attuale per XP: solo non introdurre nuove dipendenze che lo escludano.

## Gate
- Build pulita del runner su macOS (Homebrew) e su Linux/Raspberry Pi OS con SDL di sistema; `ctest` verde.
- Con `BUILD_LIBRETRO=ON` il core compila su Linux.
- `docs/SVILUPPATORI.md` e la guida Pi riportano versioni e comandi di installazione.
- Su Pi 400 con RetroPie: OpenRun compare in Ports, parte, si guida col pad e torna a EmulationStation (prova di Andrea, dopo l'ok su Mac).
