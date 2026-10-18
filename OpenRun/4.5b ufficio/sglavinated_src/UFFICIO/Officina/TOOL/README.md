# TOOL — Officina

Destinazione degli strumenti di sviluppo. Gli script esistenti restano in `../../../tools/` finché build e automazioni non vengono migrate insieme; non duplicarli qui.

## Toolchain e prove attive

La configurazione CMake attiva è `../../../motore_nuovo/CMakeLists.txt`; tutti i comandi vanno lanciati dalla radice `sglavinated_src/`.

| Componente/strumento | Comando o target | Funzione |
|---|---|---|
| Core C++17 | `cmake --build UFFICIO/Officina/build` | `sglavinated_core`, libreria indipendente da SDL |
| Runner desktop + decoder immagini SDL2_image | `cmake --build UFFICIO/Officina/build --target sglavinated_runner` | Toolchain SDL2 per test visivi e lavoro sugli sprite; SDL2_image abilita la decodifica PNG nel runner |
| Core libretro stub | Configurare con `-DBUILD_LIBRETRO=ON`, poi `cmake --build UFFICIO/Officina/build --target sglavinated_libretro` | Modulo opzionale; richiede che l'opzione sia attivata |
| Prove CTest | `sh tools/run_tests.sh` | Configura Release, compila `prova_gate` e lancia i sette test registrati quando `motore_nuovo/test/prova_gate.cpp` è presente |
| Gate integrato | `sh tools/gate_t1.sh` | Configura/compila in Release e verifica i tre livelli, replay, accelerazione, crash, sterzo e curve; log in `UFFICIO/Officina/Test area/gate_t1.log` |
| Soak gate | `sh tools/gate_t1.sh soak` | Aggiunge la prova da 30.000 frame |
| Inizializzazione SDL | `sh tools/check_sdl_init.sh` | Controlla `SDL_Init` del runner già compilato |

## SDL2_image — tool immagini per i developer

SDL2_image è una libreria di sviluppo/runtime del runner desktop: permette ai developer
di decodificare PNG durante il lavoro sugli sprite senza legare il core a SDL. È opzionale,
non serve al core, al gate headless o al core libretro.

**Limite attuale:** il loader del runner in `motore_nuovo/src/runner/sponsor_loader.cpp`
usa SDL2_image per i cartelloni sponsor. La registrazione di questa dipendenza non
implementa da sola il caricamento generico degli sprite del giocatore o degli altri
personaggi nell'engine.

Installarla sulla macchina di sviluppo prima di configurare CMake:

```sh
# macOS (Homebrew)
brew install sdl2_image

# Raspberry Pi OS / RetroPie (per compilare il runner sul Pi)
sudo apt update
sudo apt install libsdl2-image-dev
```

Dalla radice `sglavinated_src/`, riconfigurare e compilare il runner su Mac o Pi:

```sh
cmake -S motore_nuovo -B UFFICIO/Officina/build -DCMAKE_BUILD_TYPE=Release
cmake --build UFFICIO/Officina/build --target sglavinated_runner
```

CMake cerca il pacchetto SDL2_image e, come fallback portabile, il modulo `pkg-config`
`SDL2_image`; quando lo trova abilita `SGL_HAVE_SDL2_IMAGE`. Se non è installato o
visibile, il runner resta compilabile ma segnala che i cartelloni PNG sono disabilitati.
Il core, il gate headless e il core libretro non richiedono SDL2_image: è una dipendenza
solo del runner SDL desktop.

Builder/Developer e Developer/Builder Junior possono usare i wrapper di test autorizzati e aggiornare lo status condiviso dell'Officina. Non modificare a mano build, log o altri output generati; non eseguire direttamente binari fuori dai wrapper assegnati.
