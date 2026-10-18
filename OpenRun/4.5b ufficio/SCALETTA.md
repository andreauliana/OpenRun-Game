# SCALETTA OPERATIVA — V3 (open)

## 1. Squadra (cartella `sglavinated_src/.opencode/agents/`)
| Agente | Modello iniziale | Compito unico | Scrive solo in |
|---|---|---|---|
| capo | kimi-k2.5-free | coordina, assegna, pacchetto per Claude | *.md |
| architetto | nemotron-3.5-super-vl-free | architettura, formati dati, build, licenze | motore_nuovo/docs/ |
| coder | north-mini-code-free | nucleo C++ del motore | motore_nuovo/src/, CMakeLists.txt |
| traduttore | nemotron-3.5-super-vl-free | porta regole/frasi gia' fatte come dati e modulo gameplay | motore_nuovo/dati/, src/gameplay/ |
| cercaasset | nemotron-3.5-super-vl-free | cerca immagini/suoni/font con licenza libera | assets_originali/ricerca/ |
| asset | big-pickle | crea sprite, scenari, palette, percorsi originali | assets_originali/ |
| testi | nemotron-3.5-super-vl-free | frasi di Gianni e Gesu' | gianni12.py, frasi.json |
| debugger | big-pickle | build e test, cause dei bug | nessuno |
| livelli | nemotron-3.5-super-vl-free | progetta i livelli come dati (JSON) | motore_nuovo/dati/livelli/ |
| scenografo | big-pickle | grafica dei livelli: sfondi, bordo strada, palette | assets_originali/scenari/ |
| reviewer | minimax-m2.1-free | controllo codice/licenze/asset | nessuno |
Nomi dei modelli e assegnazione: ipotesi iniziale, non provata.
Font, lettering, menu, HUD e schermate sono gestiti dal ruolo grafico, non da un ruolo separato.

## 2. Scaletta a fasi
| Fase | Cosa | Responsabile | Uscita | Blocco in RICHIESTA.md |
|---|---|---|---|---|
| 0 | Verifica modelli e avvio | Andrea | `modelli.sh` tutto OK | — |
| 1 | Architettura e librerie | architetto | ARCHITETTURA, FORMATI_DATI, BUILD, LICENZE | B |
| 2 | Prototipo: nucleo + runner SDL2 + core libretro; strada a segmenti, auto, 50 fps; replay deterministico | coder (+ debugger) | compila, gira nel runner e come core, hash di replay identico | C |
| 3 | Porting regole e frasi | traduttore | SPEC_GAMEPLAY.md, frasi.json, gameplay/ | D |
| 3b | Ricerca risorse (in parallelo a 3) | cercaasset | tabelle in assets_originali/ricerca/ | E |
| 4 | Asset originali: 3 varianti -> scelta -> produzione | asset + Andrea | PNG + CREDITI.md | F |
| 5 | Prova su Pi 400 | Andrea + debugger | fps/temperatura | H |
| 6 | Review | reviewer + Claude | verdetto | I |
| 7 | Novita' di gameplay (sez. 3) | coder + traduttore | funzioni a schermo | nuovo blocco |

## 3. Quanto e' fattibile (valutazione qualitativa, non misurata)
| Novita' | Motore nuovo (pista C) | Hack Mega Drive v87/v88 |
|---|---|---|
| Razzo con tasto B | Facile-Media: proiettile come sprite che si allontana in prospettiva + collisione con traffico | Difficile, sconsigliata: slot sprite 70-76 e blocchi VRAM presi in prestito gia' al limite, budget VBlank (PBUDGET 58), ROM a 4 MB, ogni aggiunta sposta il `leak` di +-100 frame (HANDOVER) |
| Auto che si alza ad alta velocita' | Facile se cosmetico (offset verticale, ombra, scala); Media se cambia la fisica | Difficile: e' il test della v88 |
| Cannone / piu' armi | Media | Difficile |
| Fumetti senza limite di 156 | Facile (dati JSON) | Impossibile oltre 156 (limite hardware) |
| Escalation errori, inattivita' | Facile (regole gia' specificate) | Gia' fatta |
| Colori psichedelici dopo crash | Media (palette e fade con SDL) | Gia' fatta in forma limitata |
| Livelli a tema, percorsi nuovi | Media: generatore con seed + editor semplice | Difficile |
| Musica tracker (.xm) | Media (libxmp/SDL_mixer, da verificare) | Fatta con PCM, limitata dalla memoria |
| Visualizzatore audio | Media | Difficile |
| Traffico con comportamento | Media-Difficile | Difficile |
| Multigiocatore | fuori portata per ora | — |
Lettura: tutto quello che aggiunge animazioni, sprite o dati cresce in modo naturale sul motore nuovo, mentre sulla hack ogni funzione compete per lo stesso budget.

## 3b. Alternativa rapida: mod di Cannonball (A2)
Il motore OutRun riscritto in C++ ha gia' strada, traffico, audio e un core libretro per Pi. Si potrebbe aggiungere personaggi, fumetti, razzo e volo, senza i limiti della hack. Costi: serve la ROM arcade dell'utente, licenza solo non commerciale, il codice deriva da quello Sega (da far valutare a un legale). Non e' una versione «libera». I moduli di gameplay del `traduttore` restano riutilizzabili nel motore libero. Confronto completo: RAPPORTO_FATTIBILITA.md sez. 1b; il blocco B0 produce il giudizio tecnico.

## 4. Ritmo di lavoro e Claude
- Modo semplice: `cd` nella cartella e `opencode`; dai l'incarico; per fermarti e chiedere un parere scrivi `/claude`.
- Modo automatico: `sh strumenti/ciclo.sh`. Cicli di OpenCode; review di Claude quando c'e' `PER_CLAUDE.md` o ogni 60 minuti, mai oltre 120. Si ferma con `STOP`, con `FINITO` o dopo 8 cicli.
- La review automatica richiede il comando `claude` (Claude Code) sul Mac con il tuo abbonamento; sola lettura. Se non c'e', lo script ti dice cosa scrivere a Claude app.
- Il limite d'uso del piano Pro puo' contare: se Claude non risponde, la review slitta a mano.

## 5. Cambio modello
| Come | Comando |
|---|---|
| Vedere cosa c'e' | `sh strumenti/modelli.sh` |
| Cambio manuale | `sh strumenti/set_model.sh <agente> <modello>` |
| Cambio a rotazione dalla panchina | `sh strumenti/cambia_modello_auto.sh <agente>` |
| Cambio deciso da Claude | se la review contiene `CAMBIA_MODELLO: <agente>` il ciclo automatico lo applica |
Un agente non cambia modello da solo mentre lavora: il cambio avviene tra un ciclo e l'altro.

## 6. Rischi
| Rischio | Cosa fare |
|---|---|
| Modelli gratuiti deboli su C++ | passi piccoli, test a ogni passo, review di Claude |
| Troppa roba in un blocco | capo spezza in task piu' piccoli |
| Asset simili all'originale | direzione artistica propria, @reviewer |
| Licenze | CREDITI.md completo, controllo legale prima di distribuire |
