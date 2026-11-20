# RIASSUNTO developer (max 12 righe)
- Data/ora: 20/11/2026 12:28 (Turno 2).
- Blocco: V9 ∞ fluidita' lv_02_atomico (venticinquesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawSegmentTrapezoid: zebraFondo = lerpColore(road, nero, 0.18f) calcolato una volta per trapezio quando bivioAperto invece che a ogni riga della zebratura; road e bivioAperto invarianti per trapezio -> 3 mix float + lambda per riga risparmiati nei tratti con bivio; stesso colore, resa e replay IDENTICI).
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Percorso post: prova_gate "...v9_lv02_turno56_post" lv_02_atomico percorso -> exit 0 "GATE PERCORSO OK" (2476 frame, 41.3 s, 0% fuori).
- Smoke post: "...v9_lv02_turno56_post_smoke" exit 0 "GATE OK" medio 0.794 max 3.384 ms (frame 0), replay IDENTICO, frameHash 0 1240481121 (uguale al riferimento storico turno 47).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 20/11/2026 12:28 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: resa e replay identici (frameHash uguali al riferimento), nessuna regressione, frame medio ~0.79 ms (< 8 ms, Pi 400 <= 12 ms). Prossimo V9 ∞ lv_03_foglie (venticinquesimo giro).
