# Pacchetto per Claude app (capo supremo) — 2026-10-18 22:43
Incolla in Claude app (con accesso alla cartella): "Leggi questo pacchetto, RIEPILOGO_MATTINA.md, ANDREA_REVIEW.md e SCAMBIO.md. Dai verdetto, correggi la rotta, aiutami a rispondere alle decisioni a blocchi."

## Stato blocchi
11:## BLOCCO 0 — Testata
18:## BLOCCO A — @capo
20:| Stato | [x] |
22:## BLOCCO B0 — @collaudatore (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
26:| Stato | [x] |
29:## BLOCCO B — @collaudatore
34:| Stato | [x] | (consegnato T2, SCAMBIO voce 16)
36:## BLOCCO C — @sviluppatore
41:| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)
43:## BLOCCO D — @sviluppatore
47:| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)
49:## BLOCCO E — @ricercatore
52:| Stato | [x] |
54:## BLOCCO F — @grafico
58:| Stato | [x] | (@grafico, voce SCAMBIO 24, chiusa con voce 30)
60:## BLOCCO G — @grafico (opzionale)
62:| Stato | [x] | (156 frasi verificate, 16 righe corrette: out/testi/RIEPILOGO_CONSEGNA_G.md)
64:## BLOCCO H — @collaudatore (+ Andrea sul Pi)
66:| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)
68:## BLOCCO I — @collaudatore
70:| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @sviluppatore prima di H)
72:## BLOCCO J — Escalation a Claude
75:| Stato | [ ] |
79:## BLOCCO K — @sviluppatore (dopo B0; in parallelo a C)
83:| Stato | [x] | (consegnato T2, SCAMBIO voce 18)
85:## BLOCCO L — @grafico (dopo K)
88:| Stato | [x] | (@grafico, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)
90:## BLOCCO FED — @collaudatore (con B)
92:| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)
94:## BLOCCO CONS — @sviluppatore (conservazione, subito dopo B0)
97:| Stato | [x] |
99:## BLOCCO M — @grafico (in parallelo a K)
103:| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)
105:## BLOCCO N — @collaudatore (solo dopo che C gira; facoltativo)
109:| Stato | [ ] |
111:## BLOCCO X — @agente
116:| Stato | [ ] |
119:## BLOCCO P1 — @sviluppatore (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
123:| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)
125:## BLOCCO P2 — @sviluppatore (per P1)
127:| Stato | [x] |
130:## BLOCCO P3 — @sviluppatore (per P1)
132:| Stato | [x] |
135:## BLOCCO MD0 — @sviluppatore — TAPPA 1: la ROM parte e si pilota
140:| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |
142:## BLOCCO P1b — @sviluppatore — TAPPA 1: niente collisioni fantasma
145:| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)
147:## BLOCCO T1-RUN — @sviluppatore + @collaudatore — prossimo run: implementazione e test (da Claude 17 ott 14:00)
164:| Stato | [~] | (in esecuzione: @sviluppatore punti 1-4, @collaudatore punti 5-6; build e gate OK 17 ott 14:51, quota open 99,8%; v91 di Andrea pendente. Dettaglio: SCAMBIO 1144)
166:## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
171:| Stato | [ ] |
173:## BLOCCO T5 — @sviluppatore — TAPPA 5: altri veicoli, collisioni, razzo
178:| Stato | [-] (tappa 5) |

## Decisioni aperte
53:## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
62:## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
68:## D3 | struttura | architetto | [CHIUSA 2026-10-16 da Andrea]
75:## D4 | tipografia | claude_app | [CHIUSA 2026-10-16 da Andrea]
82:## D5 | sagoma auto | claude_app | [CHIUSA 2026-10-16 da Andrea]
87:## D2b | sponsor | capo | [CHIUSA 2026-10-16 da Andrea]
93:## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
98:## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
103:## D8 | HUD elemento "tempo" | tipografo | [CHIUSA 2026-10-16 da @capo DEFAULT A]
111:## D9 | build | capo | [CHIUSA 2026-10-17 da Andrea]
117:## D10 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
125:## D11 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]

## Ultime righe di SCAMBIO.md
blocchi fatti [x]: 19
blocchi da fare [ ]: 7

OK (controlli automatici verdi: review del modello saltata, zero crediti)

### CHECKPOINT 1 | REVIEWER | 2026-10-18 22:28 | [APERTA]
OK      sintassi giro.sh
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
== TUTTO OK ==
GATE GARA OK
screenshot P1: curva frame 2596 (scarto 127 px), sponsor frame 884 (3629 px), personaggio+fumetto frame 372
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.5% (4695501 di 4718263 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 19
blocchi da fare [ ]: 7

REVIEW SALTATA: nessun file di codice, dati o asset testuale modificato dall'ultimo checkpoint.

### CHECKPOINT 1 | REVIEWER | 2026-10-18 22:41 | [APERTA]
OK      sintassi giro.sh
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
== TUTTO OK ==
GATE GARA OK
screenshot P1: curva frame 2596 (scarto 127 px), sponsor frame 884 (3629 px), personaggio+fumetto frame 372
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_personaggio_fumetto.png (nessuna)
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_curva.png (nessuna)
  senza licenza aperta: out/sviluppatore/screenshot_p1/p1_cartelloni_sponsor.png (nessuna)
quota open: 99.5% (4695501 di 4718263 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 19
blocchi da fare [ ]: 7

REVIEW SALTATA: nessun file di codice, dati o asset testuale modificato dall'ultimo checkpoint.
