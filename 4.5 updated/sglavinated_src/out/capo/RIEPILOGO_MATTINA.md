# RIEPILOGO MATTINA — capo | 18/10 2026 | Tappa 1 (Gate T1)

## Fatto per blocco/agente (oggi)
T1-RUN (@sviluppatore + @collaudatore): `sh tools/gate_t1.sh soak` exit 0, GATE T1: TUTTO OK — 3 livelli, replay 2x IDENTICO, speed@60=161.79, offroad raggiunto (lv1 f278 / lv2 f167 / lv3 f285), ms/frame medio <=1.67 (max 71 ms lv2, da tarare), soak 30000 OK. Quota open 99,8% >= 50%. prova_gate carica i 21 .sglraw prima di init(); screenshot con cartelloni veri in out/collaudatore/lv_0*/. Codice: accel/freno/coast/sterzo/offroad/ribaltamento/HUD su disco, build cmake OK, prova_gate compilato.
P1 (@sviluppatore): punti 1-3 e 5 fatti (Pad::A accelera, B/Down freno, segmenti da livello.json, wiring show). Mancano 3 screenshot in out/sviluppatore/ (shell negata 6x `Permission denied: shell`; binary build_gate/prova_gate pronto).
H (@collaudatore + Andrea): in attesa v91 Andrea (runner Mac SDL2 + giudizio guida + fps/temperatura Pi 400).
G1 [ ], J [ ], MD0 [-], T5 [-]: non avviati (fuori scope Tappa 1 / non richiesti da Andrea).

## Non funziona / manca
- 3 screenshot out/sviluppatore/ per chiusura P1 (shell negata).
- v91 Andrea mancante: blocca H e le promozioni a [x] di T1-RUN/P1 (solo Andrea/Claude).
- Sprite/cartelloni veri nel gate headless: prova_gate usa .sglraw segnaposto; D10 DEFAULT B (conversione PNG->raw in strumenti_py) da attuare.
- Rallentamento fuoristrada troppo brusco (D11: DEFAULT A graduale, tuning parametro alla volta).
- ms/frame max 71 ms su lv_02 (limite 20): spike da verificare/attutire.
- fps/temperatura reali su Pi 400 ancora non misurati (Andrea).

## Decisioni aperte in DECISIONI_ANDREA.md (3 piu' importanti)
D1 | Chiudere T1-RUN a [x]? DEFAULT: sì. Gate soak verde, screenshot out/collaudatore/lv_0*/ già fatti.
D2 | Promuovere P1 a [x] dopo 3 screenshot out/sviluppatore/? DEFAULT: sì.
D3 | Andrea punto 8 (v91 FEEDBACK_ANDREA.md + fps/temp Pi 400) per chiudere H? DEFAULT: sì, appena possibile.
Altre aperte (già risposte Andrea, da attuare): D10 conversione PNG->raw DEFAULT B; D11 offroad graduale DEFAULT A.

## Prossimo passo consigliato
1) @sviluppatore riprova shell e produce 3 screenshot in out/sviluppatore/ con `./build_gate/prova_gate out/sviluppatore/ lv_01_neon/lv_02_atomico/lv_03_foglie smoke` -> chiude P1.
2) Andrea scrive v91 in FEEDBACK_ANDREA.md e le misure Pi 400 -> chiude H.
3) Solo allora Andrea/Claude promuovono T1-RUN e P1 a [x] -> chiusura Tappa 1, poi Tappa 2.
