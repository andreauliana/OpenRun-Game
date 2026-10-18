# V4 (OpenWay) — regole per tutti gli agenti

Build di test; i nomi interni e alcuni comandi conservano la sigla V3. Lavoro principale: motore nuovo in `motore_nuovo/` (pista C di `../PIANO_V3.md`). L'hack Mega Drive v87 (`tools/`, `lr/`, `shots/`: HANDOVER.md -> BUILD.md) e' solo manutenzione. Testo integrale delle regole prima della compattazione del 17 ott: `archivio_per_claude/AGENTS_prima_compattazione_17OTT.md`.

## Regole generali
1. Nessun materiale Sega: niente ROM, `.gen`, sprite, musiche dell'originale. Codice di altri solo con licenza libera e fonte nel commento.
2. Ogni risorsa esterna in `assets_originali/CREDITI.md` (autore, licenza, URL).
3. Lavora per blocchi di `RICHIESTA.md` (sola lettura per te). Un blocco = un risultato verificabile. A fine blocco: voce `VOCE_SCAMBIO_<n>.md` e stato proposto nella riga di diario (regola 27).
4. Resta nel tuo ruolo e nei tuoi permessi. Se serve un altro ruolo, scrivilo nella voce: il ciclo ti avvia comunque a ogni turno.
5. Pi 400, 50 fps, niente decorazioni non richieste; leggi VISIONE.md (vincoli duri e architettura). 5b) Determinismo: tempo = contatore di frame, RNG con seed, nessun orologio di sistema; il nucleo non dipende da SDL (solo il runner di test).
6. Non cancellare file. Non toccare `../` (la v2 e' di altri).

## Squadra e cartelle (regola 15)
Scrivi solo qui, oltre a `bacheche/<agente>/`, `out/<agente>/` e `BACHECA_PUBBLICA.md`. Leggere le cartelle altrui e' permesso, scriverci no.
| Agente | Fa | Scrive in |
|---|---|---|
| capo | coordina, propone stati e deleghe | `out/capo/`, `DECISIONI_ANDREA.md`, `ASSEMBLEA.md`, `PER_CLAUDE.md`, `out/INDICE.md`, `libreria/INDICE.md`, `FINITO` |
| architetto | architettura, formati dati, build, licenze | `motore_nuovo/docs/` |
| coder | nucleo C++ (strada, rendering, gioco, input, audio) | `motore_nuovo/src/`, `motore_nuovo/CMakeLists.txt`, `tools/patch_z.py` |
| traduttore | porta nel motore regole gia' fatte (personaggi, fumetti, escalation) | `motore_nuovo/dati/personaggi/`, `motore_nuovo/dati/frasi.json`, `motore_nuovo/src/gameplay/`, `motore_nuovo/docs/` |
| livelli | progetto livelli come dati JSON | `motore_nuovo/dati/livelli/` |
| scenografo | grafica dei livelli | `assets_originali/scenari/` |
| tipografo | font, titoli, menu, HUD | `assets_originali/ui/`, `motore_nuovo/dati/ui/` |
| asset | sagome, sprite, palette | `assets_originali/` |
| cercaasset | cerca immagini/suoni/font con licenza libera | `assets_originali/ricerca/`, `assets_originali/CREDITI.md` |
| testi | frasi di Gianni e Gesu' | `tools/gianni12.py`, `motore_nuovo/dati/testi/`, `motore_nuovo/dati/frasi.json` |
| debugger | build e test, cerca le cause; non modifica il codice di prodotto | `motore_nuovo/test/`, `libreria/test/` |
| reviewer | controlla codice, licenze, asset; non modifica il prodotto | `out/REVIEW/` |
Sprite e vocabolari nuovi stanno nei dati della tua cartella e nel CATALOGO; la galleria li mostra.

## Regola 6 — reparti e catalogo
Reparto grafico (@cercaasset, @asset, @testi, @scenografo, @tipografo) e tecnico (@architetto, @coder, @traduttore, @livelli, @debugger). Ogni funzione extra e' un modulo in `motore_nuovo/src/fx/<nome>/` con flag on/off e dati propri. Ogni personaggio/asset ha cartella + manifest + riga in `motore_nuovo/docs/CATALOGO_PERSONAGGI.md`; prima di consegnare: `python3 motore_nuovo/strumenti_py/check_catalogo.py`. Dettagli: `motore_nuovo/docs/MODULI_E_REPARTI.md`.

## Regola 7 — conservazione e livelli
Il lavoro della v87 non si butta: `motore_nuovo/docs/ARCHIVIO_V87.md` (sola lettura). Ogni livello ha il suo scenario e il suo `livello.json`; tutte le varianti dell'albero di tappe vanno considerate, ciascuna con stile diverso.

## Regola 8 — UI e musica
Font, titoli, menu, HUD: @tipografo. La musica del tracker di Andrea e' solo RIFERIMENTO: l'audio e' guidato da dati (`motore_nuovo/dati/audio/playlist.json`), nessun brano e' definitivo.

## Regola 9 — versioni
Il v3 si numera da v90 (v91, ...); ogni build/consegna riporta il numero nella voce. ROM Mega Drive originale (blocco N): facoltativa, solo dopo che il motore C funziona.

## Regola 10 — quota open source
Almeno 50% open source: ogni file nuovo ha licenza dichiarata (SPDX o `manifest.json` 'licenza'). Prima di consegnare: `python3 motore_nuovo/strumenti_py/quota_open.py`, valore nella voce. La licenza e' decisione di Andrea (`../OPENSOURCE_PLAN.md`).

## Regola 11 — decisioni e comandi
Non aspettare Andrea: scrivi la decisione (domanda, opzioni, DEFAULT, raccomandazione) nella tua voce, usa il DEFAULT e vai avanti sui blocchi indipendenti. I comandi Bash sono concessi per ruolo (build/test a coder e debugger; Python/Pillow nelle aree grafiche; generatore frasi a testi; web solo ad architetto e cercaasset). Un comando COMPOSTO negato (pipe, `2>&1`, `;`, `&&`, `echo`, `which`) non significa shell negata: regola 26.

## Regola 12 — controlli
Ogni 2 ore c'e' un CHECKPOINT automatico: @reviewer controlla i file modificati (max 3 rilievi) e salta se non ci sono modifiche pertinenti. Niente review per ogni consegna. Revisione umana dopo 6-8 ore.

## Regola 13 — difficolta' e passaggi
Dopo 2 tentativi falliti sullo stesso problema, o a fine di un blocco importante: non insistere. Scrivi nella voce una riga `PASSAGGIO: <da> -> <a> | blocco <X> | motivo | cosa resta da fare` (esempio: @coder -> @debugger se e' un bug) o `RICHIESTA_CAMBIO_MODELLO: <agente>`, e passa a un altro punto (regola 28). Formato di `PER_CLAUDE.md` (lo scrive @capo):
```
AGENTE: <agente in difficolta' o ->
PROBLEMA: <una riga>
PROVATO: <comandi e numeri>
DOMANDE: <max 5>
FILE DA CONTROLLARE: <percorsi>
```
Si riparte dal file, non da zero: nessun passaggio cancella il lavoro fatto.

## Regola 14 — assemblee
Diritti: ogni agente puo' obiettare, proporre e votare sulla sua area. Per una scelta fra team (grafica vs tecnica, stile di un livello, formato dei dati) @capo apre una voce in `ASSEMBLEA.md`: domanda, opzioni, partecipanti; modera, max 2 giri, interventi max 5 righe solo di argomenti, un voto per agente (A/B/astenuto); maggioranza vince, parita' decide @capo; scelte di fondo (licenze, tono, soldi, legale) vanno ad Andrea. Esito e motivazioni restano scritti. Non aprire assemblee per decisioni di routine; leggi `ASSEMBLEE_APERTE.md` solo se pertinente. Galleria: `python3 motore_nuovo/strumenti_py/galleria.py`.

## Regola 16 — fedelta' e contenuti
Almeno 50% di somiglianza di struttura/sensazione con l'originale, misurata in `motore_nuovo/docs/FEDELTA.md` (solo meccaniche). Stili e limiti: `assets_originali/DIREZIONE_ARTISTICA.md`.

## Regola 17 — bacheca pubblica
`BACHECA_PUBBLICA.md`: spazio facoltativo per note leggere. Aggiungi in fondo, max 2 righe, non modificare i messaggi altrui; niente decisioni, richieste di lavoro o dati personali; tono rispettoso. @capo vede solo le ultime righe e non risponde ai messaggi informali.

## Regola 18 — dove va cosa
Ogni informazione sta in UN posto; gli altri file la richiamano per nome.
| Cosa | Dove | Chi scrive |
|---|---|---|
| Riga di diario a fine turno (regola 27) | `bacheche/<agente>/diario.md` | tu |
| Stato corrente, max 12 righe, riscritto a ogni turno (stato e cosa resta, senza ripetere il diario) | `bacheche/<agente>/RIASSUNTO.md` | tu |
| Bozze, script di prova, appunti | `bacheche/<agente>/bozze/` | tu |
| Consegne finite | `out/<agente>/` | tu |
| Voce per SCAMBIO.md | `VOCE_SCAMBIO_<n>.md` (regola 27) | tu |
| `DIARIO_RECENTE.md` (ultime 6 righe), `diario_archivio.md`, `log_agenti/*.log` | cartelle del ciclo | solo il ciclo: non scriverli, non aprire archivio e log |
Inizio turno: leggi `RIASSUNTO.md` e `DIARIO_RECENTE.md`. Bacheca max 300 MB (comprimi bozze vecchie, mai il diario); non e' consegna e non conta per quota open o catalogo.

## Regola 19 — flusso dei file
Bozza (bacheca) -> consegna (cartella ufficiale e `out/<agente>/`) -> controllo @reviewer al checkpoint -> `libreria/` (materiale riusabile e approvato; scrittura per ruolo, tabella in `libreria/LEGGIMI.md`). Quota open e catalogo contano `out/` e `libreria/`.

## Regola 21 — perimetro
Lavori SOLO dentro la cartella del progetto. Mai altre cartelle del Mac (soprattutto Outrun_OpenCode v2 e la vecchia v3), altre app, installazioni, comandi di sistema. cmake, make, g++/clang++ e `sh tools/gate_t1.sh` NON sono comandi di sistema: sono gli strumenti di build di @coder e @debugger (regola 26). Rete solo per @architetto e @cercaasset. Se un comando e' negato non aggirarlo: annotalo nella voce e continua (regola 28).

## Regola 22 — collegamento fra agenti
Nessuno passa un parametro `model` quando delega: ogni agente usa il modello del proprio `.opencode/agents/<nome>.md`. Se una delega fallisce NON ritentare: il ciclo avvia comunque ogni agente direttamente a ogni turno ("giro diretto"), quindi il lavoro non dipende dal capo. Il canale reale sono i file: `RICHIESTA.md` (cosa), `SCAMBIO.md` (messaggi), `out/` e `libreria/` (risultati).

## Regola 23 — libreria REA
Prima di un lavoro che tocca il motore o le aree: `libreria/REA_AGENTI.md` (regole vincolanti) e `libreria/REA_METODO.md` (metodo). Per aree, sprite, musica e testi solo asset originali del progetto e le SPEC_*; no derivazioni da OutRun o da opere protette. In conflitto con le regole 16 o 21 vince la piu' restrittiva.

## Regola 24 — cartella ANDREA
Il file principale e' `../README.md` (INIZIA QUI). `../ANDREA/` e' solo di Andrea: leggi, mai scrivere. Se in `../ANDREA/sprite_ritoccati/` c'e' un file con lo stesso nome di un asset, usa quello e non rigenerarlo.

## Regola 25 — risparmio risorse
- Bar: `BAR.md` e `.registro_bar/` sono di Andrea e materiale storico: non aprirli, non scriverci, non archiviarli.
- Leggi solo gli estratti: `RICHIESTA_APERTA.md` e `SCAMBIO_RECENTE.md` (ultime 3 voci, troncate), `BACHECA_RECENTE.md`. `RICHIESTA.md` e `SCAMBIO.md` interi non servono (sono di Andrea e Claude).
- Risposte corte: niente riepiloghi lunghi, niente copie di file, niente rilettura di file gia' letti nel turno.

## Regola 26 — build e test
- cmake e il compilatore C++ funzionano e sono gia' permessi a @coder e @debugger. Un solo comando dalla radice: `sh tools/gate_t1.sh` (smoke 10 s sui 3 livelli + replay 2x) oppure `sh tools/gate_t1.sh soak` (in piu' 30000 frame). Senza pipe, `2>&1`, `;`, `&&`, `echo`, `which`: i permessi confrontano il comando INTERO. Dettaglio in `out/debugger/gate_t1.log`; exit 0 = OK.
- Un solo passo: `cmake -S motore_nuovo -B build_gate`, poi `cmake --build build_gate --target prova_gate` (due comandi separati).
- Scrivi "build non eseguibile" SOLO se `sh tools/gate_t1.sh` stesso e' negato, con il testo esatto dell'errore. Se un composto e' negato, rilancia la versione semplice.
- Un esito vale solo con exit code e righe di riepilogo del log (`replay 2x`, `ms/frame`, `GATE OK`/`FALLITO`): mai "OK" dedotto dal codice.
- Il gate controlla: speed>0 al frame 60, replay deterministico (frameHash identico fra due run), ms/frame <= 20, offroad raggiungibile; 3 screenshot in `out/debugger/<livello>/`. NON misura gli fps reali su Pi 400.

## Regola 27 — diario, voci, fogli
- A fine turno UNA riga in `bacheche/<agente>/diario.md`: `[GG/MM hh:mm] blocco | stato | esito misurato (exit code) | prossimo`. Esempio: `[17/10 14:05] H | [~] | sh tools/gate_t1.sh soak exit 0, replay 2x IDENTICO, 0,09 ms/frame | Pi 400 fps/temperatura (Andrea)`. «Esito misurato» = comando + exit code + numero letto dal log; se non hai eseguito nulla: `non eseguito`.
- `stato` e' una PROPOSTA. `RICHIESTA.md` e `SCAMBIO.md` li scrivono solo Andrea e Claude (e il ciclo, per i CHECKPOINT): nessun agente OpenCode, @capo compreso. Per H e P1 serve anche il via di Andrea (T1-RUN punto 8).
- La voce pronta va in `VOCE_SCAMBIO_<n>.md` nella tua cartella (coder e debugger: `motore_nuovo/test/`); la riga di diario chiude con `prossimo: Andrea/Claude incollano VOCE_SCAMBIO_<n>`. Mettici anche decisioni, passaggi (regola 13) e comandi negati.
- @capo propone stati e deleghe in `out/capo/STATI_PROPOSTI.md` (lo sostituisce a ogni turno); domande ad Andrea in `DECISIONI_ANDREA.md`, richieste a Claude in `PER_CLAUDE.md`.
- `RICHIESTA.md`: celle Stato di una riga (~200 caratteri) e un solo `## RIEPILOGO CORRENTE`. Storico: `out/capo/RICHIESTA_STORICO_17OTT.md`.

## Regola 28 — flusso continuo (dal 17 ott)
- Non aspettare nessuno (Andrea, capo, voci incollate): se un punto e' bloccato, annotalo nel diario e passa al successivo del tuo blocco; poi riparti dal tuo `RIASSUNTO.md`.
- Nessuna pausa programmata: il ciclo gestisce da solo i limiti di richieste e i cambi di modello. Pausa solo se sei in difficolta' (regola 13).
- Turno max 15 minuti: lascia i file in stato coerente; un file lasciato a meta' si segnala nel diario e si chiude al turno dopo, partendo dai blocchi [~].
- Un comando negato: una sola volta la versione semplice, poi annota e continua. Una delega fallita: non ritentare.
