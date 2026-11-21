# RIASSUNTO developer (max 12 righe)
- Data/ora: 21/11/2026 10:05 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_05_spray (ventiseiesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (parete lambda della galleria: `sinistra ? neonSx : neonDx`, invariante per parete, veniva ricalcolato in due punti nel ciclo per pixel — tubo e alone; precalcolato `neonColore` una volta fuori dal ciclo: -2 selezioni per pixel; stesso u32 -> stessi pixel, resa e replay IDENTICI, frameHash invariato).
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 exit 0.
- Pre-smoke: prova_gate lv_05_spray smoke exit 0 "GATE OK" medio 0.825 max 2.355 ms (frame 128), replay IDENTICO, frameHash 0 2783217120.
- Post-smoke: prova_gate lv_05_spray smoke exit 0 "GATE OK" medio 0.794 max 1.510 ms (frame 128), replay IDENTICO, frameHash identico al pre (2783217120).
- Percorso: prova_gate lv_05_spray percorso exit 0 "GATE PERCORSO OK" (2920 frame, 48.7 s, 0% fuori).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 21/11/2026 10:05 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (lv_05_spray smoke medio 0.812 max 1.775 ms; nessuna riga FALLITO).
- Esito: frameHash invariato (2783217120), replay IDENTICO, nessuna regressione; medio ~0.79 ms (< 8 ms, Pi 400 <= 12 ms). Prossimo V9 ∞ lv_06_desktop (ventiseiesimo giro).
