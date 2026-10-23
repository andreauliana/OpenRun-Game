# Compilare e provare OpenRun su Mac (nota per Andrea)

Non serve spostare file: la build va da sola in `UFFICIO/Officina/build/` e il runner trova i dati da solo.
Fallo a ciclo fermo (file `STOP` presente), perché gli agenti usano la stessa cartella di build.

## Una volta sola
```sh
brew install cmake sdl2 sdl2_image
```

## Ogni volta: entra nella cartella giusta
```sh
cd ~/Documents/GitHub/OpenRun-Game/"OpenRun/4.5b ufficio/sglavinated_src"
```
Tutti i comandi qui sotto partono da questa cartella (`sglavinated_src`), non da `motore_nuovo`.

## 1. Test automatici (senza finestra)
```sh
sh UFFICIO/Officina/TOOL/run_tests.sh
```
Alla fine deve comparire `100% tests passed`.

Gate completo (stesso controllo degli agenti, dettagli in `UFFICIO/Officina/Test area/gate_t1.log`):
```sh
sh UFFICIO/Officina/TOOL/gate_t1.sh
```
Deve finire con `GATE T1: TUTTO OK`. La versione lunga è `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (qualche minuto).

## 2. Compilare il gioco (runner SDL2)
```sh
cmake -S motore_nuovo -B UFFICIO/Officina/build -DCMAKE_BUILD_TYPE=Release
cmake --build UFFICIO/Officina/build --target sglavinated_runner
```
Controllo rapido facoltativo: `sh UFFICIO/Officina/TOOL/check_sdl_init.sh` (deve dire `Controllo SDL_Init: OK`).

## 3. Giocare
```sh
UFFICIO/Officina/build/sglavinated_runner
```
- Una sola pista in tondo: `UFFICIO/Officina/build/sglavinated_runner --livello lv_03_foglie`
- Schermo intero: aggiungi `--fullscreen` (oppure premi F11 durante il gioco)
- Se l'immagine si blocca: aggiungi `--no-vsync`

Tasti: frecce o WASD per guidare, Z (o Ctrl) per accelerare, X (o Alt) per il razzo, R per il brano successivo, Esc per uscire.

## Se qualcosa non va
- `SDL2 non trovata`: rilancia `brew install sdl2` e poi rifai il passo 2.
- `SDL2_image non trovata` durante cmake: manca `sdl2_image` (`brew install sdl2_image`). Il gioco parte comunque, con i cartelloni presi dai file raw.
- Dopo un `git pull` basta rifare il passo 2: si ricompila solo quello che è cambiato.
