# RIEPILOGO MATTINA — v4.5, turno 18/10 (max 40 righe)

## Fatto per blocco/agente
- T1-RUN (capo/collaudatore/sviluppatore): `sh tools/gate_t1.sh soak` exit 0, out/collaudatore/gate_t1.log "GATE T1: TUTTO OK" su lv_01_neon/lv_02_atomico/lv_03_foglie. speed@60=161.79 > 0 al frame 60, replay 2x IDENTICO 3 livelli, ms/frame medio ~0.36 max 0.651 <= 20, offroad raggiunto f278/f167/f285, soak 30000 OK, avvolgimento+inattivita show OK, drawHud attivo, colline quota assoluta + camera sotto auto, drawSprites zRel modulo totalLen.
- H (@collaudatore/Andrea): gate già verde, attende solo Andrea punto 8 T1-RUN: provare `sglavinated_runner` su Mac SDL2, scrivere v91 in FEEDBACK_ANDREA.md (guida OK o ritocco un parametro alla volta), fps/temperatura Pi 400 (vcgencmd). Non eseguibile qui (regole 9/21).
- P1 (@sviluppatore): punti 1-3 e 5 fatti, build cmake OK, prova_gate compilato. Mancano solo 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto): shell `Permission denied: shell` 6x (anche `date` negata); out/sviluppatore/ scrivibile solo via write tool ma le immagini PPM le produce il binary.
- J, M, G1: non avviati (fuori scope Tappa 1; G1 solo su richiesta Andrea, D4 DEFAULT no).
- DECISIONI_ANDREA.md: D1-D4 già presenti con DEFAULT (D1 sì, D2 sì, D3 sì, D4 no); D10=B, D11=A già risposte Andrea. Nessuna decisione nuova.

## Non funziona / aperto
- Shell `Permission denied: shell` (anche comandi semplici): screenshot P1 e qualsiasi esecuzione diretta bloccati; unica via permessa `sh tools/gate_t1.sh` [soak] per sviluppatore/collaudatore.
- T1-RUN e P1 non promossi a [x]: competenza solo Andrea/Claude, dopo v91 Andrea.
- H: v91 guida runner Mac + fps/temp Pi 400 ancora mancanti.
- Nessun PER_CLAUDE.md, nessun FINITO (P1 e H non [x]).

## Prossimo passo consigliato
Andrea v91 su FEEDBACK_ANDREA.md (giudizio guida punto 8 + fps/temperatura Pi 400) + risposte D1-D4 -> chiude H. Poi sblocco shell (o write tool) per i 3 screenshot P1 in out/sviluppatore/, rieseguire gate se serve, promuovere T1-RUN + P1 a [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e tappa 2.
