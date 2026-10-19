# Pacchetto per Claude app (capo supremo) — 2026-10-17 17:01
Incolla in Claude app (con accesso alla cartella): "Leggi questo pacchetto, RIEPILOGO_MATTINA.md, DECISIONI_ANDREA.md e SCAMBIO.md. Dai verdetto, correggi la rotta, aiutami a rispondere alle decisioni a blocchi."

## Stato blocchi
10:## BLOCCO 0 — Testata
17:## BLOCCO A — @capo
19:| Stato | [x] |
21:## BLOCCO B0 — @architetto (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
25:| Stato | [x] |
28:## BLOCCO B — @architetto
33:| Stato | [x] | (consegnato T2, SCAMBIO voce 16)
35:## BLOCCO C — @developer
40:| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)
42:## BLOCCO D — @traduttore
46:| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)
48:## BLOCCO E — @cercaasset
51:| Stato | [x] |
53:## BLOCCO F — @asset
57:| Stato | [x] | (@asset, voce SCAMBIO 24, chiusa con voce 30)
59:## BLOCCO G — @testi (opzionale)
61:| Stato | [x] | (156 frasi verificate, 16 righe corrette: out/testi/RIEPILOGO_CONSEGNA_G.md)
63:## BLOCCO H — @debugger (+ Andrea sul Pi)
65:| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/debugger/ESITO_GATE_T1_17OTT_1400.md)
67:## BLOCCO I — @reviewer
69:| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @developer prima di H)
71:## BLOCCO J — Escalation a Claude
74:| Stato | [ ] |
78:## BLOCCO K — @livelli (dopo B0; in parallelo a C)
82:| Stato | [x] | (consegnato T2, SCAMBIO voce 18)
84:## BLOCCO L — @scenografo (dopo K)
87:| Stato | [x] | (@scenografo, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)
89:## BLOCCO FED — @architetto (con B)
91:| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)
93:## BLOCCO CONS — @traduttore (conservazione, subito dopo B0)
96:| Stato | [x] |
98:## BLOCCO M — @developer (in parallelo a K)
102:| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)
104:## BLOCCO N — @architetto (solo dopo che C gira; facoltativo)
108:| Stato | [ ] |
110:## BLOCCO X — @agente
115:| Stato | [ ] |
118:## BLOCCO P1 — @developer (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
122:| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/debugger/)
124:## BLOCCO P2 — @livelli (per P1)
126:| Stato | [x] |
129:## BLOCCO P3 — @traduttore (per P1)
131:| Stato | [x] |
134:## BLOCCO MD0 — @developer — TAPPA 1: la ROM parte e si pilota
139:| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |
141:## BLOCCO P1b — @developer — TAPPA 1: niente collisioni fantasma
144:| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)
146:## BLOCCO T1-RUN — @developer + @debugger — prossimo run: implementazione e test (da Claude 17 ott 14:00)
162:| Stato | [~] | (in esecuzione: @developer punti 1-4, @debugger punti 5-6; build e gate OK 17 ott 14:51, quota open 99,8%; v91 di Andrea pendente. Dettaglio: SCAMBIO 1144)
164:## BLOCCO T5 — @developer + @livelli — TAPPA 5: altri veicoli, collisioni, razzo
169:| Stato | [-] (tappa 5) |

## Decisioni aperte
13:## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
22:## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
28:## D3 | struttura | architetto | [CHIUSA 2026-10-16 da Andrea]
35:## D4 | tipografia | claude_app | [CHIUSA 2026-10-16 da Andrea]
42:## D5 | sagoma auto | claude_app | [CHIUSA 2026-10-16 da Andrea]
47:## D2b | sponsor | capo | [CHIUSA 2026-10-16 da Andrea]
53:## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
58:## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
63:## D8 | HUD elemento "tempo" | tipografo | [CHIUSA 2026-10-16 da @capo DEFAULT A]
71:## D9 | build | capo | [CHIUSA 2026-10-17 da Andrea]

## Ultime righe di SCAMBIO.md
VERDETTO: CORREGGI

Problemi trovati (2, in ordine di gravità):

1. (alta) `assets_originali/DIREZIONE_ARTISTICA.md:11` — riga `Personaggi`: Gianni, Gesù, UFO "vengono presi da Andrea da precedente integrazione ROM .md Sega e da importare nel asset C++". Viola la regola 1 AGENTS.md (nessun materiale della ROM originale Sega: sprite, tracciati, personaggi derivati) e contrasta con `motore_nuovo/docs/LICENZE.md:71` ("Nessun codice, asset, sprite, tracciato o musica della ROM originale Sega entra nel motore nuovo") e `FONTI.md:69` (dati portati, mai codice/asset Sega).
   Correzione: dichiarare i personaggi come creazioni originali di Andrea/progetto (nessuna provenienza da ROM Sega), rimuovere il riferimento all'integrazione ROM .md Sega, e registrarli in `assets_originali/CREDITI.md` e `motore_nuovo/docs/CATALOGO_PERSONAGGI.md` prima di qualsiasi import in C++.

2. (media) `assets_originali/DIREZIONE_ARTISTICA.md:1` (testata) — manca la dichiarazione di licenza (SPDX o manifest.json), richiesta dalla regola 10 AGENTS.md e da `motore_nuovo/docs/LICENZE.md:77` per ogni file dati/testi del progetto.
   Correzione: aggiungere in testata `# SPDX-License-Identifier: CC-BY-4.0` (testo creativo, per `LICENZE.md:14`) oppure dichiarare la licenza nel `manifest.json` della cartella `assets_originali/`.

Cosa ho verificato (sola lettura, contesto stretto): `assets_originali/DIREZIONE_ARTISTICA.md` (43 righe), `assets_originali/CREDITI.md` (19 righe, tutte le voci interne al progetto, CC0-1.0 / CC-BY-4.0, nessuna risorsa esterna ancora usata), `motore_nuovo/docs/LICENZE.md` e `FONTI.md`. Non ho scansionato il resto del progetto. I controlli automatici già riportati sono OK (nessuna ROM Sega, open 99.8% ≥ 50%, sintassi script OK); nessun rischio prestazioni rilevabile da questo file di direzione artistica.

### CHECKPOINT 1 | REVIEWER | 2026-10-17 16:58 | [APERTA]
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
catalogo: 11 righe, 2 cartelle
  senza licenza aperta: out/.DS_Store (nessuna)
quota open: 99.9% (13866379 di 13874575 byte), soglia 50.0%
bacheche controllate, limite 500.0 MB
blocchi fatti [x]: 19
blocchi da fare [ ]: 6

[0m
> reviewer · nemotron-3.5-super-vl-free
[0m→ [0mRead assets_originali/INDICE_ASSET.md 
[0m→ [0mRead assets_originali/INDICE_ASSET.json 
[0m→ [0mRead motore_nuovo/src/game/game.cpp 
[0m→ [0mRead motore_nuovo/test/prova_gate.cpp 
[0m→ [0mRead motore_nuovo/test/VOCE_SCAMBIO_1146.md 
[0m→ [0mRead motore_nuovo/src/player/player.h 
[0m→ [0mRead motore_nuovo/src/render/renderer.h 
[0m→ [0mRead motore_nuovo/src/render/renderer.cpp 
[0m→ [0mRead motore_nuovo/src/road/road.cpp 
> reviewer · nemotron-3.5-super-vl-free
Let me verify the credits file (required by rule 2) and confirm license completeness.
[0m→ [0mRead assets_originali/CREDITI.md 
> reviewer · nemotron-3.5-super-vl-free
[0m→ [0mRead assets_originali/font/antonio/manifest.json 
> reviewer · nemotron-3.5-super-vl-free
VERDETTO: CORREGGI

Ho letto solo i 9 file indicati più CREDITI.md e il manifest del font (contesto stretto per la regola 2). Controlli eseguiti: intestazioni SPDX/MIT su tutto il codice; licenze in INDICE_ASSET.json (CC0-1.0 per asset interni, OFL-1.1 per il font, tutte compatibili); nessun materiale Sega (commenti citano OutRun/Jake Gordon/Lou's solo come riferimenti di progettazione, non codice/asset); determinismo regola 5b (`dt = 1/50` fisso, RNG con seed, nessun orologio di sistema nella logica di gioco, `std::chrono` solo nel test harness); prestazioni (gate: ms/frame medio 0,071 max 0,157, limite 20; replay 2x identico; offroad raggiunto) ben dentro il budget Pi 400/50 fps; INDICE_ASSET.json e .md coerenti (293 file, stesse lacune già note e dichiarate come non difetti: traffico, Gianni, Gesù, UFO); prova_gate.cpp:96-97 è un commento giustificato (falso positivo del reviewer già risolto, non un bug).

Problema azionabile (1/3):

1. `assets_originali/CREDITI.md` — manca la risorsa esterna scelta con decisione D4 (regola 2: ogni risorsa esterna va in CREDITI con autore, licenza, URL). Il font `assets_originali/font/antonio/Antonio-Bold.ttf` è segnato `originale: false` in INDICE_ASSET.json e usato per titoli/fumetti, ma non è in CREDITI.md.
   Correzione: aggiungi in fondo alla tabella (dopo riga 19) i dati già verificati in `assets_originali/font/antonio/manifest.json`:
   `| assets_originali/font/antonio/Antonio-Bold.ttf | Impallari Type (Pablo Impallari) | OFL-1.1 | https://fonts.google.com/specimen/Antonio |`

Nessun altro problema nei file revisionati. Una volta corretto CREDITI.md, il blocco è pulito per il checkpoint.
