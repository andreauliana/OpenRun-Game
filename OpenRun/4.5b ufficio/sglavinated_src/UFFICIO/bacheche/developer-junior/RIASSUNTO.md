# RIASSUNTO @developer-junior
Riscritto 11/11 08:41. MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1 [x] (su V5 e V6), VJ2 [~], VJ3 [~] 4/6 livelli.
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate` in Officina (sola lettura di `src/`).
- **VJ3 50/60 Hz [~] (11/11 08:41):** `cmake --build ... --target prova_gate` exit 0, poi `prova_gate vj3_freq_1111b <lv> frequenza 60|50` exit 0 su **lv_03_foglie** 37.88 vs 37.88 s e **lv_04_cromo** 34.47 vs 34.48 s; replay IDENTICO, diff 50/60 <=0.03%, ms/frame medio 1.37-1.87, config.json ripristinato. Da fare: lv_05, lv_06 (2 per turno), poi VJ4 soak.
- **VJ5 (11/11 08:41):** su V7 di @developer, `src/render/renderer.cpp:29` `terraOrdine_.reserve(64)` sotto l'attesa (~63 oggetti in 300 segmenti di mare, `paesaggio.cpp:244-287`) → rialloca a riga 141; consiglio `reserve(Road::DRAW_DIST + 8)`. Minore: `spriteOrd_.reserve(512)` vs `game.cpp:3084`.
- **VJ1 su V6 [x]:** V6 tocca solo `src/libretro/...`; regressione sul gioco comune esclusa (ctest 27/27). Prova del core libretro non fattibile con `prova_gate` → RetroArch ad Andrea.
- **Rilievo V6:** audio fisso `AUDIO_FRAME_SAMPLES=44100/60` (`game.h:48`) con `sample_rate` 44100 (`sglavinated_libretro.cpp:187`): a 50 Hz musica ~16% lenta. Vale anche per `runner/main.cpp:312`. Per @developer.
- **VJ2 ponte/riva [~]:** 27 PPM in `Test area/vj2_riva_run/vj2_riva/` (lv_01/lv_04/lv_06, exit 0); ispezione visiva ad Andrea.
- **J3 MIG1 [~]**: bloccato fix src @developer (`game.cpp:2386` soglia D2 0.220 > bound 0.200; `game.cpp:2390` clamp 240 vs min_dx_finestra 242).
- **VJ1 percorso [x]**: lv_01-lv_06 exit 0 (0% fuori).
- REVIEW: `REVIEW_V47.md` riscritto (VJ3 lv_03/lv_04, VJ5 su V7, VJ1/V6, rilievo audio). gate_t1.sh non eseguito questo turno (ultimo exit 0 11/11 ~08:10, @developer).
- Mai: HUD, `motore_nuovo/src/` senza autorizzazione, file fuori perimetro.
