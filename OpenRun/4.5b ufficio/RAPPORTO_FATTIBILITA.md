# Rapporto di fattibilità — motore C++ per la versione libera

Data: 15 ott 2026. Fonti nel blocco in fondo. Cosa e' verificato e cosa no e' indicato per ogni riga.

## 1. Esito in breve
| Pista | Esito | Probabilita' di riuscita |
|---|---|---|
| C. Motore NUOVO scritto da zero (C++/SDL2, tecnica pseudo-3D nota) | **raccomandata** | alta (tecnica) / media (tempo) |
| B. Base di codice GPL esistente (ZgzInfinity/OutRun) con TUTTI gli asset sostituiti | alternativa, dopo controllo asset | media |
| A. Cannonball con asset sostituiti | **sconsigliata** come base libera | bassa |
| A2. Mod di Cannonball (con ROM dell'utente, non commerciale) | alternativa rapida, non «libera»: vedi 1b | media-alta (tecnica) / legale da verificare |
| D. Traduzione C++ -> ROM Mega Drive | scartata | molto bassa |

## 1b. Aggiornamento: pista A2 «Cannonball come base di una mod» (richiesta di Andrea)
Cannonball e' il motore OutRun gia' riscritto in C++ (la parte «motore Sega» riusabile). Dal testo della licenza (verificato): uso e opere derivate sono permessi, le versioni modificate devono includere il sorgente completo, **ma vendita e uso commerciale sono vietati**. I dati di gioco vengono dalla romset arcade Rev B che deve mettere l'utente.
| | A2: mod di Cannonball | C: motore nuovo libero |
|---|---|---|
| Velocita' | alta: strada, traffico, fisica, audio, core libretro per Pi gia' esistono | bassa: si parte da zero |
| Prestazioni su Pi | provate dal progetto stesso (gira su Pi 4/400) | da dimostrare |
| Limiti di memoria della hack (156 frasi, 4 MB) | spariscono | spariscono |
| «Libero» / senza Sega | **no**: serve la ROM Sega dell'utente; licenza non commerciale; il codice deriva da quello Sega (questione legale) | si |
| Distribuzione | solo codice/patch, mai la ROM; solo non commerciale | anche commerciale (a scelta) |
| Aggiunte (personaggi, razzo B, volo) | sprite nuovi come livello sovrapposto, logica nuova nel codice C++ (la parte piu' delicata: il codice segue la struttura dell'originale) | libere, struttura nostra |
| Rischio principale | legale + dipendenza dalla ROM | tempo |
Lettura: A2 realizza l'idea creativa molto prima, ma non e' «open source libero». Si puo' fare A2 come laboratorio del gameplay e poi portare i moduli di gameplay (dati e regole) nel motore C: il lavoro di `traduttore` e' valido per entrambi.
Non verificato: struttura interna del sorgente di Cannonball (punti d'innesto), difficolta' reale di aggiungere logica; la valutazione e' il compito del blocco B0 di RICHIESTA.md.

## 2. Candidati verificati
| | Cannonball (djyt) | ZgzInfinity/OutRun | Motore da zero |
|---|---|---|---|
| Linguaggio / build | C++11, CMake, SDL2 + header Boost (verificato) | C++14, SFML 2.5.1 (verificato) | C++17 + SDL2 (a scelta) |
| Licenza | personalizzata tipo BSD, ma **vieta uso commerciale** e le versioni modificate devono includere il sorgente completo; pacchetti Linux la classificano "MAME" / repo nonfree (verificato) | **GPL-3.0** (verificato) | scelta tua (es. MIT o GPL) |
| Asset | **richiede la romset arcade Rev B** per grafica, audio, dati (verificato) | "tributo/remake"; provenienza asset non dichiarata; un commento del 2018 cita sprite "ripped" (verificato il commento, non se esistono ancora) | tuoi, nuovi |
| Origine del codice | dichiarato "codice 68000/Z80 originale riscritto in C++" (verificato) -> deriva dal codice Sega | non dichiarata (remake) | nuova |
| Piattaforme | Windows, Mac, Linux, Pi, Pandora (verificato) | Windows, Linux (verificato); Pi non citato | Pi 4/400 con SDL2 |
| Dimensione / attivita' | 440 commit, 36 issue (verificato) | 334 commit, 92 stelle (verificato); data ultimo commit non letta | n/a |
| Rischio principale | licenza non libera + derivato da codice Sega + dati legati all'hardware arcade | asset da sostituire tutti; da leggere `Resources` | tempo di sviluppo |

## 3. Perche' Cannonball non va bene come base libera
1. Licenza non commerciale: non e' software libero e blocca ogni uso commerciale futuro.
2. Il motore legge strada, sprite, tile, audio dalla ROM arcade: sostituire gli asset significa riscrivere i caricatori e rifare i dati nei formati dell'hardware arcade.
3. Il codice e' una riscrittura del codice originale Sega: se questo basta a renderlo derivato e' una domanda per un legale (mia valutazione, non verificata).
Uso corretto: riferimento tecnico (come funziona la strada a segmenti, la logica di gioco), senza copiare codice.

## 4. Compatibilita' con il Pi 400 (stima, da provare)
| Aspetto | Valutazione |
|---|---|
| Carico CPU/GPU | una strada pseudo-3D con sprite 2D e' leggera; Cannonball (piu' pesante) gira gia' su Pi secondo il README |
| Librerie | SDL2: disponibile su Raspberry Pi OS / RetroPie; SFML: da verificare su ARM |
| Obiettivo 50 fps | probabile, da misurare (`ARCHITETTURA_CHECK.md`) |

## 5. Piano consigliato (pista C, con B come scorciatoia)
| Fase | Attivita' | Verifica |
|---|---|---|
| 1 | Prototipo: strada a segmenti (curve, salite), auto, scorrimento, 50 fps | gira sul Pi 400 |
| 2 | Formato dati nostro: livelli in JSON/CSV, sprite PNG, audio da tracker | un livello completo caricato da file |
| 3 | Gianni/Gesu'/UFO e fumetti portati dal nostro stile (nessun limite 156) | fumetti a schermo |
| 4 | Asset originali (@asset) + musiche .xm | `CREDITI.md` completo |
| 5 | Menu, tempo, checkpoint, punteggio | partita completa |
| 6 | Audit licenze e file (reviewer + legale) | checklist firmata |
Scorciatoia B: solo se `Resources` di ZgzInfinity/OutRun risulta sostituibile e il codice e' originale; in tal caso si parte dal suo codice (GPL-3.0: il derivato resta GPL).

## 6. Rischi e mitigazioni
| Rischio | Mitigazione |
|---|---|
| Modelli gratuiti deboli su C++ | piccoli passi, test a ogni passo, escalation a Claude (SCAMBIO.md) |
| Licenza | GPL/MIT scelta fin dall'inizio |

## 7. Non verificato
- SFML su Pi 400; SDL2 nella versione di RetroPie.
- Interpretazione legale della derivazione di Cannonball.

## Fonti
- Cannonball (djyt): https://github.com/djyt/cannonball ; licenza: https://raw.githubusercontent.com/djyt/cannonball/master/docs/license.txt
- Pacchetto Cannonball, campo licenza "MAME" (RPM Fusion nonfree): https://download1.rpmfusion.org/nonfree/fedora/releases/42/Everything/x86_64/os/repoview/cannonball.html
- Cannonball per portatili (GCW0): https://github.com/gameblabla/Cannonballs
- ROM richieste e Pi 4/400: https://wiki-next.recalbox.com/en/emulators/ports/out-run
- ZgzInfinity/OutRun (GPL-3.0, C++14, SFML): https://github.com/ZgzInfinity/OutRun e https://zgzinfinity.itch.io/out-run
- Elenco progetti pseudo-3D: https://repos.ecosyste.ms/topics/pseudo3d
