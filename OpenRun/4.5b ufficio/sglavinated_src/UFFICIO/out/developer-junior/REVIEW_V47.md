# REVIEW V47 (riscritta) — @developer-junior
Turno 12/11 09:19, VJ5 ∞.

## Cambio rivisto
- File: `motore_nuovo/src/core/replay.cpp` (file diverso dai turni prima: road.h, partenza.cpp, salvataggi.cpp, fasi.cpp, libretro.cpp, game.cpp/player.*). Review statica; ultimo cambio @developer in src: nessuna modifica locale (`git status --short -- motore_nuovo/src` vuoto), commit recenti 578f086/3df3114/da6c52b gia' rivisti.

## Rilievi
1. **replay.cpp:9,19-21 — registrazione troncata silenziosamente oltre la capacita':** `startRecord()` fa `reserve(4096)` (riga 9) ma `record()` scarta i frame se `inputs.size() >= inputs.capacity()` (riga 20) senza segnale/errore. Per replay lunghi (>4096 frame) la registrazione viene troncata e in playback `next()` restituisce 0 a fine buffer (riga 24), producendo divergenza non evidente. Possibile errore se usato per run lunghe; semplificazione/mitigazione: permettere crescita oltre la riserva (accettando riallocazioni) o segnalare overflow/troncamento esplicito.
2. **replay.cpp:14,23-25 — playback sicuro a lettura:** `startPlayback` usa move, indice azzerato; `next()` fa bounds check e ritorna 0 a fine stream. Coerente.

## Prova
- `sh UFFICIO/Officina/TOOL/run_tests.sh` avviato 12/11 (timeout strumento 120s, interrotto in corso ~test 7/27, primi 6 Passed visibili); exit code non disponibile.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_turno1_0919" lv_01_neon caricamento` exit 0 (`GATE CARICAMENTO OK`, 12/11).
- gate_t1.sh: non eseguito.
