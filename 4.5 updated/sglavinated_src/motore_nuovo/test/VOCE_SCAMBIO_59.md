### 59. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026
(Da incollare in fondo a SCAMBIO.md da @capo: SCAMBIO.md e' write-protetto per @debugger, regola 21/15. Chiude la voce 54.)

Richiesta/Esito (voce 54, BLOCCO H/C verifica compilazione nucleo, sola lettura):
- `clang++ -std=c++17 -fsyntax-only` sul nucleo: NON ESEGUIBILE. Il canale shell e' respinto dal sistema ("Permission denied: shell") sia su `find motore_nuovo/src -type f` sia sul comando `clang++ -std=c++17 -fsyntax-only motore_nuovo/src/core/types.h`. 2 tentativi, poi stop (regola 21): non si insiste.
- Quindi il numero di file "che compilano" NON e' misurabile qui; la misura manca e va eseguita dove c'e' clang++ (Mac di Andrea o Pi 400). Comando pronto (nucleo, runner/libretro esclusi): per ogni file -> `clang++ -std=c++17 -fsyntax-only -Imotore_nuovo/src <file>`.
- Verifica statica (read/grep, senza shell) OK: 35/35 file .h/.cpp di src/ con `// SPDX-License-Identifier: MIT` in riga 1; 0 include malformati (`#include "..." >` = 0); percorsi include coerenti; nucleo privo di SDL (core/ 0 hit SDL/chrono/gettimeofday; SDL solo runner/libretro); frameHash ogni 60 frame (runner/main.cpp:173); dt 1/50 Hz (game.cpp:178); game.cpp legge `cartelloni_sponsor` da livello.json (voce 56 integrata, nessun alloc nel ciclo); 0 riferimenti Sega in src/.
- Perimetro: 30 file da passare a -fsyntax-only (core 7 incl. json_mini.h, road 3, render 7, input 1, audio 2, player 2, game 2, gameplay 7). Esclusi i 4 che usano SDL: runner/main.cpp, runner/sponsor_loader.{h,cpp}, libretro/sglavinated_libretro.cpp.
- Esito completo: motore_nuovo/test/ESITO_CLANG_TURNO2.md. Blocco C [~] e Blocco H [~] restano tali (build cmake, replay 2x stesso input, ms/frame <=20 ms obiettivo <=12, fps 50, temperatura su Pi 400 con toolchain, Andrea).
