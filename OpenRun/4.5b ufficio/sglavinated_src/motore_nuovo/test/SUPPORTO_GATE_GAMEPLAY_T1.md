# Supporto per compilazione e gate gameplay T1

SPDX-License-Identifier: MIT

Nota operativa per @developer e @debugger. Basata sull'ispezione statica dei sorgenti
attuali; non attesta che build o test siano stati eseguiti o superati.

## Priorita' 0 — rendere il target di prova compilabile e affidabile

1. In `src/render/renderer.cpp` si usa `std::fmod`, ma il file non include
   esplicitamente `<cmath>`. Aggiungere l'include diretto: non affidarsi a include
   transitivi.
2. In `test/prova_gate.cpp` gli include sono scritti come
   `"motore_nuovo/src/game/game.h"` e `"motore_nuovo/src/render/framebuffer.h"`,
   mentre CMake aggiunge `src` agli include del target. Allinearli agli header
   pubblici del core, ad esempio `"game/game.h"` e `"render/framebuffer.h"`,
   oppure cambiare in modo coerente la directory di include del target.
3. `prova_gate` scrive in `out/debugger/` senza creare la directory. Crearla
   esplicitamente prima di aprire log e screenshot; se una scrittura fallisce,
   terminare con codice non-zero invece di riportare un test riuscito.
4. Il terzo screenshot non attende l'off-road: il ramo
   `screenshot_idx == 2 && !sc_taken[2]` scatta subito dopo il frame 180, e se
   l'off-road non e' ancora avvenuto salva su `prova_gate_partenza.ppm`,
   sovrascrivendo la partenza. Attendere `playerOffroad`, salvare allora lo
   screenshot fuori strada; se non viene raggiunto entro la durata della prova,
   fallire il test esplicitamente senza sovrascrivere altri file.
5. Il codice di `prova_gate` attualmente esegue 500 frame (10 s): trattarlo come
   smoke test previsto dal feedback. La voce 93 di `SCAMBIO.md` specifica inoltre
   una prova d'accettazione di 10 minuti senza crash. Eseguire entrambe e
   distinguerne chiaramente i log.

## Priorita' 1 — mantenere coerenti le coordinate della strada e degli sprite

- `Road::project()` restituisce `ProjectedSeg::worldZ` relativo al giocatore.
  `SpriteInstance::worldZ` e' invece una posizione assoluta sul tracciato.
  Calcolare la distanza in avanti con la stessa convenzione e con wrap sulla
  lunghezza effettiva del tracciato, poi cercare quella distanza in `projs_`.
  Il confronto e la ricerca devono usare la medesima coordinata relativa.
- Il tracciato ammette lunghezze variabili per segmento, ma i cartelloni in
  `Game` sono posizionati con `segmento * SEG_LEN`. Derivare la posizione Z
  accumulando le lunghezze effettive dei segmenti del livello; altrimenti sprite,
  curve e colline possono non allinearsi.
- Ordinare gli sprite per profondita' relativa al giocatore, non per Z assoluta,
  quando si disegnano percorsi circolari. Verificare anche il passaggio oltre la
  fine del livello e il ritorno all'inizio.
- Le strisce di corsia usano una fase basata su `SEG_LEN`, mentre il gameplay puo'
  avere segmenti di lunghezza variabile. Calcolare la fase dal segmento corrente
  e dalla sua lunghezza; controllare che sia continua ai confini dei segmenti e
  che avanzi in modo deterministico con la velocita'.
- In `drawRoad()`, la scelta del colore usa `vicino.scale > 0`; per i segmenti
  proiettati validi questa condizione e' sempre vera. Usare un criterio reale e
  deterministico, ad esempio l'indice del segmento, se si vuole alternare i
  colori della carreggiata.
- Proteggere `Road::project()` da strada vuota o `totalLength() <= 0` prima di
  calcolare il modulo con `std::fmod`.

Questi punti richiedono verifica visiva oltre alla compilazione: strada continua
dall'orizzonte al primo piano, auto sulla carreggiata durante le curve, strisce
visibilmente scorrevoli, sprite appoggiati al segmento corretto, nessun salto o
sparizione al wrap.

## Priorita' 2 — test minimi del gameplay

Per ogni caso registrare input, frame, velocita', `playerX`, `playerZ`,
`playerOffroad`, hash e risultato atteso/osservato:

1. Stato iniziale fermo; `Pad::A` accelera senza superare `MAX_SPEED`.
2. Rilascio dell'acceleratore riduce gradualmente la velocita'; `Pad::B` la riduce
   piu' rapidamente; nessuna velocita' negativa.
3. Sterzo sinistra/destra cambia la posizione e resta ripetibile a parita' di
   seed e input; l'auto resta visibile e la strada segue coerentemente la camera.
4. Superare `OFFROAD_LIMIT` attiva `playerOffroad` e il rallentamento; rientrare
   sotto la soglia lo disattiva.
5. Curva e collina del JSON diventano visibili nella proiezione; segmento
   variabile, fine livello e wrap non causano salti o crash.
6. Sprite davanti, dietro e vicino al wrap sono posizionati e ordinati
   correttamente.
7. Due corse con seed e input identici producono gli stessi hash ogni 60 frame.

Il target di prova deve fallire se non accelera, non raggiunge l'off-road richiesto,
non scrive uno screenshot, o diverge nel replay. Gli screenshot attesi sono
partenza, curva e fuori strada, con nomi distinti.

## Compilazione e criteri di chiusura

Da `sglavinated_src/`, con toolchain autorizzata e la procedura D9:

```sh
cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target sglavinated_core
cmake --build build --target prova_gate
cmake --build build --target sglavinated_runner
```

Il runner richiede SDL2; l'assenza di SDL2_image puo' disabilitare gli sprite
PNG, ma non deve impedire la build del core o del runner. Non installare
`nlohmann/json` per questo codice: il parser usato e' `src/core/json_mini.h`;
allineare a questo la documentazione `docs/BUILD.md` e l'elenco licenze.

Non dichiarare il gate chiuso sulla sola base della sintassi o della build.
Registrare separatamente:

- build core, target di prova e runner;
- smoke test headless di 10 secondi;
- prova senza crash di 10 minuti (durata di accettazione indicata nella voce 93
  di `SCAMBIO.md`);
- tre screenshot non vuoti e leggibili;
- replay 2x con hash identici ogni 60 frame;
- frame <= 20 ms (obiettivo <= 12 ms), 50 fps e temperatura su Pi 400.

Se una prova non puo' essere eseguita per vincoli dell'ambiente, indicarla come
non eseguita e lasciare P1/P1b/H nello stato pendente previsto da `RICHIESTA.md`.

## Perimetro gameplay

La logica show/personaggi e' definita in `dati/SPEC_GAMEPLAY.md`; il gate T1
verifica prima di tutto la guida e la strada. Non bloccare questa verifica
sull'approvazione della grafica definitiva o sugli sprite interattivi, che il
feedback di Andrea rimanda a un momento successivo. Il modulo gameplay deve
comunque restare incluso nel core e deterministico.
