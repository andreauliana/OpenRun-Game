### 76. DEBUGGER -> CAPO | stato [CHIUSA] | 17 ott 2026
H [~] verificato (risponde a voce 74 CAPO -> DEBUGGER). Questa voce chiude la 74 [APERTA] -> [CHIUSA].
- COMANDO: `clang++ -std=c++17 -fsyntax-only -Isrc` sui 31 file del nucleo, esclusi runner/sponsor_loader/libretro; controprova `make -f motore_nuovo/test/syntax.mk all`.
- ESITO: **OK, 31/31 file, exit 0, 0 errori** (Apple clang 14.0.0, clang-1400.0.29.202). Con `-Wall -Wextra` 5 warning non bloccanti (road.cpp:59 `playerX` inutilizzato; renderer.cpp:56 `x0`/`x1`; renderer.cpp:76 cattura `playerZ`; player.cpp:9 `dt`).
- SHELL: **non negata** stavolta (`clang++ -std=c++17 ...` in allowlist opencode.json:12); negati `clang++ --version`, `which`, catene con `echo`. Nessun aggiramento, nessun `make`/`cmake` necessario per la sintassi.
- CONTROLLO NEGATIVO: `motore_nuovo/test/neg_control_sintassi.cpp` (fuori da ogni target) -> `error: expected '}'`, exit 1: il gate non e' vuoto.
- NON MISURATO QUI (misura che manca): build cmake, fps/temperatura, replay deterministico (stesso input 2x -> stesso hash ogni 60 frame) e ms/frame (<= 20 ms, obiettivo <= 12 ms) -> Pi 400 (Andrea, regole 9/21).
- Esito completo + diff proposte a @coder: `motore_nuovo/test/ESITO_SINTASSI_BLOCCO_H_T3.md`.
- NOTA: edit di SCAMBIO.md **negato 2 volte** ("Permission denied: edit", regola 21): questa voce e' depositata qui perche' @capo la copi in SCAMBIO.md e marchi la voce 74 come [CHIUSA]. Nessun file di prodotto toccato.
