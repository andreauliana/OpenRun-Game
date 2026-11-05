# VOCE_SCAMBIO_1151 @sviluppatore — 18/10 09:50

## Blocco: T1-RUN / P1 (sviluppatore) — stato proposto [~]

### Eseguito (esito misurato)
`sh tools/gate_t1.sh` exit 0.
- `GATE T1: TUTTO OK` su lv_01_neon, lv_02_atomico, lv_03_foglie + soak 30000 frame.
- speed@60=161.79 (speed@1=9.90), accelerazione progressiva OK (10 s = 246.66, clamp a MAX_SPEED 250).
- ms/frame medio 0.404 max 0.833 (limite 20); replay 2x IDENTICO su tutti i livelli.
- offroad raggiunto (lv_01 frame 278); ribaltamento OK; sterzo OK (sinistra -218.3, destra 218.3); curva continua OK (salto pendenza max 0.50).
- Inattività 10 s: show/fumetto visibile, maxBollaDopoIdle=4655 px >= 100, personaggio 6561 px; replay IDENTICO.
- Log: out/collaudatore/gate_t1.log.

### Screenshot prodotti dal gate
PPM in out/collaudatore/lv_01_neon/: prova_gate_partenza.ppm, prova_gate_curva.ppm, prova_gate_fuoristrada.ppm (stessa cosa per lv_02_atomico, lv_03_foglie). PNG di sintesi anche in out/collaudatore/ (prova_gate_partenza.png, curva, fuoristrada).

### Blocco (persiste da 6+ tentativi)
`./build_gate/prova_gate out/sviluppatore/p1 lv_01_neon smoke` e `cp out/collaudatore/lv_01_neon/prova_gate_*.ppm out/sviluppatore/` => `Permission denied: shell`.
Solo `sh tools/gate_t1.sh` è eseguibile. I 3 screenshot P1 restano in out/collaudatore/; non posso spostarli in out/sviluppatore/ da shell.

### Prossimo
Sblocco shell o copia dei 3 screenshot in out/sviluppatore/ (fuori dalla mia portata con gli strumenti attuali). Poi chiudere P1 con Andrea. Pendente da Andrea: v91 (test runner Mac + giudizio guida, punto 8 FEEDBACK v90).
