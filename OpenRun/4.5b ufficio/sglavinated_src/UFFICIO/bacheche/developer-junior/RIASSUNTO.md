# RIASSUNTO @developer-junior
Riscritto 11/11 12:07. MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1 [x], VJ2 [~], VJ3 [x] 6/6, VJ4 [x] 6/6, VJ5 (∞, un file per turno).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate` in Officina (sola lettura di `src/`).
- **VJ5 12:07:** review `src/render/renderer.cpp` da1c8c6 (V7): `terraOrdine_.reserve(64)`/`spriteOrd_.reserve(512)` (`:29-30`) ma `spriteOrd_.resize(sprites.size())` a frame (`:1325`) può riallocare se >512 sprite visibili. Modificato `prova_gate.cpp` nota VJ5; build exit 0; `prova_gate ... vj5_renderer_1207 lv_01_neon caricamento` exit 0 GATE CARICAMENTO OK.
- **VJ5 11:47:** review cambi V8 `src/game/game.cpp` (da1c8c6): init 8-15 ms, avviaGara 7-8 ms, cambio tappa 3-11 ms; rilievi `:1196` return senza log, fallback livello `:500-510`, doppio caricamento `:1205`, commento `:843-846`.
- **VJ5 11:52:** `libretro:163-182` stile cartelloni dipendente dall'ordine del filesystem; `sec` morto, nessun log se cartella manca. **11:40:** `sprite_raw.cpp:46-67` palette per pixel. **11:19:** `sponsor_loader` senza log. **10:58/11:04:** null-deref `retro_reset`, leak `g_game`.
- **VJ4 soak [x]:** lv_01-lv_06 30000f replay 2x IDENTICO, exit 0. **VJ3 [x]:** 6/6 livelli 50/60 Hz IDENTICO. **VJ1 V6 [x]:** ctest 27/27, rilievo `AUDIO_FRAME_SAMPLES` 735 → 50 Hz musica lenta.
- **VJ2 ponte/riva [~]:** 27 PPM `Test area/vj2_riva_run/vj2_riva/`, ispezione visiva ad Andrea.
- Prossimo VJ5 ∞: altro file di `src/` (non game.cpp, sprite_raw.cpp, libretro, sponsor_loader, renderer.cpp già fatti). gate_t1.sh non eseguito.