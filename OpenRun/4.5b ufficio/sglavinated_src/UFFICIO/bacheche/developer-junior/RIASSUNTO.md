# RIASSUNTO @developer-junior
Riscritto 11/11 10:21. MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1 [x], VJ2 [~], VJ3 [x] 6/6, VJ4 [~] 4/6, VJ5 review fatta.
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate` in Officina (sola lettura di `src/`).
- **VJ4 soak [~] (11/11):** lv_01_neon 09:49 exit 0, 30000f IDENTICO, 1.984/133.015. lv_02_atomico 10:01 exit 0, 30000f IDENTICO, 1.807/96.036. lv_03_foglie 10:11 exit 0, 30000f IDENTICO, 1.715/84.185. lv_04_cromo 10:21 exit 0 GATE OK, 30000f IDENTICO, 1.573/124.742, nessun crash.
- **VJ3 50/60 Hz [x]:** 6/6 livelli exit 0, replay IDENTICO a 50 e 60 Hz.
- **VJ5 (11/11 08:41):** `renderer.cpp:29` `terraOrdine_.reserve(64)` basso vs ~63 oggetti/300 segmenti mare → possibile realloc; consiglio `reserve(Road::DRAW_DIST + 8)`.
- **VJ1 su V6 [x]:** solo `src/libretro/...`; ctest 27/27. Rilievo: `AUDIO_FRAME_SAMPLES` fisso 735 → a 50 Hz musica ~16% lenta.
- **VJ2 ponte/riva [~]:** 27 PPM in `Test area/vj2_riva_run/vj2_riva/` (lv_01/lv_04/lv_06); ispezione visiva ad Andrea.
- **J3 MIG1 [~]:** bloccato fix src @developer (`game.cpp:2386` soglia 0.220>0.200; `game.cpp:2390` clamp 240 vs 242).
- **VJ1 percorso [x]:** lv_01-lv_06 exit 0 (0% fuori).
- **Prossimo:** VJ4 soak lv_05_spray, poi lv_06_desktop. gate_t1.sh non eseguito questo turno.
- Mai: HUD, `motore_nuovo/src/` senza autorizzazione, file fuori perimetro.
