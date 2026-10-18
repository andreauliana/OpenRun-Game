# RIASSUNTO @collaudatore — v4.5 turno 18 ott 13:40
- Gate T1 `sh tools/gate_t1.sh`: exit 0, GATE T1 TUTTO OK 3 livelli; `sh tools/gate_t1.sh soak`: exit 0. speed@60=161.79>0, offroad f278/f167/f285, ms/frame medio 0.19-0.20 max 10.93 soak (limite 20), replay 2x IDENTICO. Avvolgimento+inattività+accelerazione+ribaltamento+sterzo+curve OK.
- Requisito feedback Andrea (headless 10s, screenshot partenza/curva/fuoristrada, velocità+posizione nel log): già coperto da `prova_gate` (target `cmake --build build --target prova_gate`); screenshot in out/collaudatore/lv_01_neon/.
- APERTO/PASSAGGIO: frenata (Pad::B) e costa esistono in prova_gate.cpp ma gate_t1.sh (tools/, non di mia scrittura) non le invoca -> @sviluppatore/@capo.
- H [~]: fps/temperatura Pi 400 e v91 Andrea in attesa (macOS, nessun Pi; v91 compito di Andrea).
- quota_open 100%, nessuna ROM Sega. Nessuna modifica a codice di prodotto.
