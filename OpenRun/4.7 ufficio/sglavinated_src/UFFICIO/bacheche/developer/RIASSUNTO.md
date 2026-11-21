# RIASSUNTO developer (max 12 righe)
- Data/ora: 21/11/2026 12:32 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_04_cromo (ventottesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawTerraSospesa ramo spiaggia/sabbia sfumata: `BAYER4[y&3]` invariante per riga, prima ricalcolato a ogni pixel; precalcolato `bayerRigaSpiaggia` una volta per riga — stessi indici Bayer, resa e replay IDENTICI, frameHash 0 4118572995).
- Pre: smoke lv_04_cromo exit 0 "GATE OK" medio 0.694 max 2.906 ms (frame 0), replay IDENTICO, frameHash 0 4118572995; percorso exit 0 "GATE PERCORSO OK" (2125 frame, 35.4 s, 0% fuori).
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 exit 0.
- Post: smoke exit 0 "GATE OK" medio 0.657 max 2.887 ms (frame 0), replay IDENTICO, frameHash identici al pre; percorso exit 0 "GATE PERCORSO OK" (2125 frame, 35.4 s, 0% fuori).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 21/11/2026 12:32 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (lv_04_cromo smoke medio 0.665 max 3.049 ms frame 0; nessuna riga FALLITO).
- Esito: frameHash invariato, replay IDENTICO, nessuna regressione; ms/frame < 8 ms (Pi 400 limite 12 ms). Prossimo V9 ∞ lv_05_spray (ventottesimo giro).
