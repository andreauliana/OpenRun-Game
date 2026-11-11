# Riassunto developer — 11/11/2026 10:19 (Turno 2)

- Stato: MIG1 D1-D5 [x], V47 V1-V5 V7 V8 [x]. V6 [~] build OK. V9 ∞ in corso (lv_01-lv_05 fatti; prossimo lv_06_desktop).
- V6 libretro: `motore_nuovo/src/libretro/sglavinated_libretro.cpp` con sprite raw rivali/traffico/giocatore, partenza 3-2-1, Splash->Menu, 50/60 Hz, salvataggi/cwd. Build OK (11/11 10:10): cmake -DBUILD_LIBRETRO=ON exit 0, build target exit 0 (sglavinated_libretro.dylib). Manca vs runner: cartelloni sponsor da PNG (core senza SDL2_image; usa solo raw/dati interni).
- V9 (questo turno, 11/11 10:19): lv_05_spray percorso exit 0 GATE PERCORSO OK (2659 frame, 0% fuori strada); smoke exit 0 GATE OK ms/frame medio 1.198 max 3.808 (<8 ms), replay IDENTICO, nessun alleggerimento.
- V9 precedenti: lv_04 2068 0% 1.471/8.913, lv_03 2273 0% 1.088/3.950, lv_02 2267 0% 1.096/4.013, lv_01 1941 0% (sotto 8 ms salvo spike/max noti in log). Nessun cambio resa.
- Prove globali recenti (11/11): run_tests.sh exit 0 ctest 27/27, gate_t1.sh exit 0 "GATE T1: TUTTO OK" 6/6. Questo turno: gate_t1.sh non eseguito.
- Prossimo: V9 lv_06_desktop (percorso+smoke) oppure affinare gap V6.
- Nota @developer-junior: VJ1 resta (prova dopo ogni passo V + riga in REVIEW_V47.md).
