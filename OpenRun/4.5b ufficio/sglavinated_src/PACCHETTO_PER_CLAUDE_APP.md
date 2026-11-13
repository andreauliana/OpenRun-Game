# Pacchetto per Claude — 2026-11-12 16:24
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
63:| Stato | [x] | (156 frasi verificate, 16 righe corrette: UFFICIO/out/testi/RIEPILOGO_CONSEGNA_G.md)
65:## BLOCCO H — @developer (+ Andrea sul Pi)
67:| Stato | [-] | (Claude 08/11: sospeso finche' Andrea non prova il Pi 400; nessun lavoro degli agenti qui) |
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
104:| Stato | [-] | (Claude 08/11: chiuso. Varianti UI e HUD fermi per scelta di Andrea 08/11; D8 "tempo" e' gia' nell'HUD: non spostarlo) |
106:## BLOCCO N — @developer (solo dopo che C gira; facoltativo)
110:| Stato | [-] |
112:## BLOCCO X — @agente
117:| Stato | [ ] |
120:## BLOCCO P1 — @developer (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
124:| Stato | [x] | (Claude 08/11: superato, il gioco ha sprite veri da fine ottobre) |
126:## BLOCCO P2 — @developer (per P1)
128:| Stato | [x] |
131:## BLOCCO P3 — @developer (per P1)
133:| Stato | [x] |
136:## BLOCCO MD0 — @developer — TAPPA 1: la ROM parte e si pilota
141:| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |
143:## BLOCCO P1b — @developer — TAPPA 1: niente collisioni fantasma
146:| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)
148:## BLOCCO T1-RUN — @developer + @developer-junior — prossimo run: implementazione e test (17 ott 14:00)
165:| Stato | [x] | (Claude 08/11, su mandato di Andrea: chiuso. Il gioco gira sul Mac di Andrea (prove 06-07/11), gate T1 TUTTO OK 08/11, 6/6 percorsi OK. D19 superata) |
167:## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
172:| Stato | [x] | (Claude 23/10: `check_catalogo_grafico.py` exit 0, 159 elementi, PNG non catalogati: 0; @grafico 22/10) |
174:## BLOCCO T5 — @developer — TAPPA 5: altri veicoli, collisioni, razzo
179:| Stato | [-] (tappa 5) |
183:## BLOCCO R1 — @developer + @developer-junior — tracciati modificabili per le 6 piste (da Andrea e Claude 19 ott)
191:| Stato | [x] | (Claude 22/10: gate 6/6 verde 21/10 10:44 `UFFICIO/out/developer/ESITO_GATE_21OTT_1044.md`, `ctest` 17/17 ricontrollato su main ae16132; review registrate: `UFFICIO/out/developer-junior/REVIEW_CP005_PUNTI1-3_DEVELOPER_2026-10-20_1714.md` POSITIVA, `UFFICIO/out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md` POSITIVA. Resta solo a @capo: chiudere CP-005 nel registro. Prossimo: F60) |
195:## BLOCCO R2 — @developer + @developer-junior + @grafico — ciclo di gioco completo (da Andrea 19 ott)
204:| Stato | [x] | (Claude 08/11: ciclo completo fatto: splash, menu, gara, fine tempo, risultati, nome, ripartenza) |
208:## BLOCCO F60 — @developer + @developer-junior — gioco a 60 Hz, tempi in secondi (Andrea 19 ott; dopo R1, prima di R2)
214:| Stato | [x] | (Claude 23/10: gate verde 22/10 20:06, `gate_t1.sh` exit 0, GATE FREQUENZA 60 Hz OK e 50 Hz OK, replay IDENTICO, 6/6 percorsi OK, `UFFICIO/out/developer-junior/ESITO_F60_22OTT_2006.md`. A @capo resta solo il checkpoint CP-006. Prossimo: FORK1) |
218:## BLOCCO FORK1 — @developer + @developer-junior + @grafico — bivio e cambio tappa fluidi, transizione di tema, bordi strada (Andrea 22 ott, dopo prove su Mac; dopo F60, prima di RIV)
229:| Stato | [x] | (Claude 08/11: review positive del 26/10, chiuso) |
233:## BLOCCO DECO1 — @grafico + @developer + @developer-junior — elementi decorativi di ogni livello (Andrea 22 ott; dopo FORK1, prima di RIV e prima di ogni prova su Pi 400/RetroPie)
248:| Stato | [x] | (Claude 08/11: chiuso, parallasse e decorazioni in gioco; gli scenari li rifa' Andrea) |
252:## BLOCCO PROVA27 — @developer + @developer-junior + @grafico — prova di Andrea 27/10 sera: corsa singola, cartelloni, oggetti, bivio (PRIMA di RIV b/c e di PIST1)
262:| Stato | [x] | (Andrea 27/10 sera; codice di Claude, review incrociata aperta) |
266:## BLOCCO PIST1 — @developer + @developer-junior + @grafico — percorsi piu' vari: ponti, colline, chicane, autostrada, citta' (Andrea 23 ott, dopo la prova su Mac; dopo DECO1, prima di RIV)
276:| Stato | [x] | (Claude 07/11 sera: chiuso. P3 parapetti annullato da Andrea 06/11, ponte aperto; ponte e dintorni rifatti in PR #60 e #68. Storico:) (Claude 31/10, Andrea: "il ponte in mezzo a dei livelli, non a tutti": P1 e P2 FATTI da Claude. Ponte a meta' di lv_01_neon, lv_04_cromo e lv_06_desktop (dopo `curva_larga_dx`); lv_02, lv_03, lv_05 senza ponte. Acqua ai lati, parapetto grigio/bianco al posto del rumble, niente oggetti e cartelloni sui segmenti con acqua. NON spostare il ponte e non aggiungerlo ad altri livelli. Ponte ALTO (Andrea 31/10, "piu' cinematografico e inaspettato"): tratti `rampa_ponte` (sale in curva a collina 1.0, curva e cresta nascondono il ponte), `ponte` (impalcato a collina 1.0, acqua a `acqua_quota` 0, torri a portale ogni 60 segmenti in `Renderer::drawPilone`), `discesa_ponte` (curva opposta); nei tre livelli tolto un `rettilineo_corto` (lv_01, lv_04) per la durata. Non cambiare altezza o forma senza una prova di Andrea. Restano: @grafico G-P1 (PNG parapetto e guardrail, voci NUOVE nel catalogo), @developer P3 (parapetti del grafico come oggetti), @developer-junior J1 e J2.) |
280:## BLOCCO RIV — @developer + @developer-junior + @grafico — rivali veri e urto con sbandata (Andrea 22 ott; dopo F60, prima di R2)
292:| Stato | [x] | (Claude 08/11: chiuso. 1 fatto (rivali con i propri bivi), 3 fatto (sprite rivali 7/7)) |
296:## BLOCCO AUTO1 — @grafico + @developer + @developer-junior — auto del giocatore a fotogrammi, finto 3D (Andrea 05/11 sera)
311:| Stato | [x] | (Claude 07/11: superato dalle pose disegnate da Andrea (PR #58/#59, REGOLE_AUTO_GIOCATORE.md). I componenti degli agenti restano solo per i vecchi fotogrammi 64x32 di riserva, mai sulle pose di Andrea.) |
315:## BLOCCO ENG1 + ARC1 — @developer + @developer-junior — tutto in inglese e nome del giocatore salvato (Andrea 05/11 notte; dopo AUTO1 A4)
322:| Stato | [x] | (Claude 08/11: chiuso, ctest 27/27) |
326:## BLOCCO KO1 — @developer + @developer-junior + @grafico — rivali messi fuori gara a forza di urti (Andrea 05/11 notte; dopo ENG1+ARC1)
332:| Stato | [x] | (Andrea 07/11: il rivale colpito 4 volte NON esce dalla gara: si fa da parte per 4 s e poi riparte, -3% di punta a ogni botta (PR #61, REGOLE_AUTO_GIOCATORE.md). Il KO con OUT fatto dagli agenti il 07/11 e' stato tolto da Claude. Non rifarlo.) |
336:## BLOCCO FIN — @developer + @developer-junior + @grafico — finale animato a fine corsa (Andrea 22 ott; dopo R2 e RIV)
345:| Stato | [x] | (Claude 29/10: arrivo sulla strada come chiesto da Andrea; review incrociata aperta) |
347:## BLOCCO PERC1 — @developer + @developer-junior + @grafico — percorsi piu' vari e migliorie di guida (Andrea 08/11)
359:| Stato | [x] | (Claude 08/11 sera: chiuso, approvato da Andrea; si prosegue con MIG1) |
365:## BLOCCO MIG1 — @grafico + @developer + @developer-junior — AREA MIG1: migliorie di grafica e di gara (Andrea 08/11 sera)
401:| Stato | [x] | (Claude 10/11: chiuso) |
406:## BLOCCO V47 — avanti verso la v4.7 (continuo) @developer @developer-junior @grafico @capo
448:| Stato | [~] | (Claude 10/11: seconda lista, lavoro continuo, nessuna attesa) |

## Decisioni aperte

## Ultime righe di SCAMBIO.md
OK      comando claude presente (review automatica possibile)
== TUTTO OK ==
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: UFFICIO/Dipartimento Grafico/Libreria/assets_originali/font/born2bsporty/Born2bSportyV2.ttf (CC0-1.0 (pubblico dominio))
quota open: 99.3% (8278918 di 8334450 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 43
blocchi da fare [ ]: 4

OK (controlli automatici verdi: review del modello saltata, zero crediti)

### CHECKPOINT 3 | REVIEWER | 2026-11-12 16:20 | [APERTA]
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
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: UFFICIO/Dipartimento Grafico/Libreria/assets_originali/font/born2bsporty/Born2bSportyV2.ttf (CC0-1.0 (pubblico dominio))
quota open: 99.3% (8278918 di 8334450 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 43
blocchi da fare [ ]: 4

OK (controlli automatici verdi: review del modello saltata, zero crediti)

### CHECKPOINT 4 | REVIEWER | 2026-11-12 16:24 | [APERTA]
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
GATE SCREENSHOT P1 OK
GATE T1: TUTTO OK
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: UFFICIO/Dipartimento Grafico/Libreria/assets_originali/font/born2bsporty/Born2bSportyV2.ttf (CC0-1.0 (pubblico dominio))
quota open: 99.3% (8278918 di 8334450 byte), soglia 50.0%
bacheche controllate, limite 5.0 MB
blocchi fatti [x]: 43
blocchi da fare [ ]: 4

OK (controlli automatici verdi: review del modello saltata, zero crediti)
