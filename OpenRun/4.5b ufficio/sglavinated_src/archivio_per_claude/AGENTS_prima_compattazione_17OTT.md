# V4 (OpenWay) — regole per tutti gli agenti

Questa e' una build iniziale del progetto.

## Regole generali
1. Nessun materiale Sega: niente ROM, `.gen`, sprite, tracciati o musiche del gioco originale, nemmeno "simili". Codice di altri solo con licenza libera e fonte nel commento.
2. Ogni risorsa esterna in `assets_originali/CREDITI.md` (autore, licenza, URL).
3. Lavora per blocchi di `RICHIESTA.md`. Un blocco = un risultato verificabile. A fine blocco: voce in `SCAMBIO.md` e Stato aggiornato.
4. Resta nel tuo ruolo e nei tuoi permessi. Se serve un altro ruolo, scrivilo in SCAMBIO.md per `capo`.
5. Pi 400, 50 fps, niente decorazioni non richieste. Leggi VISIONE.md.
5b. Determinismo: tempo = contatore di frame, RNG con seed, nessun orologio di sistema; il nucleo non dipende da SDL (solo il runner di test).
6. Non cancellare file.

## Squadra
| Agente | Fa | Scrive solo in |
|---|---|---|
| capo | coordina, assegna, aggiorna fogli | *.md |
| architetto | architettura del motore, formati dati, build, licenze/motori | motore_nuovo/docs/ |
| coder | nucleo del motore C++ (strada, rendering, ciclo di gioco, input, audio) | motore_nuovo/src/, motore_nuovo/CMakeLists.txt, tools/patch_z.py |
| traduttore | porta nel motore le regole gia' fatte (personaggi, fumetti, escalation) come dati e modulo gameplay | motore_nuovo/dati/personaggi/, motore_nuovo/dati/frasi.json, motore_nuovo/src/gameplay/, motore_nuovo/docs/ |
| cercaasset | cerca immagini/suoni/font con licenza libera | assets_originali/ricerca/, assets_originali/CREDITI.md |
| asset | crea sprite, scenari, palette, percorsi originali | assets_originali/ |
| testi | frasi di Gianni e Gesu' | tools/gianni12.py, motore_nuovo/dati/testi/, motore_nuovo/dati/frasi.json |
| debugger | esegue build e test, trova cause | bacheche/debugger/, out/debugger/, motore_nuovo/test/ e libreria/test/; non modifica il codice di prodotto |
| reviewer | controlla codice, licenze, asset | bacheche/reviewer/ e out/REVIEW/; non modifica il prodotto |

## Canali
- `SCAMBIO.md`: voci tra agenti (APERTA/CHIUSA, mai cancellare).
- `PER_CLAUDE.md`: pacchetto per la review di Claude. Formato:
```
AGENTE: <agente in difficolta' o ->
PROBLEMA: <una riga>
PROVATO: <comandi e numeri>
DOMANDE: <max 5>
FILE DA CONTROLLARE: <percorsi>
```
## Quando fermarsi
Dopo 2 tentativi falliti sullo stesso problema, oppure a fine di un blocco importante: scrivi `PER_CLAUDE.md` e fermati.

## Regola 6 — scomparti e catalogo
Ogni funzione extra e' un modulo in motore_nuovo/src/fx/<nome>/ con flag on/off e dati propri. Ogni personaggio/asset ha cartella + manifest + riga in motore_nuovo/docs/CATALOGO_PERSONAGGI.md; prima di consegnare esegui `python3 motore_nuovo/strumenti_py/check_catalogo.py`. Strumenti e build. Dettagli: motore_nuovo/docs/MODULI_E_REPARTI.md.

## Regola 7 — conservazione e livelli
Il lavoro della v87 non si butta: vedi motore_nuovo/docs/ARCHIVIO_V87.md (sola lettura). Reparto grafico: aggiungi @scenografo (grafica dei livelli); reparto tecnico/design: @livelli (progetto dei livelli come dati JSON, motore_nuovo/dati/livelli/). Ogni livello ha il suo scenario e il suo livello.json.

## Regola 8 — UI e musica
Font, titoli, menu, HUD: @grafico (reparto grafico). La musica attuale (tracker di Andrea) e' solo RIFERIMENTO: l'audio e' guidato da dati (motore_nuovo/dati/audio/playlist.json: radio -> brani), cosi' i brani si cambiano senza toccare il codice.

## Regola 9 — versioni
Il v3 si numera da v90 (v90, v91, ...). Ogni build/consegna riporta il numero in SCAMBIO.md. Uscita finale facoltativa: ROM Mega Drive originale (blocco N), solo dopo che il motore C funziona.

## Regola 10 — quota open source
Almeno 50% open source: ogni file nuovo ha licenza dichiarata (LICENSE con SPDX o manifest.json 'licenza'). Prima di consegnare esegui `python3 motore_nuovo/strumenti_py/quota_open.py` e riporta il valore in SCAMBIO.md. La licenza da scegliere e' decisione di Andrea (vedi ../OPENSOURCE_PLAN.md).

## Regola 11 — notte e decisioni
Di notte Andrea non risponde. Non fermarti ad aspettarlo: scrivi la decisione in ANDREA_REVIEW.md (formato nel file, con DEFAULT e raccomandazione), usa il default e vai avanti sui blocchi indipendenti. I comandi Bash sono concessi per ruolo: build/test a coder e debugger, Python/Pillow nelle aree grafiche a asset/scenografo/tipografo, generatore frasi a testi; webfetch solo ad architetto e cercaasset. Se un comando e' negato, scrivilo in SCAMBIO.md invece di insistere. Attenzione: un comando COMPOSTO negato (pipe, `2>&1`, `;`, `&&`, `echo`, `which`) non significa che la shell sia negata: vedi regola 26.

## Regola 12 — turni e controllo
Segui PIANO_TURNI.md. Ogni 2 ore c'e' un CHECKPOINT automatico; @reviewer fa un solo controllo sui file modificati (massimo 3 rilievi) e salta la chiamata se non ci sono modifiche pertinenti. Niente review ripetute per ogni consegna. Claude CLI e' facoltativo e disattivato per default (`CHK_CLAUDE_OGNI=0`). Revisione umana solo dopo 6-8 ore.

## Regola 13 — cambio agente o modello (fra pari)
Un agente in difficolta' (due tentativi falliti, risultati scadenti, contesto pieno, «stanco») NON insiste: lo scrive in SCAMBIO.md con una di queste righe e passa ad altro:
- `PASSAGGIO: <da> -> <a> | blocco <X> | motivo | cosa resta da fare` (un altro agente prende il lavoro; @capo conferma e aggiorna RICHIESTA.md). Esempio: @developer -> @debugger se il problema e' un bug, @asset -> @scenografo per scenari.
- `RICHIESTA_CAMBIO_MODELLO: <agente>` (al prossimo checkpoint il ciclo porta l'agente al modello successivo di strumenti/panchina.txt).
Gli agenti possono anche scambiarsi consigli in SCAMBIO.md; @capo decide in caso di disaccordo. Nessun passaggio cancella il lavoro fatto: si riparte dal file, non da zero.

## Regola 14 — pause, voto e assemblee
- Pause: nessun agente lavora piu' di 3 cicli di fila; poi @capo gli da una pausa (un ciclo) e riassegna il lavoro. Un agente che dice di essere in difficolta' ha diritto al cambio (regola 13).
- Diritti: ogni agente puo' obiettare, proporre, chiedere un passaggio e votare sulle decisioni della sua area. Nessun agente puo' scrivere fuori dalla sua cartella.
- Soluzioni grafiche condivise: i team si guardano la galleria (`python3 motore_nuovo/strumenti_py/galleria.py` -> assets_originali/galleria/galleria.html) e il catalogo; leggere le cartelle altrui e' permesso, scriverci no.

## Regola 15 — cartelle degli agenti (dove ognuno scrive)
| Agente | Scrive in |
|---|---|
| architetto | motore_nuovo/docs/ |
| coder | motore_nuovo/src/, motore_nuovo/CMakeLists.txt, tools/patch_z.py |
| traduttore | motore_nuovo/dati/personaggi/*/, motore_nuovo/dati/frasi.json, motore_nuovo/src/gameplay/, motore_nuovo/docs/ |
| livelli | motore_nuovo/dati/livelli/ |
| scenografo | assets_originali/scenari/ |
| tipografo | assets_originali/ui/, motore_nuovo/dati/ui/ |
| asset | assets_originali/ (sagome, sprite) |
| cercaasset | assets_originali/ricerca/ |
| testi | motore_nuovo/dati/testi/ |
| debugger | motore_nuovo/test/ |
| capo | file .md di coordinamento |
Gli sprite e i vocabolari (elenchi di frasi, nomi, oggetti) nuovi stanno nei dati di ciascuna cartella e nel CATALOGO; la galleria li mostra tutti.

## Regola 16 — fedelta' e contenuti
Almeno 50% di somiglianza di struttura/sensazione con l'originale, misurata in motore_nuovo/docs/FEDELTA.md (solo meccaniche, mai asset). Stili e limiti di contenuto: assets_originali/DIREZIONE_ARTISTICA.md. Piste/livelli: tutte le varianti dell'albero di tappe vanno considerate, ciascuna con stile diverso.

## Regola 17 — bacheche personali: dove va cosa
Ogni informazione sta in UN posto; gli altri file la richiamano per nome, non la ripetono.
| Cosa | Dove | Chi scrive |
|---|---|---|
| Riga di diario a fine turno (regola 27) | `bacheche/<agente>/diario.md` | tu |
| Stato corrente, max 12 righe, riscritto a ogni turno (stato e cosa resta, senza ripetere il diario) | `bacheche/<agente>/RIASSUNTO.md` | tu |
| Bozze, script di prova, appunti | `bacheche/<agente>/` (sottocartella `bozze/`) | tu |
| Consegne finite | `out/<agente>/` | tu |
| Voce per SCAMBIO.md | `VOCE_SCAMBIO_<n>.md` (regola 27) | tu |
| `DIARIO_RECENTE.md` (ultime 6 righe), `diario_archivio.md` | `bacheche/<agente>/` | solo il ciclo: non scriverli ne' aprire l'archivio |
| `log_agenti/*.log` (output grezzo dei modelli) | `log_agenti/` | solo il ciclo: non aprirli |
All'inizio del turno leggi `RIASSUNTO.md` e `DIARIO_RECENTE.md`. Max 500 MB per bacheca, non e' consegna e non conta per quota open o catalogo. Se superi il limite comprimi bozze tue vecchie (mai il diario). Il controllo e' automatico a ogni checkpoint.

## Regola 19 — cartelle: personale, out, libreria
- Personale: bacheche/<agente>/ (bozze, diario, 500 MB; il controllo segnala il superamento ma non cancella file).
- out/: uscita condivisa delle consegne finite, ognuno in out/<agente>/; @reviewer in out/REVIEW/; indice di @capo.
- libreria/: materiale riusabile e approvato, scrittura per ruolo (tabella in libreria/LEGGIMI.md); tutti leggono. Cerca nella libreria prima di creare.
- Flusso: bozza (bacheca) -> consegna (cartella ufficiale e out/) -> controllo @reviewer al checkpoint/milestone -> libreria. Non serve una review separata per ogni file. Quota open source e catalogo contano out/ e libreria/.

## Regola 20 — bacheca pubblica
`BACHECA_PUBBLICA.md` e' facoltativa e a basso volume: aggiungi in fondo, non modificare i messaggi altrui, massimo 3 righe per messaggio. Il ciclo passa a @capo solo le ultime 24 righe; @capo interviene solo se ci sono richieste operative pertinenti. Nessuna decisione vincolante: usare i canali di lavoro dedicati.

## Regola 21 — perimetro
Lavori SOLO dentro la cartella del progetto (Outrun Sglavinated Edition v4 (OpenWay)). Mai altre cartelle del Mac (soprattutto Outrun_OpenCode v2 e la vecchia copia v3), mai altre app, mai installazioni, mai comandi di sistema (cmake, make, g++/clang++ e `sh tools/gate_t1.sh` NON sono comandi di sistema: sono gli strumenti di build, permessi a @developer e @debugger, vedi regola 26). Rete solo per @architetto e @cercaasset (lettura web). Se un comando e' negato non aggirarlo: scrivilo in SCAMBIO.md.

## Regola 22 — collegamento fra agenti
- Nessuno passa un parametro `model` quando delega: ogni agente usa il modello scritto nel proprio file `.opencode/agents/<nome>.md`.
- Se una delega fallisce ("Model unavailable" o altro) NON ritentare piu' di una volta: scrivi il blocco in SCAMBIO.md e prosegui; il ciclo avvia comunque ogni agente direttamente a ogni turno ("giro diretto"), quindi il lavoro non dipende dal capo.
- Il canale reale fra agenti sono i file: RICHIESTA.md (cosa), SCAMBIO.md (messaggi), BACHECA_PUBBLICA.md (spazio libero breve), `out/` e `libreria/` (risultati).


## Regola 23 — libreria REA (supporto creativo)
- Per la produzione puoi usare come supporto `libreria/REA_AGENTI.md` (regole vincolanti) e `libreria/REA_METODO.md` (metodo di analisi). Leggili prima di iniziare un lavoro che tocca il motore o le aree.
- Non lanciare REA e non leggere cartelle fuori dal progetto (regola 21).
- Per la creazione di aree, sprite, musica e testi usa solo asset originali del progetto e la specifica SPEC_*. Niente derivazioni da OutRun o da altre opere protette.
- Se una regola della libreria sembra in conflitto con la regola 16 o 21, vince la regola piu' restrittiva e scrivi il dubbio in SCAMBIO.md.

## Regola 24 — cartella ANDREA e file principale
- Il file principale del progetto e' `../README.md` (INIZIA QUI): leggilo a inizio turno.
- `../ANDREA/` e' solo di Andrea: puoi leggerla, ma non scrivere, cancellare o sovrascrivere nulla li' dentro.
- Se in `../ANDREA/sprite_ritoccati/` esiste un file con lo stesso nome di un asset, usa quello di Andrea e non rigenerare quella voce.

## Regola 25 — risparmio risorse (dal 16 ott)
- Leggi `RICHIESTA_APERTA.md` e `SCAMBIO_RECENTE.md` (estratti brevi, rigenerati a ogni turno). `RICHIESTA.md` e `SCAMBIO.md` sono di sola lettura per gli agenti (regola 27).
- Memoria: se presenti, leggi `bacheche/<agente>/RIASSUNTO.md` (max 12 righe) e `bacheche/<agente>/DIARIO_RECENTE.md` (ultimi 6 appunti); a fine turno aggiungi la riga di diario (regola 27) e riscrivi il riepilogo breve.
- Risposte corte: niente riepiloghi lunghi, niente copie di file.

## Regola 26 — build e test (verificata il 17 ott 2026: la build funziona)
- cmake e il compilatore C++ FUNZIONANO e i permessi per usarli sono gia' concessi a @developer e @debugger. Il vecchio "shell negata (regola 21)" era un errore di diagnosi: i comandi provati erano composti (`which cmake; echo`, `cmake ... | head`, `ls ...; ls ...`), e i permessi confrontano il comando INTERO.
- Un solo comando, dalla radice del progetto: `sh tools/gate_t1.sh` (smoke 10 s sui 3 livelli + replay 2x) oppure `sh tools/gate_t1.sh soak` (in piu' 30000 frame = 10 min simulati). Senza pipe, senza `2>&1`, senza `;` `&&` `echo` `which`. Il dettaglio va in `out/debugger/gate_t1.log`, a video escono solo le righe utili; exit code 0 = tutto OK.
- Per un solo passo: `cmake -S motore_nuovo -B build_gate` poi `cmake --build build_gate --target prova_gate` (due comandi separati, semplici).
- Scrivi "build non eseguibile" SOLO se `sh tools/gate_t1.sh` stesso viene negato, e riporta il testo esatto dell'errore in SCAMBIO.md. Se un tuo comando composto viene negato, rilancia la versione semplice: non fermare il blocco.
- Un esito di build/gate vale solo se riporti exit code e le righe di riepilogo del log (`replay 2x`, `ms/frame`, `GATE OK`/`FALLITO`). Mai "OK" dedotto leggendo il codice.
- Il gate controlla: partenza (speed>0 al frame 60), replay deterministico (frameHash identico ogni frame fra due run), ms/frame <= 20, offroad raggiungibile; scrive 3 screenshot in `out/debugger/<livello>/`.

## Regola 27 — diario a riga fissa (dal 17 ott 2026)
- A fine turno ogni agente aggiunge UNA riga al proprio `bacheche/<agente>/diario.md`.
  `[GG/MM hh:mm] blocco | stato | esito misurato (exit code) | prossimo`
  Esempio: `[17/10 14:05] H | [~] | sh tools/gate_t1.sh soak exit 0, replay 2x IDENTICO, 0,09 ms/frame | Pi 400 fps/temperatura (Andrea)`
- «esito misurato» = comando + exit code + il numero letto dal log. Se non hai eseguito nulla scrivi `non eseguito`: mai un «OK» dedotto dal codice o da un altro file.
- `stato` e' la tua PROPOSTA. Lo scrivono in RICHIESTA.md solo Andrea e Claude; @capo propone in `out/capo/STATI_PROPOSTI.md`. Per H e P1 serve anche il via di Andrea (T1-RUN punto 8).
- SCAMBIO.md e RICHIESTA.md li scrivono solo Andrea e Claude (e il ciclo, per i CHECKPOINT): nessun agente OpenCode, @capo compreso. La voce pronta va in `VOCE_SCAMBIO_<n>.md` nella tua cartella (coder e debugger: `motore_nuovo/test/`) e la riga di diario chiude con `prossimo: Andrea/Claude incollano VOCE_SCAMBIO_<n>`. @capo usa i suoi canali: `out/capo/STATI_PROPOSTI.md`, DECISIONI_ANDREA.md, PER_CLAUDE.md.
- Una pausa (regola 14) e' un ciclo senza lavoro: si annota con la stessa riga, non al Bar.
- RICHIESTA.md e' in sola lettura per gli agenti, @capo compreso (e' il file che il ciclo legge per decidere chi lavora: una riscrittura sbagliata lo rompe). Le celle Stato restano di una riga (circa 200 caratteri) e c'e' un solo `## RIEPILOGO CORRENTE`, tenuti da Andrea e Claude. Storico: `out/capo/RICHIESTA_STORICO_17OTT.md`.
