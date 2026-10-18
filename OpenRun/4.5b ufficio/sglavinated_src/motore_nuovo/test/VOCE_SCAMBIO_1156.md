# VOCE_SCAMBIO_1156 — @collaudatore, 18/10 11:05
## Esito Gate T1 (smoke)
Comando: `sh tools/gate_t1.sh` dalla radice sglavinated_src. Exit code: 0.
Risultato: GATE T1 TUTTO OK su 3 livelli (lv_01_neon, lv_02_atomico, lv_03_foglie).
- speed@60=161.79 > 0 (tutti i livelli); speed@1=9.90
- offroad raggiunto: frame 278 / 167 / 285
- ms/frame medio 0.36-0.52, max 1.559 (obiettivo <= 12, limite 20)
- replay 2x: IDENTICO su tutti i livelli (determinismo OK)
- avvolgimento playerZ oltre totalLen_: OK (nessuna discesa intermedia)
- inattivita 10s -> show/fumetto visibile: OK
## Estesi (modi dedicati di prova_gate invocati da gate_t1.sh)
- accelerazione 0-10s: 9.90 / 160.83 / 246.66 @ frame 1-60-600 — OK
- ribaltamento e ripartenza: OK
- sterzo: sinistra -218.3 / destra +218.3 — OK
- continuita' curve: 1199 campioni, salto pendenza max 0.50 — OK
## Aperti (non bloccanti per il gate)
- frenata (Pad::B) e costa (COAST): implementati in prova_gate.cpp (modi frenata/costa) ma non invocati da gate_t1.sh; esecuzione diretta di ./build_gate/prova_gate negata e script non riscrivibile dal collaudatore -> numeri runtime mancanti. Richiede modifica di tools/gate_t1.sh o autorizzazione esecuzione diretta.
- H [~]: fps/temperatura Pi 400 (vcgencmd) e v91 guida di Andrea; non misurabili su macOS, in attesa di Andrea.
- Lacune asset: traffico, Gianni, Gesu', UFO (competenza @grafico G1).
## Review T1-RUN punto 7
File rivisti (sola lettura): renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh.
- Colline (punto 2 sviluppatore): confermato road.cpp project senza ri-integrazione hill; camY = camWorldY + CAM_HEIGHT, x relativa al punto sotto l'auto.
- Wrap sprite (punto 3 sviluppatore): confermato drawSprites zRel = fmod(worldZ - playerZ, totalLen).
Consegna: gate_t1.log in out/collaudatore/gate_t1.log; screenshot per livello in out/collaudatore/<livello>/.
