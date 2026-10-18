# RIEPILOGO MATTINA — 18/10 (max 40 righe)

## Fatto per blocco/agente

@sviluppatore — P1 [~]: punti 1-3 e 5 fatti (Pad::A accelera, B/Down freno, coast 0.995, segmenti da livello.json, build cmake OK, prova_gate compilato). T1-RUN punti 1,4 eseguiti (interpolazione trapezio, sprite/cartelloni veri nel gate). Shell negata 6x (`Permission denied: shell`, anche `date`): 3 screenshot in out/sviluppatore/ NON generabili, binari pronti.

@collaudatore — gate T1 verde: `sh tools/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK` (speed@60=161.79 > 0 al frame 60, replay 2x IDENTICO su 3 livelli, ms/frame medio 0.36 max 0.651 <= 20, offroad f278/f167/f285, soak 30000 frame OK). Screenshot per livello in out/collaudatore/lv_0*/.

@capo — STATI_PROPOSTI.md, DECISIONI_ANDREA.md, deleghe, diario e RIASSUNTO aggiornati. Nessuna promozione a [x] (solo Andrea/Claude).

@grafico / @ricercatore — non attivati (D4 DEFAULT no, fuori scope Tappa 1).

## Cosa non funziona / blocca

- Shell negata `Permission denied: shell`: i 3 screenshot P1 (pista curva, cartelloni, personaggio con fumetto) non si generano -> P1 non promovibile.
- H [~]: attende Andrea punto 8 T1-RUN (prova runner Mac SDL2, v91 FEEDBACK_ANDREA.md, fps/temperatura Pi 400).
- Andrea v91 + risposte D1-D4 pendenti: senza, T1-RUN/P1/H non chiudono e Tappa 1 non si chiude.

## Decisioni aperte in DECISIONI_ANDREA.md (top 3)

1. D1 — Chiudere T1-RUN a [x]? DEFAULT: sì (gate già verde).
2. D2 — Promuovere P1 a [x] con 3 screenshot? DEFAULT: sì (bloccato dalla shell).
3. D3 — Andrea esegue punto 8 T1-RUN per chiudere H? DEFAULT: sì, appena possibile.

(D4 = no per ora, G1 fuori scope.)

## Prossimo passo consigliato

Andrea: v91 in FEEDBACK_ANDREA.md + risposte D1-D4 (DEFAULT: sì/sì/sì). Poi @sviluppatore riprova shell per i 3 screenshot P1 -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi avvio tappa 2. Nessun PER_CLAUDE.md, nessun FINITO.
