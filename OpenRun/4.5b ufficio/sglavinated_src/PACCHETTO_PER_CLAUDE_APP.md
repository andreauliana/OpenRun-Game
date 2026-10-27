# Pacchetto per Claude — 2026-10-27 21:14
Incolla in Claude (con accesso alla cartella): "Leggi questo pacchetto, RIEPILOGO_MATTINA.md, ANDREA_REVIEW.md e SCAMBIO.md. Dai verdetto, correggi la rotta, aiutami a rispondere alle decisioni a blocchi."

## Stato blocchi
12:## BLOCCO 0 — Testata
19:## BLOCCO A — @capo
21:| Stato | [x] |
23:## BLOCCO B0 — @developer (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
27:| Stato | [x] |
30:## BLOCCO B — @developer
35:| Stato | [x] | (consegnato T2, SCAMBIO voce 16)
37:## BLOCCO C — @developer
42:| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)
44:## BLOCCO D — @developer
48:| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)
50:## BLOCCO E — @developer
53:| Stato | [x] |
55:## BLOCCO F — @grafico
59:| Stato | [x] | (@grafico, voce SCAMBIO 24, chiusa con voce 30)
61:## BLOCCO G — @grafico (opzionale)
63:| Stato | [x] | (156 frasi verificate, 16 righe corrette: out/testi/RIEPILOGO_CONSEGNA_G.md)
65:## BLOCCO H — @developer (+ Andrea sul Pi)
67:| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)
69:## BLOCCO I — @developer
71:| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @developer prima di H)
73:## BLOCCO J — Escalation a Claude
76:| Stato | [ ] |
80:## BLOCCO K — @developer (dopo B0; in parallelo a C)
84:| Stato | [x] | (consegnato T2, SCAMBIO voce 18)
86:## BLOCCO L — @grafico (dopo K)
89:| Stato | [x] | (@grafico, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)
91:## BLOCCO FED — @developer-junior (con B)
93:| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)
95:## BLOCCO CONS — @developer (conservazione, subito dopo B0)
98:| Stato | [x] |
100:## BLOCCO M — @grafico (in parallelo a K)
104:| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)
106:## BLOCCO N — @developer (solo dopo che C gira; facoltativo)
110:| Stato | [ ] |
112:## BLOCCO X — @agente
117:| Stato | [ ] |
120:## BLOCCO P1 — @developer (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
124:| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)
126:## BLOCCO P2 — @developer (per P1)
128:| Stato | [x] |
131:## BLOCCO P3 — @developer (per P1)
133:| Stato | [x] |
136:## BLOCCO MD0 — @developer — TAPPA 1: la ROM parte e si pilota
141:| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |
143:## BLOCCO P1b — @developer — TAPPA 1: niente collisioni fantasma
146:| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)
148:## BLOCCO T1-RUN — @developer + @developer-junior — prossimo run: implementazione e test (17 ott 14:00)
165:| Stato | [~] | (gate FALLITO 19/10 09:39 da Andrea su Mac, exit 13: giro guidato FALLITO su 5/6 livelli — "percorso poco vario o non guidabile"; solo lv_02_atomico OK. Gate core OK: soak, accelerazione, ribaltamento, sterzo, curve, gara, screenshot P1, ms/frame ~0.19 max 0.94. Percorsi risolti con R1 (gate 6/6 verde 21/10 10:44). Resta solo la v91 di Andrea sul runner SDL2 (revisione in corso: non toccare `src/runner/`). Dettaglio: ANDREA_REVIEW.md T1-Q3, STATUS_OFFICINA.md report 09:39)
167:## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
172:| Stato | [x] | (Claude 23/10: `check_catalogo_grafico.py` exit 0, 159 elementi, PNG non catalogati: 0; @grafico 22/10) |
174:## BLOCCO T5 — @developer — TAPPA 5: altri veicoli, collisioni, razzo
179:| Stato | [-] (tappa 5) |
183:## BLOCCO R1 — @developer + @developer-junior — tracciati modificabili per le 6 piste (da Andrea e Claude 19 ott)
191:| Stato | [x] | (Claude 22/10: gate 6/6 verde 21/10 10:44 `out/developer/ESITO_GATE_21OTT_1044.md`, `ctest` 17/17 ricontrollato su main ae16132; review registrate: `out/developer-junior/REVIEW_CP005_PUNTI1-3_DEVELOPER_2026-10-20_1714.md` POSITIVA, `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md` POSITIVA. Resta solo a @capo: chiudere CP-005 nel registro. Prossimo: F60) |
195:## BLOCCO R2 — @developer + @developer-junior + @grafico — ciclo di gioco completo (da Andrea 19 ott)
204:| Stato | [~] | (Andrea 25/10: R2 parte subito DOPO DECO1, PRIMA di PIST1 e RIV. Ordine: FORK1 review -> DECO1 -> R2 -> PIST1 -> RIV -> FIN. Struttura voluta: intro (caricamento), selezione, partita, risultati, ripartenza. Per R2 si tocca `src/runner/` solo per chiamare la fase corrente.) |
208:## BLOCCO F60 — @developer + @developer-junior — gioco a 60 Hz, tempi in secondi (Andrea 19 ott; dopo R1, prima di R2)
214:| Stato | [x] | (Claude 23/10: gate verde 22/10 20:06, `gate_t1.sh` exit 0, GATE FREQUENZA 60 Hz OK e 50 Hz OK, replay IDENTICO, 6/6 percorsi OK, `out/developer-junior/ESITO_F60_22OTT_2006.md`. A @capo resta solo il checkpoint CP-006. Prossimo: FORK1) |
218:## BLOCCO FORK1 — @developer + @developer-junior + @grafico — bivio e cambio tappa fluidi, transizione di tema, bordi strada (Andrea 22 ott, dopo prove su Mac; dopo F60, prima di RIV)
229:| Stato | [~] | (Claude 26/10: le due review incrociate sono FATTE e POSITIVE: `out/developer-junior/REVIEW_FORK1_26OTT.md` (07:14) e `out/developer/REVIEW_FORK1_26OTT.md` (07:20). Resta SOLO @capo: nel prossimo turno apre CP-007 per FORK1, lo chiude (gate ctest verde + 2 review positive), registra in REGISTRO_REVISIONI e aggiorna STATO_CORRENTE e STATI_PROPOSTI con FORK1 e DECO1; poi propone FORK1 [x]. Developer e developer-junior NON aspettano CP-007: passano a DECO1.) (Claude 25/10: codice e gate FORK1 FATTI, ctest 20/20. Restano SOLO: a) review incrociata, UN turno ciascuno: @developer rivede i gate 4, 5, 7 di `prova_gate.cpp`; @developer-junior rivede la modifica di `game.cpp` (corsia a scelta aperta) e la transizione tema; esito in `out/<ruolo>/REVIEW_FORK1_<data>.md`. b) @capo apre CP-007 per FORK1 e lo chiude quando le due review sono positive. c) @grafico elemento di passaggio al bivio: FACOLTATIVO, non blocca. Nessuno aspetta piu' `game.cpp`: chi trova FORK1 senza lavoro suo passa a DECO1.) |
233:## BLOCCO DECO1 — @grafico + @developer + @developer-junior — elementi decorativi di ogni livello (Andrea 22 ott; dopo FORK1, prima di RIV e prima di ogni prova su Pi 400/RetroPie)
248:| Stato | [~] | (Claude 27/10: codice @developer passi 2a-2e FATTO, `gate_t1.sh` 07:55 exit 0, lv_06 smoke OK 08:42. Restano solo: review incrociata (un turno ciascuno, esito in `out/<ruolo>/REVIEW_DECO1_<data>.md`), @grafico rivede gli screenshot in `out/developer-junior/` e `Test area/`, @capo apre e chiude il checkpoint. Nessuno aspetta altro codice DECO1: il @developer passa a R2.) |
252:## BLOCCO PIST1 — @developer + @developer-junior + @grafico — percorsi piu' vari: ponti, colline, chicane, autostrada, citta' (Andrea 23 ott, dopo la prova su Mac; dopo DECO1, prima di RIV)
261:| Stato | [ ] |
265:## BLOCCO RIV — @developer + @developer-junior + @grafico — rivali veri e urto con sbandata (Andrea 22 ott; dopo F60, prima di R2)
276:| Stato | [~] | (Andrea 27/10: punti 2 e 3 anticipati prima di PIST1, vedi 'Chi tocca adesso') |
280:## BLOCCO FIN — @developer + @developer-junior + @grafico — finale animato a fine corsa (Andrea 22 ott; dopo R2 e RIV)
287:| Stato | [ ] |

## Decisioni aperte
74:## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
80:## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
86:## D3 | struttura | architetto | [CHIUSA 2026-10-19 da Andrea]
92:## D4 | tipografia | claude_app | [CHIUSA 2026-10-16 da Andrea]
99:## D5 | sagoma auto | claude_app | [CHIUSA 2026-10-16 da Andrea]
104:## D2b | sponsor | capo | [CHIUSA 2026-10-16 da Andrea]
110:## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
115:## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
120:## D8 | HUD elemento "tempo" | tipografo | [CHIUSA 2026-10-16 da @capo DEFAULT A]
128:## D9 | build | capo | [CHIUSA 2026-10-17 da Andrea]
134:## D10 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
139:## D11 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
146:## D12 | R1 completamento e ri-verifica Mac | builder-developer | [CHIUSA 2026-10-23 da Andrea]
167:## D17 | Stati e deleghe Turn 1 (ora 1 di 12) | capo | [CHIUSA 2026-10-23 da Andrea]
228:## D13 | Chiusura T1-RUN (Tappa 1) | capo | [CHIUSA 2026-10-23 da Andrea]
240:## D14 | Ordine test post-R1 e Pi 400/RetroPie | capo | [CHIUSA 2026-10-23 da Andrea]
250:## D15 | Attivazione G1 e delega @grafico | capo | [CHIUSA 2026-10-23 da Andrea]
258:## D16 | Attivazione G1 e delega @grafico | Andrea | [CHIUSA 2026-10-23 da Andrea]
268:## D18 | Chiusura T1-RUN e avvio R2 | capo | [CHIUSA 2026-10-23 da Andrea]
287:## D19 | T1-RUN chiusura dopo prova Mac 23/10 | capo | [APERTA]

## Ultime righe di SCAMBIO.md

OK (controlli automatici verdi: review del modello saltata, zero crediti)

### CHECKPOINT 1 | REVIEWER | 2026-10-27 21:00 | [APERTA]
OK      sintassi modelli.sh
OK      sintassi modo_locale.sh
OK      sintassi recap.sh
OK      sintassi set_model.sh
OK      sintassi start_process_brains.sh
OK      sintassi verifica.sh
OK      sintassi vivo.sh
OK      sintassi run.sh
OK      nessuna ROM Sega presente
OK      opencode installato
OK      comando claude presente (review automatica possibile)
== TUTTO OK ==
GATE FREQUENZA 50 Hz OK
screenshot P1: curva frame 1587 (scarto 173 px), sponsor frame 672 (40516 px), personaggio+fumetto frame 446
GATE SCREENSHOT P1 OK
GATE T1: FALLITO (exit 15)
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/developer/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.6% (6147590 di 6170352 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 25
blocchi da fare [ ]: 8

[0m
> developer-junior · nemotron-3.5-super-vl-free
CORREGGI: gate_t1.sh -> @developer.

### CHECKPOINT 2 | REVIEWER | 2026-10-27 21:13 | [APERTA]
OK      sintassi modelli.sh
OK      sintassi modo_locale.sh
OK      sintassi recap.sh
OK      sintassi set_model.sh
OK      sintassi start_process_brains.sh
OK      sintassi verifica.sh
OK      sintassi vivo.sh
OK      sintassi run.sh
OK      nessuna ROM Sega presente
OK      opencode installato
OK      comando claude presente (review automatica possibile)
== TUTTO OK ==
GATE FREQUENZA 50 Hz OK
screenshot P1: curva frame 1587 (scarto 173 px), sponsor frame 672 (40516 px), personaggio+fumetto frame 446
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/developer/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.6% (6147590 di 6170352 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 25
blocchi da fare [ ]: 8

OK (controlli automatici verdi: review del modello saltata, zero crediti)
