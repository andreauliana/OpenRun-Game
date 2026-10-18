# BUILD — motore nuovo "Outrun Sglavinated Edition v3 (open)" (pista C)

SPDX-License-Identifier: MIT

Autore: @architetto. Data: 16 ott 2026. Stato: consegna BLOCCO B (Turno 1).
Percorso: motore_nuovo/docs/BUILD.md

---

## 1. Obiettivo

Due target dallo stesso codice C++17 del core (niente SDL nel core):

1. **runner SDL2** — finestra/terminal per test su Mac e Pi 400.
2. **core libretro** (`sglavinated_libretro.so`) — per RetroArch/RetroPie su Pi 400 (API: `retro_init`, `retro_load_game`, `retro_run`, `retro_unload_game`, `retro_get_system_av_info`, `retro_serialize`/`retro_unserialize` per replay; video una volta per chiamata; audio int16 stereo; input RetroPad; niente sleep/timer nel core; istanza singola, non thread-safe — vedi RISORSE_TECNICHE.md).

Build tool: **CMake**. Strumenti di conversione e controlli: **Python 3** (pipeline unica come nella hack v87, controlli che fanno fallire la build).

Tutti i comandi sotto sono lanciati dalla cartella `sglavinated_src/` (percorso relativo `motore_nuovo/`); non si tocca la hack v87 (`tools/`, `lr/`, separata, regola 6).

---

## 2. Dipendenze (licenze in LICENZE.md)

| Cosa | Mac | Raspberry Pi OS / RetroPie | Licenza | Stato |
|---|---|---|---|---|
| Compilatore + CMake | `xcode-select --install`; `brew install cmake` | `sudo apt install build-essential cmake` | CMake: Kitware (BSD-like) | [C] da verificare |
| SDL2 (finestra, input) | `brew install sdl2` | `sudo apt install libsdl2-dev` | SDL2: zlib | [C] |
| SDL2_image (PNG nel runner desktop, opzionale) | `brew install sdl2_image` | `sudo apt install libsdl2-image-dev` | zlib | [C] |
| SDL2_mixer (audio runner, opzionale) | `brew install sdl2_mixer` | `sudo apt install libsdl2-mixer-dev` | zlib | [C] |
| JSON | `src/core/json_mini.h` (parser interno, nessuna dipendenza) | idem | MIT | [x] |
| Musica tracker .xm (opzionale) | libxmp via brew | libxmp via apt | LGPL | [C] |
| Test Pi 400 | n/a | `vcgencmd measure_temp`, `vcgencmd get_throttled` | n/a | [C] |

Prima di installare un pacchetto, controllare che la licenza sia compatibile e riportarla in `motore_nuovo/docs/LICENZE.md`.

---

## 3. Struttura CMake (da scrivere da @coder)

```
motore_nuovo/
  CMakeLists.txt
  src/
    core/        # no SDL: frame, rng, stato, replay, hash
    road/        # strada a segmenti, traffico, collisioni
    render/      # framebuffer software 320x224, sprite, palette
    audio/       # mixer int16 stereo
    input/       # pad tipo RetroPad
    gameplay/    # gameplay Gianni/Gesu'/UFO (blocco D, dati in dati/personaggi/*/)
    fx/<nome>/   # moduli extra con flag on/off
    ui/          # menu, HUD da dati/ui/*.json
  runner/
    main.cpp     # SDL2 finestra + ciclo frame
  src/libretro/
    sglavinated_libretro.cpp  # wrapper retro_* sopra il core (stub, BLOCCO C)
```

Target CMake:
- `sglavinated_core` (statica, solo `src/`, zero dipendenze piattaforma).
- `sglavinated_runner` (eseguibile, linka SDL2).
- `sglavinated_libretro` (shared object `sglavinated_libretro.so`, linka il core + API libretro; OFF di default).

---

## 4. Comandi di build

### Mac (Homebrew, dalla cartella `sglavinated_src/`)

```bash
# dipendenze core + runner desktop con supporto PNG sponsor
brew install cmake sdl2 sdl2_image

# build isolata del core C++17 (non richiede SDL2)
cmake -S motore_nuovo -B motore_nuovo/build_local -DCMAKE_BUILD_TYPE=Release
cmake --build motore_nuovo/build_local --target sglavinated_core -j2

# configurazione e build del runner SDL2 per test
cmake -S motore_nuovo -B motore_nuovo/build_local -DCMAKE_BUILD_TYPE=Release
cmake --build motore_nuovo/build_local -j$(sysctl -n hw.ncpu)

# esecuzione test runner
./motore_nuovo/build_local/sglavinated_runner

# (se fattibile) core libretro
cmake -S motore_nuovo -B motore_nuovo/build_libretro -DCMAKE_BUILD_TYPE=Release -DBUILD_LIBRETRO=ON
cmake --build motore_nuovo/build_libretro -j$(sysctl -n hw.ncpu)
# output: motore_nuovo/build_libretro/sglavinated_libretro.so
```

Su un Mac con `clang++` e `cmake` già disponibili non occorre installare altro per il
target `sglavinated_core`. La directory `motore_nuovo/build_local` evita di riutilizzare
cache CMake di altri checkout; il Pi 400 serve per la verifica su hardware, non per
compilare il core.

Il runner cerca automaticamente la radice `sglavinated_src/` partendo dalla directory
corrente e dal percorso dell'eseguibile. Perciò CMake Tools può avviarlo dal proprio
output `build/` senza perdere i percorsi relativi a livelli e sponsor.

Per verificare separatamente l'inizializzazione SDL usata dal runner, senza aprire la
finestra del gioco, dalla radice `sglavinated_src/` eseguire:

```bash
sh tools/check_sdl_init.sh
```

Lo script controlla l'exit code e la riga `SDL_Init: OK`. Per un eseguibile in un
percorso diverso, passare il path come primo argomento.

I controlli `prova_gate` sono registrati come test CTest: CMake-capable test
discovery li puo' elencare senza un'integrazione diretta con il tool `runTests`
di VS Code/Copilot. Per compilare ed eseguire la suite tramite l'entry point
del repository, dalla radice `sglavinated_src/` eseguire:

```bash
sh tools/run_tests.sh
```

Lo script configura la build isolata `build/tests/`, compila `prova_gate` e
lancia i test registrati con CTest. La directory e' distinta da `build/current/`
e da `build_gate/`; quest'ultima e' usata da `gate_t1.sh`. Lo script rifiuta percorsi `BUILD_DIR` che si sovrappongono a `build_gate/`.
Per scegliere un'altra directory indipendente:
`BUILD_DIR=/percorso/build sh tools/run_tests.sh`.

### Raspberry Pi 400 (Raspberry Pi OS / RetroPie, dalla cartella `sglavinated_src/`)

```bash
# dipendenze core + runner SDL2 con supporto PNG sponsor
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev

# configurazione e build
cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17
cmake --build build -j$(nproc)

# esecuzione test runner
./build/sglavinated_runner

# monitor termico Pi 400 (durante i test)
watch -n1 'vcgencmd measure_temp; vcgencmd get_throttled'
```

---

## 5. Obiettivo prestazioni Pi 400

- Frame <= 20 ms, obiettivo <= 12 ms, nessun picco ripetuto (VISIONE.md).
- 50 fps stabili (PAL).
- Nessuna allocazione nel ciclo di gioco.

---

## 6. Test e determinismo

- Build gira con `cmake`; @debugger misura fps (BLOCCO H).
- Replay deterministico: stesso file di input = stessi hash di frame ogni 60 frame su due esecuzioni (verifica @debugger).
- Su Pi 400: temperatura e throttling con `vcgencmd`; nessun surriscaldamento che riduca i fps sotto 50.

---

## 7. Note

- Il core C++ non include SDL2: solo runner e core libretro lo fanno.
- SDL2_image è opzionale e serve solo al runner desktop per caricare i cartelloni PNG; core e gate headless non dipendono da essa.
- La build della hack v87 (`tools/`, `lr/`) è separata e non si tocca (AGENTS.md regola 6).
- Ogni nuova dipendenza deve avere licenza verificata in `motore_nuovo/docs/LICENZE.md` prima dell'uso.


---

## 5. Gate Tappa 1: `prova_gate` (verificato 17 ott 2026, build reale)

Programma senza finestra, solo core. **Si lancia dalla radice `sglavinated_src/`** (i dati sono letti con
percorsi relativi `motore_nuovo/dati/...`; da un'altra cartella esce con codice 4).

Comando unico per gli agenti (nessuna pipe, nessun `;`): `sh tools/gate_t1.sh` oppure `sh tools/gate_t1.sh soak`.
Passo per passo:

```bash
cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target prova_gate
./build/prova_gate out/debugger                      # smoke 10 s (500 frame) + replay 2x
./build/prova_gate out/debugger lv_02_atomico        # altro livello
./build/prova_gate out/debugger lv_01_neon soak      # accettazione: 30000 frame = 10 min simulati
echo $?
```

| Exit | Significato |
|---|---|
| 0 | OK: partenza (speed>0 al frame 60), replay 2x identico, ms/frame medio <= 20 |
| 1 | nessuna partenza | 
| 2 | replay non deterministico (stampa il primo frame divergente) |
| 3 | troppo lento |
| 4 | cartella di lavoro errata |

Output: `out/debugger/prova_gate.log` (frame velocita X Z offroad hash), `prova_gate_partenza.ppm`,
`prova_gate_curva.ppm`, `prova_gate_fuoristrada.ppm` (PPM P6, apribili con Anteprima/GIMP).

Esito 17 ott (cloud Linux, g++/cmake): lv_01/02/03 OK, replay 2x identico, 0,05 ms/frame, soak 30000 frame OK,
ASan+UBSan puliti. **Non misurato**: fps/temperatura su Pi 400 (tocca ad Andrea), runner SDL2 (SDL2 assente in cloud).

---

## Opzioni del runner SDL2 (dal 18 ott)

| Opzione | Effetto |
|---|---|
| `--data <cartella>` | cartella `sglavinated_src` con i dati; senza, il runner la cerca da solo (cartella corrente, cartella dell'eseguibile) |
| `--livello <id>` | pista da guidare, cartella in `motore_nuovo/dati/livelli/` (default `lv_01_neon`; 6 piste: lv_01_neon … lv_06_desktop) |
| `--fullscreen` / `--window` | schermo intero (consigliato sul Pi) o finestra; F11 alterna durante il gioco |
| `--no-vsync` | disattiva la sincronizzazione con lo schermo (solo per driver che si bloccano) |

Tasti: ESC esce; sul pad Select+Start esce, Start+B (o R sulla tastiera) cambia brano.
La logica resta a passo fisso di 1/50 s (replay e hash invariati); il video si presenta con vsync e l'audio va in coda (`SDL_QueueAudio`), senza buchi.
