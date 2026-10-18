# RIASSUNTO @sviluppatore — v4.5 turno 18/10 13:49
- Stato blocchi: P1 [~], T1-RUN [~] (in attesa v91 Andrea su test runner Mac; nessun altro blocco aperto per il ruolo).
- Gate: sh tools/gate_t1.sh exit 0, GATE T1: TUTTO OK 3 livelli (speed@60=161.79, speed@1=9.90, ms/frame medio 0.19-0.20 max 0.393 <=20, replay 2x IDENTICO, offroad frame 278/167/285, acc/ribalt/sterzo/curve OK, avvolgimento + inattivita OK).
- Engine guida FEEDBACK_ANDREA punti 1-3 già nel nucleo e coperto dal gate: accel Pad::A con spinta che cala verso MAX_SPEED, freno B/Down, coast 0.995, sterzo dipendente da velocità, spinta esterna in curva, rallentamento offroad, ribaltamento, HUD velocità/marcia/offroad.
- Colline (punto 2): camera segue quota sotto l'auto + CAM_HEIGHT, x relativa al punto sotto l'auto. Confermato da gate curve continue (salto pendenza max 0.50).
- Sprite (punti 3-4): zRel con fmod(totalLen), proiezione strada SPRITE_WORLD_PX, cartelloni diradati (lv_01 56, lv_02 53, lv_03 61 voci).
- Consegna: out/sviluppatore/VOCE_SCAMBIO_1155.md.
- Prossimo: attendere v91 Andrea (runner Mac) per chiudere P1/T1-RUN.
