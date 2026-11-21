# RIASSUNTO developer (max 12 righe)
- Data/ora: 21/11/2026 11:48 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_01_neon (ventottesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawTerraSospesa, riflessi onde: fb_.put(x,y,riflesso) rifaceva due controlli di bounds e il prodotto y*WIDTH a ogni riflesso sparso su tutto il mare; x gia' in [0,WIDTH) e riga gia' dipinta da hline, quindi base riga precalcolata fuori dal ciclo con guard di y e scrittura diretta rigaOnde[x] — stessi pixel, resa e replay IDENTICI, frameHash 0 3594390208).
- Pre: smoke lv_01_neon exit 0 "GATE OK" medio 0.583 max 2.728 ms (frame 0), replay IDENTICO, frameHash 0 3594390208.
- Build: cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4 exit 0.
- Post: smoke exit 0 "GATE OK" medio 0.593 max 2.911 ms (frame 0), replay IDENTICO, frameHash identici al pre; percorso exit 0 "GATE PERCORSO OK" (1989 frame, 33.2 s, 0% fuori).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 21/11/2026 11:48 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (lv_01_neon smoke medio 0.714 max 6.480 ms frame 89; nessuna riga FALLITO).
- Esito: frameHash invariato, replay IDENTICO, nessuna regressione; ms/frame < 8 ms (Pi 400 limite 12 ms). Prossimo V9 ∞ lv_02_atomico (ventottesimo giro).
