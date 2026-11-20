# RIASSUNTO developer (max 12 righe)
- Data/ora: 20/11/2026 14:13 (Turno 3).
- Blocco: V9 ∞ fluidita' lv_02_atomico (venticinquesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (parete lambda della galleria: `sinistra = xBordoF > xBordoN` e `neonOn = sinistra ? neonSxOn : neonDxOn`, invarianti per parete — dipendono solo dai bordi del muro e dallo stato dei neon del trapezio, non dal pixel x — venivano ricalcolati a ogni pixel del muro; calcolati una volta fuori dal ciclo per pixel: -1 confronto e -1 selezione per pixel nel tubo di lv_02; stessi bool -> stessi pixel, resa e replay IDENTICI, frameHash identici).
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 -> exit 0.
- Percorso: prova_gate "...v9_lv02_turno62_post" lv_02_atomico percorso -> exit 0 "GATE PERCORSO OK" (2476 frame, 41.3 s, 0% fuori).
- Smoke: "...v9_lv02_turno62_post_smoke" exit 0 "GATE OK" medio 0.793 max 3.168 ms (frame 0), replay IDENTICO, frameHash 0 1240481121 (uguale al riferimento storico -> resa invariata).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 20/11/2026 (Turno 3) -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: resa e replay identici (frameHash uguale al riferimento), nessuna regressione, frame medio ~0.79 ms (< 8 ms, Pi 400 <= 12 ms). Prossimo V9 ∞ lv_03_foglie (venticinquesimo giro).
