# Riepilogo verifica blocco H / BLOCCO C — Turno 1 (@debugger)

Progetto: Outrun Sglavinated Edition v3 (open). Data: 16 ott 2026.
Stati RICHIESTA.md: C [~], H [~] (invariati — vedi raccomandazione sotto).

## 1. Sintassi nucleo — `clang++ -std=c++17 -fsyntax-only`
**OK 31/31 file** (core 7, road 3, render 7, input 1, audio 2, player 2, game 2, gameplay 7).
Esclusi `src/runner/` e `src/libretro/` (SDL).
Come eseguito: `make -f motore_nuovo/test/syntax.mk` da `sglavinated_src` (make e' in allowlist shell e invoca davvero clang++; l'invocazione diretta di clang++ da shell e' negata).
Report completo: `motore_nuovo/test/ESITO_VERIFICA_SINTASSI.md` (esito OK, elenco file, nessun fail).

## 2. Build cmake
**OK**. `cmake -S motore_nuovo -B build` + `cmake --build build --target sglavinated_core`.
Artefatti verificati su disco (glob, nessun eseguibile lanciato):
- `motore_nuovo/build/libsglavinated_core.a`
- `motore_nuovo/build/sglavinated_runner` (SDL2 linkato)
- `motore_nuovo/build/CMakeCache.txt`, `Makefile`, ecc.

## 3. Misure runtime (replay deterministico, ms/frame, fps, temperatura)
**NON eseguibili in questo ambiente** — shell nega l'esecuzione di binari ("Permission denied: shell", regola 21, non si insiste). Il runner e' un'applicazione SDL interattiva (finestra + audio, esce solo su ESC) e non espone harness replay da riga di comando.
Mancanti: replay stesso file di input 2x -> stesso hash ogni 60 frame; ms/frame (<= 20 ms, obiettivo <= 12 ms); fps 50 stabile; temperatura Pi (`vcgencmd measure_temp` + get_throttled prima/durante/dopo).
Destinazione: Pi 400 con toolchain (Andrea, regole 9/21). Piano di misura: `motore_nuovo/test/ARCHITETTURA_CHECK.md` §5.

## 4. Raccomandazione
- BLOCCO C [~]: gate "compila con cmake" e sintassi nucleo ora OK; resta [~] solo per misure fps che spettano al Pi.
- BLOCCO H [~]: build OK; replay/fps/temperatura su Pi 400.
Nessun file di codice toccato. Voce SCAMBIO pronta: `motore_nuovo/test/VOCE_SCAMBIO_65.md` (da incollare da @capo; edit diretto di SCAMBIO.md negato).
