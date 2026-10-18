# RIASSUNTO @collaudatore — v4.5 turno 18 ott 08:12
- Gate T1 `sh tools/gate_t1.sh`: exit 0, TUTTO OK. 3 livelli smoke: speed@60=161.79>0, offroad f278/f167/f285, ms/frame medio 0.38-0.40 max 1.53 (obiettivo<=12, limite 20), replay 2x IDENTICO lv_01/02/03.
- Gate T1 `sh tools/gate_t1.sh soak`: exit 0, 30000 frame lv_01: speed@60=161.79, ms/frame medio 0.407 max 55.601 (picco singolo frame), replay IDENTICO, GATE OK.
- Estesi OK: accelerazione (9.90/160.83/246.66 @ 1-60-600 f), ribaltamento, sterzo -218.3/+218.3, curve (1199 campioni, salto pendenza 0.50).
- Aperto: frenata (Pad::B) e costa (COAST) compilati in prova_gate.cpp ma non invocati da gate_t1.sh (script non scrivibile da me): numeri runtime mancanti.
- H [~]: mancano fps/temperatura Pi 400 (Andrea, vcgencmd) e v91 guida; non misurabili su macOS.
- quota_open 100%, check_catalogo OK, nessuna ROM Sega. Review gate_t1.sh/prova_gate.cpp/renderer/road/game.h: nessun problema.
