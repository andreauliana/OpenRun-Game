# RIASSUNTO @collaudatore — v4.5 turno 18 ott 12:40
- Gate T1 `sh tools/gate_t1.sh`: exit 0, GATE OK 3 livelli (log out/collaudatore/gate_t1.log). speed@60=161.79>0, offroad f278/f167/f285, ms/frame medio 0.349-0.377 max ~0.9 (limite 20), replay 2x IDENTICO. Avvolgimento+inattività OK (in prova_gate.cpp). accelerazione/ribaltamento/sterzo/curve OK.
- VOCE_SCAMBIO_1158.md scritta (motore_nuovo/test/).
- Review punto 7 (renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, gate_t1.sh): nessun problema, coerente con VISIONE.md.
- quota_open 100%, nessuna ROM Sega.
- Aperto: frenata/costa — modi esistono in prova_gate.cpp ma gate_t1.sh (tools/, non di mia scrittura) non li invoca. PASSAGGIO a @sviluppatore/@capo per aggiungere le invocazioni.
- H [~]: fps/temperatura Pi 400 e v91 Andrea in attesa (non misurabili su macOS; v91 compito di Andrea).
