# Andrea — Feedback, decisioni e Review

Fonte canonica unica per feedback e decisioni di Andrea. Il Capo cita nel checkpoint soltanto gli ID e le direttive pertinenti; log, status di reparto e risultati di review restano nei rispettivi documenti. Solo una risposta esplicita di Andrea vale come sua approvazione.

## Uso nel ciclo 4.6

- Il Capo consulta questa fonte quando un task o una review dipende da feedback o decisioni di Andrea.
- Assegnazioni, criteri, prove e review indipendenti restano nel dossier `Checkpoint Revision/CP-NNN`; gli status Designer/Officina restano i rispettivi handoff.
- Le voci legacy sotto sono contesto storico: il Capo ne conferma la pertinenza nel checkpoint prima di riaprirle. Non prevalgono sulle regole 4.6.
- Una risposta registrata non equivale a verifica tecnica o chiusura del checkpoint. L'autore non approva il proprio lavoro.

## Nuovo feedback o decisione

Andrea aggiunge qui feedback, correzioni richieste o decisioni nuove con data e ambito/ID del checkpoint. Il Capo non riscrive le risposte di Andrea; registra nel checkpoint solo l'ID, l'esito e il collegamento a questa fonte. Le richieste che richiedono una decisione esplicita usano il modello seguente:

Formato (copiare per ogni voce):
```
## D<numero> | <blocco> | <agente> | [APERTA]
Domanda:
Opzioni: A) ... B) ...
Raccomandazione:
DEFAULT usato nel frattempo:
Impatto se cambi idea:
RISPOSTA DI ANDREA:
```

## Tappa attiva: 1 — Gate T1 (blocco T1-RUN)

Le quattro domande T1 sottostanti sono rinominate `T1-Q1`–`T1-Q4` per non collidere con le decisioni storiche D1-D4. Sono richieste legacy senza risposta registrata in questa fonte.

### T1-Q1 | Chiusura proposta del gate T1

Gate soak dichiarato nella registrazione v4.5: `sh tools/gate_t1.sh soak`, exit 0, replay identico su tre livelli, soglie di frame rispettate e screenshot registrati. La prova non basta da sola a chiudere il checkpoint 4.6.
DEFAULT usato nel frattempo: sì — chiudi T1-RUN a [x] se `sh tools/gate_t1.sh soak` exit 0, replay 2x IDENTICO, ms/frame <= 20.
Risposta di Andrea: sì.

### T1-Q2 | Promozione P1

Promozione storicamente subordinata a screenshot del gameplay e build CMake verificata.
DEFAULT usato nel frattempo: sì — promuovi P1 a [x] (screenshot bloccati da shell negata, prova differita).
Risposta di Andrea: sì.

### T1-Q3 | Prova manuale su Mac e Pi 400/Linux

Richiesta storica: provare `sglavinated_runner` su Mac con SDL2 e registrare guida, FPS e temperatura/throttling su Pi 400.
DEFAULT usato nel frattempo: sì — Andrea esegue appena possibile (v91 + misure chiudono H); intanto il gate headless resta verde.
Esito/risposta di Andrea (2026-10-19, Mac): `sh tools/gate_t1.sh soak` eseguito su Mac. Risultato: **GATE T1: FALLITO (exit 13)**.

Dettaglio misure (headless, Mac):
- ms/frame medio ~0.19, max 0.94 — ampiamente sotto il limite 20. Prestazioni OK.
- Replay 2x: IDENTICO su tutti i 6 livelli. Determinismo OK.
- Gate smoke, soak (30000 frame), accelerazione, ribaltamento, sterzo, curve continue, gara a tappe (4 combinazioni), screenshot P1: tutti GATE OK.
- **Problema: giro guidato FALLITO su 5 livelli su 6** ("percorso poco vario o non guidabile"):
  - lv_01_neon: rettilineo 13%, curva max 0.62 — FALLITO
  - lv_02_atomico: rettilineo 40%, curva max 0.86 — **GATE OK** (unico passato)
  - lv_03_foglie: rettilineo 18%, curva max 1.02 — FALLITO
  - lv_04_cromo: rettilineo 23%, curva max 0.41 — FALLITO
  - lv_05_spray: rettilineo 21%, curva max 0.90 — FALLITO
  - lv_06_desktop: rettilineo 12%, curva max 0.56 — FALLITO

Azione richiesta a @sviluppatore: rivedere la generazione/parametri dei percorsi per aumentare varietà (alternanza curve/rettilinei, soglie gate). Il blocco H resta aperto; Pi 400 da fare.

### T1-Q4 | Attivazione del lavoro grafico G1

Richiesta storica: catalogo grafico e controllo catalogo. DEFAULT allora indicato: non attivare, fuori scope T1. Risposta di Andrea: attivare.

### T1-Q5 | Soglie giro guidato e tuning percorsi (T1-RUN)

Azione richiesta a @sviluppatore (ANDREA_REVIEW T1-Q3): rivedere generazione/parametri percorsi per varietà (alternanza curve/rettilinei) così il giro guidato passa su tutti i 6 livelli.
Domanda: confermare soglie giro guidato (rettilineo ~>=35-40%, curva max coerente 0.6-1.0, riferimento lv_02_atomico rettilineo 40% / curva max 0.86) e che builder-developer proceda col tuning un parametro alla volta con `sh tools/gate_t1.sh soak`?
Opzioni: A) sì, allinea tutti i livelli al profilo guidabile lv_02_atomico  B) no, ridefinire soglie e parametri
Raccomandazione: A
DEFAULT usato nel frattempo: A — builder-developer allinea i percorsi e rilancia `sh tools/gate_t1.sh soak`; developer-builder-junior esegue peer review indipendente del diff/log.
Impatto se cambi idea: modificare soglie gate e parametri livello.json; rieseguire gate.
RISPOSTA DI ANDREA:

## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
Domanda: licenza del tuo codice e delle tue grafiche/testi (serve per il 50% open source).
Opzioni: A) codice MIT, grafiche CC-BY  B) codice GPL-3.0, grafiche CC-BY-SA  C) codice aperto, grafiche riservate
RISPOSTA DI ANDREA: A
Esito: confermata l'opzione A. Applicare: LICENSE, manifest, CREDITI.md.

## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
Esito: Contenuti in Libreria indicizzati da @grafico secondo DIREZIONE_ARTISTICA.md.
Da decidere (D2b, non urgente): Elementi decorativi dei 6 percorsi, come banner sponsor, ed altri, etc etc, per lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop -> @grafico verifica ed aggiorna gli asset Libreria 'Dipartimento Grafico' per essere utilizzati.  @sviluppatore verifica le implementazioni di questi asset grafici nei percorsi.
DEFAULT: @sviluppatore verifica ed inserisce le risorse del @grafico inserite e indicizzate nella Libreria del 'Dipartimento Grafico'.
RISPOSTA DI ANDREA: sì

## D3 | struttura | architetto | [CHIUSA 2026-10-19 da Andrea]
Domanda: quante tappe/finali ha l'albero di bivi usare come STRUTTURA?
DEFAULT: 6 livelli/percorsi con temi confermati da @grafico: lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop.
3 finali con animazione (premiazione/arrivo) al arrivo/fine livello. 3 bivi transitori.
RISPOSTA DI ANDREA: sì, 6 Percorsi con Temi (lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop), 3 finali.

## D4 | tipografia | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: quale configurazione tipografica per il gioco?
Scelta di Andrea (semplificata):
- Fumetti e titolazioni: Antonio Bold (OFL-1.1) — un solo font vettoriale per tutto
- UI pixel (menu, HUD, velocita, tempo): variante B (Lofi Jungle) pixel 5x7, disegnato negli asset del @grafico nella Libreria del 'Dipartimento Grafico'
RISPOSTA DI ANDREA: Antonio Bold per titoli e peso 'regular' per fumetti, B per pixel UI. Confermato.

## D5 | sagoma auto | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: quale stile per la sagoma dell'auto del giocatore?
Scelta di Andrea: silhouette GT europea anni '70-'90, ispirata a: Citroën CX/DS, Ford Sierra XR4i/Cosworth, Capri, Escort RS, Mustang, Cortina. 3 varianti di colore.
RISPOSTA DI ANDREA: sì

## D2b | sponsor | capo | [CHIUSA 2026-10-16 da Andrea]
Domanda: nome e slogan degli Sponsor, dove compare (Banner stradali).
RISPOSTA DI ANDREA: "Curve Pericolose" — slogan "appoggiati al peggio". Compare ai lati delle strade come cartelloni pubblicitari sparsi al 20% dei segmenti dei diversi livelli.
Esito: @grafico ordina i Temi ai 6 Percorsi di gioco con Temi (lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop), @sviluppatore aggiungono punti cartellone al 20% dei segmenti nei livello.json.
Sponsor 2: "Brinis Udine — Ricambi per motorini" — pubblicita locale. Cartelloni separati da Curve Pericolose, distribuiti indipendentemente al 10-15% dei segmenti. Totale cartelloni sponsor: ~20-30% dei percorsi di gara.

## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: 6 varianti di scenario prodotte per i 6 livelli. Quale variante per ciascun livello?
RISPOSTA DI ANDREA: variante A, OK
Esito: lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop.

## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: layout menu e HUD.
RISPOSTA DI ANDREA: Lofi Jungle (B) per tema menu/HUD. Basato sulla posizione degli elementi della referenza del 'feeling' percepito dello standard gioco 'OutRun', con tema grafico in bianco e nero.
Esito: @grafico conferma utilizzo dei menu/HUD creati, variante B, palette bianco/nero, layout ispirato al 'feeling' percepito dello standard gioco 'OutRun' (ad esempio: velocita, tempo, punteggio, visualizzatore accelerazione). Usare asset originali della Libreria del 'Dipartimento Grafico' @grafico che verifica la disposizione grafica. @sviluppatore integra il menu e HUD nel gioco allineandosi al @grafico.

## D8 | HUD elemento "tempo" | tipografo | [CHIUSA 2026-10-16 da @capo DEFAULT A]
Domanda: hud.json (variante B) manca dell'elemento "tempo" citato da D7. Posizione e dato da definire.
Opzioni: A) tempo residuo (secondi contati all'indietro), sotto la velocità  B) tempo totale percorso  C) nessun elemento tempo (tempo solo nel menu pausa)
Raccomandazione: A (coerente con arcade 'OutRun Style', D7 già cita "tempo" in HUD).
DEFAULT usato nel frattempo: A (tempo residuo sotto velocità, a sinistra).
Impatto se cambi idea: aggiornare hud.json e regenerare hud_preview.png con genera_ui_b.py.
RISPOSTA DI ANDREA: A  @grafico aggiunge elementi grafici interfaccia di gioco (no menu) con elemento "tempo" (residuo, secondi contati all'indietro, in alto a sinistra, sotto velocità, a destra) a motore_nuovo/dati/ui/hud.json variante B e rigenera assets_originali/ui/hud/varianteB/hud_preview.png con genera_ui_b.py (script legge da JSON, preview coerente). Manifest resta "bozza" fino ad approvazione PNG; @sviluppatore espone i dati del 'cruscotto' nel motore nel prossimo ciclo. Nessun blocco fermo in attesa.

## D9 | build | capo | [CHIUSA 2026-10-17 da Andrea]
Domanda: quando clang++/make è negato (regola 21), come si compila il nucleo?
Opzioni: utilizzare: 'cmake --build'.
RISPOSTA DI ANDREA: OK, cmake --build.
Esito: @collaudatore esegue `cmake --build build --target prova_gate` su Pi 400/Mac/Linux con toolchain Andrea (regole 9/21). Build cmake e test reali (fps, replay, gaming feeling, temperatura, off road, gestione spirte e elementi decorativi livelli); in questo ambiente solo verifica statica e scrittura codice.

## D10 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
Domanda: sprite/cartelloni veri nel gate headless (prova_gate disegna ancora rettangoli): usare caricatore PNG minimo nel core oppure conversione PNG->raw offline in strumenti_py?
Opzioni: A) caricatore PNG minimo nel core (nuova dipendenza/lib, core senza SDL)  B) conversione PNG->raw in strumenti_py, prova_gate legge raw (zero nuove dipendenze)
RISPOSTA DI ANDREA: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).

## D11 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
Domanda: fuoristrada: velocità crolla da ~160 a ~6 in pochi frame (troppo brusco). Come tarare il rallentamento fuori strada in player.cpp?
Opzioni: A) fattore graduale ~0.85 al frame fino a velocità minima  B) decelerazione secca a ~6  C) nessun rallentamento (velocità invariata fuori strada) 
DEFAULT usato nel frattempo: A, tuning un parametro alla volta, poi `sh tools/gate_t1.sh` (replay 2x IDENTICO).
Impatto se cambi idea: modificare la costante OFFROAD in player.cpp e rieseguire gate.
RISPOSTA DI ANDREA: A (dinamica OutRun: rallentamento fuori strada controllato, non blocco).

## Feedback storico v90 — 2026-10-17

Migrato dalla precedente versione di `FEEDBACK_ANDREA.md`; si conserva qui come contesto storico, non come specifica attiva per OpenRun 4.6. La decisione di progetto allora registrata privilegiava il motore C++ in `motore_nuovo/`, sospendeva ROM/export Mega Drive e chiedeva di concentrarsi sul primo gate giocabile. Le assegnazioni personali v4.5 non prevalgono sui ruoli 4.6.

Andrea aveva valutato la tappa 1 **NON OK** per l'assenza di una dinamica di guida convincente. Le priorita' indicate erano:

1. Rendere strada, elementi decorativi, fluidita del percorso, scorrimento delle animazioni del gioco, risposta ai controlli di gioco, animazione e fluidita del mondo esterno rispettivamente ad auto, scorrevoli, meno grezzi; collegare al gioco gli asset creati e indicizzati dal @grafico nella Libreria del 'Dipartimento Grafico', per @collaudatore sviluppa conversione PNG->raw in strumenti_py, prova_gate legge raw.
2. Implementare accelerazione con limite, freno, rallentamento a gas rilasciato, sterzo dipendente dalla velocita', spinta in curva, rallentamento fuori strada, ribaltamento/collisioni e pannello menu.
3. Rappresentare nel pannello velocita' e risposta del motore simulato con cambio automatico.

Personaggi animati e relativi effetti erano rinviati a una fase successiva. La richiesta di test headless con screenshot e misure e' una registrazione storica: usare gli esiti documentati nei checkpoint/status correnti, senza considerarli superati o completati per deduzione.