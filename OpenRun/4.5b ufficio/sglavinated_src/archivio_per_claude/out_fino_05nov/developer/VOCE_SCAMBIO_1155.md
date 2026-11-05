# Voce scambio @sviluppatore — 18/10 13:49 (turno 1)

Stato blocchi: P1 [~], T1-RUN [~].

Fatto questo turno:
- Rieseguito `sh tools/gate_t1.sh` dalla radice sglavinated_src → exit 0, GATE T1: TUTTO OK su lv_01_neon, lv_02_atomico, lv_03_foglie (log: out/collaudatore/gate_t1.log):
  - speed@60=161.79, speed@1=9.90; ms/frame medio 0.19-0.20, max 0.393 (obiettivo <=12, limite 20)
  - replay 2x: IDENTICO su tutti i livelli (determinismo OK)
  - offroad raggiunto: frame 278 / 167 / 285
  - modi dedicati prova_gate: accelerazione OK (frame1=9.90, frame60=160.83, frame600=246.66), ribaltamento e ripartenza OK, sterzo OK (sinistra -218.3 / destra 218.3), continuita curve OK (1199 campioni, salto pendenza max 0.50), avvolgimento playerZ oltre totalLen_ OK, inattivita 10s -> show/fumetto visibile OK.
- Nessuna modifica di codice necessaria: engine di guida (FEEDBACK_ANDREA punti 1-3 — accelerazione Pad::A con spinta che cala verso MAX_SPEED, freno B/Down, coast 0.995, sterzo dipendente dalla velocita, spinta verso l'esterno in curva, rallentamento fuori strada, ribaltamento, HUD velocita/marcia/offroad), proiezione colline (camera segue quota sotto l'auto + CAM_HEIGHT, x relativa al punto sotto l'auto) e wrap sprite (zRel con fmod totalLen) sono già nel nucleo e coperti dal gate. Cartelloni già diradati (lv_01 56, lv_02 53, lv_03 61 voci; sponsor invariati dal catalogo approvato).
- Confermati T1-RUN punti 2-3 (colline, wrap sprite) e densità cartelloni (punto 4).

Consegna: questa voce (1155) in `out/sviluppatore/`.

Prossimo: attendere v91 Andrea (test runner `sglavinated_runner` su Mac) per chiudere P1/T1-RUN; nessun altro blocco aperto per questo ruolo.
