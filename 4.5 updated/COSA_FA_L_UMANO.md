# COSA DEVE FARE ANDREA (aggiornato 16 ott 2026)

Gli agenti e i comandi sono gia' dentro questa build di test. I nomi interni `V3` e `v3.sh` restano invariati.
Cartella: `$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)`

## A. UNA VOLTA SOLA
Gli script `.sh` vengono eseguiti con `sh`, quindi non serve renderli eseguibili. I file `.command` di questa copia sono gia' eseguibili; se macOS non li apre con il doppio clic:
```
cd "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)"
chmod u+x comandi/*.command
```

La configurazione `sglavinated_src/opencode.json` e' gia' presente in questa build: build/test sono concessi a coder e debugger, gli script Python/Pillow grafici solo nelle aree grafiche dei rispettivi agenti, e l'accesso web ad architetto e cercaasset. La directory esterna punta alla cartella v4. Non sostituirla con quella della v3.

Controlla i modelli:
```
sh strumenti/modelli.sh
```
Se una riga dice MANCA, passa l'agente al modello successivo disponibile:
```
sh strumenti/cambia_modello_auto.sh NOME_AGENTE
```

## B. AVVIO DEL CICLO (comando unico)
```
cd "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)"
sh comandi/v3.sh start
```
Altri comandi, dalla cartella del progetto:

| Comando | Cosa fa |
|---|---|
| `sh comandi/v3.sh stato` | stato del ciclo e ultimi log |
| `sh comandi/v3.sh vivo` | segue il log in tempo reale |
| `sh comandi/v3.sh pannello` | pannello web di sola lettura su http://localhost:8089 |
| `sh comandi/v3.sh stop` | ferma il ciclo a fine turno |
| `sh comandi/v3.sh stop-subito` | ferma il ciclo e la richiesta OpenCode corrente di questa build; non termina processi di altri progetti |
| `sh comandi/v3.sh aggiorna` | ferma in sicurezza e riavvia usando gli script attualmente installati |

Anche il doppio clic su `V3 Avvia.command`, `V3 Stato.command`, `V3 Ferma.command` e `V3 Aggiorna e riavvia.command` usa la cartella di questa build.

Note:
- Il ciclo tiene sveglio il Mac con `caffeinate` (se disponibile) e si ferma dopo 12 ore (`MAX_ORE`) o 1000 cicli (`MAX_CICLI`).
- In caso di limite richieste aspetta 15, 30, 60 o 120 minuti.
- Checkpoint ogni 2 ore; il reviewer controlla solo i file cambiati, al massimo 3 rilievi, e salta il turno se non ci sono modifiche. Niente review a ogni consegna; Claude CLI resta disattivato (`CHK_CLAUDE_OGNI=0`).
- Prima di ogni turno il ciclo prepara gli estratti di richieste, scambi, bacheca pubblica e assemblee aperte; la memoria breve conserva 12 righe di riepilogo e gli ultimi 20 appunti per agente.
- Lascia il Mac collegato alla corrente.

## C. MODO MANUALE (per provare un singolo agente)
```
cd "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)/sglavinated_src"
opencode
```
Poi scrivi:
```
Leggi ../TAPPE.md e lavora solo sul Gate della tappa attiva. Per la tappa 1, concentrati sul primo test C++ giocabile: riprendi in RICHIESTA.md solo blocchi [ ] o [~] necessari a quel Gate, usa coder/debugger, non avviare lavoro di tappe future. Aggiorna lo Stato e registra in SCAMBIO.md prove, esito e blocchi rimasti.
```

## D. COSE SOLO TUE
| Quando | Cosa |
|---|---|
| Prototipo pronto | provalo sul Pi 400 o sul Mac; scrivi in SCAMBIO.md fps e temperatura |
| Varianti di asset | scegli tu (scenografo: A/B/C per livello) |
| Sprite principali (Gianni, Gesu', UFO) | li disegni tu |
| Scelte aperte | `DECISIONI_ANDREA.md`; D9 (regola make/clang++) non ancora aperta |
| Build completa (`cmake --build`) | quando vuoi, sul Mac o sul Pi 400 |
| Prima di distribuire qualcosa (sponsor compreso) | legale |

## E. NON FARE
- Non lavorare in `Outrun_OpenCode v2` (e' degli altri).
- Niente ROM Sega ne' file `.gen` in questa cartella.
- Non cancellare voci di `SCAMBIO.md`.

## F. FILE OBSOLETI
Non eseguire la vecchia riga `mv` verso il Cestino: si riferiva alla cartella v3 e poteva spostare file al di fuori di questa build. Prima di qualsiasi pulizia, controlla manualmente i file effettivamente presenti.

Da controllare prima di spostarli, perche' li genera ancora qualche script:
`PIANO_TURNI.md`, `PACCHETTO_PER_CLAUDE_APP.md`.

## Dove sta cosa
| File | A cosa serve |
|---|---|
| `comandi/v3.sh` | comando unico di avvio, stato e stop |
| `strumenti/ciclo.sh` | ciclo degli agenti (lo lancia `v3.sh`) |
| `sglavinated_src/RICHIESTA_APERTA.md`, `SCAMBIO_RECENTE.md` | estratti brevi generati dal ciclo |
| `sglavinated_src/BACHECA_RECENTE.md`, `ASSEMBLEE_APERTE.md` | estratti recenti per coordinamento e decisioni trasversali |
| `strumenti/modelli.sh` | controlla i modelli assegnati |
| `strumenti/cambia_modello_auto.sh` | passa l'agente al modello successivo della panchina |
| `strumenti/pannello.py` | pannello web di sola lettura |
| `sglavinated_src/CONTROLLO.md` | stato automatico (non modificare) |
| `sglavinated_src/RICHIESTA.md` | foglio incarichi |
| `sglavinated_src/SCAMBIO.md` | risposte degli agenti |
| `sglavinated_src/DECISIONI_ANDREA.md` | le tue decisioni |
| `sglavinated_src/bacheche/<agente>/DIARIO_RECENTE.md` | ultimi 20 appunti personali, preparati dal ciclo |
| `SCALETTA.md`, `PIANO_V3.md`, `RISORSE_TECNICHE.md` | piano e riferimenti |
