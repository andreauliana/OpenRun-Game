# RIASSUNTO developer (max 12 righe)
- Data/ora: 21/11/2026 08:18 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_03_foglie (ventiseiesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawSegmentTrapezoid, fascia di cambio tema: soglia Bayer `((BAYER4+0.5f)/16.0f) < misto` con `misto` invariante per riga -> precalcolata `sogliaBayer = misto*16` una volta per riga, confronto `(BAYER4+0.5f) < sogliaBayer`; divisione e prodotto per potenze di 2 esatti -> stessa resa, -1 divisione per pixel).
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Pre smoke: prova_gate "...v9_lv03_turno63_pre_smoke" lv_03_foglie smoke exit 0 "GATE OK" medio 0.900 max 3.340 ms, frameHash 0 3593059909 (riferimento storico).
- Post smoke: prova_gate "...v9_lv03_turno63_post_smoke" lv_03_foglie smoke exit 0 "GATE OK" medio 0.897 max 3.253 ms, replay IDENTICO, frameHash identici al pre.
- Percorso: prova_gate "...v9_lv03_turno63_post" lv_03_foglie percorso exit 0 "GATE PERCORSO OK" (2392 frame, 39.9 s, 0% fuori).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 21/11/2026 08:18 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: resa e replay identici (frameHash invariato), nessuna regressione, medio ~0.89 ms (< 8 ms, Pi 400 <= 12 ms). Prossimo V9 ∞ lv_04_cromo (ventiseiesimo giro).
