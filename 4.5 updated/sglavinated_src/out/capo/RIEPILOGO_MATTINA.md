# Riepilogo mattina — Tappa 1 (18 ott 2026)
Tappa attiva: 1 — motore C++ (strada pseudo-3D, guida, gate T1).
Stati: T1-RUN [~] proposto [x] | P1 [~] | H [~] | J [ ] | M [~] | G1 [ ].

Fatto per blocco/agente:
- @collaudatore: `sh tools/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK` (out/collaudatore/gate_t1.log): speed@60=161.79 > 0 frame 60, replay 2x IDENTICO su lv_01_neon/lv_02_atomico/lv_03_foglie, ms/frame medio 0.407 max 55.601 (limite 20), offroad raggiunto frame 167-285, soak 30000 frame OK; accelerazione, ribaltamento, sterzo, curve continue OK; screenshot partenza/curva/fuoristrada per livello in out/collaudatore/lv_0*; quota_open 100%, check_catalogo OK, review renderer/road/game.h/prova_gate OK.
- @sviluppatore: codice su disco — player.cpp accel A/Up, freno B/Down, COAST 0.995, sterzo vs velocità; road.cpp colline quota assoluta + camera sotto auto; renderer.cpp sprite veri zRel mod totalLen, disegno lontano->vicino; 21 .sglraw sponsor caricati da prova_gate.cpp; CMakeLists target prova_gate. Gate da confermare (shell negata, vedi sotto).
- @capo: out/capo/STATI_PROPOSTI.md riscritto (T1-RUN [x] proposto, P1 [~], H [~], J [ ], M [~], G1 [ ]), DECISIONI_ANDREA.md D1-D4 (DEFAULT sì/sì/sì/no), bacheche/capo/diario.md aggiornato.
- Andrea (punto 8 T1-RUN): v91 su FEEDBACK_ANDREA.md (runner Mac SDL2, guida OK o ritocco parametro) + fps/temperatura Pi 400 (vcgencmd) — in attesa, non eseguibile qui (regole 9/21 AGENTS.md); chiude H.

Cosa non funziona / aperti:
- shell negata per @sviluppatore: `sh tools/gate_t1.sh` e i 3 screenshot in out/sviluppatore/ non generabili -> P1 non chiudibile (mancano screenshot: pista curva, traffico, cartelloni, personaggio con fumetto).
- Runner gate: SDL2_image mancante, cartelloni sponsor disabilitati (gioco comunque funzionante).
- Densità cartelloni in livello.json troppo fitti (T1-RUN punto 4, resta dopo v91).

Decisioni aperte in DECISIONI_ANDREA.md (le 3 più importanti):
- D1: chiudere T1-RUN a [x]? DEFAULT: sì.
- D2: promuovere P1 a [x] dopo 3 screenshot in out/sviluppatore/? DEFAULT: sì.
- D3: Andrea esegue punto 8 (v91 + misure Pi 400) per chiudere H? DEFAULT: sì, appena possibile.

Prossimo passo consigliato:
Andrea v91 su FEEDBACK_ANDREA.md (+ misure Pi 400) -> @sviluppatore 3 screenshot in out/sviluppatore/ (se shell si sblocca) -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1 -> Tappa 2.
