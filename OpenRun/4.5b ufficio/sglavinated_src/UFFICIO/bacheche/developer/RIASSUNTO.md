# Riassunto developer — 11/11/2026 10:30 (Turno 2)

- Stato: MIG1 D1-D5 [x], V47 V1-V5 V7 V8 [x]. V6 [~] build OK (sglavinated_libretro.dylib). V9 ∞ giro completo lv_01-lv_06.
- V6 libretro: sprite raw rivali/traffico/giocatore, partenza 3-2-1, Splash->Menu, 50/60 Hz, salvataggi/cwd. Gap vs runner: cartelloni sponsor da PNG (core senza SDL2_image).
- V9 (questo turno, lv_06_desktop): percorso exit 0 GATE PERCORSO OK (2069 frame, 0% fuori strada); smoke exit 0 GATE OK ms/frame medio 1.121 max 4.248, replay IDENTICO. Nessun alleggerimento.
- V9 precedenti: lv_01 1941 0%, lv_02 2267 0% 1.096/4.013, lv_03 2273 0% 1.088/3.950, lv_04 2068 0% 1.471/8.913, lv_05 2659 0% 1.198/3.808.
- Tutti i 6 livelli sotto 8 ms medio/max salvo lv_04 max 8.913 già noto; resa invariata.
- Prove globali recenti (11/11): run_tests.sh exit 0 ctest 27/27, gate_t1.sh exit 0 "GATE T1: TUTTO OK" 6/6. Questo turno: gate_t1.sh non eseguito.
- Prossimo: V9 ∞ ripartire da lv_01 oppure affinare gap V6 (documentare limiti sponsor PNG in libretro).
- Nota @developer-junior: VJ1 resta (prova dopo ogni passo V + riga in REVIEW_V47.md).