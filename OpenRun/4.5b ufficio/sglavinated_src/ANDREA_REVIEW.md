# Andrea — Feedback, decisioni e Review

Fonte canonica unica per feedback e decisioni di Andrea. Il @capo cita nel checkpoint soltanto gli ID e le direttive pertinenti; log, status di reparto e risultati di review restano nei rispettivi documenti. Solo una risposta esplicita di Andrea vale come sua approvazione.

## Uso nel ciclo v4.6

- Il Capo consulta questa fonte quando un task o una review dipende da feedback o decisioni di Andrea.
- Assegnazioni, criteri, prove e review indipendenti restano nel dossier `Checkpoint Revision/CP-NNN`; gli status Designer/Officina restano i rispettivi handoff.
- Le voci legacy sotto sono contesto storico: il @capo ne conferma la pertinenza nel checkpoint prima di riaprirle.
- Una risposta registrata non equivale a verifica tecnica o chiusura del checkpoint. L'autore non approva il proprio lavoro.

Andrea aggiunge qui feedback, correzioni richieste o decisioni nuove con data e ambito/ID del checkpoint. Il @capo non riscrive le risposte di Andrea; registra nel checkpoint solo l'ID, l'esito e il collegamento a questa fonte. Le richieste che richiedono una decisione esplicita usano il modello seguente:

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

Le quattro domande T1 sottostanti sono rinominate `T1-Q1`–`T1-Q4` per non collidere con le decisioni storiche D1-D4.

### T1-Q1 | Chiusura proposta del gate T1

Gate soak dichiarato nella registrazione v4.5: `sh tools/gate_t1.sh soak`, exit 0, replay identico su tre livelli, soglie di frame rispettate e screenshot registrati. La prova non basta da sola a chiudere il checkpoint 4.6.
DEFAULT usato nel frattempo: sì — chiudi T1-RUN a [x] se `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, replay 2x IDENTICO, ms/frame <= 20.
Risposta di Andrea: sì.

### T1-Q2 | Promozione P1

Promozione storicamente subordinata a screenshot del gameplay e build CMake verificata.
DEFAULT usato nel frattempo: sì — promuovi P1 a [x] (screenshot bloccati da shell negata, prova differita).
Risposta di Andrea: sì.

### T1-Q3 | Prova manuale su Mac e Pi 400/RetroPie

Richiesta storica: provare `sglavinated_runner` su Mac con SDL2 e registrare guida, FPS e temperatura/throttling su Pi 400/RetroPie.
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

Azione richiesta a @developer: rivedere la generazione/parametri dei percorsi per aumentare varietà (alternanza curve/rettilinei, soglie gate). Il blocco H resta aperto.

### T1-Q4 | Attivazione del lavoro grafico G1

Richiesta storica: catalogo grafico e controllo catalogo. DEFAULT allora indicato: non attivare, fuori scope T1. Risposta di Andrea: attivare.

### T1-Q5 | Soglie giro guidato e tuning percorsi (T1-RUN)

Azione richiesta a @developer (ANDREA_REVIEW T1-Q3): rivedere generazione/parametri percorsi per varietà (alternanza curve/rettilinei) così il giro guidato passa su tutti i 6 livelli.
Domanda: confermare soglie giro guidato (rettilineo ~>=35-40%, curva max coerente 0.6-1.0, riferimento lv_02_atomico rettilineo 40% / curva max 0.86) e che builder-developer proceda col tuning un parametro alla volta con `sh tools/gate_t1.sh soak`?
Opzioni: A) sì, allinea tutti i livelli al profilo guidabile lv_02_atomico  B) no, ridefinire soglie e parametri
Raccomandazione: A
RISPOSTA DI ANDREA (2026-10-19): la dinamica dei percorsi va ottimizzata tra i livelli e tra i tratti (la curvatura non era ottimale) e segnalata nei test per @developer. Serve una logica di tracciati consecutivi per la gara. Compito operativo: blocco R1 in RICHIESTA.md (tratti con tema, tracciati per percorsi, gate che indica il tratto del percorse da correggere). Ruoli confermati: il @grafico verifica le risorse e ne crea di nuove; il @capo controlla i due dipartimenti e aggiorna le revisioni in `Checkpoint Revision/`.
ORDINE DEI TEST (Andrea, 2026-10-19): 1) Andrea compila e prova su macOS; 2) solo quando su Mac il flusso di gioco e' buono (R1 + R2), prova su Raspberry Pi 400/RetroPie. Finche' non c'e' l'ok di Andrea su Mac, nessun lavoro dedicato a Raspberry Pi 400/RetroPie oltre a non rompere la build Linux. Note per allora: Raspberry Pi 400/RetroPie usa il core libretro (`BUILD_LIBRETRO=ON`), che oggi non compila perche' manca `libretro.h`; in alternativa si avvia il runner SDL2 da EmulationStation come porta. Guida: `guide/BUILD_E_AVVIO_RASPBERRY_PI.md` nella cartella del progetto.

## Feedback Andrea 29/10 (prova della PR #44) — regole per tutti
- Menu semplice: niente scelta della musica nel menu. In gara a schermo solo lo spettro audio, senza titolo del brano. Niente scritta MARCIA HIGH/LOW.
- Musica: un brano per il menu e i brani di gara in `dati/audio/playlist.json` (voci `menu` e `gara`; dal 29/10 sera in gara ce n'e' uno solo, Andrea ne aggiungera' o togliera'); i brani .xm di Andrea stanno in `dati/audio/musica/`.
- Il giocatore non attraversa le altre auto; bump diverso tra traffico (guidatori qualunque) e concorrenti; i rivali non scappano e non restano appiccicati al giocatore; classifica d'arrivo sul tempo.
- Arrivo: NIENTE scena finale disegnata a parte ("non c'entra niente"). Linea a scacchi per terra, pubblico che aspetta, l'auto si ferma con un'animazione, poi i risultati.
- Cruscotto (box info e HUD): deve diventare meno invasivo, Andrea ci sta ragionando. Per ora solo: separatore al centro del box, sfondo nero al 10%. Non cambiare altro dell'HUD senza una sua decisione.
- Asset grafici ordinati e modificabili da Andrea: persone, auto, oggetti e scenari sono PNG in `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/`, registrati nel catalogo e convertiti con `png_a_raw.py` (stesso nome e misura per ridisegnarli). Nel codice restano solo effetti semplici (scacchi del traguardo, fumo, spettro audio).
- Palette piccola e dithering (Andrea 29/10 sera): pochi colori pieni e trame al posto delle sfumature, cosi' il gioco resta portabile (Pi, browser, Linux, Windows XP). Tecnica scelta: Bayer (ordinato 4x4). Trama solo sulle zone grandi (cielo, facciate, sfondi), mai su dettagli piu' stretti di ~10 px, al massimo una o due zone per immagine; se a grandezza reale sembra rumore, si toglie. Skyline: `UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py` scrive `skyline_trama.png` accanto all'originale (che resta da modificare) e il raw del gioco; regolazioni per livello in `assets_originali/scenari/trama.json`. Dopo aver ridisegnato uno skyline, rilanciare lo script.
- Spettro audio in gara (`render/visualizzatore.*`): niente sotto la linea di base, colori neon pieni con ombra di 1 px per il contrasto, animazioni pazze sui picchi. Non aggiungere riflessi o pannelli.

## D1 | PIANO_V3 | capo | [CHIUSA 2026-10-16 da Andrea]
Domanda: licenza del tuo codice e delle tue grafiche/testi (serve per il 50% open source).
Opzioni: A) codice MIT, grafiche CC-BY  B) codice GPL-3.0, grafiche CC-BY-SA  C) codice aperto, grafiche riservate
RISPOSTA DI ANDREA: A
Esito: confermata l'opzione A. Applicare: LICENSE, manifest, CREDITI.md.

## D2 | contenuti | capo | [CHIUSA 2026-10-15 da Andrea]
Esito: Contenuti in Libreria indicizzati da @grafico secondo DIREZIONE_ARTISTICA.md.
Da decidere (D2b, non urgente): Elementi decorativi dei 6 percorsi, come banner sponsor, ed altri, per lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop -> @grafico verifica ed aggiorna gli asset per essere utilizzati dal @developer che verifica le implementazioni.
DEFAULT: @developer inserisce le risorse del @grafico.
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
Esito: @grafico ordina i Temi ai 6 Percorsi di gioco con Temi (lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop), @developer patore aggiungono punti cartellone al 20% dei segmenti nei livello.json.
Sponsor 2: "Brinis Udine — Ricambi per motorini" — pubblicita locale. Cartelloni separati da Curve Pericolose, distribuiti indipendentemente al 10-15% dei segmenti. Totale cartelloni sponsor: ~20-30% dei percorsi di gara.

## D6 | scenari livelli | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: 6 varianti di scenario prodotte per i 6 livelli. Quale variante per ciascun livello?
RISPOSTA DI ANDREA: variante A, OK
Esito: lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop.

## D7 | variante UI tipografo | claude_app | [CHIUSA 2026-10-16 da Andrea]
Domanda: layout menu e HUD.
RISPOSTA DI ANDREA: Lofi Jungle (B) per tema menu/HUD. Basato sulla posizione degli elementi della referenza del 'feeling' percepito dello standard gioco 'OutRun', con tema grafico in bianco e nero.
Esito: @grafico conferma utilizzo dei menu/HUD creati, variante B, palette bianco/nero, layout ispirato al 'feeling' percepito dello standard gioco 'OutRun' (ad esempio: velocita, tempo, punteggio, visualizzatore accelerazione). Usare asset originali della Libreria del 'Dipartimento Grafico' @grafico che verifica la disposizione grafica. @developer  integra il menu e HUD nel gioco allineandosi al @grafico.

## D8 | HUD elemento "tempo" | tipografo | [CHIUSA 2026-10-16 da @capo DEFAULT A]
Domanda: hud.json (variante B) manca dell'elemento "tempo" citato da D7. Posizione e dato da definire.
Opzioni: A) tempo residuo (secondi contati all'indietro), sotto la velocità  B) tempo totale percorso  C) nessun elemento tempo (tempo solo nel menu pausa)
Raccomandazione: A (coerente con arcade 'OutRun Style', D7 già cita "tempo" in HUD).
DEFAULT usato nel frattempo: A (tempo residuo sotto velocità, a sinistra).
Impatto se cambi idea: aggiornare hud.json e regenerare hud_preview.png con genera_ui_b.py.
RISPOSTA DI ANDREA: A  @grafico aggiunge elementi grafici interfaccia di gioco (no menu) con elemento "tempo" (residuo, secondi contati all'indietro, in alto a sinistra, sotto velocità, a destra) a motore_nuovo/dati/ui/hud.json variante B e rigenera assets_originali/ui/hud/varianteB/hud_preview.png con genera_ui_b.py (script legge da JSON, preview coerente). Manifest resta "bozza" fino ad approvazione PNG; @developer  espone i dati del 'cruscotto' nel motore nel prossimo ciclo. Nessun blocco fermo in attesa.

## D9 | build | capo | [CHIUSA 2026-10-17 da Andrea]
Domanda: quando clang++/make è negato (regola 21), come si compila il nucleo?
Opzioni: utilizzare: 'cmake --build'.
RISPOSTA DI ANDREA: OK, cmake --build.
Esito: @@developer esegue `cmake --build build --target prova_gate` su Pi 400/Mac/Linux con toolchain Andrea (regole 9/21). Build cmake e test reali (fps, replay, gaming feeling, temperatura, off road, gestione spirte e elementi decorativi livelli); in questo ambiente solo verifica statica e scrittura codice.

## D10 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
Domanda: sprite/cartelloni veri nel gate headless (prova_gate disegna ancora rettangoli): usare caricatore PNG minimo nel core oppure conversione PNG->raw offline in strumenti_py?
Opzioni: A) caricatore PNG minimo nel core (nuova dipendenza/lib, core senza SDL)  B) conversione PNG->raw in strumenti_py, prova_gate legge raw (zero nuove dipendenze)
RISPOSTA DI ANDREA: B (zero nuove dipendenze, core resta senza SDL, licenza MIT/CC0).

## D11 | T1-RUN | capo | [RISPOSTA REGISTRATA — verifica tecnica da checkpoint]
Domanda: fuoristrada: velocità crolla da ~160 a ~6 in pochi frame (troppo brusco). Come tarare il rallentamento fuori strada in player.cpp?
Opzioni: A) fattore graduale ~0.85 al frame fino a velocità minima  B) decelerazione secca a ~6  C) nessun rallentamento (velocità invariata fuori strada) 
DEFAULT usato nel frattempo: A, tuning un parametro alla volta, poi `sh UFFICIO/Officina/TOOL/gate_t1.sh` (replay 2x IDENTICO).
Impatto se cambi idea: modificare la costante OFFROAD in player.cpp e rieseguire gate.
RISPOSTA DI ANDREA: A (dinamica OutRun: rallentamento fuori strada controllato, non blocco).

## D12 | R1 completamento e ri-verifica Mac | builder-developer | [CHIUSA 2026-10-23 da Andrea]

Domanda:
Dopo peer review incrociata su R1 e gate `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` verde su tutti e 6 i percorsi, Andrea ri-verifica il runner su Mac (v91, SDL2) prima dei test Pi 400/RetroPie?
Opzioni: A) sì, Andrea ri-verifica su Mac runner prima di procedere a Pi 400  B) no, si procede direttamente a Pi 400 dopo peer review
RISPOSTA DI ANDREA: A

## Feedback storico v90 — 2026-10-17

Migrato dalla precedente versione di `FEEDBACK_ANDREA.md`; si conserva qui come contesto storico. La decisione di progetto privilegiava il motore C++ in `motore_nuovo/`.

Andrea aveva valutato la tappa 1 **NON OK** per l'assenza di una dinamica di guida convincente. Le priorita' indicate erano:

1. Migliorare percorso stradale e overall feeling della velocita', elementi decorativi nei percorsi distribuiti assieme a Banner pubblicitari che si visualizzano correttamente dal 23 Ottobre, fluidita' di gioco e scorrevolezza del percorso, con variazioni come ponti, colline, curve e 'chicane', come un vero circuito sportivo, aggiornamento a 60hz, risposta reattiva e dinamica ai controlli di gioco, sterzo non reattivo da veicolo fermo, percezione di velocita' e fluidita' del movimento 'external world' rispetto a veicolo guidato, con elementi grafici piu affinati basati sulla documentazione DIREZIONE_ARTISTICA.md @grafico; collegare al gioco gli asset creati e indicizzati dal @grafico, per @collaudatore sviluppa conversione PNG->raw in strumenti_py, prova_gate legge raw.
2. Migliorare principi di guida: accelerazione con limite da migliorare, freno, rallentamento a gas rilasciato, sterzo dipendente dalla velocita', reazione alla curva, rallentamento toccando il bordo strada, animazione ribaltamento/collisioni del fuori strada (riparte con animazione di rientro nel percorso e prosecuzione della tragitto).
3. Rappresentare nel pannello velocita' (HUV) e risposta del motore animazioni brani audio radio interpretando segnali dei brani .mx Traker.

Perfezionamento e implementazione dei personaggi animati sono rinviati alla fase successiva. La richiesta di test headless con screenshot e misure e' una registrazione storica: usare gli esiti documentati nei checkpoint/status correnti, senza considerarli superati o completati per deduzione.


## D17 | Stati e deleghe Turn 1 (ora 1 di 12) | capo | [CHIUSA 2026-10-23 da Andrea]

Domanda:
Confermare stati proposti in `out/capo/STATI_PROPOSTI.md` per Tappa 1 / Gate T1-RUN e deleghe 4.6 (developer, developer-junior, designer = @grafico; consegne `out/developer/`, `out/developer-junior/`, `out/designer/`)?
Opzioni: A) sì, procedo con questi stati e DEFAULT  B) no, revisionare stati prima di proseguire
Raccomandazione: A
RISPOSTA DI ANDREA: A

## Nuovo feedback o decisione (Andrea, 19 ott)
**frequenza di gioco: 60 Hz predefinito, opzione 50 Hz; dopo R1, prima di R2). Parte 2 nello stesso file: SDL, compilazione e compatibilita' multipiattaforma.**

# Nota di Andrea per sviluppatori — frequenza di gioco (19 ott 2026)
Destinatari: @developer (autore), @developer-junior (revisore). @capo apre un checkpoint dedicato.
Ordine: dopo R1, prima di R2.

## Decisione
1. **Tutte le misure del gioco in secondi, non in frame.** Tempo limite, durata show (oggi 260 frame), soglia di inattivita' (500 frame), timer di checkpoint, animazioni, accelerazioni e attriti devono usare `dt` in secondi.
2. **Frequenza logica predefinita: 60 Hz.** Oggi e' 50 Hz (`src/game/game.cpp:678` `dt = 1/50`, `game.cpp:831` `/50`, `UNITA_PER_KM` in `game.cpp:17`, `src/libretro/sglavinated_libretro.cpp:37` `fps = 50` e `:78` `RETRO_REGION_PAL`).
3. **Opzione 50 Hz** in configurazione, es. `dati/config.json` `"frequenza_hz": 60` oppure opzione del runner `--hz 50`. Il core libretro dichiara la frequenza scelta (60 -> NTSC).

## Perche'
Mac, Pi 400 (solo HDMI), monitor e TV moderni, lavorano a 60 Hz; anche Windows XP gira su monitor da 60 Hz in su. A 50 Hz logici su uno schermo a 60 il runner ripete un'immagine ogni 6: la strada che scorre fa un piccolo scatto continuo. Una possibile versione per Windows XP (dopo Mac e Pi) non cambia questa scelta.

## Gate
- `ctest` verde a 60 Hz; il giro guidato dura gli stessi secondi (+/- 2%) con le due frequenze.
- Replay 2x IDENTICO a ciascuna frequenza (i replay registrati a una frequenza si riproducono a quella).
- Il runner su MacOS con vsync a 60 Hz: nessun frame ripetuto in 60 s di gioco (contatore nel log del runner).


# SDL, compilazione e compatibilita' multipiattaforma (Andrea, 19 ott)
Andrea: punto importante. Destinatari come sopra; puo' andare nello stesso checkpoint o in uno a parte.
**Piattaforme, in quest'ordine:** 1) MacOS, 2) Linux, 3) RetroPie su Raspberry Pi 400 4), Windows XP in futuro. Ogni modifica deve compilare su 1-3; per 4 basta non introdurre dipendenze che lo escludano.

## Stato verificato (main 162dc5d)
- Il core (`sglavinated_core`) e' C++17 puro, senza SDL e senza `std::filesystem`: bene, resta cosi'.
- Solo il runner usa SDL2 (+ SDL2_image opzionale per i PNG sponsor). Funzioni usate: video/renderer, texture streaming, `SDL_QueueAudio`, GameController/Joystick, `SDL_RenderSetIntegerScale`. **Versione minima reale: SDL 2.0.5** (per `SDL_RenderSetIntegerScale`).
- Il core libretro (per RetroPie) non usa SDL: dipende solo da `libretro.h`, che oggi manca, quindi `BUILD_LIBRETRO=ON` non compila.

## Da fare
1. **Include portabile:** in `src/runner/main.cpp` usare `#include <SDL.h>` (con il target `SDL2::SDL2`) invece di `<SDL2/SDL.h>`, che non funziona con alcune installazioni Windows e con il framework macOS.
2. **SDL2main solo se esiste:** in `CMakeLists.txt` collegare `SDL2::SDL2main` solo `if(TARGET SDL2::SDL2main)`; su alcune distribuzioni Linux il target non c'e' e CMake si ferma.
3. **Versione minima dichiarata:** `find_package(SDL2 2.0.5)` e un messaggio chiaro se manca. Scrivere in `docs/SVILUPPATORI.md` come installarla: macOS `brew install sdl2 sdl2_image`; Raspberry Pi OS / Debian `sudo apt install libsdl2-dev libsdl2-image-dev`; Windows con vcpkg o i pacchetti MinGW ufficiali di SDL.
4. **`libretro.h`:** aggiungere l'header ufficiale (licenza MIT) in `src/libretro/` cosi' il core RetroPie compila su Linux/Pi; la frequenza dichiarata segue la Parte 1.
5. **Avvio del runner in RetroPie (Andrea, 19 ott):** il runner deve partire da RetroPie come voce del sistema **Ports**, senza desktop. Da preparare:
   - lo script `pi/retropie/OpenRun.sh` (da copiare in `~/RetroPie/roms/ports/`), che entra nella cartella del gioco, imposta `SDL_VIDEODRIVER=kmsdrm` e `SDL_AUDIODRIVER=alsa` e lancia `sglavinated_runner` a schermo intero;
   - l'uscita pulita con Select+Start (pad) ed Esc (tastiera), che torna a EmulationStation;
   - il pad configurato in RetroPie funzionante anche nel gioco (SDL GameController);
   - le istruzioni passo passo nella guida Pi (`guide/BUILD_E_AVVIO_RASPBERRY_PI.md` del progetto e `docs/SVILUPPATORI.md`): compilare su Pi, copiare lo script, riavviare EmulationStation.
   Il core libretro (punto 4) resta la seconda strada, per giocarlo come "sistema" di RetroArch.
6. **Windows XP (piu' avanti, dopo Mac e Pi):** verificare l'ultima versione di SDL2 che supporta ancora XP e un compilatore C++17 che produca eseguibili per XP. Nel runner `std::filesystem` puo' essere un problema su XP e sui macOS vecchi (serve 10.15+): sostituirlo con `SDL_GetBasePath` + funzioni C standard, dato che il runner usa gia' SDL. Non bloccare il lavoro attuale per XP: solo non introdurre nuove dipendenze che lo escludano.

## Gate
- Build pulita del runner su macOS (Homebrew) e su Linux/Raspberry Pi OS con SDL di sistema; `ctest` verde.
- Con `BUILD_LIBRETRO=ON` il core compila su Linux.
- `docs/SVILUPPATORI.md` e la guida Pi riportano versioni e comandi di installazione.
- Su Pi 400 con RetroPie: OpenRun compare in Ports, parte, si guida col pad e torna a EmulationStation (prova di Andrea, dopo l'ok su Mac).

---

## D13 | Chiusura T1-RUN (Tappa 1) | capo | [CHIUSA 2026-10-23 da Andrea]

Domanda:
Confermare passaggio di T1-RUN a [x] non appena registrata la review incrociata indipendente su CP-005 e rieseguito ctest verde, dato che il gate `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` e' gia' verde (2026-10-20 ~14:05, exit 0, 6/6 percorsi OK, replay 2x IDENTICO, ms/frame <= 20)?
Opzioni: A) sì, chiudi T1-RUN [x] dopo review CP-005 + ctest  B) no, attendere ulteriore prova/decisione di Andrea
Raccomandazione: A
DEFAULT usato nel frattempo: sì (T1-Q1 risposta Andrea: sì; gate verde registrato in `STATUS_OFFICINA.md` e `STATO_CORRENTE.md`)
Impatto se cambi idea: T1-RUN resta [~] e si attende nuova prova di Andrea prima della tappa 2.
RISPOSTA DI ANDREA: B

---

## D14 | Ordine test post-R1 e Pi 400/RetroPie | capo | [CHIUSA 2026-10-23 da Andrea]

Domanda:
Dopo T1-RUN [x] e CP-005 chiuso, Andrea ri-verifica il runner su Mac (v91, SDL2) prima di ogni lavoro dedicato a Pi 400/RetroPie, come da D12 (risposta A registrata) e ordine test T1-Q5?
Opzioni: A) sì, ri-verifica Mac prima di Pi 400  B) no, si procede direttamente a Pi 400 dopo review
Impatto se cambi idea: tenere la versione MacOs e testare la build per Raspberry Pi 400/RetroPie (serve `libretro.h` in `src/libretro/`, e `BUILD_LIBRETRO=ON`).
RISPOSTA DI ANDREA: A — (D12 A già registrato; conferma richiesta)

---

## D15 | Attivazione G1 e delega @grafico | capo | [CHIUSA 2026-10-23 da Andrea]

Domanda:
Confermare attivazione G1 (catalogo grafico) con delega al @designer (= @grafico): catalogare i 100 PNG fuori elenco, `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato`?
Opzioni: A) sì, attiva G1 su @designer  B) no, tieni G1 fuori scope T1
RISPOSTA DI ANDREA: A — (T1-Q4 già risposto: attivare; conferma richiesta)
Impatto se cambi idea: G1 resta [ ]; sprite veri (P1 punto 4, traffico, personaggi, UFO) restano bloccati.

## D16 | Attivazione G1 e delega @grafico | Andrea | [CHIUSA 2026-10-23 da Andrea]
Andrea is currently testing the review of the Mac runner (v91, SDL2).
In the meantime, continue working autonomously on the game development and design.
**Priorities:**
1. @developer Run and improve automated tests across all 6 themed percorsi with correct related theme elements. @developer-junior make sure the percorsi has each own themed design element provided by @grafico.
2. @developer improve vehicle physics, improve the steering (feels weird to go against the steer while in the middle of the curve, Andrea is not really sure about it yet), overall animation render of 'external world' and driver prospective, parts of the percorsi as hill, highway option, wider road where's needed, engaging collisions with other player, and nice road forks that takes the driver to the next themed percorso. The other players (automated competitors) looks way too small from latest test, improve design using internal Library if possible: @designer works together @developer to improve a little.
3. Test for regressions, crashes, performance issues and missing decorative assets, and game feeling.

**Important:** Do not mark Andrea's manual Mac/SDL2 review as passed. Keep it explicitly pending until Andrea confirms that he has tested the correct build.

## D18 | Chiusura T1-RUN e avvio R2 | capo | [CHIUSA 2026-10-23 da Andrea]

Domanda: dopo chiusura formale CP-005 (review incrociata registrata) e `ctest` verde su tutte le 6 piste, confermare T1-RUN [x] e avvio R2, subordinato alla prova runner Mac di Andrea su build corretta (ordine test T1-Q5: prima Mac, poi Pi 400/RetroPie; nessun lavoro dedicato Pi prima dell'ok Mac)?

Opzioni: A) sì, chiudi T1-RUN [x] e avvia R2 dopo CP-005 + ctest + ok Andrea Mac  B) no, attendere ulteriore prova/decisione di Andrea prima di R2

RISPOSTA DI ANDREA: A

AGGIORNAMENTO DI ANDREA 27/10: D18 e' chiusa. R2 e i blocchi dopo (DECO1 2f, RIV, PIST1, FIN) **non aspettano** D18 ne' D19: si lavora senza altri ok. D19 = B vale solo per chiudere T1-RUN.

**Important:** 23/10 test Mac: ctest 17/17, runner ok, al bivio, in alcuni livelli, glitch di ricalcolo e la tappa dopo parte con posizionamento percorso sbagliato, senza un transizione graduale, incrementare leggermente la curvatura dei due Fork, perche la macchina puo andare in crush toccando i bordi della parte centrale (FORK1). The other players are going a bit too fast and far, they should act a bit more realistically, as in a race.

## D19 | T1-RUN chiusura dopo prova Mac 23/10 | capo | [APERTA]

Domanda:
Confermare chiusura di T1-RUN a [x] ora che il gate e' verde (`sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, 6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame <= 20, ctest 17/17 verde 22/10), CP-005 chiusa con review incrociata indipendente positiva registrata, e Andrea ha provato il runner Mac il 23/10 (ctest 17/17, runner ok, registrato in ANDREA_REVIEW.md)?
Opzioni: A) sì, T1-RUN [x] proposto e PRONTO PER ANDREA (avvio R2 subordinato a decisione formale; nessun lavoro dedicato Pi 400/RetroPie prima di ok Mac)  B) no, T1-RUN resta [~] in attesa di ulteriore prova/decisione di Andrea.
Impatto se cambi idea: T1-RUN resta [~], R2 non parte, si attende nuova prova di Andrea prima del lavoro dedicato a Pi 400/RetroPie.
RISPOSTA DI ANDREA: B
**NOTA DI ANDREA 31/10 (quota open):** il "49.7% sotto soglia 50%" era un falso allarme: contava gli screenshot `.ppm` e i log dei test in `out/`. Da oggi `quota_open.py` esclude `out/`. Nessuno deve correggere manifest o licenze per questo: non scrivetelo piu' nei RIASSUNTO e non passatelo come CORREGGI. Continuate con i blocchi aperti.
