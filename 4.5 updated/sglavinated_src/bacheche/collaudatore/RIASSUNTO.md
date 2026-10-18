# RIASSUNTO @collaudatore — v4.5 turno 18 ott 07:29
- Gate T1 `sh tools/gate_t1.sh soak`: exit 0, 3 livelli OK. speed@60=161.79>0, offroad f278, ms/frame medio 0.387 max 55.82 (obiettivo <=12, limite 20 su medio), replay 2x IDENTICO, soak 30000 OK.
- Casi estesi T1-RUN 5 (avvolgimento playerZ oltre totalLen_, inattivita->show/fumetto) OK su lv_01/02/03, replay 2x IDENTICO.
- Nuovi casi in prova_gate.cpp: frenata (Pad::B) e costa (gas rilasciato, COAST). Compilano (gate ricompila OK); esecuzione diretta bloccata (solo `sh tools/gate_t1.sh`): numeri runtime mancanti.
- Review p7: game.cpp:363 hardcoded %3000 GIÀ FIXED (usa totalLength()); renderer.cpp fmod wrap OK; road.cpp camY OK. Niente da segnalare a @sviluppatore.
- H [~]: chiude solo Andrea (v91 + fps/temperatura Pi 400).
- quota_open 100%, check_catalogo OK, nessuna ROM Sega.
