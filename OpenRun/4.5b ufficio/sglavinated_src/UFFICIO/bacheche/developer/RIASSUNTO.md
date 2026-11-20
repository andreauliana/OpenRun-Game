# RIASSUNTO developer (max 12 righe)
- Data/ora: 20/11/2026 11:22 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_05_spray (ventiquattresimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawSprite: testo `inTubo(scrX, screenYRow)` per pixel sostituito con accesso diretto `tuboRiga[scrX]` su base di riga di `tubo_` precalcolata una volta per riga schermo; `clipTubo = clipAttivo_ && dentroGalleria_` costante in tutto drawSprite; coordinate gia' ritagliate al framebuffer, stessi pixel e frameHash).
- Build pre/post: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Percorso pre/post: prova_gate "...v9_lv05_turno50_pre"/"_post" lv_05_spray percorso -> exit 0 "GATE PERCORSO OK" (2920 frame, 48.7 s, 0% fuori).
- Smoke pre: "...v9_lv05_turno50_pre_smoke" -> exit 0 "GATE OK" medio 0.809 max 3.390 ms, replay IDENTICO, frameHash 0 4250821778.
- Smoke post: "...v9_lv05_turno50_post_smoke" -> exit 0 "GATE OK" medio 0.796 max 3.184 ms, replay IDENTICO, frameHash identici al pre.
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 20/11/2026 11:22 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: frame medio ~0.80 ms (< 8 ms), max al frame 0 di caricamento; nessun punto lento. Prossimo V9 ∞ lv_06_desktop (ventiquattresimo giro).
