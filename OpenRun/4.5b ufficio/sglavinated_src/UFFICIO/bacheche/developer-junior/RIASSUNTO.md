# RIASSUNTO @developer-junior
Riscritto 11/11 12:55. MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1 [x], VJ2 [~], VJ3 [x] 6/6, VJ4 [x] 6/6, VJ5 (∞, un file per turno).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate` in Officina (sola lettura di `src/`).
- **VJ5 12:55:** review `src/game/salvataggi.cpp/.h` (creaDirectorySeManca false su EEXIST/probe inaffidabile, salva non normalizza classifica come carica). Nota in `prova_gate.cpp:102`. `cmake --build ... prova_gate` exit 0; `prova_gate ... vj5_salvataggi_1255 lv_01_neon caricamento` exit 0 GATE CARICAMENTO OK (init 8.3 ms).
- **VJ5 12:49:** review `src/libretro/sglavinated_libretro.cpp` sponsor raw (ordine fs non deterministico, split '_' fragile). REVIEW_V47 riscritto.
- **VJ2 12:32:** screenshot vj2_riva lv_01/lv_04/lv_06 rigenerati; densità `paesaggio.cpp` moderata, ispezione visiva ad Andrea confermata.
- **VJ4 soak [x]:** lv_01-lv_06 30000f replay 2x IDENTICO. **VJ3 [x]:** 6/6 livelli 50/60 Hz IDENTICO. **VJ1 V6 [x]:** ctest 27/27.
- Prossimo VJ5 ∞: altro file di `src/` non ancora revisionato. gate_t1.sh non eseguito.
