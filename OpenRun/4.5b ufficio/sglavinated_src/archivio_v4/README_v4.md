# OutRun Sglavinated Edition v4

Versione v4 focalizzata sulla creazione delle risorse funzionanti, per emulare la esperienza di gioco 'OutRun', rispetto alle referenze, partendo prima da compilazione C++, poi *eventualmente* in ROM *.md* per Sega Megadrive (abbiamo la rom originale per retro-enginering), e sul risparmio di crediti per il calcolo.

## Ottimizzazioni attive
- Il ciclo lavora un agente alla volta e ruota fra gli agenti; di default esegue al massimo 3-4 agenti per giro (per risparmio delle risorse e focus progettuale realistico: creare una prima emulazione 'funzionante' rispetto alle referenze).
- Il ciclo termina dopo 12 ore o 1000 iterazioni e applica pause crescenti (15, 30, 60, 120 minuti) in caso di limiti di richieste.
- I checkpoint sono ogni 120 minuti. Il cambio modello usa la panchina in `strumenti/panchina.txt`.
- Per contenere il contesto, il ciclo prepara estratti di richieste, scambi, bacheca pubblica e assemblee aperte. La memoria personale usa un riepilogo di massimo 12 righe e un estratto degli ultimi 20 appunti del diario.
- Le build CMake possono riutilizzare la cache nativa del build system. Il ciclo degli agenti non implementa una cache MD5 e non salta i turni in base agli hash.

## Integrazioni non automatiche
- Ollama e' un test locale opzionale in `locale/`; non e' collegato al ciclo e non instrada automaticamente gli agenti. Per provarlo: `sh locale/prova_locale.sh prova`.
- Non sono configurati hook di lifecycle OpenCode/Claude. L'automazione e' nel controller `comandi/v3.sh` e negli script `strumenti/`; `/claude` e' un comando OpenCode, non un hook.
- I riferimenti agli hook VBlank 68000 riguardano il filone Mega Drive archiviato, non il ciclo del motore C++ nuovo.

I nomi interni `V3` e `v3.sh` sono mantenuti per compatibilita' con questa build di test v4.

## Avvio
```
cd "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)"
sh comandi/v3.sh start
```
Stato e stop: `sh comandi/v3.sh stato` e `sh comandi/v3.sh stop`. Il pannello di questa build e' su http://localhost:8089.

## Technical review

I tre problemi citati nel log sono realistici, locali e risolvibili senza rifare l'intero motore: riguardano logica di gameplay, wiring di un modulo di scena e determinismo di replay/hash. In pratica, non sono bug di architettura generale, ma bug di flusso dati e di regole di stato.

### 1) Offroad: logica di soglia e clamp

Il problema tecnico è che `offroad` viene calcolato come condizione sul valore `x` della macchina, ma il successivo clamp laterale può cancellare l'errore percepito dal gioco. In codice si ha una differenza netta tra:

- `OFFROAD_LIMIT` (soglia di fuori carreggiata)
- `X_LIMIT` (massimo valore ammesso prima del clamp)

Se la soglia offroad è troppo bassa o se il clamp viene applicato prima della valutazione, allora la condizione `offroad == true` diventa “dead logic” o non riflette mai il vero stato della vettura. Il codice di commento nel progetto dice esplicitamente che `OFFROAD_LIMIT = 700` è minore di `X_LIMIT = 900`, quindi un caso in cui la vettura è fuori strada viene nascosto dal clamp e lo stato logico resta falso. Questo è un bug classico di gameplay: l'auto viene resa visivamente stabile ma l'engine continua a pensare di essere in carreggiata, e quindi non attiva comportamento corretto (cambio colore, FX, collisioni, penalità, HUD).

Per renderlo fattibile basta allineare le due soglie:

- definire `OFFROAD_LIMIT` in relazione alla larghezza della strada e al fattore di scala del renderer;
- applicare la verifica di offroad su `x` prima del clamp, oppure usare una soglia coerente con il rendering;
- mantenere il valore massimo di `x` durante la visualizzazione in un range che non faccia sparire l'auto fuori schermo.

Questo è un fix piccolo e concentrato nel modulo `player.cpp`/`player.h`, non una revisione del sistema fisico del gioco.

### 2) Show non collegato: `show` calcolato ma non usato

Il secondo problema è un classico “data flow break”. Il gioco costruisce un `DatiShow` o una scena di show, ma il valore prodotto viene scartato o ignorato. Nelle note del progetto compare proprio il caso di `show` lasciato in `(void)show;`, cioè il risultato del calcolo viene consumato solo come valore dummy e quindi non influenza la renderizzazione finale.

Tecnicamente questo significa:

- `ShowManager::aggiorna()` produce dati del frame (es. personaggio, UFO, bolla, animazione, frase);
- `disegnoShow_.aggiorna()` deve trasformare quel dato in una scena renderizzabile;
- la scena va poi consumata dal renderer o dal pass di compositing del frame.

Se il risultato viene scartato, il motore può sembrare “funzionante” ma non mostra mai i dialoghi, le vignette, i personaggi o gli effetti di show. Questo è il tipo di bug che blocca il “first playable” non perché il rendering del mondo sia rotto, ma perché il modulo di interazione/FX non è wired nel pipeline. La soluzione è semplice: far sì che il dato `show` sia davvero usato nel render stage, senza variabili inutilizzate né placeholder vuoti, e garantire che `ScenaShow` venga disegnata in ordine corretto (UFO → personaggio → fumetto).

### 3) `frameHash`: determinismo e replay

Il ramo di debug e replay ha un requisito tecnico fondamentale: due esecuzioni con gli stessi input devono produrre lo stesso hash, mentre differenze di stato devono cambiare il valore. Il progetto lo cerca di fare includendo:

- frame count;
- speed del giocatore;
- posizione `playerX` / `playerZ`;
- flag offroad;
- stato `rng_`.

Questo è una scelta corretta e facile da giustificare. Senza questi campi, un replay potrebbe risultare identico anche quando il motore si trova in uno stato differente, rendendo il debug praticamente inefficace e impedendo verifiche di determinismo. In altre parole, il frame hash non è solo un debug tool: è un invariant test del comportamento del motore.

Il problema tecnico è che se una parte dello stato non viene inclusa, o se l'ordine del hash è incoerente, si ottengono false positives/false negatives. Per fare il fix basta mantenere lo schema di hash costante e includere tutte le grandezze che modifichino il risultato del frame, senza utilizzare valori non deterministici (time, clock di sistema, RNG senza seed).

### Fattibilità

Questi sono bug pronti da correggere, non problemi architetturali da riscrivere:

- `player.cpp`/`player.h`: logica di offroad e clamp laterale;
- `game.cpp`: wiring del `show` e hash di frame;
- `rng.h`/`game.h`: esposizione dello stato RNG e dei campi da includere nel hash.

Il punto importante è che il motore è già organizzato in moduli separati, quindi le correzioni sono localizzate e facilmente testabili con build C++/determinismo. In pratica, il progetto è tecnicamente fattibile come proto “first playable” perché i difetti identificati sono di integrazione e coerenza di stato, non di mancanza di fondazione architetturale.

## Render engine build plan: first interaction, menu, music, and agent workflow

Per arrivare a un vero primo contatto giocabile con la sensazione di OutRun, il lavoro va costruito in tre passaggi: motor core stabile, esperienza di gioco minimale ma convincente, poi controlli/feedback avanzati. Il focus non è “riempire il motore di funzionalità” a tutti i costi, ma creare una prima interazione che funzioni in modo affidabile e riconoscibile, rispettando la direzione artistica definita da Andrea.

### Obiettivo di sviluppo

- Primo loop giocabile: auto controllabile, strada visibile, curva e velocità percepibili, collisioni e offroad di base.
- Menu di impostazioni minimo ma funzionale: scelta di pista, input, qualità, audio, pausa/ritorno.
- Audio: musica di base e feedback sonori legati a rpm, accelerazione, offroad e menu.
- Feeling OutRun: sensazione di velocità, linea di guida, curva, pulizia visiva, senso di spazio e movimento, senza distrazioni superflue.
- Gestione economica del credito: agenti specializzati, compiti concentrati, build controllate, niente lavoro duplicato o asset non usati.

### C++ issues da risolvere per il render engine

Per fare un build di render engine davvero adatto a un “first interaction” si devono chiudere i punti critici già evidenziati dal ciclo di sviluppo:

- `Player::update()` e clamp laterale: la x deve essere limitata in modo che l’auto non sparisca dallo schermo ma mantenga un offroad leggibile e coerente con la carreggiata.
- `Renderer::drawPlayerCar()`: la prospettiva deve essere calibrata con il fattore di scala del road projection, in modo che l’auto resti sempre in campo visivo e non fuoriesca o si “scompaia” ai bordi.
- `Game::render()`: la composizione del frame va eseguita in ordine corretto: sfondo, strada, oggetti, auto, overlay/show, HUD.
- `Game::spawnTraffic()`: il traffico di placeholder deve essere disattivato o reso innocuo finché non ci sono asset reali, per evitare collisioni false o auto invisibili.
- `ShowManager` / `disegnoShow_`: il risultato di show e dialoghi non può essere scartato, altrimenti i sistemi di interazione e i fumetti non raggiungono il frame.
- `frameHash()`: va tenuto deterministico, in modo che input uguali e stato uguale producano lo stesso hash; ciò è essenziale per replay, debug e verifica del comportamento del motore.

### Struttura della build per un primo gioco funzionante

Il piano deve rispettare il flusso già presente nel progetto:

1. Core deterministico e compilabile
   - CMake build del core senza dipendenze non necessarie.
   - Verifica sintattica del nucleo separata dal runner e dai layer grafici.
   - Nessun asset pesante prima del primo loop giocabile.

2. Render e world loop
   - Proiezione della strada e movimento del player.
   - Curve e pendenza percepibili, con un rapporto visivo chiaro tra velocità, sterzo e movimento laterale.
   - Caratteristiche di base del feedback: offroad, accelerazione, frenata, shear visivo.

3. Interazione e menu
   - Menu base con selezione pista / impostazioni / audio / esci.
   - Input da tastiera o pad riconoscibile e stabile.
   - Stati logici chiari: menu, guida, pausa, crash/offroad, fine gara.

4. Audio e feeling
   - Musica di base sintetica o data-driven. Non serve ancora musica da asset reale; serve un motore audio adeguato a dare ritmo e presenza.
   - RPM, accelerazione, offroad e menu devono avere feedback coerenti.
   - La sensazione “OutRun” arriva più da velocità + prospettiva + ritmo di guida che da vistoso numero di effetti di scena.

5. Contenuto visivo e art direction
   - Non toccare la direzione artistica di Andrea: palette, mood, tono, leggibilità, stampa grafica e uso dei colori devono restare fedeli alle scelte già deliberate.
   - Il primo giocabile usa placeholder grafici e geometrie semplici, ma senza alterare lo stile visivo previsto in documentazione e asset.

### Tool disponibili per gli agenti operativi

Per far lavorare in modo intelligente i diversi agenti, il sistema deve avere a disposizione strumenti chiari e ripetibili, senza costi inutili:

- Compilatore C++ con CMake e verifica sintattica (`cmake --build`, `clang++ -fsyntax-only` su moduli critici).
- Runner headless per test deterministici e hash.
- Logging e check di frame, per osservare velocità, posizione, offroad e render senza dover lanciare tutto il sistema grafico.
- Script di controllo del ciclo e dello stato, mantenendo il modello agent-based già presente nel progetto.
- Accesso separato ai task: rendering, gameplay, player/input, audio, menù, asset, documentazione.

Questo permette di usare ogni agente con un compito specifico, riducendo context pollution e mantenendo il budget di credito basso. L'operatore può così far lavorare in sequenza architetto, coder, debugger, livelli, testi, tipografo, traduttore, senza sovrapporre responsabilità.

### Policy di uso del credito e dell'agent rotation

Il progetto va gestito come un sistema a turni, non come una “full team all at once”:

- un agente attivo per volta in modalità costante;
- compiti atomici e verificabili;
- build minima per ogni gate;
- niente lavoro parallelo su asset o documenti non collegati al gate attivo;
- quando un modulo è stabile, passa di mano; se è aperto, resta in stato di lavoro finché il gate non è chiuso.

Questo preserva il tempo di calcolo e riduce il rischio di feature sprecate in varianti non decisive.

### Regola fondamentale

Il progetto deve restare fedele alla visione iniziale:

- prima il C++ giocabile;
- poi la sensazione di OutRun;
- poi il layering avanzato di menu, musica e contenuti;
- sempre senza sovrascrivere la direzione artistica stabilita da Andrea.

In sintesi, la build render engine oggi deve essere pensata come un percorso controllato di “first interaction” successivamente evoluto verso menu, audio, sensazione di guida e completezza del gioco. I problemi tecnici esistenti non sono blocchi architetturali, ma punti di integrazione e coerenza di stato: correggendoli in modo mirato, il motore può diventare un prototipo credibile, elegante e economicamente sostenibile per il resto del ciclo di sviluppo.
