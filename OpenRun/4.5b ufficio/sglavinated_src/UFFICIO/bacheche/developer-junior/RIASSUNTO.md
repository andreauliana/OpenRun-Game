# RIASSUNTO @developer-junior
Riscritto 11/11 11:40. MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1 [x], VJ2 [~], VJ3 [x] 6/6, VJ4 [x] 6/6, VJ5 (∞, un file per turno).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate` in Officina (sola lettura di `src/`).
- **VJ5 11:40:** `sprite_raw.cpp:46-67` scansione palette per pixel al caricamento (consiglio cache colore->indice, incide su V8); `:73-75` errore silenzioso senza distinzione mancante/corrotto.
- **VJ5 11:40 test:** `prova_gate.cpp` check `spriteVisibile` (raw decodificati ma tutti trasparenti): sponsor/rivali/traffico tutti visibili, 0 warning.
- **VJ5 11:19:** `sponsor_loader.cpp:84-110` PNG illeggibile senza log, raw ignora `radice`; caricamento sponsor duplicato in 3 punti (helper core consigliato). `libretro:167-170` check `sec` morto → zero cartelloni in silenzio.
- **VJ5 11:04/10:58:** `libretro:250-254` `retro_reset` senza check null su `g_game`; `:126-134` leak `g_game` su ritorno anticipato. **08:41:** `renderer.cpp:29` reserve(64) basso.
- **VJ4 soak [x]:** lv_01-lv_06 30000f, replay 2x IDENTICO, exit 0, nessun crash (1.471-1.984 ms med, max 84-133 ms).
- **VJ3 50/60 Hz [x]:** 6/6 livelli replay IDENTICO, exit 0; **VJ1 su V6 [x]:** ctest 27/27, rilievo `AUDIO_FRAME_SAMPLES` fisso 735 → 50 Hz musica ~16% lenta.
- **VJ2 ponte/riva [~]:** 27 PPM `Test area/vj2_riva_run/vj2_riva/` (lv_01/04/06), ispezione visiva ad Andrea.
- Prossimo VJ5 ∞: altro file di `src/` (non sprite_raw.cpp, già fatto).
