# RIASSUNTO @sviluppatore — v4.5 turno 18/10 07:43
- Gate OK: `sh tools/gate_t1.sh` exit 0 su 3 livelli + accelerazione/ribaltamento/sterzo/curve; speed@60=161.79, ms/frame medio ~0.4 (max 3.150, limite 20), replay 2x IDENTICO, offroad raggiunto. Log: out/collaudatore/gate_t1.log.
- P1 [~]: player.cpp (A accelera, B freno, coast, sterzo dipendente dalla velocita') e game.cpp (segmenti da livello.json, spinta in curva, rallentamento offroad, ribaltamento) gia fatti; gate verde. Mancano 3 screenshot in out/sviluppatore/ (gate P1).
- T1-RUN [~]: colline quota assoluta + camera sotto auto, sprite zRel modulo totalLen, sprite veri .sglraw (21 file, cartelloni sponsor nel gate), HUD pannello info (barra velocita, marcia, offroad) in renderer.h/.cpp + game.cpp. Pendente: v91 di Andrea (punto 8).
- png_a_raw.py (SPDX MIT) e 21 .sglraw gia generati; quota open OK.
- Prossimo: attendere v91 Andrea e screenshot runner per chiudere P1/T1-RUN.
