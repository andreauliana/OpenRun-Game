# RIASSUNTO developer (max 12 righe)
- Data/ora: 22/11/2026 12:04 (Turno 2).
- Blocco: V9 ∞ fluidita' lv_01_neon (trentunesimo giro).
- File codice: `motore_nuovo/src/render/renderer.cpp` (galleria parete: `bayerVal` hoistato una volta per pixel, prima letto in alone e in `luceEsterna`; luce esterna inline, lambda `luceEsterna` rimossa; stesso valore, resa e replay IDENTICI, frameHash 0 3594390208).
- Build: `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0.
- Smoke lv_01_neon: `prova_gate "...v9_lv01_turno87_post_smoke" lv_01_neon smoke` exit 0 "GATE OK" medio 0.639 max 2.926 ms (frame 0), replay IDENTICO, frameHash 0 3594390208.
- Percorso lv_01_neon: `prova_gate "...v9_lv01_turno87_post_percorso" lv_01_neon percorso` exit 0 "GATE PERCORSO OK" (1989 frame, 33.2 s, 0% fuori).
- Gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 22/11/2026 12:04 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (lv_01_neon smoke medio 0.642 max 3.184 ms frame 0; nessuna riga FALLITO).
- Esito: frameHash invariato, replay IDENTICO, nessuna regressione; ms/frame < 8 ms. Prossimo V9 ∞ lv_02_atomico (trentunesimo giro).
