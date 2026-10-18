# Voce scambio @collaboratore -> Andrea/Claude — 18 ott 07:29
## Gate T1 (`sh tools/gate_t1.sh soak`)
- exit 0, `GATE T1: TUTTO OK`. 3 livelli (lv_01_neon, lv_02_atomico, lv_03_foglie).
- speed@60=161.79 > 0 (accelerazione OK), offroad raggiunto frame 278.
- ms/frame medio 0.387, max singolo frame 55.82 (obiettivo <=12; il gate governa solo il medio; picco singolo da tenere d'occhio su Pi 400).
- replay 2x IDENTICO ogni 60 frame su tutti i livelli; soak 30000 frame OK.
- verifica accelerazione/ribaltamento/sterzo/curve: OK.
## Casi nuovi (`motore_nuovo/test/prova_gate.cpp`)
- Aggiunti `frenata` (Pad::B) e `costa` (gas rilasciato, COAST=0.995). Codice compila (ricompilazione gate OK).
- Non eseguibili direttamente in questo ambiente (solo `sh tools/gate_t1.sh` consentito; `tools/` non scrivibile dal ruolo): numeri runtime mancanti. Da rilevare con `./build_gate/prova_gate out/collaudatore/<x> lv_01_neon frenata|costa`.
## Review punto 7
- game.cpp:363 hardcoded %3000 già fixato (usa totalLength()); renderer.cpp fmod wrap OK; road.cpp camY OK. Niente correzione da proporre a @sviluppatore.
## Stato
- H [~] in attesa di v91 Andrea (guida) + fps/temperatura Pi 400.
- T1-RUN [~]. Prossimo: attendere Andrea; misurare frenata/costa quando eseguibile.
