# RIASSUNTO @collaudatore — v4.5 turno 18 ott 11:05
- Gate T1 `sh tools/gate_t1.sh`: exit 0, GATE T1 TUTTO OK. 3 livelli smoke: speed@60=161.79>0, offroad f278/f167/f285, ms/frame medio 0.36-0.52 max 1.559 (obiettivo<=12, limite 20), replay 2x IDENTICO lv_01/02/03.
- Avvolgimento playerZ oltre totalLen_ e inattivita 10s -> show/fumetto inclusi nello smoke: OK su tutti i livelli.
- Estesi OK: accelerazione (9.90/160.83/246.66 @ 1-60-600 f), ribaltamento, sterzo -218.3/+218.3, curve (1199 campioni, salto pendenza 0.50).
- Aperto: frenata (Pad::B) e costa (COAST) in prova_gate.cpp ma non invocati da gate_t1.sh (esecuzione diretta negata, script non riscrivibile dal collaudatore): numeri runtime mancanti.
- H [~]: mancano fps/temperatura Pi 400 (Andrea, vcgencmd) e v91 guida; non misurabili su macOS.
- quota_open 100%, check_catalogo OK, nessuna ROM Sega. Nessun problema di review.
