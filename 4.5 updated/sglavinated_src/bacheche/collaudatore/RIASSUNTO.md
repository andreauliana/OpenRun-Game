# RIASSUNTO @collaudatore — v4.5 turno 17 ott 18:17
- Gate T1 soak: `sh tools/gate_t1.sh soak` exit 0 (3 livelli + 30000 frame). speed@60=33.76>0, offroad raggiunto, ms/frame medio 0.08 (obiettivo <=12, limite 20), replay 2x IDENTICO ogni 60 frame. Log e screenshot (partenza/curva/fuoristrada/avvolgimento) in out/collaudatore/<livello>/.
- T1-RUN 5: casi avvolgimento playerZ oltre totalLen_ e inattivita->show/fumetto già in prova_gate.cpp e verificati OK su lv_01/02/03 (replay 2x IDENTICO).
- T1-RUN 6: `python3 motore_nuovo/strumenti_py/indice_assets.py` eseguito: 293 file, lacune 4/11 (traffico, Gianni, Gesu', UFO — già note).
- quota_open.py 100.0% (soglia 50%), check_catalogo.py OK, nessuna ROM Sega (SCAMBIO checkpoint).
- H [~]: chiude solo Andrea (punto 8: v91 su FEEDBACK_ANDREA.md + fps/temperatura Pi 400; da Mac non misurabile).
- T1-RUN 5-6-7: [~] (verificato, proposta [x] a Andrea/Claude). T1-RUN 4 (sprite veri nel gate, SDL2_image assente) aperto: e' di @sviluppatore.
