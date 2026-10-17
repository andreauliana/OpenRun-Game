# Feedback di Andrea

Gli agenti leggono questo file a ogni turno (copia di sola lettura). Le scelte scritte qui hanno la precedenza.

Tappa attiva: 1

## Scelte principali
Percorso attivo: motore C++ in motore_nuovo/. ROM Mega Drive/export .md sospesi; non avviare né modificare il relativo lavoro.

Per il test (notturno): priorità al primo Gate C++ giocabile, solo attività necessarie in RICHIESTA.md. Squadra: coder e debugger; aggiungere altri agenti solo se indispensabile al Gate. Non iniziare tappe o funzionalità successive.

- Grafica, testi e tipografia: Andrea.
- Grafica provvisoria finché non c'è un file in `ANDREA/sprite_ritoccati/`.

## Versione v90
- **NON OK tappa 1** (Andrea, 17 ott, 9:40 AM, dopo la build di stamattina): rispetto alla prima prova cambia poco, manca la meccanica 'base' di guida: partenza, accelerazione tramite pulsante principale, principi di sterzo, rallentamento, rotazione finta 3D della macchina principale (sprite simil 3D), e freno. **Manca tutto l'engine della macchina e la dinamica della guida alla 'OutRun'**: risponde ai comandi destra e sinistra, ma poi basta.
- Cosa serve per chiudere la tappa 1 (priorità in quest'ordine):
  1. **Strada visibile e corretta**: oggi è disegnata solo come elemento grezzo, l'auto non e' ancora definita, reagisce positivamente a input di sterzo (destra e sinistra) (prova di Claude, 17 ott). Correggere renderer e proiezione delle colline; strisce di corsia che scorrono, così la velocità deve essere implementata correttamente, secondo le referenze delle librerie, in modo che il motore sia correttamente funzionante. Gli elementi grafici e di decorazione della libreria interna e generati nuovi, per i diversi livelli progettati, devono essere inseriti in una lista utile per i diversi tracciati dei percorsi stradali, personaggi, Banner pubblicitari (che erano stati gia prodotti) e indicizzare questi elementi per essere usati nel percorso di prova. Cercare di verificare quello che e' stato prodotto nelle precedenti build (sponsor intimo femminile, Brinis ricambi, Coffe Shop Negozio, etc etc). 
  Gli sprite e le funzioni dei personaggi interattivi verra implementata in un secondo momento, implementando i 3 personaggi Foglia 'Gianni', Gesu, UFO. 
  Credo che per adesso la esperienza di guida non sia sufficentemente buona, non e' abbastanza sviluppata.
  2. **Dinamica di guida stile 'OutRun' implementata**: accelerazione che cala avvicinandosi alla velocità massima (oggi cresce senza limite), sistema di 'freno' con pulsante secondario, rallentamento a gas rilasciato, sterzo che dipende dalla velocità, spinta verso l'esterno in curva, rallentamento fuori strada.
  3. **Motore dell'auto**: emulare il funzionameno base della esperienza di guida in base alle referenze di funzione di OutRun, delle nuove referenze in libreria, sviluppare funzione per la velocita.
- Controllo del gate: non basta la sintassi. Coder e debugger aggiungono un programma di prova senza finestra (target CMake, lanciato con `cmake --build build --target prova_gate`) che guida da solo per 10 secondi e salva 3 screenshot (partenza, curva, fuori strada) in `out/debugger/`, con velocità e posizione nel log.
