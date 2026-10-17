# v4.5 — regole per tutti gli agenti

Lavoro principale: motore nuovo C++17 in `motore_nuovo/`. Prima di tutto leggi `VISIONE.md` (idea, vincoli duri, architettura).
Queste regole sono CONGELATE per tutta la v4.5: nessun agente le modifica, le cambia solo Andrea. Le 28 regole della v4 sono in `archivio_v4/AGENTS_v4.md` (sola lettura, solo come storico).

## Le 5 regole d'oro
1. **Un foglio, uno scrittore.** Ogni file ha un solo ruolo che lo scrive (tabella sotto). Leggere i file degli altri si', scriverli no.
2. **Una riga di diario per turno**, in `bacheche/<ruolo>/diario.md`: `[GG/MM hh:mm] blocco | stato | esito misurato | prossimo`. «Esito misurato» = comando + exit code + numero letto dal log; se non hai eseguito nulla scrivi `non eseguito`.
3. **Promuovono a [x] solo Andrea e Claude.** Gli agenti propongono uno stato; `RICHIESTA.md` e `SCAMBIO.md` li scrivono solo Andrea e Claude. H e P1 passano a [x] solo con l'ok di Andrea.
4. **Regole congelate.** Niente nuove regole, niente modifiche a questo file durante la v4.5. Un dubbio su una regola va in `DECISIONI_ANDREA.md` (lo scrive @capo).
5. **Script rosso = torna allo sviluppatore.** Se un controllo automatico (`sh tools/gate_t1.sh`, `check_catalogo.py`, `quota_open.py`) esce diverso da 0, il passo torna a @sviluppatore con il log; non si chiama nessun altro modello per discuterne.

## Squadra: 5 ruoli
Nel ciclo base lavorano solo @sviluppatore e @collaudatore. @capo coordina un giro su tre. @grafico e @ricercatore partono solo su richiesta (Andrea li aggiunge con `AGENTI_ATTIVI`).
| Ruolo | Fa (ex ruoli v4) | Scrive in (oltre a `bacheche/<ruolo>/`, `out/<ruolo>/`, `BACHECA_PUBBLICA.md`) |
|---|---|---|
| capo | coordina, propone stati e deleghe (capo) | `out/capo/STATI_PROPOSTI.md`, `DECISIONI_ANDREA.md`, `ASSEMBLEA.md`, `PER_CLAUDE.md`, `out/INDICE.md`, `libreria/INDICE.md`, `FINITO` |
| sviluppatore | nucleo C++, gameplay, livelli come dati (coder, traduttore, livelli) | `motore_nuovo/src/`, `motore_nuovo/CMakeLists.txt`, `motore_nuovo/dati/livelli/`, `motore_nuovo/dati/personaggi/`, `motore_nuovo/dati/sprite_raw/`, `motore_nuovo/strumenti_py/`, `motore_nuovo/dati/SPEC_GAMEPLAY.md`, `motore_nuovo/docs/LIVELLI.md`, `motore_nuovo/docs/CATALOGO_PERSONAGGI.md`, `libreria/codice/`, `libreria/livelli/` |
| collaudatore | build, test, review, architettura (debugger, reviewer, architetto) | `motore_nuovo/test/`, `motore_nuovo/docs/` (tranne i due file dello sviluppatore), `libreria/test/`, `libreria/docs/` |
| grafico | sprite, scenari, palette, font, UI, frasi (asset, scenografo, tipografo, testi) | `assets_originali/` (tranne `ricerca/`), `motore_nuovo/dati/ui/`, `motore_nuovo/dati/testi/`, `motore_nuovo/dati/frasi.json`, `libreria/sprite/`, `libreria/palette/`, `libreria/scenari/`, `libreria/font/`, `libreria/testi/` |
| ricercatore | cerca risorse con licenza libera, su richiesta (cercaasset) | `libreria/ricerca/` (proposte; le righe di `CREDITI.md` le scrive @grafico) |
Il lavoro dei vecchi ruoli e' in `bacheche/<ruolo>/v4_<vecchio_ruolo>/` e `out/<ruolo>/v4_<vecchio_ruolo>/`: si legge, non si riscrive.

## Fogli comuni
| Cosa | Dove | Chi scrive |
|---|---|---|
| Blocchi di lavoro e stati | `RICHIESTA.md` (leggi l'estratto `RICHIESTA_APERTA.md`) | Andrea e Claude |
| Messaggi e voci consegnate | `SCAMBIO.md` (leggi l'estratto `SCAMBIO_RECENTE.md`) | Andrea e Claude |
| Voce pronta da incollare | `out/<ruolo>/VOCE_SCAMBIO_<n>.md` (collaudatore: `motore_nuovo/test/`) | tu |
| Stato corrente, max 12 righe, riscritto ogni turno | `bacheche/<ruolo>/RIASSUNTO.md` | tu |
| Bozze e prove | `bacheche/<ruolo>/bozze/` | tu |
| Proposte di stato e deleghe | `out/capo/STATI_PROPOSTI.md` | capo |
| Scelte di Andrea | `FEEDBACK_ANDREA.md` (copia di `../ANDREA/FEEDBACK.md`) | solo il ciclo |
| `DIARIO_RECENTE.md`, estratti, `CONTROLLO.md`, `log_agenti/` | cartelle del ciclo | solo il ciclo |
Inizio turno: `RIASSUNTO.md` e `DIARIO_RECENTE.md` della tua bacheca, poi gli estratti. Non aprire `RICHIESTA.md` e `SCAMBIO.md` interi.

## Vincoli che restano dalla v4
- **Niente materiale Sega** (ROM, sprite, musiche, tracciati, palette, codice 68000). Codice di altri solo con licenza libera e fonte nel commento. Ogni risorsa esterna in `assets_originali/CREDITI.md`.
- **Pi 400 a 50 fps**, 320x224, determinismo: tempo = contatore di frame, RNG con seed, nessun orologio di sistema; il nucleo non dipende da SDL.
- **Perimetro**: solo dentro la cartella del progetto, percorsi relativi. Mai `../` tranne la lettura di `../ANDREA/` e dei documenti in radice. Non cancellare file. Rete solo per @collaudatore e @ricercatore.
- **Quota open** almeno 50%: ogni file nuovo ha licenza (SPDX o `manifest.json`). Prima di consegnare: `python3 motore_nuovo/strumenti_py/quota_open.py`; asset e personaggi: `python3 motore_nuovo/strumenti_py/check_catalogo.py`.
- **Cartella ANDREA**: `../ANDREA/` e' solo di Andrea. Se in `../ANDREA/sprite_ritoccati/` c'e' un file con lo stesso nome di un asset, usa quello.
- **Musica**: il tracker di Andrea e' riferimento; l'audio e' guidato da `motore_nuovo/dati/audio/playlist.json`.
- **MD0 (hack Mega Drive) e' sospeso**: non lavorarci.

## Build e test (ex regola 26)
- Un solo comando dalla radice `sglavinated_src/`: `sh tools/gate_t1.sh` (smoke sui 3 livelli + replay 2x) oppure `sh tools/gate_t1.sh soak` (in piu' 30000 frame). Senza pipe, `2>&1`, `;`, `&&`, `echo`, `which`: i permessi confrontano il comando INTERO, e un composto negato non vuol dire shell negata.
- Log in `out/collaudatore/gate_t1.log`, screenshot in `out/collaudatore/<livello>/`. Exit 0 = OK. Il gate controlla: velocita' > 0 al frame 60, frameHash identico fra due run, ms/frame <= 20, fuoristrada raggiungibile. Non misura gli fps reali sul Pi 400.
- Un esito vale solo con exit code e righe del log (`replay 2x`, `ms/frame`, `GATE OK`/`FALLITO`), mai dedotto dal codice.

## Turni e difficolta' (ex regole 13 e 28)
- Turno max 15 minuti; lascia i file coerenti. Non aspettare nessuno: se un punto e' bloccato, annotalo nel diario e passa al successivo.
- Dopo 2 tentativi falliti sullo stesso problema: una riga `PASSAGGIO: <da> -> <a> | blocco | motivo | cosa resta` nella voce, e passa ad altro. @capo scrive `PER_CLAUDE.md` (AGENTE, PROBLEMA, PROVATO, DOMANDE max 5, FILE DA CONTROLLARE).
- Nessuno passa un parametro `model`: ogni ruolo usa il modello del proprio `.opencode/agents/<ruolo>.md`. Una delega fallita non si ritenta.
- Risposte corte: niente riepiloghi lunghi, niente copie di file.
