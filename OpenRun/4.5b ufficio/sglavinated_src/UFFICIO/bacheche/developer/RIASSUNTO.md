# RIASSUNTO developer (max 12 righe)
- Data/ora: 19/11/2026 12:32 (Turno 3).
- Blocco: V9 ∞ fluidita' lv_03_foglie (ventitreesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp + renderer.h (drawSprite: quattro array per-colonna uniti in un unico array di record ColCamp; un solo puntatore avanza nel loop colonna, campi contigui in cache; stessi valori, frameHash identici).
- Build pre/post: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Percorso pre/post: UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv03_turno42(_post)" lv_03_foglie percorso -> exit 0 "GATE PERCORSO OK" (2392 frame, 39.9 s, 0% fuori).
- Smoke pre: "...v9_lv03_turno42_smoke" -> exit 0 "GATE OK" ms/frame medio 0.999 max 3.573, replay IDENTICO, frameHash 0 3593059909.
- Smoke post: "...v9_lv03_turno42_post_smoke" -> exit 0 "GATE OK" ms/frame medio 1.162 max 5.559, replay IDENTICO, frameHash identici al pre.
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 19/11/2026 12:32 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: caching dei dati per-colonna senza regressioni, frame medio ~1 ms (< 8 ms). Prossimo V9 ∞ lv_04_cromo (ventitreesimo giro).
