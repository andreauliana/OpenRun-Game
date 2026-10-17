# RIASSUNTO @collaudatore — v4.5 turno 17 ott 18:38
- Gate T1: `sh tools/gate_t1.sh` exit 0 (smoke, 3 livelli). speed@60=33.76>0, offroad raggiunto (f476), ms/frame medio 0.072 max 0.436 (limite 20), replay 2x IDENTICO ogni 60 frame.
- Casi estesi T1-RUN 5: avvolgimento playerZ oltre totalLen_ e inattivita->show/fumetto OK su lv_01/02/03 (replay 2x IDENTICO).
- Review punto 7: renderer.cpp drawSprites fix fmod wrap OK; road.cpp project() camY=camWorldY+CAM_HEIGHT OK; game.cpp:363 hardcoded `% 3000` invece di `totalLength()` (bug latente, sprite=nullptr).
- out/sviluppatore/png_a_raw.py assente (spostato in strumenti_py/ con header MIT). SCAMBIO 18:00 superato.
- H [~]: chiude solo Andrea (v91 + fps/temperatura Pi 400).
- T1-RUN 4 [~]: di @sviluppatore (setSponsorSprites core headless); punto 4b collaudatore bloccato.
- quota_open 100%, check_catalogo OK, nessuna ROM Sega.
