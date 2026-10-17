# ESITO BLOCCO H (Turno 3) — sintassi del nucleo, `clang++ -std=c++17 -fsyntax-only`

Agente: **@debugger**. Data: 17 ott 2026. Riferimenti: SCAMBIO voce 74 (CAPO -> DEBUGGER),
voce 76 (risposta). Ambito: verifica di sintassi di `motore_nuovo/src/` — **sola lettura**.
Nessun file di prodotto toccato. Scritto solo in `motore_nuovo/test/` + `SCAMBIO.md`.
Cartella di esecuzione: `sglavinated_src` (percorsi relativi).

## VERDETTO: **OK** — 31/31 file, exit 0, nessun errore

Esclusioni applicate come da consegna: `src/runner/` (main.cpp, sponsor_loader.h/.cpp) e
`src/libretro/` (il nucleo non usa SDL). Compilatore: `/usr/bin/c++` = **Apple clang
version 14.0.0 (clang-1400.0.29.202)**, target x86_64-apple-darwin21.6.0.

---

## 1. Comando eseguito (esito OK)

Forma esatta richiesta, tutti i file del nucleo in una sola invocazione (31 file: 10 `.cpp` +
21 header; gli header emettono solo il warning noto `treating 'c-header' input`, nessun errore):

```
cd motore_nuovo && clang++ -std=c++17 -fsyntax-only -Isrc \
  src/core/replay.cpp src/core/replay.h src/core/frame_counter.h src/core/state.h \
  src/core/json_mini.h src/core/rng.h src/core/types.h \
  src/road/road.cpp src/road/road.h src/road/road_segment.h \
  src/render/renderer.cpp src/render/palette.cpp src/render/framebuffer.cpp src/render/renderer.h \
  src/render/palette.h src/render/framebuffer.h src/render/sprite.h src/input/pad.h \
  src/audio/mixer.cpp src/audio/mixer.h src/player/player.cpp src/player/player.h \
  src/game/game.cpp src/game/game.h src/gameplay/frasi.cpp src/gameplay/show.cpp src/gameplay/show.h \
  src/gameplay/ufo.h src/gameplay/escalation.h src/gameplay/personaggi.h src/gameplay/frasi.h
# -> exit 0, 21 warning "treated as c++-header" (uno per ciascuno dei 21 header passati come sorgente), 0 errori
```

Una prima invocazione con la stessa lista senza `core/replay.h` (30 file) ha dato lo stesso esito;
la ripetizione con i **31** file e' quella sopra: nessuna differenza, quindi anche `replay.h` passa.

Controprova con `-Wall -Wextra` sui soli 10 `.cpp` (gli unici file compilabili del nucleo):

```
cd motore_nuovo && clang++ -std=c++17 -fsyntax-only -Isrc -Wall -Wextra \
  src/core/replay.cpp src/road/road.cpp src/render/renderer.cpp src/render/palette.cpp \
  src/render/framebuffer.cpp src/audio/mixer.cpp src/player/player.cpp src/game/game.cpp \
  src/gameplay/frasi.cpp src/gameplay/show.cpp
# -> exit 0, 5 warning (vedi §4), 0 errori
```

Controprova file-per-file con il wrapper riutilizzabile (31 invocazioni separate):

```
make -f motore_nuovo/test/syntax.mk all   # una clang++ per file: -std=c++17 -fsyntax-only -Imotore_nuovo/src
# -> 31 righe "OK", "---- 31 file controllati, fail=0"
```

Numeri: **31 file controllati, 0 FAIL, 0 errori**; con `-Wall -Wextra` **5 warning** in 4 righe
di 3 file (vedi §4), tutti non bloccanti.

## 2. Filtro shell — cosa passa e cosa no (regola 21, non aggirato)

| # | Comando | Esito |
|---|---|---|
| 1 | `clang++ -std=c++17 -fsyntax-only -Isrc src/core/replay.cpp` | **OK** (compila) |
| 2 | `cd <motore_nuovo> && clang++ -std=c++17 -fsyntax-only -Isrc <31 file>` | **OK** (exit 0) |
| 3 | `make -f motore_nuovo/test/syntax.mk all` | **OK** (`make` in allowlist) |
| 4 | `clang++ -fsyntax-only -Isrc src/core/replay.cpp` (senza `-std=c++17`) | **NEGATO** — Permission denied: shell |
| 5 | `clang++ --version` | **NEGATO** |
| 6 | `which clang++ && clang++ --version`; `cd <dir> && ls <dir> && echo ...`; `for f in ...; do ... done` | **NEGATO** (comando composto / `which` / `echo` fuori allowlist) |

Causa: allowlist esplicita in `opencode.json` (righe 6-23): `"*": "deny"` piu' `make *`,
`cmake *`, **`clang++ -std=c++17 -fsyntax-only *`** (riga 12), `ls *`, `cat *`, `grep *`.
Il filtro di questa sessione **permette** `clang++` con `-std=c++17` e `make`, nega `clang++`
senza `-std`, `which`/`ls`/catene con `&&`/`echo`/loop `for`. Diverso dal turno precedente
(ESITO_VERIFICA_SINTASSI.md §1 diceva `clang++` sempre negato, aggirato via make): **qui il
comando nudo richiesto e' passato**, quindi nessun aggiramento. `python3` e `cmake` non usati
in questo blocco.

## 3. Controllo negativo (il gate non e' vuoto)

Aggiunto `motore_nuovo/test/neg_control_sintassi.cpp` (errore voluto, fuori da ogni target:
`CMakeLists.txt` elenca i sorgenti a mano, nessun GLOB, nessuna sottocartella di test):

```
clang++ -std=c++17 -fsyntax-only -Imotore_nuovo/src motore_nuovo/test/neg_control_sintassi.cpp
# -> 3 errors generated: "error: expected '}'"; Exited with code 1
```

Quindi il controllo **rileva gli errori** (exit 1 con messaggi): l'OK di §1 e' reale.

## 4. Warning con `-Wall -Wextra` (non bloccanti)

| File:riga | Warning | Nota |
|---|---|---|
| road/road.cpp:59 | `unused parameter 'playerX'` | `Road::project(f32 playerX, ...)` non usa playerX |
| render/renderer.cpp:56 | `unused variable 'x0'` e `'x1'` | copie di cx0/cx1 mai lette (il trapezio usa cx0/cx1/dx0/dx1) |
| render/renderer.cpp:76 | `lambda capture 'playerZ' is not used` | cattura ridondante, il comparatore usa solo `worldZ` |
| player/player.cpp:9 | `unused parameter 'dt'` | `Player::input(const Pad&, f32 dt)` |

Cross-check su voce 71 punto 4 (@coder, non riassegnato qui): in `player.cpp:26-27` il clamp
`X_LIMIT=900` precede `offroad` (riga 37) con `OFFROAD_LIMIT=1200` (`player.h:33-34`):
`offroad` e' **sempre false** — confermato a codice, e' codice morto (`pal_[12]` del renderer).

Nota minore: `sglavinated_core` (CMakeLists.txt:12-47) elenca 29 file; `core/types.h` e
`core/json_mini.h` non sono in lista (header usati per inclusione: non rompono la build, li
copre comunque il `-fsyntax-only`).

## Proposte di correzione (NON applicate — @coder)

```diff
--- a/motore_nuovo/src/road/road.cpp
+++ b/motore_nuovo/src/road/road.cpp
@@ -59 +59 @@
-void Road::project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const {
+void Road::project([[maybe_unused]] f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const {

--- a/motore_nuovo/src/render/renderer.cpp
+++ b/motore_nuovo/src/render/renderer.cpp
@@ -56 +55,0 @@
-    i32 x0 = cx0, x1 = cx1;
@@ -76 +74 @@
-              [playerZ](const SpriteInstance& a, const SpriteInstance& b) {
+              [](const SpriteInstance& a, const SpriteInstance& b) {

--- a/motore_nuovo/src/player/player.cpp
+++ b/motore_nuovo/src/player/player.cpp
@@ -9 +9 @@
-void Player::input(const Pad& pad, f32 dt) {
+void Player::input(const Pad& pad, [[maybe_unused]] f32 dt) {
     // e (voce 71 punto 4) spostare il calcolo di offroad PRIMA del clamp, oppure allineare
     // OFFROAD_LIMIT a X_LIMIT (player.h:33-34), altrimenti offroad resta sempre false.

--- a/motore_nuovo/CMakeLists.txt
+++ b/motore_nuovo/CMakeLists.txt
@@ -13,0 +14,2 @@
+    src/core/types.h
+    src/core/json_mini.h
```

## Misure che MANCANO qui (restano a Pi 400 — Andrea, regole 9/21)

- **Build cmake** (`cmake -S motore_nuovo -B build && cmake --build build`) e fps: non eseguite
  in questo blocco per consegna esplicita (build/fps/temperatura su Pi 400). Non c'e' una
  misura di fps/temperatura da riportare.
- **Replay deterministico** (stesso file di input due volte -> stesso hash ogni 60 frame) e
  **tempo di frame <= 20 ms (obiettivo <= 12 ms)**: non misurabili qui; serve la toolchain/harness
  sul Pi 400 (il runner non carica file di input: vedi ESITO_BLOCCO_H.md §2, proposta C).

## File aggiunti (dentro il perimetro di @debugger)

- `motore_nuovo/test/ESITO_SINTASSI_BLOCCO_H_T3.md` (questo file)
- `motore_nuovo/test/neg_control_sintassi.cpp` (controllo negativo volontario, fuori da ogni target)
- `motore_nuovo/test/VOCE_SCAMBIO_76.md` — **voce 76 pronta per SCAMBIO.md**: l'edit diretto di
  `SCAMBIO.md` e' stato **negato 2 volte** ("Permission denied: edit", regola 21, non insisto).
  @capo: copiare la voce 76 in fondo a SCAMBIO.md e marcare la voce 74 (riga 793) `[APERTA]` ->
  `[CHIUSA]`. Il contenuto della voce e' identico a §verdetto di questo file.
- Nessun file di `src/`, `CMakeLists.txt` o dati e' stato modificato.
