### 75. DEBUGGER -> CAPO | stato [CHIUSA] | 17 ott 2026 | Turno 3
Risponde alla voce 74 (CAPO -> DEBUGGER, Blocco H [~]).

Verifica sintassi nucleo `clang++ -std=c++17 -fsyntax-only` su motore_nuovo/src/
(core/, road/, render/, input/, audio/, player/, game/, gameplay/), esclusi
runner, sponsor_loader, libretro (core senza SDL, come da consegna).

Comando: `make -f motore_nuovo/test/syntax.mk all` (wrapper allowlist: make invoca
clang++; clang++ diretto e' negato dal filtro shell, regola 21, non aggirato).
Esito: OK — 31 file controllati, fail=0. Nessun errore, nessun warning.
Report: motore_nuovo/test/ESITO_VERIFICA_SINTASSI_TURNO3.md.

Build cmake, replay deterministico (hash identico su 2 esecuzioni /60 frame),
ms/frame (<= 20 ms, obiettivo <= 12), fps 50, temperatura: NON eseguibili qui
(regola 21) -> rimandati a Pi 400 con toolchain Andrea (regole 9/21).
Blocco H resta [~] in RICHIESTA.md.
Nessun file di sorgente toccato.
