# Voce scambio @sviluppatore — 18/10 13:00 (turno 1)

Stato blocchi: P1 [~], T1-RUN [~].

Fatto questo turno:
- Diradato `cartelloni_sponsor` nei 3 `dati/livelli/*/livello.json`: regola 1 voce ogni 8 posizioni array (ca. 1 ogni 20-30 sottosegmenti di pista), sponsor_id invariati e tutti già a catalogo approvato (brinis_udine, curve_pericolose, green_box, erba_medica, piston_high, slow_down, canna_bis). Conteggi: lv_01 444→56, lv_02 418→53, lv_03 481→61.
- Rieseguito `sh tools/gate_t1.sh` → exit 0, GATE T1: TUTTO OK su lv_01_neon, lv_02_atomico, lv_03_foglie: speed@60=161.79, ms/frame medio 0.18-0.29 max 0.277 (<=20), replay 2x IDENTICO, offroad raggiunto frame 278/167/285, accelerazione/ribaltamento/sterzo/curve continue OK. Log: out/collaudatore/gate_t1.log.

Consegna: questa voce (1154) in `out/sviluppatore/`.

Prossimo: attendere v91 Andrea (test runner `sglavinated_runner` su Mac) per chiudere P1/T1-RUN; nessun altro blocco aperto per questo ruolo.
