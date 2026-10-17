# OutRun Sglavinated Edition v4.5 — INIZIA QUI

Copia aggiornata della v4 (OpenWay) con la struttura semplificata del canvas "Studio v4.5": 5 ruoli al posto di 12, meno file, regole congelate.
La cartella v4 originale sul Desktop non e' stata toccata: resta il riferimento completo (hack Mega Drive, log, build).

## Avvio
```
cd "$HOME/Desktop/4.5 updated"
sh comandi/v3.sh start
```
Stato e stop: `sh comandi/v3.sh stato` e `sh comandi/v3.sh stop`. Pannello su http://localhost:8090 (la v4 usa la 8089).
I nomi interni `V3` (`comandi/v3.sh`, `V3_BASE`) restano per compatibilita': i comandi funzionano come nella v4.
Il motore va ricompilato al primo avvio (le cartelle `build*` non sono state copiate): `cd sglavinated_src && sh tools/gate_t1.sh`.
Sul Pi 400: `sh "$HOME/4.5 updated/pi/compila_e_installa.sh"`.

## Cosa cambia rispetto alla v4
| | v4 | v4.5 |
|---|---|---|
| Ruoli | 12 | 5: capo, sviluppatore, collaudatore, grafico, ricercatore |
| Ciclo base | coder + debugger, capo quasi a ogni giro | sviluppatore + collaudatore, capo un giro su tre |
| Regole | 1-28 in `AGENTS.md` | 5 regole d'oro congelate + i vincoli tecnici essenziali |
| Checkpoint | @reviewer | @collaudatore |

### I 5 ruoli
| Ruolo | Unisce (v4) | Quando lavora |
|---|---|---|
| capo | capo | un giro su tre |
| sviluppatore | coder, traduttore, livelli | ciclo base |
| collaudatore | debugger, reviewer, architetto | ciclo base e checkpoint |
| grafico | asset, scenografo, tipografo, testi | su richiesta |
| ricercatore | cercaasset | su richiesta |
Per attivare anche il grafico: `AGENTI_ATTIVI="sviluppatore collaudatore grafico" sh comandi/v3.sh start`.

### Le 5 regole d'oro (in `sglavinated_src/AGENTS.md`)
1. Un foglio, uno scrittore.
2. Una riga di diario per turno: `blocco | stato | esito misurato | prossimo`.
3. Solo Andrea e Claude promuovono a [x]; H e P1 vogliono l'ok di Andrea.
4. Regole congelate per tutta la v4.5.
5. Uno script rosso rimanda il passo allo sviluppatore, senza chiamare altri modelli.

## Dove sono le cose
- `sglavinated_src/` — progetto degli agenti: `AGENTS.md`, `RICHIESTA.md`, `SCAMBIO.md`, `VISIONE.md`, `opencode.json`.
- `sglavinated_src/.opencode/agents/` — le 5 definizioni dei ruoli (modelli e permessi).
- `sglavinated_src/motore_nuovo/` — motore C++17 (src, dati, docs, test, strumenti_py).
- `sglavinated_src/assets_originali/`, `libreria/` — asset originali e materiale riusabile.
- `sglavinated_src/bacheche/<ruolo>/` e `out/<ruolo>/` — bacheche e consegne. Il lavoro dei vecchi ruoli e' in `v4_<vecchio_ruolo>/` dentro il ruolo nuovo.
- `sglavinated_src/out/collaudatore/` — log del gate e screenshot (prima in `out/debugger/`).
- `sglavinated_src/archivio_v4/` — documenti superati: `AGENTS_v4.md` (regole 1-28), le 12 definizioni agenti v4, il README v4 con la technical review, note vecchie.
- `ANDREA/` — solo di Andrea. `comandi/`, `strumenti/`, `pi/`, `locale/` — come nella v4.

## Cosa non e' stato copiato (resta nella cartella v4)
- Filone Mega Drive (MD0 sospeso): `lr/`, `shots/`, `gpgx/`, `gstatic/`, `hud/`, `rst/`, `smoke/` e gli strumenti della hack in `tools/`.
- `outrun_src/`: materiale derivato dalla ROM Sega, escluso per la regola "niente materiale Sega".
- Cartelle `build*`, `log_agenti/`, log in radice, `backup_aggiornamenti/`, la copia duplicata `.opencode/agent/`, `__pycache__`, `.DS_Store`.
- Dalle uscite del debugger: i file `.ppm` e le cartelle del soak (si rigenerano col gate).

## Decisioni v4.5 (default scelti, cambiabili)
1. Il lavoro dei 12 ruoli non e' fuso nei diari nuovi: sta in `v4_<vecchio_ruolo>/`, e ogni ruolo nuovo parte con un `RIASSUNTO.md` che riprende lo stato.
2. Modelli: sviluppatore e collaudatore `longcat-2.5-preview` (quello che faceva le build nella v4), capo `nemotron-3-ultra`, grafico `mimo-v2.6-flash`, ricercatore `nemotron-3.5-lightning`. La panchina in `strumenti/panchina.txt` non cambia.
3. Il capo lavora al primo giro e poi ogni terzo giro (3, 6, 9...).
4. Grafico e ricercatore sono spenti nel ciclo base; il giro parallelo `strumenti/giro.sh` usa i 4 ruoli operativi.
5. Un file, un ruolo: `frasi.json` e `CREDITI.md` li scrive solo il grafico; il ricercatore propone in `libreria/ricerca/`.
6. `RICHIESTA.md` e' aggiornato solo nei nomi dei ruoli (@coder -> @sviluppatore, @debugger -> @collaudatore, ecc.) e nei percorsi `out/collaudatore/`; stati e contenuti dei blocchi sono quelli della v4.

## Stato al passaggio (17 ott 2026)
Tappa 1. Gate reale OK (3 livelli, replay 2x identico, 0,09 ms/frame, soak 30000 frame). Aperti: H, P1, T1-RUN (punto 4 sprite veri nel gate, punto 8 test di Andrea su Mac e Pi 400), M, J. MD0 e T5 in attesa.
