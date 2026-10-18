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
Risposta di Andrea: non registrata.

### T1-Q2 | Promozione P1

Promozione storicamente subordinata a screenshot del gameplay e build CMake verificata.
DEFAULT usato nel frattempo: sì — promuovi P1 a [x] (screenshot bloccati da shell negata, prova differita).
Risposta di Andrea: non registrata.

### T1-Q3 | Prova manuale su Mac e Pi 400

Richiesta storica: provare `sglavinated_runner` su Mac con SDL2 e registrare guida, FPS e temperatura/throttling su Pi 400.
DEFAULT usato nel frattempo: sì — Andrea esegue appena possibile (v91 + misure chiudono H); intanto il gate headless resta verde.
Esito/risposta di Andrea: non registrati qui.

### T1-Q4 | Attivazione del lavoro grafico G1

Richiesta storica: catalogo grafico e controllo catalogo. DEFAULT allora indicato: non attivare, fuori scope T1. Risposta di Andrea: non registrata.

## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
Domanda: licenza del tuo codice e delle tue grafiche/testi (serve per il 50% open source).
Opzioni: A) codice MIT, grafiche CC-BY  B) codice GPL-3.0, grafiche CC-BY-SA  C) codice aperto, grafiche riservate
Raccomandazione: A (massima compatibilita').
DEFAULT usato nel frattempo: nessuna licenza dichiarata.
Impatto se cambi idea: aggiornare LICENSE e manifest.
RISPOSTA DI ANDREA: OK
Esito: confermata l'opzione A. Applicare: LICENSE, manifest, CREDITI.md. Il DEFAULT 'nessuna licenza' decade.

## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
Esito: sponsor (intimo femminile, Bar, Negozi). Contenuti femminili: glamour/pin-up e sponsor di intimo (vedi DIREZIONE_ARTISTICA.md.
Da decidere (D2b, non urgente): nome e slogan degli sponsor inventati (vedi esempi libreria gia creati) che compaiono durante la gara/corsa sparsi nei diversi livelli a senso, ricreando il feeling per ogni livello e lo stile descritto, utilizza gia gli elementi creati, indicizzare, e proseguire. Arrivare a un buon feeling di 'popolazione' dei vari livelli, con aree di transizione (bivi) tra i percorsi connessi.
DEFAULT: @sviluppatore e @grafico affinano le 3 varianti di cartellone sponsor e utilizzano i vari asset grafici creati.
RISPOSTA DI ANDREA: OK

## D3 | struttura | architetto | [CHIUSA 2026-10-16 da Andrea]
Domanda: quante tappe/finali ha l'albero di bivi dell'originale, da copiare come STRUTTURA (tu hai detto 16 opzioni)?
Raccomandazione: @collaudatore verifica da fonti tecniche (FEDELTA.md voce 3) e propone numero e temi; tu confermi.
DEFAULT usato nel frattempo: 5 tappe per percorso con bivi, 3 livelli di prova.
RISPOSTA DI ANDREA: OK
Esito: confermato il DEFAULT (5 tappe per percorso con bivi, 3 livelli di prova). @collaudatore puo' ancora proporre correzioni motivate da migliorie, ma si procede cosi'.

## D4 | tipografia | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: quale configurazione tipografica per il gioco?
Scelta di Andrea (semplificata):
- Fumetti e titolazioni: Antonio Bold (OFL-1.1) — un solo font vettoriale per tutto
- UI pixel (menu, HUD, velocita, tempo): variante B (Lofi Jungle) pixel 5x7, disegnato dal tipografo
RISPOSTA DI ANDREA: Antonio Bold per titoli e peso 'regular' per fumetti, B per pixel UI. Confermato.

## D5 | sagoma auto | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: quale stile per la sagoma dell'auto del giocatore?
Scelta di Andrea: silhouette GT europea anni '70-'90, ispirata a: Citroën CX/DS, Ford Sierra XR4i/Cosworth, Capri, Escort RS, Mustang, Cortina. Auto 2 porte, ruote larghe, seduta bassa, rivisitata in chiave neon/sci-fi. 3 varianti di colore.
RISPOSTA DI ANDREA: OK

## D2b | sponsor | capo | [CHIUSA 2026-10-16 da Andrea]
Domanda: nome e slogan dello sponsor inventato (intimo), dove compare (cartelloni, titolo, radio).
RISPOSTA DI ANDREA: "Curve Pericolose" — slogan "appoggiati al peggio". Compare ai lati delle strade come cartelloni pubblicitari sparsi al 20% dei segmenti dei diversi livelli. Nessun marchio reale.
Esito: @grafico produce sprite cartelloni per entrambi gli sponsor (varianti per livello), @sviluppatore aggiungono punti cartellone al 20% dei segmenti nei livello.json.
Sponsor 2: "Brinis Udine — Ricambi per motorini" — pubblicita locale, stile insegna di officina/negozio di provincia italiana. Cartelloni separati da Curve Pericolose, distribuiti indipendentemente al 10-15% dei segmenti. Totale cartelloni sponsor: ~30-35% dei segmenti con almeno un cartellone visibile.

## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: 9 varianti di scenario prodotte per i 3 livelli. Quale variante per ciascun livello?
RISPOSTA DI ANDREA: variante A, OK
Esito: lv_01_neon varianteA, lv_02_atomico varianteA, lv_03_foglie varianteA. Le varianti B/C e altre future restano in archivio per eventuali livelli e implementazioni future.

## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: layout menu e HUD.
RISPOSTA DI ANDREA: Lofi Jungle (B) per tema menu/HUD. Basato su posizione elementi dell'originale (OutRun, e referenze), con tema in bianco e nero.
Esito: @grafico produce menu/HUD definitivo variante B, palette bianco/nero, layout ispirato alla disposizione elementi del gioco originale 'OutRun' e simili (velocita, tempo, punteggio, accelerazione, distanza nelle stesse posizioni). Usare asset originale, solo verificare la disposizione come riferimento strutturale (considerare retro enginering, se utile, e librerie implementate).

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
Raccomandazione: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).
DEFAULT usato nel frattempo: B.
Impatto se cambi idea: riscrivere caricamento sprite in prova_gate e aggiungere dipendenza immagini al core.
RISPOSTA DI ANDREA: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).

## D11 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
Domanda: fuoristrada: velocità crolla da ~160 a ~6 in pochi frame (troppo brusco). Come tarare il rallentamento fuori strada in player.cpp?
Opzioni: A) fattore graduale ~0.85 al frame fino a velocità minima  B) decelerazione secca a ~6  C) nessun rallentamento (velocità invariata fuori strada)
Raccomandazione: A (dinamica OutRun: rallentamento fuori strada controllato, non blocco).
DEFAULT usato nel frattempo: A, tuning un parametro alla volta, poi `sh tools/gate_t1.sh` (replay 2x IDENTICO).
Impatto se cambi idea: modificare la costante OFFROAD in player.cpp e rieseguire gate.
RISPOSTA DI ANDREA: A, stile 'OutRun'

## Feedback storico v90 — 2026-10-17

Migrato dalla precedente versione di `FEEDBACK_ANDREA.md`; si conserva qui come contesto storico, non come specifica attiva per OpenRun 4.6. La decisione di progetto allora registrata privilegiava il motore C++ in `motore_nuovo/`, sospendeva ROM/export Mega Drive e chiedeva di concentrarsi sul primo gate giocabile. Le assegnazioni personali v4.5 non prevalgono sui ruoli 4.6.

Andrea aveva valutato la tappa 1 **NON OK** per l'assenza di una dinamica di guida convincente. Le priorita' indicate erano:

1. Rendere strada, corsie, proiezione delle colline e auto leggibili; inventariare e collegare al gameplay i contenuti grafici e gli sponsor gia' prodotti.
2. Implementare accelerazione con limite, freno, rallentamento a gas rilasciato, sterzo dipendente dalla velocita', spinta in curva, rallentamento fuori strada, ribaltamento/collisioni e pannello informativo.
3. Rappresentare nel pannello velocita' e risposta del motore simulato con cambio automatico.

Personaggi animati e relativi effetti erano rinviati a una fase successiva. La richiesta di test headless con screenshot e misure e' una registrazione storica: usare gli esiti documentati nei checkpoint/status correnti, senza considerarli superati o completati per deduzione.