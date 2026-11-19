# RIASSUNTO developer (max 12 righe)
- Data/ora: 19/11/2026 13:14 (Turno 3).
- Blocco: V9 ∞ fluidita' lv_05_spray (ventitreesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawSprite: texel dell'area di ricerca letti con puntatore che avanza di 1 per colonna e di sw per riga invece di rigaCerca[rx] a ogni campione; stessi indici, frameHash identici).
- Build pre/post: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Percorso pre/post: UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno44(_post)" lv_05_spray percorso -> exit 0 "GATE PERCORSO OK" (2920 frame, 48.7 s, 0% fuori).
- Smoke pre: "...v9_lv05_turno44_pre_smoke" -> exit 0 "GATE OK" ms/frame medio 0.943 max 3.340, replay IDENTICO, frameHash 0 4250821778.
- Smoke post: "...v9_lv05_turno44_post_smoke" -> exit 0 "GATE OK" ms/frame medio 0.957 max 3.287, replay IDENTICO, frameHash identici al pre.
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 19/11/2026 13:14 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: accesso ai texel della ricerca tramite puntatore incrementale nel loop caldo senza cambiare la resa, frame medio ~0.95 ms (< 8 ms). Prossimo V9 ∞ lv_06_desktop (ventitreesimo giro).
