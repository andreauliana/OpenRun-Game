# Pacchetto per Claude — 2026-10-21 14:41
Incolla in Claude (con accesso alla cartella): "Leggi questo pacchetto, RIEPILOGO_MATTINA.md, ANDREA_REVIEW.md e SCAMBIO.md. Dai verdetto, correggi la rotta, aiutami a rispondere alle decisioni a blocchi."

## Stato blocchi
12:## BLOCCO 0 — Testata
19:## BLOCCO A — @capo
21:| Stato | [x] |
23:## BLOCCO B0 — @builder-developer (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
27:| Stato | [x] |
30:## BLOCCO B — @builder-developer
35:| Stato | [x] | (consegnato T2, SCAMBIO voce 16)
37:## BLOCCO C — @builder-developer
42:| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)
44:## BLOCCO D — @builder-developer
48:| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)
50:## BLOCCO E — @builder-developer
53:| Stato | [x] |
55:## BLOCCO F — @grafico
59:| Stato | [x] | (@grafico, voce SCAMBIO 24, chiusa con voce 30)
61:## BLOCCO G — @grafico (opzionale)
63:| Stato | [x] | (156 frasi verificate, 16 righe corrette: out/testi/RIEPILOGO_CONSEGNA_G.md)
65:## BLOCCO H — @builder-developer (+ Andrea sul Pi)
67:| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)
69:## BLOCCO I — @builder-developer
71:| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @sviluppatore prima di H)
73:## BLOCCO J — Escalation a Claude
76:| Stato | [ ] |
80:## BLOCCO K — @sviluppatore (dopo B0; in parallelo a C)
84:| Stato | [x] | (consegnato T2, SCAMBIO voce 18)
86:## BLOCCO L — @grafico (dopo K)
89:| Stato | [x] | (@grafico, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)
91:## BLOCCO FED — @collaudatore (con B)
93:| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)
95:## BLOCCO CONS — @sviluppatore (conservazione, subito dopo B0)
98:| Stato | [x] |
100:## BLOCCO M — @grafico (in parallelo a K)
104:| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)
106:## BLOCCO N — @builder-developer (solo dopo che C gira; facoltativo)
110:| Stato | [ ] |
112:## BLOCCO X — @agente
117:| Stato | [ ] |
120:## BLOCCO P1 — @builder-developer (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
124:| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)
126:## BLOCCO P2 — @builder-developer (per P1)
128:| Stato | [x] |
131:## BLOCCO P3 — @builder-developer (per P1)
133:| Stato | [x] |
136:## BLOCCO MD0 — @builder-developer — TAPPA 1: la ROM parte e si pilota
141:| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |
143:## BLOCCO P1b — @builder-developer — TAPPA 1: niente collisioni fantasma
146:| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)
148:## BLOCCO T1-RUN — @builder-developer + @builder-developer-junior — prossimo run: implementazione e test (17 ott 14:00)
165:| Stato | [~] | (gate FALLITO 19/10 09:39 da Andrea su Mac, exit 13: giro guidato FALLITO su 5/6 livelli — "percorso poco vario o non guidabile"; solo lv_02_atomico OK. Gate core OK: soak, accelerazione, ribaltamento, sterzo, curve, gara, screenshot P1, ms/frame ~0.19 max 0.94. **Prossimo: @sviluppatore punto 2b — rivedere parametri percorsi**. v91 Andrea su runner SDL2 ancora da fare. Dettaglio: ANDREA_REVIEW.md T1-Q3, STATUS_OFFICINA.md report 09:39)
167:## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
172:| Stato | [ ] |
174:## BLOCCO T5 — @sviluppatore — TAPPA 5: altri veicoli, collisioni, razzo
179:| Stato | [-] (tappa 5) |
183:## BLOCCO R1 — @developer + @developer-junior — tracciati modificabili per le 6 piste (da Andrea e Claude 19 ott)
191:| Stato | [ ] |
195:## BLOCCO R2 — @developer + @developer-junior + @grafico — ciclo di gioco completo (da Andrea 19 ott)
203:| Stato | [ ] |

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
146:## D12 | R1 completamento e ri-verifica Mac | builder-developer | [APERTA]
167:## D17 | Stati e deleghe Turn 1 (ora 1 di 12) | capo | [APERTA]
228:## D13 | Chiusura T1-RUN (Tappa 1) | capo | [APERTA]
240:## D14 | Ordine test post-R1 e Pi 400/RetroPie | capo | [APERTA]
250:## D15 | Attivazione G1 e delega @designer | capo | [APERTA]
258:## D16 | Attivazione G1 e delega @designer | Andrea | [APERTA]
271:## D18 | Chiusura T1-RUN e avvio R2 | capo | [APERTA]

## Ultime righe di SCAMBIO.md

REVIEW SALTATA: nessun file di codice, dati o asset testuale modificato dall'ultimo checkpoint.

### CHECKPOINT 3 | REVIEWER | 2026-10-21 13:14 | [APERTA]
OK      sintassi modelli.sh
OK      sintassi recap.sh
OK      sintassi set_model.sh
OK      sintassi start_process_brains.sh
OK      sintassi verifica.sh
OK      sintassi vivo.sh
OK      sintassi run.sh
OK      nessuna ROM Sega presente
OK      opencode installato
OK      comando claude presente (review automatica possibile)
ERRORE  grafico.md: chiave duplicata nel frontmatter
== CI SONO ERRORI ==
GATE GARA OK
screenshot P1: curva frame 1232 (scarto 182 px), sponsor frame 1353 (3463 px), personaggio+fumetto frame 372
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/developer/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.5% (4267295 di 4290057 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 19
blocchi da fare [ ]: 9

REVIEW SALTATA: nessun file di codice, dati o asset testuale modificato dall'ultimo checkpoint.

### CHECKPOINT 4 | REVIEWER | 2026-10-21 14:41 | [APERTA]
OK      sintassi modelli.sh
OK      sintassi recap.sh
OK      sintassi set_model.sh
OK      sintassi start_process_brains.sh
OK      sintassi verifica.sh
OK      sintassi vivo.sh
OK      sintassi run.sh
OK      nessuna ROM Sega presente
OK      opencode installato
OK      comando claude presente (review automatica possibile)
ERRORE  grafico.md: chiave duplicata nel frontmatter
== CI SONO ERRORI ==
GATE GARA OK
screenshot P1: curva frame 1232 (scarto 182 px), sponsor frame 1353 (3463 px), personaggio+fumetto frame 372
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/developer/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/developer/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.5% (4267295 di 4290057 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 19
blocchi da fare [ ]: 9

REVIEW FALLITA (exit 1): [0m
> developer-junior · nemotron-3.5-super-vl-free
[91m[1mError: [0mRate limit exceeded. Please try again later.
