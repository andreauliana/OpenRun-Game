# VISIONE — Sglavinated (versione libera)

## L'idea in breve
Gioco di guida arcade, notturno, sci-fi + lofi hip hop + jungle. Simpatico e dinamico. Compagni di viaggio: **Gianni** (la foglia, tenta con slang e droghe), **Gesu'** (corregge, messaggio forte di sicurezza stradale e pro-droga), **UFO** (apparizioni allucinate). Fumetti con frasi che reagiscono a cio' che fa il giocatore in preda ai deliri di droghe pesanti allucinogene, sexy donne circondano.

## Cosa deve succedere in gioco
| Evento | Reazione |
|---|---|
| Uscita di strada | Gianni commenta (frasi "off-road") |
| Crash | Gesu' interviene; effetto colori psichedelici temporaneo |
| Errori ripetuti | escalation: frasi piu' dure e personaggi piu' frequenti (livelli 0-3, decadimento a tempo) |
| 10 s senza input | Gianni parla |
| Velocita' alta | l'auto si alza (volo breve con ombra) |
| Tasto B | razzo: parte in prospettiva e colpisce gli altri concorrenti della corsa automatizzati (utilizzano gli stessi modelli di macchine per la prima fase) |
| Rettilineo / curve / rettilineo / variazione / curve |  scelta | cambio percorso  / curve / rettilineo / variazione / curve  | scelta | percorso  / curve / rettilineo / variazione / curve  | finale a tema  / animazione personaggi a tema - donne premiano, arrivano ufo, confisca dei beni, arresto velocita alta , varie |
Musica: solo i tracker (.xm) di Andrea. Grafica: tutta originale creata o editata da Andrea (vedi assets_originali/DIREZIONE_ARTISTICA.md).

## Vincoli duri (gate)
| Vincolo | Soglia | Come si misura |
|---|---|---|
| Fluidita' | 50 fps stabili (PAL) sul Pi 400 | tempo di frame <= 20 ms, obiettivo <= 12 ms; nessun picco ripetuto |
| Nessun lavoro nascosto | niente allocazioni nel ciclo di gioco | revisione codice |
| Riproducibilita' | stesso input = stesso risultato | replay di input -> hash del frame ogni 60 frame identico su due esecuzioni |
| Emulatore | gira come core libretro in RetroArch/RetroPie | caricamento del core e partita di prova |
| Niente extra inutili | ogni effetto ha un costo misurato | nota in SCAMBIO.md |

## Architettura di riferimento (decisa)
- Nucleo in C++17, **indipendente dalla piattaforma**: render software a 320x224, 50 Hz, audio int16 stereo, input tipo RetroPad.
- Tempo = contatore di frame; numeri casuali con seed fisso; nessun orologio di sistema -> comportamento identico a ogni esecuzione.
- Due involucri sottili: **core libretro** (per RetroArch/RetroPie) e **runner da terminale/desktop** (SDL2) per i test.
- Dati in file: livelli, frasi, sprite PNG, musica .xm.

## Ordine di sviluppo
1. Strada a segmenti + auto + 50 fps nel core libretro e nel runner.
2. Replay deterministico e hash di test.
3. Gianni/Gesu'/UFO e fumetti (dati + modulo gameplay).
4. Razzo (B) e auto che si alza.
5. Effetti psichedelici e musica .mx gestita intelligentemente per i diversi livelli e sezioni gioco.
6. Livelli a tema e rifinitura dei dettagli di gioco ed esperienza (menu, score, personaggi, animazioni, grafica, fumetti, testi, logo, cruscotto del giocatore durante game play).
Ogni passo chiude con: build, test di replay, misura frame, review.

## Come la usano gli agenti
`capo` legge questo file all'inizio di ogni ciclo e scompone il lavoro in blocchi di RICHIESTA.md. Se una funzione rompe un vincolo duro, si ferma e scrive PER_CLAUDE.md senza cancellare altre note precedenti o di altri utenti.
