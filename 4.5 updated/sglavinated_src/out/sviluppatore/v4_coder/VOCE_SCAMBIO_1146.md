### 1146. CODER -> CAPO/ANDREA | stato [~] | 17 ott 2026 16:20
Blocco T1-RUN (RICHIESTA.md): punti 1-3 eseguiti e verificati col gate.
- 1) Guida/offroad (player.h): OFFROAD_SLOW 0.85f -> 0.99f. Il fuoristrada non crolla piu' da ~160 a ~6 in pochi frame; decadenza gentile (~1%/frame a 50 Hz). Accelerazione esponenziale, freno B/Down, attrito, sterzo vs velocita', spinta centrifuga gia' presenti.
- 2) Colline (road.cpp project()): collina/curva non piu' ri-integrate (la doppia integrazione faceva salire la strada nel cielo); si usano le quote assolute p1x/p2x, p1y/p2y già costruite in build(). Camera = punto sotto l'auto + CAM_HEIGHT; ascissa relativa al punto sotto l'auto.
- 3) Wrap sprite (renderer.h/cpp, game.cpp): drawSprites accetta totalLen; zRel = (worldZ - playerZ) avvolto con modulo totalLen, cosi' gli sprite non spariscono vicino al punto di wrap di playerZ.
Build: `cmake -S motore_nuovo -B build_gate -DCMAKE_BUILD_TYPE=Release` + `cmake --build build_gate --target prova_gate`: OK (core e prova_gate ricompilati).
Gate: `sh tools/gate_t1.sh` exit 0 su lv_01_neon, lv_02_atomico, lv_03_foglie. GATE T1: TUTTO OK. speed@60=33.76 (>0); replay 2x IDENTICO su tutti i livelli; ms/frame medio 0.071-0.073 max 0.199 (limite 20); offroad raggiunto frame 475-476; avvolgimento playerZ oltre totalLen OK (wrapCount=4); inattivita show/fumetto OK. 3 screenshot PPM per livello in out/debugger/<livello>/.
Rimanente (T1-RUN punto 4): sprite veri nel gate headless (setSponsorSprites e' chiamato solo dal runner SDL2_image; prova_gate disegna ancora rettangoli). Richiede caricatore PNG minimo nel core o conversione PNG->raw in strumenti_py (nessuna nuova dipendenza). Non fatto in questo turno. Punto 8 (Andrea v91 + fps/temperatura Pi 400) non eseguibile qui (regole 9/21).
Comandi negati: esecuzione diretta di ./build_gate/prova_gate e rm -rf build_gate (permessi); si usa solo sh tools/gate_t1.sh.
Stati proposti: C [x], T1-RUN [~] (punto 4 aperto), H [~] (in attesa v91 Andrea), P1 [~].
Prossimo: sprite veri nel gate (punto 4); poi Andrea/Claude incollano questa voce.
