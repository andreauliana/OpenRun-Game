# Voce scambio @sviluppatore — 18/10 11:17 (da incollare in SCAMBIO.md)

## Stati blocchi (proposti, restano [~])
- P1 [~]: engine guida completo e misurato (gate OK). Manca: 3 screenshot in out/sviluppatore/ (shell negata per la copia da out/collaudatore/).
- T1-RUN [~]: punti 1-4 fatti (colline, wrap sprite zRel, sprite visibili/scalati, accelerazione/freno/sterzo/offroad/ribaltamento/HUD). Aperti: densita' cartelloni in livello.json (troppo fitti, uno ogni 1-4 segmenti); sprite veri bloccati su G1 @grafico (catalogo [ ], da @grafico prima dell'uso nel motore).

## Esiti misurati questo turno
- sh tools/gate_t1.sh exit 0 -> GATE T1: TUTTO OK lv_01/lv_02/lv_03 + prove estese (accelerazione, ribaltamento+ripartenza, sterzo, curve continue).
- speed@60=161.79 su tutti i livelli; ms/frame medio 1.0-1.7 (limite 20); replay 2x IDENTICO su ogni livello; offroad raggiunto (frame 278/167/285); sterzo sinistra=-218.3 destra=218.3; salto pendenza curve 0.50 <= 0.51.
- Dettaglio: out/collaudatore/gate_t1.log. Screenshot gate: out/collaudatore/<livello>/ (prova_gate_partenza/curva/fuoristrada.png).

## Blocco
- Shell negata (`Permission denied: shell`) per comandi diretti ./build_gate/prova_gate ... e cp ... out/sviluppatore/. Eseguibile solo: sh tools/gate_t1.sh. I 3 screenshot P1 esistono in out/collaudatore/<livello>/ ma non sono copiabili in out/sviluppatore/.

## Prossimo passo
- Sfoltire cartelloni_sponsor in lv_01/lv_02/lv_03 (uno ogni ~10 segmenti, lato alternato, sponsor_id ciclati: brinis_udine, curve_pericolose, piston_high, erba_medica, green_box, slow_down), poi consegna a Andrea. v91 pendente: Andrea prova sglavinated_runner sul Mac e scrive il giudizio guida in FEEDBACK_ANDREA.md.
