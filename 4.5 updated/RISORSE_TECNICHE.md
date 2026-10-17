# Risorse tecniche per il motore di guida (pista C)

Stato: [V] = letto in questa sessione · [C] = dalla mia conoscenza, da controllare prima di usare.

## Tecnica esperienza di guida pseudo-3D ()
| Risorsa | Contenuto | Licenza | Stato |
|---|---|---|---|
| Lou's Pseudo 3d Page https://www.extentofthejam.com/pseudo/ | proiezione, Z-map, curve a segmenti, colline, sprite, dati del tracciato; cita anche OutRun | solo "(C) 2013", nessuna licenza dichiarata | [V] |
| Jake Gordon, JavaScript Racer https://jakesgordon.com/writing/javascript-racer/ (v1 rettilinei, v2 curve, v3 colline, v4 finale) | tutorial passo passo | [V] |
| Codice: https://github.com/jakesgordon/javascript-racer/ | sorgente JS | [C] |

## Riferimenti da NON usare come base
| Risorsa | Motivo |
|---|---|
| Cannonball https://github.com/djyt/cannonball | licenza non commerciale, richiede ROM, codice riscritto di referenza | [V] |
| ZgzInfinity/OutRun https://github.com/ZgzInfinity/OutRun | GPL-3.0 (C++14 + SFML), ma provenienza asset non dichiarata: solo scorciatoia B dopo controllo | [V] |

## Emulatore: core libretro [V]
https://docs.libretro.com/development/cores/developing-cores/ — un core e' una libreria (`<nome>_libretro.so`) con funzioni come `retro_init`, `retro_load_game`, `retro_run` (un frame per chiamata, video una volta per chiamata), audio int16 stereo, input RetroPad; frequenze fisse (niente sleep/timer nel core); i core sono a istanza singola e non thread-safe. Installazione di un core custom su RetroPie: [C], da verificare.

## Strumenti di build
| Cosa | Mac | Raspberry Pi OS / RetroPie | Stato |
|---|---|---|---|
| Compilatore + CMake | `xcode-select --install`, `brew install cmake` | `sudo apt install build-essential cmake` | [C] |
| SDL2 (finestra, input, grafica) | `brew install sdl2 sdl2_image sdl2_mixer` | `sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev` | [C] |
| Compilare SDL2 da sorgente su Pi (guida 2015, vecchia) | | https://solarianprogrammer.com/2015/01/22/raspberry-pi-raspbian-getting-started-sdl-2/ | [V] |
| Musica tracker .xm | libxmp (LGPL) o SDL_mixer con supporto moduli | idem | [C] |
| JSON per i dati | nlohmann/json (MIT, un solo header) | idem | [C] |
| Test su Pi | `vcgencmd measure_temp`, `vcgencmd get_throttled` | | [C] |

Prima di installare un pacchetto, controlla la licenza (l'architetto la riporta in `motore_nuovo/docs/LICENZE.md`).
