# REVIEW_V47 @developer-junior (riscritto 11/11 turno 10:21)

- **VJ4 soak [~] (4/6) — 11/11:** lv_01_neon 09:49 `.../lv_01_neon soak` exit 0 GATE OK, 30000f IDENTICO, 1.984/133.015. lv_02_atomico 10:01 `.../lv_02_atomico soak` exit 0, 30000f IDENTICO, 1.807/96.036. lv_03_foglie 10:11 `.../lv_03_foglie soak` exit 0, 30000f IDENTICO, 1.715/84.185. lv_04_cromo 10:21 `.../lv_04_cromo soak` exit 0 GATE OK, 30000f IDENTICO, 1.573/124.742, nessun crash.
- **VJ3 50/60 Hz [x] — 6/6 livelli** (11/11): lv_01-lv_06 replay IDENTICO a 50 e 60 Hz, tempi coerenti, exit 0; max iniziali ~83 ms da caricamento, regime 1.66-2.05 ms. Dettagli in turni 08:12/08:41/09:42 (`vj3_freq_1111*`).
- **VJ5 review codice [11/11 08:41] — V7 @developer, `src/render/renderer.cpp:29`:** `terraOrdine_.reserve(64)` basso per ~63 oggetti attesi/300 segmenti mare (`paesaggio.cpp:244-287`) → possibile realloc in loop (riga 141). Consiglio: `reserve(Road::DRAW_DIST + 8)`. Nota: `spriteOrd_.reserve(512)` (riga 30) da legare al tetto `game.cpp:3084`.
- **VJ1 su V6 [x] (11/11):** solo `src/libretro/sglavinated_libretro.cpp`; ctest 27/27, prova core in RetroArch ad Andrea. Rilievo: `AUDIO_FRAME_SAMPLES=44100/60` fisso (`game.h:48`, libretro riga 187, runner riga 312) → a 50 Hz musica ~16% lenta.
- **VJ1 percorso [x] (10/11):** lv_01-lv_06 exit 0, 0% fuori strada.
- **VJ2 ponte/riva [~]:** 27 PPM in `UFFICIO/Officina/Test area/vj2_riva_run/vj2_riva/` (lv_01/lv_04/lv_06, exit 0); ispezione visiva ad Andrea.
- **J3 MIG1 [~] bloccato:** fix src D2 `game.cpp:2386` (soglia 0.220 > bound 0.200) e `game.cpp:2390` (clamp 240 vs min_dx_finestra 242). `src/game` invariato.
- **Gate:** `gate_t1.sh` non eseguito questo turno.
