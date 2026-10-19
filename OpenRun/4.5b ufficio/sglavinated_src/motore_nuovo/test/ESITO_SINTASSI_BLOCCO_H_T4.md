# ESITO BLOCCO H (2° giro, dopo le modifiche P1b di @developer) — sintassi del nucleo

Agente: **@debugger**. Data: 17 ott 2026. Seguito della voce 76 (SCAMBIO.md) e di
`motore_nuovo/test/ESITO_SINTASSI_BLOCCO_H_T3.md`. Ambito: **sola lettura** dei sorgenti;
nessun file di prodotto toccato. Scritto solo in `motore_nuovo/test/` + `SCAMBIO.md`.
Esecuzione da `sglavinated_src`.

## VERDETTO: **OK** — 34/34 file, 0 errori (`clang++ -std=c++17 -fsyntax-only`)

Il nucleo e' cresciuto da 31 a **34** file (3 nuovi: `gameplay/disegno_show.h`,
`gameplay/testi_frasi.h`, `gameplay/testi_frasi.cpp`). Tutti e 34 passano.

---

## 1. Comando richiesto e numeri

```
cd motore_nuovo && clang++ -std=c++17 -fsyntax-only -Isrc <34 file: 11 .cpp + 23 header>
# -> 23 warning di rumore "treating 'c-header' input as 'c++-header'" (uno per header),
#    0 errori -> exit 0
```

Forma file-per-file (riutilizzo `motore_nuovo/test/syntax.mk`, 34 invocazioni separate):

```
make -f motore_nuovo/test/syntax.mk all
# -> 34 righe "OK   motore_nuovo/src/...", poi "---- 34 file controllati, fail=0"
```

Controprova sui soli 11 `.cpp` con `-Wall -Wextra` (gli header come sorgente non danno
warning di codice):

```
cd motore_nuovo && clang++ -std=c++17 -fsyntax-only -Isrc -Wall -Wextra \
  src/audio/mixer.cpp src/core/replay.cpp src/game/game.cpp src/gameplay/frasi.cpp \
  src/gameplay/show.cpp src/gameplay/testi_frasi.cpp src/player/player.cpp \
  src/render/framebuffer.cpp src/render/palette.cpp src/render/renderer.cpp src/road/road.cpp
# -> 5 warning, 0 errori (elenco in §4)
```

Numeri: **34 file controllati, 0 FAIL, 0 errori**; **5 warning** `-Wall -Wextra`
(gli stessi 5 del turno precedente: nessun warning nuovo dai 3 file aggiunti).
Compilatore: `/usr/bin/c++` = **Apple clang version 14.0.0 (clang-1400.0.29.202)**,
target x86_64-apple-darwin21.6.0.

### Controllo negativo (il gate non e' vuoto)

```
clang++ -std=c++17 -fsyntax-only -Imotore_nuovo/src motore_nuovo/test/neg_control_sintassi.cpp
# -> "error: expected '}'" x3 -> "3 errors generated." / "Exited with code 1"
```

Quindi l'OK di §1 e' un esito reale e non un gate che non guarda nulla.

## 2. Filtro shell di questa sessione (regola 21, non aggirato)

| # | Comando | Esito |
|---|---|---|
| 1 | `clang++ -std=c++17 -fsyntax-only -Isrc src/core/replay.cpp` | **OK** |
| 2 | `clang++ -std=c++17 -fsyntax-only -Isrc <34 file>` | **OK** (0 errori) |
| 3 | `make -f motore_nuovo/test/syntax.mk all` | **OK** (`34 file controllati, fail=0`) |
| 4 | `make -f motore_nuovo/test/syntax.mk version` | **OK** (stampa Apple clang 14.0.0) |
| 5 | `ls` (nudo, senza argomenti) | **NEGATO** — Permission denied: shell |
| 6 | `echo ok` | **NEGATO** |

Differenza rispetto alla voce 76: qui **`ls` ed `echo` sono negati**, mentre
`clang++ -std=c++17 -fsyntax-only …` e `make …` passano. Non ho ritentato `which`/
`clang++ --version` nudi (versione ottenuta via `make … version`): nessun aggiramento.
`cmake` non e' stato usato (build rimandata a Pi 400, §5).

## 3. Rilievi (3, per gravita') — nessuno dei 3 e' un errore di sintassi

### A. [da verificare in build — il modulo nuovo NON entra nella build cmake]

`testi_frasi.h`, `testi_frasi.cpp` e `disegno_show.h` (P3, @traduttore) **non sono elencati**
nel target `sglavinated_core` (`motore_nuovo/CMakeLists.txt:12-47`) e **nessun file li include**:

- `CMakeLists.txt` usa una lista fissa di sorgenti, nessun `GLOB`/`file()` (grep: 0 match per
  `GLOB|file(|aux_source`), quindi non "li prende da solo";
- grep di `testi_frasi|disegno_show` su tutto `motore_nuovo/`: **1 solo match**, ed e'
  l'auto-include in `src/gameplay/testi_frasi.cpp:2`; `disegno_show` -> **0 match**.

Effetto: il modulo P3 compila con `-fsyntax-only` (controllato in §1) ma **non viene compilato
ne' linkato** dal build cmake, e `Game` non lo chiama (`game.cpp:224-225` continua a scartare
`show` con `(void)show`). Non e' un errore di link (0 riferimenti = nessun simbolo mancante):
e' un guasto **silenzioso**, la build passa e la funzione non c'e'. Da decidere con @capo se
completare il collegamento (C) o togliere i file dalla cartella di prodotto fino a che non
servono.

### B. [aperto] `offroad` e' **ancora** codice morto: il rilievo 3 del @reviewer non e' risolto, solo spostato

@developer ha giustamente spostato il calcolo di `offroad` **prima** del clamp
(`player.cpp:25` contro clamp a `player.cpp:30-31`), ma con i numeri attuali la condizione
non puo' mai diventare vera:

- `STEER_INC = 90.0f` unita' x per secondo di sterzo (`player.h:32`), `dt = 1/50 s`
  (`game.cpp:184`), fattore velocita' `(1 + speed/MAX_SPEED)` <= 2 (`MAX_SPEED = 220`,
  `player.h:29`) -> **incremento massimo di x per frame = 90 * 0.02 * 2 = 3.6 unita'**;
- il clamp a `X_LIMIT = 900` viene applicato **a ogni fine update** (`player.cpp:30-31`),
  quindi all'inizio del frame successivo `|x| <= 900` e dopo un frame di sterzo
  `|x| <= 903.6`;
- `OFFROAD_LIMIT = 1200` (`player.h:33`) -> `|x|` non arriva mai a 1200 ->
  `offroad` **sempre false** -> il ramo offroad di `drawPlayerCar` (`renderer.cpp:121`,
  `pal_[12]`) resta irraggiungibile e il gioco non da' nessun segnale di fuori pista.

E' una scelta di gameplay (quanto largo e' il fuori pista), quindi la decisione e' di
@developer/@capo; le due vie sono nel diff di §4-B.

### C. [proposta, per il gate del blocco H] replay deterministico e hash: manca l'attrezzo, non solo il Pi

- `Game::frameHash()` (`game.cpp:269-282`) copre framebuffer + `state_.frame` + `playerSpeed`,
  ma **non** `playerX`, `playerZ`, `playerOffroad` ne' lo stato RNG: due esecuzioni che
  divergono in posizione/offroad potrebbero dare lo stesso hash (il framebuffer mitiga, ma
  la copertura non e' dichiarata). Proposta minima: aggiungere i 4 campi in coda all'hash.
- `core/replay.h` **non ha I/O su file** (`startPlayback(std::vector<u64>)`): il runner
  (`src/runner/main.cpp`) non legge argomenti, non ha `--record`/`--replay` e prende l'input
  dalla tastiera (`main.cpp:121-123`). Quindi **oggi "stesso file di input riprodotto due
  volte" non e' eseguibile da nessuna parte**, indipendentemente dal Pi 400: serve prima
  l'attrezzo (registrazione/scrittura del replay + modalita' headless con numero di frame
  fisso e nessun orologio nel core). Va assegnato a @developer prima che il gate H sia misurabile.

## 4. Warning `-Wall -Wextra` (5, non bloccanti) e diff proposte (NON applicate)

| File:riga | Warning |
|---|---|
| `player/player.cpp:9` | `unused parameter 'dt'` in `Player::input(const Pad&, f32 dt)` |
| `render/renderer.cpp:56` | `unused variable 'x0'` e `'x1'` |
| `render/renderer.cpp:76` | `lambda capture 'playerZ' is not used` |
| `road/road.cpp:59` | `unused parameter 'playerX'` in `Road::project` |

```diff
--- a/motore_nuovo/CMakeLists.txt          # rilievo A
+++ b/motore_nuovo/CMakeLists.txt
@@ -44,0 +45,3 @@
+    src/gameplay/disegno_show.h
+    src/gameplay/testi_frasi.h
+    src/gameplay/testi_frasi.cpp
+    # ^ da fare SOLO insieme al collegamento in game.cpp: oggi nessuno li include,

--- a/motore_nuovo/src/player/player.h        # rilievo B (una delle due vie)
+++ b/motore_nuovo/src/player/player.h
@@ -33 +33 @@
-    static constexpr f32 OFFROAD_LIMIT = 1200.0f; // |x| oltre cui si e' offroad
+    static constexpr f32 OFFROAD_LIMIT = 700.0f;  // |x| oltre cui si e' offroad (deve essere
+                                                  // < X_LIMIT=900, altrimenti e' codice morto:
+                                                  // vedi test/ESITO_SINTASSI_BLOCCO_H_T4.md §3-B)

--- a/motore_nuovo/src/player/player.cpp        # rilievo B (variante: rimuovere il clamp)
+++ b/motore_nuovo/src/player/player.cpp
@@ -30,2 +30,2 @@
-    if (x > X_LIMIT)  x = X_LIMIT;
-    if (x < -X_LIMIT) x = -X_LIMIT;
+    // variante: niente clamp netto, rientro graduale oltre X_LIMIT e clamp solo a
+    // OFFROAD_LIMIT (poi il rientro e' gestito da attrito/sterzo): da tarare a mano.

--- a/motore_nuovo/src/player/player.cpp        # warning
+++ b/motore_nuovo/src/player/player.cpp
@@ -9 +9 @@
-void Player::input(const Pad& pad, f32 dt) {
+void Player::input(const Pad& pad, [[maybe_unused]] f32 dt) {

--- a/motore_nuovo/src/render/renderer.cpp      # warning
+++ b/motore_nuovo/src/render/renderer.cpp
@@ -56 +55,0 @@
-    i32 x0 = cx0, x1 = cx1;
@@ -76 +74 @@
-              [playerZ](const SpriteInstance& a, const SpriteInstance& b) {
+              [](const SpriteInstance& a, const SpriteInstance& b) {

--- a/motore_nuovo/src/road/road.cpp            # warning
+++ b/motore_nuovo/src/road/road.cpp
@@ -59 +59 @@
-void Road::project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const {
+void Road::project([[maybe_unused]] f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const {

--- a/motore_nuovo/src/game/game.cpp            # rilievo C (proposta)
+++ b/motore_nuovo/src/game/game.cpp
@@ -278,0 +279,4 @@
+    h ^= (u32)state_.playerX;
+    h ^= (u32)state_.playerZ;
+    h ^= state_.playerOffroad ? 1u : 0u;   // + stato RNG se esposto da Game
+    h *= 0x01000193u;
```

## 5. Misure che restano a Pi 400 (regole 9/21) — **non** eseguite qui

Tutte e tre **rimandate a Pi 400** (toolchain di Andrea) e, per il replay, **bloccate dalla
mancanza dell'attrezzo** (§3-C):

| Misura | Stato qui | Perche' |
|---|---|---|
| `cmake -S motore_nuovo -B build && cmake --build build` | **non eseguita** | build su Pi 400 (consegna); qui sarebbe comunque priva di valore perche' i target non sono quelli di produzione |
| fps (target 50) | **non misurato** | serve il runner su Pi 400 |
| ms/frame (<= 20 ms, obiettivo <= 12 ms) | **non misurato** | idem; il runner non ha un contatore di tempo per frame (`main.cpp:166-170` usa solo `SDL_Delay` sul target 50 fps) |
| temperatura Pi 400 | **non misurata** | hardware assente |
| replay deterministico: stesso input 2x -> stesso hash ogni 60 frame | **non eseguibile** | l'hash esiste ed e' stampato ogni 60 frame (`main.cpp:173-178`), ma `Replay` non legge/scrive file e il runner non accetta un file di input (§3-C): da completare a @developer, poi misura su Pi 400 |

Nota: la consegna di questo giro diceva esplicitamente che build/replay/ms/frame/fps/temperatura
non sono eseguibili qui; confermo che **lo restano**, con la precisazione che il replay ha anche
un pezzo mancante nel codice, non solo il vincolo hardware.

## File scritti (perimetro @debugger)

- `motore_nuovo/test/ESITO_SINTASSI_BLOCCO_H_T4.md` (questo file)
- `motore_nuovo/test/VOCE_SCAMBIO_77.md` — voce 77 pronta per `SCAMBIO.md`: l'edit diretto di
  `SCAMBIO.md` e' stato **negato 2 volte** ("Permission denied: edit", regola 21, non insisto).
  @capo: copiare il contenuto di quel file in fondo a `SCAMBIO.md` (voce 74 e' gia' `[CHIUSA]`).
- Nessun file di `src/`, `CMakeLists.txt`, `dati/` o docs e' stato modificato.
