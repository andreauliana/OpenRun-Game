# Riepilogo mattina — Tappa 1 (18 ott 2026)
Stati: T1-RUN [~] proposto [x] | P1 [~] | H [~] | J [ ] | M [~] | G1 [ ].

Fatto per blocco/agente:
- @collaudatore: gate T1 smoke+soak exit 0, "GATE T1: TUTTO OK" (out/collaudatore/gate_t1.log): speed@60=161.79 > 0 frame 60, replay 2x IDENTICO su 3 livelli, ms/frame medio 0.35-0.40 max 0.833 (<=20), offroad f278/f167/f285, avvolgimento+inattivita OK; screenshot partenza/curva/fuoristrada per livello in out/collaudatore/lv_0*; indice_assets 294 file, lacune 4/11 (traffico, Gianni, Gesu', UFO); review T1-RUN 7 OK; quota_open 100% >= 50%.
- @sviluppatore: engine guida OutRun (accel decrescente vs MAX_SPEED 250, freno BRAKE 120, coast 0.995, sterzo vs velocita', spinta in curva, offroad 0.99, ribaltamento, HUD); build cmake OK, prova_gate compilato, sprite_raw.h/.cpp e png_a_raw.py scritti. Blocco: shell negata 6x `Permission denied: shell` -> 3 screenshot in out/sviluppatore/ non generabili; P1 non chiuso.
- @capo: STATI_PROPOSTI.md, DECISIONI_ANDREA.md (D1-D3 DEFAULT sì/sì/sì, D4 DEFAULT no), diario aggiornato.
- Andrea punto 8 T1-RUN: v91 su FEEDBACK_ANDREA.md (runner Mac SDL2, guida OK o ritocco un parametro alla volta) + fps/temperatura Pi 400 (vcgencmd) — in attesa, non eseguibile qui (regole 9/21); chiude H.

Non funziona / mancante:
- Shell negata per ./build_gate/prova_gate e cp in out/sviluppatore/: 3 screenshot P1 mancanti (gate_t1.sh eseguibile, shell diretta no).
- Frenata (Pad::B) e costa (COAST) non misurati nel gate (gate_t1.sh non li invoca, script non scrivibile).
- fps/temperatura Pi 400 non misurabili su macOS (servono Andrea); v91 guida in attesa.
- Sprite veri nel gate headless ancora rettangoli (D10 opzione B approvata: conversione PNG->raw, .sglraw non generati per shell negata).
- Fuoristrada rallentamento brusco ~160->6 (D11 opzione A, taratura da confermare).

Decisioni aperte in DECISIONI_ANDREA.md (3 piu' importanti):
- D1: chiudere T1-RUN a [x]? DEFAULT: sì.
- D2: promuovere P1 a [x] dopo 3 screenshot in out/sviluppatore/? DEFAULT: sì.
- D3: Andrea esegue punto 8 (v91 + misure Pi 400) per chiudere H? DEFAULT: sì, appena possibile.
(D4: attivare @grafico per G1? DEFAULT: no.)

Prossimo passo consigliato:
v91 Andrea su FEEDBACK_ANDREA.md (+ misure Pi 400) -> @sviluppatore 3 screenshot in out/sviluppatore/ (se shell si sblocca) -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1 -> Tappa 2.
