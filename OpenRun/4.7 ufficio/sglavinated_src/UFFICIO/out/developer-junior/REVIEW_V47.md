# REVIEW_V47 — @developer-junior

## 26/11 19:15 — VJ5 ∞ review src c146cb5 (turno 1)
- **Src rivisto**: `c146cb5` (26/11 10:43) — `motore_nuovo/src/render/renderer.cpp`, V9 direct-write.
- **Build/test**: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (prova_gate.cpp modificato in tree, non da questo turno). `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 140.17s). gate_t1.sh non eseguito.
- **Semplificazione (no bug, no resa/replay)**: `motore_nuovo/src/render/renderer.cpp:2806` — `drawRotatedRect` (lambda in `drawPlayerCar`, 2797-2816) clampa y con `yMin/yMax` ma loop x resta `[centerX-extent, centerX+extent]` con solo guard per-pixel `(u32)x < WIDTH` (2812). Incoerente con altri blocchi V9 dello stesso commit (drawDetrito 2080-2081, drawCerchio 2979-2982, fumo 2938-2939) che clampano x/y. Aggiungere `xMin/xMax` come y eviterebbe iterazioni fuori range; **REGOLA BLOCCATA drawPlayerCar**: solo rilievo, nessuna modifica.
- **Rilievo precedente (18:53, stesso commit)**: `renderer.cpp:2940` `u32* dataD = fb_.data()` dentro loop 4 particelle `danni->fumo` (2927-2950), hoistabile prima del loop; REGOLA BLOCCATA.
- **Aperti**: VJ3 lv_05_spray 2.40%>2.00% FALLITO deterministico (exit 22) per @developer. RILIEVO BLOCCANTE drawPlayerCar 5 hunk invariato. VJ4 chiuso 6/6.
