# Andrea — legacy, vocabolario di gioco e raggio d'azione (v4.7)

Unico documento di Andrea che gli agenti leggono. **Precedenza assoluta** su tutto il resto. Lo scrivono solo Andrea e Claude. Lo storico (decisioni D1-D19, feedback di ottobre, note di cantiere) e' in `UFFICIO/archivio_per_claude/ANDREA_REVIEW_storico_fino_08nov.md`: **non leggerlo**, non serve e consuma crediti. Il messaggio e' semplice: questo e' quello che e' stato fatto (non toccatelo) e questo e' il vostro raggio d'azione.

## 1. Cosa leggere all'avvio del turno (e nient'altro)

1. Questo file.
2. `DIREZIONE_ARTISTICA.md` — documento di Andrea: mood, stili dei 6 livelli (neon, atomico, foglie, cromo, spray, desktop), sponsor finti, regole grafiche. Riferimento per ogni lavoro grafico, di scenario, di nomi e di temi.
3. `AGENTS.md` — cartelle e chi scrive dove.
4. `RICHIESTA_APERTA.md` — il vostro blocco (oggi MIG1).
5. La vostra bacheca (`RIASSUNTO.md`, `DIARIO_RECENTE.md`).

Solo se il blocco lo chiede: `REGOLE_AUTO_GIOCATORE.md` (numeri di guida e urti), `motore_nuovo/dati/tracciati/INDICE_TRACCIATI.md` (tracciati), `UFFICIO/Dipartimento Grafico/OGGETTI_LIVELLI.md` (oggetti e PNG), `CATALOGO_GRAFICO.json`.

## 2. Base legacy 08/11 — fatto, approvato, NON si tocca

Andrea 08/11: "segna tutto come ok per me". Tutto quello che c'e' oggi nel gioco e' approvato. Si migliora solo **aggiungendo** con un blocco nuovo di RICHIESTA; non si rifa', non si "sistema", non si toglie.

- **Auto del giocatore e guida**: pose disegnate da Andrea (sprite unico per posa + variante con gli stop accesi), sterzo, curve, derapata, sollevamento leggero alla punta (niente razzo), fuori strada, ciglio, incidenti, tempi delle tappe. Numeri in `REGOLE_AUTO_GIOCATORE.md`.
- **HUD** (`dati/ui/hud.json`, `drawHudDati`), spettro audio, **premiazione**, **musica e reazioni della musica**.
- **Audio di oggi = standard** (urti, sportellate, paletti, stridio, clacson, bip della partenza, musica che reagisce): si possono solo AGGIUNGERE effetti nuovi.
- **Tracciati**: vari e buoni (Andrea 08/11). Niente curve tolte, niente tratti spostati, niente tempi cambiati. Indice: `motore_nuovo/dati/tracciati/INDICE_TRACCIATI.md`.
- **Ponte aperto** (mai parapetti), mare, riva, barche, gabbiani, faro; **galleria** di lv_02 e lv_05 (neon, luci rotte, graffito "OPEN RUN" dentro, imbocco senza scritte); ingresso graduale nelle citta'.
- **Incroci a 90 gradi in citta'** (lv_05, tratto `citta`, campo `incroci` in `tratti.json`): un incrocio a meta' tratto: strada perpendicolare, marciapiedi, strisce pedonali, linee di stop, semafori sempre verdi, auto che passano di continuo (Claude e Andrea 08/11 notte).
- **Partenza** (griglia, pubblico, 3-2-1-START) se la PR #84 e' nel main.
- **Menu**: logo SEGNAPOSTO "OPEN RUN" (`Libreria/assets_originali/ui/logo/logo_openrun.png`) + le voci. **Niente altre scritte, decorazioni o elementi**: il logo e il menu li fa Andrea.
- **Grafica di Andrea**: auto, logo, sfondi dei livelli e oggetti li ridisegna lui. Se un suo PNG cambia, lo si converte (`png_a_raw.py` o lo script indicato nel catalogo) e basta, senza ritoccarlo. Le vecchie varianti non usate (gruppo B) sono state cancellate da Andrea l'08/11: non si rigenerano.

## 3. Vocabolario di gioco (azioni definite e testate)

Queste azioni sono il linguaggio del gioco. **Restano come sono.** Una novita' si aggiunge come voce nuova, senza cambiare le voci esistenti.

| Azione | Cosa succede | Dove |
|---|---|---|
| Accelerare / frenare | velocita' fino alla punta; in frenata gli stop si accendono | player, `auto_stop_*` |
| Sterzare | pieno da 1/4 della punta, cala appena alla punta | `STEER_*` |
| Curva | spinge verso l'esterno col quadrato della velocita'; si tiene sterzando dentro | `CURVA_SPINTA_V2` |
| Derapata | curva forte + sterzo pieno dentro + sopra 60%: gomme che strisciano, fumo, stridio, perde velocita' | `DERAPATA_*` |
| Punta in rettilineo | dopo 1 s l'auto si stacca di 3 px, ombra piu' scura | `SOLLEVAMENTO_*` |
| Dosso | piccolo salto, atterraggio con sospensioni e polvere | `SALTO_*` |
| Fuori strada | rallenta e trema; incidente solo dopo 2 s e sopra il 30% | `OFFROAD_*`, `RIENTRO_S` |
| Ciglio | primo tocco rallenta e si ricorda, il secondo forte e' incidente | game.cpp |
| Tamponare il traffico | botta forte, l'auto rimbalza, clacson | `URTO_*_TRAFFICO` |
| Sportellata a un rivale | sterzo pieno verso l'auto affiancata, il rivale viene spostato | `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE` |
| Sportellate ripetute | memoria dei colpi: a 3.5 sopra il 40% l'auto vola e fa incidente | `colpiMemoria_` |
| Incidente | sale fino a 30 px girando, atterra, riparte da fermo nello stesso punto | game.cpp |
| Danni al rivale | -3% di punta a botta, alla 4a KO per 4 s, triangolo acceso | `RIVALE_DANNO`, `RIVALE_KO_*` |
| Paletti | si abbattono passandoci sopra, volano, -3% | `abbattiPali`, `PALO_X` |
| Cartelloni | sopra meta' punta incidente, sotto botta secca | game.cpp |
| Incrocio | semaforo sempre verde: le auto di lato passano di continuo (aspettano solo il traffico normale). Prenderne una: sopra meta' punta incidente, sotto botta di traffico. Alla punta, sollevato, ci si passa sopra. Anche i rivali ci sbattono e rallentano | `aggiornaIncroci`, `urtoIncrocio` |
| Pausa | Start da solo sul pad, o Invio (anche del tastierino): tutto fermo, scritta PAUSE, musica ovattata; Start di nuovo riprende | `Game::update` |
| Bivio | si sceglie il ramo, la tappa dopo cambia tema in 2 s | road, `percorso_gara.json` |
| Tempo | `tempo_limite_s` per livello, +80% al cambio tappa | livello.json, config.json |
| Partenza | auto ferme in griglia, gruppi di supporto ai lati che chiacchierano, GET READY, 3-2-1 con semaforo e bip, START!: musica dall'inizio, tutti partono insieme, ogni rivale col suo spunto (solo runner e libretro, i gate partono subito) | `render/partenza.*`, `Game::preparaPartenza` |
| Arrivo | frenata, pubblico, premiazione per tema e posizione | `premiazione.*` |
| Consulto finale | FINAL CHECK coi tempi che girano, poi classifica (senza colonna ROUTE, Andrea 09/11) e verdetto: WINNER! col trofeo o YOU FINISHED Nth | `RisultatiFase::disegnaArrivo` |

### Parole dei test (non sono azioni di gioco)

Prove automatiche di `prova_gate` (`UFFICIO/Officina/build/prova_gate <out> <livello> <modo>`). Ognuna gioca da sola due volte con lo stesso seme e controlla che i due giri siano identici e che il frame resti nei tempi.
- **smoke**: la prova piu' rapida (500 frame, circa 8 s): il gioco parte, l'auto si muove, niente crash, tempo di frame misurato. Il nome viene dall'elettronica: accendi e guardi se esce fumo. E' il primo controllo dopo ogni modifica.
- **soak**: come smoke ma lunghissima (30000 frame, circa 8 minuti): cerca problemi che escono solo col tempo.
- **percorso**, **gara**: un giro intero con guida automatica; tutte le tappe e i bivi arrivano in fondo.
- **curve**, **sterzo**, **accelerazione**, **crash**, **urto**: controllano una voce del vocabolario qui sopra (le curve si tengono, lo sterzo risponde, l'incidente riparte, la sportellata non ribalta).
- **screenshot**: salva immagini per guardarle; nessun giudizio automatico.

## 4. Il vostro raggio d'azione (v4.7)

- **Il BLOCCO MIG1** di `RICHIESTA.md`, poi il **BLOCCO V47** (sezione 6): @grafico cartelloni leggibili (G1); @developer paletti che atterrano sull'erba (D1), sportellata efficace con recupero del rivale in ~5 s (D2), rivali con carattere (D3), nomi dei rivali da culture diverse legati ai temi (D4), audio solo extra (D5); @developer-junior prove e review (J1-J4).
- Costanti aperte: SOLO `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, danni/KO/recupero dei rivali, volo dei paletti. Tutto il resto del vocabolario e' in sola lettura.
- Passi piccoli: dopo ogni passo di codice `ctest` verde e `gate_t1.sh` exit 0. Niente test nuovi per ritocchi grafici.

## 5. Regole fisse

- **Risoluzione 320x224, fissa.** Il runner la ingrandisce a scala intera (resa dei pixel pulita anche su un CRT da 15"). Non si cambia.
- **Fluidita' prima di tutto:** 60 Hz costanti (opzione 50 Hz), Pi 400 <= 12 ms/frame, niente allocazioni nel ciclo di gioco. Ogni effetto o animazione nuova si misura; se fa calare il frame, si semplifica.
- **Portabile e leggero:** solo C++17 + SDL2 + libxmp-lite + PNG. macOS oggi; Raspberry Pi 400 / RetroPie (core libretro e runner SDL2); poi browser, Windows XP/98. Repo sotto i 20-25 MB (senza build e log).
- **Grafica:** palette piccola condivisa + dithering (Bayer, Atkinson, halftone), dettagli in `DIREZIONE_ARTISTICA.md`. Visualizzatore solo sopra l'orizzonte, niente esplosioni nello spettro.
- **Testi a schermo in inglese semplice.** Marchi e nomi sempre inventati.
- **Cartelle:** si scrive solo dentro `UFFICIO/` e in `motore_nuovo/` quando il blocco lo chiede. Niente cartelle nuove, niente file alla radice, niente documenti nuovi. Screenshot e log solo in `UFFICIO/Officina/Test area/`.

## 6. Dopo MIG1: avanti verso la v4.7, senza aspettare

Nessuno aspetta l'ok di Andrea. Finiti i propri passi MIG1, ogni ruolo va avanti nel **BLOCCO V47** di `RICHIESTA.md`: guida piu' interessante con sole azioni nuove (scia, sorpasso al pelo), pausa, fluidita' misurata su tutti i livelli, passaggi morbidi fra le schermate, testi in inglese, controllo di spiagge/gabbiani/barche, quadro grafico di cosa manca. Sempre dentro le sezioni 2-5. Andrea guarda le parti grafiche quando puo': il suo parere arriva come blocco nuovo e non ferma il lavoro.

## 7. Decisioni aperte

Nessuna. Formato per una nuova voce:
```
## D<numero> | <blocco> | <agente> | [APERTA]
Domanda:
Opzioni: A) ... B) ...
DEFAULT usato nel frattempo:
RISPOSTA DI ANDREA:
```
