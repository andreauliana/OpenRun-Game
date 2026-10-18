# Riepilogo mattina — 18 ott 2026

## Per blocco / agente (letto, non dedotto)
- @sviluppatore (T1-RUN/P1): punti 1-4 su disco: player.cpp dinamica OutRun (accel A/Up, freno B/Down, offroad OFFROAD_LIMIT=700<X_LIMIT=900, sterzo vs velocita'), road.cpp proiezione/curva/collina corretta, renderer.cpp sprite veri zRel modulo totalLen, 21 .sglraw sponsor in dati/sprite_raw/sponsor/, prova_gate.cpp casi smoke/soak/avvolgimento/inattivita/sterzo/curve/crash.
- @collaudatore (T1-RUN 5-7): sh tools/gate_t1.sh e soak exit 0; 3 livelli speed@60=161.79>0, offroad f278, ms/frame medio ~0.38 (limite 20), replay 2x IDENTICO, avvolgimento+inattivita OK; indice_assets.py 293 file (4 lacune: traffico, Gianni, Gesu', UFO); quota_open 100%; check_catalogo OK; review p7 OK (game.cpp:363 %3000 fixato).
- @grafico / @ricercatore: non attivati.
- Andrea (H punto 8): runner Mac + fps/temperatura Pi 400 pendenti (v91 in FEEDBACK_ANDREA.md).

## Non funziona / ancora aperto
- v91 Andrea pendente: chiude H e permette promozione T1-RUN/P1 a [x].
- Lacune grafiche non catalogate: traffico, sprite personaggi (Gianni, Gesu', UFO).
- Nessun bug noto in codice; game.cpp:363 gia' corretto.

## Decisioni aperte (3 piu' importanti, DECISIONI_ANDREA.md)
1. D10 T1-RUN (capo): sprite/cartelloni veri nel gate — PNG nel core vs conversione PNG->raw offline. RISPOSTA Andrea: B (zero dipendenze). DEFAULT B. Da chiudere formalmente.
2. D11 T1-RUN (capo): fuoristrada troppo brusco (160->6). RISPOSTA Andrea: A (graduale ~0.85/frame). DEFAULT A. Da chiudere formalmente.
3. Nessuna altra aperta: D1-D9 CHIUSE, nessun PER_CLAUDE.md.

## Prossimo passo consigliato
@collaudatore: report gate soak (exit code + numeri gate_t1.log) + 3 screenshot out/collaudatore/<livello>/; poi v91 Andrea (runner Mac, guida OK + fps/temp Pi 400) -> T1-RUN/P1/H a [x] (solo Andrea/Claude); se tutti i blocchi tappa 1 sono [x], creare FINITO.
