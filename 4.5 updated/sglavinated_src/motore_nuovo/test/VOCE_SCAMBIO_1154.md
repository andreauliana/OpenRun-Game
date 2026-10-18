# VOCE_SCAMBIO_1154 — @collaboratore @collaudatore, 18/10 09:44
## Esito Gate T1 (smoke)
Comando: `sh tools/gate_t1.sh` dalla radice sglavinated_src. Exit code: 0.
Risultato: GATE T1 TUTTO OK su 3 livelli (lv_01_neon, lv_02_atomico, lv_03_foglie).
- speed@60=161.79 > 0 (tutti i livelli); speed@1=9.90
- offroad raggiunto: frame 278 / 167 / 285
- ms/frame medio 0.35-0.39, max 0.765 (obiettivo <= 12, limite 20)
- replay 2x: IDENTICO su tutti i livelli (determinismo OK)
- avvolgimento playerZ oltre totalLen_: OK (nessuna discesa intermedia)
- inattivita 10s -> show/fumetto visibile: OK
## Estesi (prova_gate modi dedicati)
- accelerazione 0-10s: 9.90 / 160.83 / 246.66 @ frame 1-60-600 — OK
- ribaltamento e ripartenza: OK
- sterzo: sinistra -218.3 / destra +218.3 — OK
- continuita' curve: 1199 campioni, salto pendenza max 0.50 — OK
## Aperti (non bloccanti per il gate)
- frenata (Pad::B) e costa (COAST): implementati in prova_gate.cpp (modi frenata/costa) ma non invocati da gate_t1.sh; script non scrivibile dal collaudatore e shell diretto negato -> numeri runtime mancanti. Richiede intervento su tools/gate_t1.sh o autorizzazione esecuzione diretta.
- H [~]: fps/temperatura Pi 400 (vcgencmd) e v91 guida di Andrea; non misurabili su macOS, in attesa di Andrea.
- Lacune asset: traffico, Gianni, Gesu', UFO (competencia @grafico G1).
## Review T1-RUN punto 7
File rivisti (sola lettura): renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh.
- Colline (punto 2 sviluppatore): road.cpp project non ri-integra hill; camY = camWorldY + CAM_HEIGHT, x relativa al punto sotto l'auto. Confermato.
- Wrap sprite (punto 3 sviluppatore): renderer.cpp drawSprites zRel = fmod(worldZ - playerZ, totalLen). Confermato.
Consegna: gate_t1.log in out/collaudatore/gate_t1.log; screenshot per livello in out/collaudatore/<livello>/.
