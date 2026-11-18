# REVIEW V47 (@developer-junior)
Riscritto 18/11 12:05 (turno VJ5).

## VJ4 soak (uno per turno) — completato 17/11
- 17/11 09:28 `lv_01_neon`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.926 max 60.243.
- 17/11 09:23 `lv_02_atomico`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay IDENTICO, ms/frame medio 2.136 max 96.164.
- 17/11 09:26 `lv_03_foglie`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 2.008 max 107.658.
- 17/11 09:35 `lv_04_cromo`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.869 max 125.090.
- 17/11 09:42 `lv_05_spray`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 2.047 max 126.451.
- 17/11 09:49 `lv_06_desktop`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.761 max 72.711.
- gate_t1.sh non eseguito. VJ4 chiuso (tutti lv_01-lv_06, nessun crash).

## VJ5 ∞ review src — ultimo cambio `3ea407e` (renderer.cpp, V7/V9 buffer per-frame)
- 18/11 12:05: `motore_nuovo/src/render/renderer.cpp:29` `terraOrdine_.reserve(384)` (prima 128, commit `3ea407e`). Capienza numerata a mano senza garanzia ne' log: il riempimento a `:157-166` e' limitato solo da quanti oggetti di `oggettiTerra_` cadono nella finestra di `Road::DRAW_DIST` (300 segmenti, `road.h:15`); il dato vero e' `oggettiTerra_->size()` (costruito una volta in `Game::caricaOggettiStrada`, `game.cpp:833`). Se una pista ponte/mare piu' densa supera 384, il `push_back` di `renderer.cpp:165` rialloca l'heap dentro `drawRoad`: violazione silenziosa della regola V7 "zero allocazioni nel ciclo" (nessun log, nessun contatore). Semplificazione: riservare `oggettiTerra_->size()` in `Renderer::impostaOggettiTerra` (`renderer.h:177`), oppure limitare il riempimento a `terraOrdine_.capacity()` con log una tantum.
- 17/11 (cumulativo, stesso gruppo): `game.cpp:734-736` skyline senza warning se raw mancante/vuoto; `game.cpp:1037-1046` ruote/luci_stop/fumo/auto_giocatore.json senza warning; `game.cpp:3201-3204,3254-3256` warning folla/supporto solo indice 0; `game.cpp:1026-1048` warning solo per `auto_dritta`; `game.cpp:1032` pointer arithmetic `(POSE[k]+5)` fragile.
- Verifica del turno: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 — ctest **27/27 Passed** (275.42 s), compila il fix escalation in `motore_nuovo/test/prova_gate.cpp` (working tree, "corretto 18/11"). gate_t1.sh non eseguito.

## Rilievi aperti (cumulativi, sintesi)
- `game.cpp:3719,3693` divisione per `a.passo` senza guardia; `:1645,1673,1678,1679` `trafficoAt` non clampato; `:1809-1814` bravura/curva invertita.
- `mixer.cpp:91-99` fail post-load stato residuo; `salvataggi.cpp:78-83/:145-147/:108-109`; `fasi.cpp:512-536`.
- `gameplay/escalation.h:24,35,62` `errori_` non saturato. `core/json_mini.h` BOM/`0x`/dup; `replay.cpp:20-21`; `paesaggio.cpp:113-114/:238-239`. VJ3 chiuso.
