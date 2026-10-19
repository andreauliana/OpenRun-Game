# RIASSUNTO @developer/@developer-junior — 17 ott 2026 16:20
- T1-RUN punti 1-3 fatti e verificati col gate:
  1) player.h: OFFROAD_SLOW 0.85f -> 0.99f (fuoristrada non piu' brusco: decadenza ~1%/frame a 50 Hz).
  2) road.cpp project(): collina/curva non ri-integrate; quote assolute p1x/p2x p1y/p2y da build(); camera = punto sotto l'auto + CAM_HEIGHT; ascissa relativa al punto sotto l'auto (strada non sale piu' nel cielo).
  3) renderer.h/cpp + game.cpp: drawSprites(..., totalLen); zRel = (worldZ - playerZ) con modulo totalLen (sprite non spariscono al wrap).
- Build `cmake -S motore_nuovo -B build_gate` + `cmake --build build_gate --target prova_gate`: OK.
- `sh tools/gate_t1.sh` exit 0 su lv_01_neon, lv_02_atomico, lv_03_foglie: GATE T1 TUTTO OK; speed@60=33.76>0; replay 2x IDENTICO; ms/frame medio 0.071-0.073 max 0.199 (limite 20); offroad raggiunto f.475-476; avvolgimento e inattivita OK.
- Rimanente: T1-RUN punto 4 (sprite veri nel gate headless: richiede caricatore PNG minimo core o PNG->raw in strumenti_py; prova_gate e' di @debugger). Punto 8 (Andrea v91 + fps/temp Pi 400) non eseguibile qui.
- Stati proposti: C [x], T1-RUN [~], H [~], P1 [~]. Voce in out/coder/VOCE_SCAMBIO_1146.md (scrittura motore_nuovo/test/ negata).
