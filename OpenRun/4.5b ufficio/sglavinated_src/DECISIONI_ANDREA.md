# Decisioni per Andrea (a blocchi)
Gli agenti aggiungono qui; tu rispondi nella riga RISPOSTA. Nessun agente aspetta: usa il DEFAULT e prosegue dove possibile.
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

D1 | Chiudere T1-RUN a [x]? Gate soak eseguito dal capo: `sh tools/gate_t1.sh soak` exit 0, `out/collaudatore/gate_t1.log` "GATE T1: TUTTO OK" (speed@60=161.79 > 0 frame 60, replay 2x IDENTICO 3 livelli, ms/frame <= 20, offroad raggiunto, 3 screenshot partenza/curva/fuoristrada per livello in out/collaudatore/lv_01_neon, lv_02_atomico, lv_03_foglie). DEFAULT: sì.

D2 | Autorizzare promozione P1 a [x] una volta che @sviluppatore consegna 3 screenshot in out/sviluppatore/ (pista curva, traffico, cartelloni pubblicitari, elementi decorativi percorso, personaggio con fumetto; build cmake già ok)? DEFAULT: sì.

D3 | Andrea esegue punto 8 T1-RUN (v91 FEEDBACK_ANDREA.md su runner Mac SDL2, guida OK o ritocco un parametro alla volta; fps/temperatura Pi 400 vcgencmd) per chiudere H? DEFAULT: sì, appena possibile.

D4 | Attivare @grafico per G1 (catalogo grafico CATALOGO_GRAFICO.json, check_catalogo_grafico.py exit 0)? DEFAULT: no (non richiesta, fuori scope Tappa 1).

Rispondi in FEEDBACK_ANDREA.md o qui; se non rispondo entro il prossimo turno attendere e proseguire test debug.


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

## D10 | T1-RUN | capo | [APERTA]
Domanda: sprite/cartelloni veri nel gate headless (prova_gate disegna ancora rettangoli): usare caricatore PNG minimo nel core oppure conversione PNG->raw offline in strumenti_py?
Opzioni: A) caricatore PNG minimo nel core (nuova dipendenza/lib, core senza SDL)  B) conversione PNG->raw in strumenti_py, prova_gate legge raw (zero nuove dipendenze)
Raccomandazione: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).
DEFAULT usato nel frattempo: B.
Impatto se cambi idea: riscrivere caricamento sprite in prova_gate e aggiungere dipendenza immagini al core.
RISPOSTA DI ANDREA: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).

## D11 | T1-RUN | capo | [APERTA]
Domanda: fuoristrada: velocità crolla da ~160 a ~6 in pochi frame (troppo brusco). Come tarare il rallentamento fuori strada in player.cpp?
Opzioni: A) fattore graduale ~0.85 al frame fino a velocità minima  B) decelerazione secca a ~6  C) nessun rallentamento (velocità invariata fuori strada)
Raccomandazione: A (dinamica OutRun: rallentamento fuori strada controllato, non blocco).
DEFAULT usato nel frattempo: A, tuning un parametro alla volta, poi `sh tools/gate_t1.sh` (replay 2x IDENTICO).
Impatto se cambi idea: modificare la costante OFFROAD in player.cpp e rieseguire gate.
RISPOSTA DI ANDREA: A, stile 'OutRun'