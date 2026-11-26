# REVIEW V47 — VJ5 ∞ (26/11/2026, turno 17:59)

- **Ruolo**: @developer-junior (review/collaudo; perimetro test consentito). Nessun cambio nuovo in `src/` (HEAD `c146cb5`, renderer.cpp 10:11:16, `git diff 8ab971a c146cb5 -- renderer.cpp` = soliti 11 hunk gia' review).

## VJ5 ∞ — angolo: driver gate `pali` (prova_gate.cpp) su lv_01 vs lv_06
- **Contesto**: turno 17:46 aveva trovato gate `pali` lv_06 FALLITO exit 27 (hit_rilevati=0) con driver tarato lv_01. In questo turno ho tentato fix nel perimetro test (`motore_nuovo/test/prova_gate.cpp:2604-2613`).
- **Tentativo (revertito)**: driver adattivo — se offroad&&speed<150 rientra, se x<880 spingi a destra, fascia 880-1200 dritto, se x>1200 rientra. Build: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0.
  - `prova_gate .../vj5_pali_lv01_fix2_2611_1808 lv_01_neon pali` -> **exit 27** REGRESSIONE: x stabilizzato 864-888, speed 238-242, offroad=0 ma hit_rilevati=0 (troppo vicino a PALO_X=860, ratio 3% non rilevato; driver vecchio otteneva drift naturale a x=1532, 10 hit).
  - `prova_gate .../vj5_pali_lv06_fix_2611_1803 lv_06_desktop pali` -> **exit 27**: vel 72-92, x 677-710, hit 0. Soglia `spPrima>150.0f` (prova_gate.cpp:2624) mai vera su lv_06 con questo driver.
- **Ripristino**: driver originale ripristinato identico; `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0.
  - `prova_gate .../vj5_pali_lv01_revert_2611_1812 lv_01_neon pali` -> **exit 0** (GATE PALI OK, hit 10, frame_primo_hit 1931, ratio 0.9701) — conferma nessuna regressione.
- **Diagnosi (errore/limite test, non di gioco)**: `verificaPali` (prova_gate.cpp:2583-2655) dipende da (a) raggiungere `|xRel|>=860` (game.cpp:3641-3642) e (b) rilevare calo speed 3% con `spPrima>150` (:2624). Su lv_01 il cap a 700 lascia drift naturale fino a x~1532 in curva ad alta velocita' -> funziona. Su lv_06 la geometria/offroad tiene vel 72-92 al bordo: condizione (b) irraggiungibile col driver attuale. Non regressione di gioco (src invariato, ctest verde).
- **Rilievo per @capo/@developer**: dichiarare gate `pali` lv_01-only nel runner/ctest, oppure progettare driver che costruisca speed>150 in rettilineo e poi attraversi |xRel|>=860 con slancio, con prova di non-regressione su lv_01.

## Rilievi invariati
- **RILIEVO BLOCCANTE** (procedurale): 5 hunk in `drawPlayerCar` renderer.cpp (:2798-2816,:2875-2889,:2890-2905,:2910-2922,:2932-2952), REGOLA BLOCCATA Andrea 06/11 -> autorizzazione o revert. Area grigia `drawEffettiAuto` non bloccante.
- **VJ3**: lv_05_spray 2.40% FALLITO (deterministico, exit 22) -> aperto per @developer. **VJ4**: chiuso 6/6.

## Prove (26/11/2026 17:59-18:15)
- `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0 (piu' volte, durante tentativo/revert).
- `ctest --test-dir UFFICIO/Officina/build --output-on-failure` -> **exit 0** (27/27, 163.99 s).
- `prova_gate .../vj5_pali_lv01_revert_2611_1812 lv_01_neon pali` -> **exit 0** (GATE PALI OK, hit 10).
- `prova_gate .../vj5_pali_lv06_fix_2611_1803 lv_06_desktop pali` -> **exit 27** (FALLITO, hit 0).
- `gate_t1.sh`: **non eseguito** in questo turno.