# Esito verifica rilievi reviewer CHECKPOINT 2 (09:09) — @debugger, 17 ott 2026 09:58

Ambito: sola lettura motore_nuovo/src/ (game.cpp, player.h, player.cpp, CMakeLists.txt). Nessun file di prodotto modificato (regola 15: debugger non tocca src/).

## Causa isolata

Tutti e 3 i rilievi del reviewer CHECKPOINT 2 delle 09:09 (SCAMBIO voci 892-912) sono già sanati nel codice consegnato da @coder (voce 87). Il reviewer ha letto una versione precedente del codice; la verifica per lettura sui file correnti conferma:

| # | Rilievo | File:riga | Stato | Evidenza nel codice corrente |
|---|---------|-----------|-------|------------------------------|
| 1 | offroad codice morto (OFFROAD_LIMIT=1200 > X_LIMIT=900, duplicato) | player.h:33; player.cpp:25, 30-31 | SANO | `OFFROAD_LIMIT = 700.0f` (< X_LIMIT=900); `offroad = (x < -OFFROAD_LIMIT) \|\| (x > OFFROAD_LIMIT)` calcolato PRIMA del clamp (linee 30-31); nessun duplicato (linea 29 è commento). |
| 2 | show inattivo (void)show, disegnoShow_ mai chiamato | game.cpp:296-305; 96-102; 335-344 | SANO | `disegnoShow_.aggiorna(clock_.frame, showManager_)` in update() (305); init() con `testiFrasi_.carica`/`verifica`/`setTesti`/`setLayout` (96-102); render() disegna scena() dopo drawPlayerCar (335-344, placeholder rettangoli). |
| 3 | frameHash debole (manca playerX/Z/offroad/RNG) | game.cpp:361-380 | SANO | Hash include già `state_.playerX`, `state_.playerZ`, `playerOffroad`, `rng_.state()` (linee 374-377). |
| 4 | Rilievo A build: moduli P3 non in CMake | CMakeLists.txt:48-52 | SANO | `disegno_show.*` e `testi_frias.*` già elencati in `sglavinated_core`. |

Sintassi nucleo: già OK 34/34 (voci 76/78; non rieseguita qui, shell negata).

## Cosa resta per chiudere H [~] (non eseguibile in questo ambiente)

- Build cmake (`cmake -S motore_nuovo -B build && cmake --build build`): cmake assente (exit 127, voce ESITO_BLOCCo_H) + shell negata regola 21 -> Pi 400 con toolchain Andrea (ARCHITETTURA_CHECK.md §5).
- Replay deterministico: stesso file di input 2x -> stesso hash ogni 60 frame. Bloccato anche dall'attrezzo mancante (`--record`/`--replay` non esistono nel runner; `core/replay` ha solo `startPlayback(std::vector<u64>)` senza I/O file).
- ms/frame <= 20 ms (obiettivo <= 12), fps 50, temperatura: solo Pi 400.
- Gate FEEDBACK_ANDREA (tappa 1): strada visibile con strisce di corsia che scorrono + velocità percepibile; dinamica guida OutRun (già parzialmente in player.cpp: accel che tende a maxSpeed 220, freno BRAKE, attrito 0.98, sterzo proporzionale a velocità); target CMake `prova_gate` (programma headless senza finestra, 10 s = 500 frame, 3 screenshot in out/debugger/, log velocità e posizione).

## Correzioni proposte (NON applicate — spetta a @coder)

Diff A — CMakeLists.txt: target `sglavinated_prova_gate` (eseguibile senza finestra SDL, linka solo `sglavinated_core`, sorgente nuovo `src/test/prova_gate.cpp`) che esegue 500 frame con input schema fisso (accelerazione + sterzo sx/dx alternato), stampa `state().playerSpeed`, `state().playerX`, `state().playerZ` a ogni frame su stdout e salva `fb().data()` raw ai frame 0 (partenza), ~250 (curva), 499 (fuori strada) in `out/debugger/`.

```diff
--- a/motore_nuovo/CMakeLists.txt
+++ b/motore_nuovo/CMakeLists.txt
@@ -83,6 +83,15 @@
    message(STATUS "SDL2 non trovata: target sglavinated_runner escluso. "
                   "Installare libsdl2-dev (o brew install sdl2) e rilanciare cmake.")
 endif()
+
+# ---------------------------------------------------------------------------
+# Target: sglavinated_prova_gate — prova headless senza finestra per il Gate T1.
+# Guida da solo 10 s (500 frame a 50 Hz) con input predefinito, stampa
+# velocita' e posizione a ogni frame e salva 3 screenshot (partenza, curva,
+# fuori strada) in out/debugger/. Non usa SDL: solo sglavinated_core.
+# ---------------------------------------------------------------------------
+add_executable(sglavinated_prova_gate
+    src/test/prova_gate.cpp
+)
+target_link_libraries(sglavinated_prova_gate PRIVATE sglavinated_core)
```

Diff B — replay deterministico: `core/replay.h/cpp` aggiungere `startRecord(const std::string& path)` / `startPlayback(const std::string& path)` con I/O su file di input (registro per frame); `runner/main.cpp` aggiungere argomenti `--record <file>` / `--replay <file>` / `--frames <n>` e confronto `frameHash` ogni 60 frame su due esecuzioni con lo stesso file. Da progettare con @architetto; misura su Pi 400.

Fatto da @debugger, sola ispezione + proposte. Nessun file di `src/`, `CMakeLists.txt` o `dati/` toccato.