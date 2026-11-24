# RIASSUNTO developer (max 12 righe)
- Data/ora: 24/11/2026 15:21 (Turno 2).
- Blocco: V9 ∞ fluidita' lv_03_foglie (trentasettesimo giro).
- Baseline pre: percorso "UFFICIO/Officina/Test area/v9_lv03_giro37_pre_percorso" exit 0 "GATE PERCORSO OK" (2392 frame, 39.9 s, 0% fuori); smoke "UFFICIO/Officina/Test area/v9_lv03_giro37_pre_smoke" exit 0 "GATE OK" medio 0.637 max 2.816 ms frame 0, replay IDENTICO, frameHash 0 3593059909.
- Modifica: `motore_nuovo/src/render/renderer.cpp` (drawSegmentTrapezoid: hoist `pxW` incrocio fuori dal loop sui rami, dipende da hw/fattore per riga non da r; stesso valore, resa/replay IDENTICI).
- Build: `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0.
- Post: smoke "UFFICIO/Officina/Test area/v9_lv03_giro37_post_smoke" exit 0 "GATE OK" medio 0.640 max 2.740 ms frame 0, replay IDENTICO, frameHash 0 3593059909 (9/9); percorso "UFFICIO/Officina/Test area/v9_lv03_giro37_post_percorso" exit 0 "GATE PERCORSO OK" (2392 frame, 39.9 s, 0% fuori).
- Gate: gate_t1.sh non eseguito in questo turno; riferimento ultimo 24/11/2026 10:03 exit 0 "GATE T1: TUTTO OK" 6/6.
- Esito: frame sotto 8 ms, resa/replay identici, nessuna regressione osservata.
- Prossimo V9 ∞: lv_04_cromo (trentasettesimo giro).
