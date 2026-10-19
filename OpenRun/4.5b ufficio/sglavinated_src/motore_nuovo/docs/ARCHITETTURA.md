# ARCHITETTURA — motore nuovo "Outrun Sglavinated Edition v3 (open)" (pista C)

SPDX-License-Identifier: MIT

Autore: @architetto. Data: 16 ott 2026. Stato: consegna BLOCCO B (Turno 1).
Percorso: motore_nuovo/docs/ARCHITETTURA.md

---

## 1. Scopo e contesto

Motore nuovo scritto da zero in C++17, pista C decisa da Andrea (2026-10-15; PIANO_V3.md BLOCCO 3). Nessun codice, asset, sprite, tracciato o musica della ROM originale Sega. Idee e tecniche solo da fonti citate in FONTI.md e LICENZE.md §4 (Lou's Pseudo 3d Page: matematica della strada a segmenti; JavaScript Racer: struttura del ciclo di gioco; Cannonball: solo numeri di gameplay come analisi; ZgzInfinity/OutRun: organizzazione moduli solo se GPL accettata — non è il caso, D1 = MIT; hack Sglavinated v87: personaggi, fumetti e ritmo degli show come dati, opere di Andrea, sola lettura, non si portano i limiti hardware della hack).

Target: Raspberry Pi 400 e PC/Mac. Render software 320x224, 60 Hz, audio int16 stereo, input tipo RetroPad. Dati in file: livelli JSON, sprite PNG, musica .xm (tracce musicali sostituibile senza toccare il codice, con possibile 'visual' nel HUV).

---

## 2. Vincoli duri (dichiarati e rispettati)

| Vincolo | Valore | Come si garantisce |
|---|---|---|
| Linguaggio | C++17 | CMake, codice nuovo, niente SDL nel core |
| Render | software, 320x224 | framebuffer proprio in `src/render/` |
| Frequenza | 50 Hz (PAL) | ciclo a contatore di frame, nessun orologio di sistema nel core |
| Audio | int16 stereo | mixatore in `src/audio/` |
| Input | tipo RetroPad | `src/input/` mappa pad tasti/assi |
| Tempo | contatore di frame | `src/core/frame_counter`, nessun `std::chrono`/`time()` nel core |
| RNG | seme fisso | `src/core/rng`, stessa seed = stessa sequenza |
| Determinismo | stesso input = stesso risultato | replay di input + hash del frame ogni 60 frame |
| Allocazioni nel ciclo di gioco | zero | pool statici, allocazione solo a livello/cambio scena |
| Budget frame Pi 400 | <= 20 ms, obiettivo <= 12 ms | misura @debugger (blocco H), nessun picco ripetuto |
| Piattaforme | runner SDL2 (test) + core libretro (RetroArch/RetroPie) | due involucri sottili sopra lo stesso core |
| Niente Sega | zero byte/asset Sega | review @reviewer, FONTI.md, LICENZE.md §4 |

---

## 3. Moduli e responsabilita' (per MODULI_E_REPARTI.md)

Tutti i moduli stanno in `motore_nuovo/src/`. Nessun modulo dipende da SDL2 (solo runner e core libretro lo fanno). Ogni modulo extra in `src/fx/<nome>/` ha `init()`, `update(stato)`, `draw(fb)`, flag on/off in `dati/config.json` e i propri dati; spegnendolo il gioco gira uguale (regola dei moduli extra, MODULI_E_REPARTI.md).

| Modulo | Cartella | Compito | Dati di riferimento |
|---|---|---|---|
| core | `src/core/` | contatore frame, RNG con seme, stato globale, replay input, hash frame ogni 60 | `dati/config.json` |
| road | `src/road/` | strada a segmenti, curve, colline, posizione auto, traffico base, scorrimento | `dati/livelli/*/livello.json` |
| render | `src/render/` | framebuffer software 320x224, disegno strada, sprite scalati, palette | `assets_originali/` (PNG) |
| audio | `src/audio/` | mixer int16 stereo, brani e radio da `dati/audio/playlist.json` | `dati/audio/playlist.json` |
| input | `src/input/` | lettura pad tipo RetroPad, mappatura tasti | `dati/config.json` |
| personaggi / gameplay | `src/gameplay/` (era `src/personaggi/`) | Gianni, Gesu', UFO: logica show, scelta frasi, escalation, inattivita' | `dati/frasi.json`, `dati/personaggi/*/`, `dati/show_logica.md` |
| fx (esempio) | `src/fx/razzo_b/`, `src/fx/sollevamento_auto/` | funzione extra con flag on/off, propri dati, test | `dati/config.json` |
| ui | `src/ui/` | menu, titolo, HUD letti da `dati/ui/*.json` (font e grafica: @grafico) | `dati/ui/menu.json`, `dati/ui/hud.json` |

Interfacce (descritte a parole, no codice C++ in questa fase):
- `core` espone `frame()`, `rng()`, `replay_save/load()`, `frame_hash()`.
- `road` espone `aggiorna_segmenti(frame)`, `posizione_auto()`, `collisione_traffico()`.
- `render` espone `disegna_frame(fb)`, `disegna_sprite()`.
- `audio` espone `mixa(int16* stereo, n_frame)`, `seleziona_brano(radio)`.
- `input` espone `stato_pad()` (RetroPad: direzioni, A/B, start, select).
- `personaggi` espone `show_event(...)`, `show_update(frame, ...)` (dettaglio in `dati/show_logica.md`).

---

## 4. Ciclo di gioco (un frame = una chiamata del core)

1. `input` legge lo stato del pad (RetroPad) per questo frame.
2. `core` incrementa il contatore di frame e avanza il RNG (seme fisso).
3. `road` aggiorna segmenti, curva/collina, scorrimento, auto giocatore, traffico.
4. Collisioni auto/traffico e fuori-strada.
5. `personaggi` valuta trigger (crash, uscita strada, inattivita' 10 s, show periodico, UFO) e decide se parte un fumetto.
6. Moduli fx attivi (razzo B, sollevamento auto ad alta velocita', effetti colore post-crash).
7. `render` compila il framebuffer 320x224 (strada, sprite, HUD, fumetti).
8. `audio` mixa il buffer int16 stereo per questo frame (50 Hz -> ~882 campioni stereo per frame a 44100 Hz, gestito dall'involucro).
9. `core` calcola l'hash del frame se il numero di frame e' multiplo di 60 (replay/test determinismo).

Il core NON chiama mai funzioni di orologio di sistema. Ogni involucro (runner SDL2 o core libretro) chiama `core.frame()` una volta per frame video e passa l'input del frame.

---

## 5. Determinismo e replay

- Tempo = contatore di frame (intero, parte da 0 a ogni nuova partita).
- RNG: generatore con seme fisso dichiarato in `dati/config.json` (es. seme per livello e seme globale).
- Replay: ogni input di frame viene salvato in file (sequenza di stati pad). Rigiocando lo stesso file si ottiene identico stato e stessi hash.
- Hash del frame: checksum leggero (es. FNV-1a su framebuffer + stato) scritto ogni 60 frame. Due esecuzioni con stessi input devono produrre hash identici (verifica @debugger, blocco H).
- Nessun uso di `rand()`, `time()`, thread o ordine di mappa non deterministico nel core.

---

## 6. Formati dati di riferimento

Tutti i formati sono in `motore_nuovo/docs/FORMATI_DATI.md`:
- `dati/livelli/<id>/livello.json`: tema, stile, lunghezza, seme, traffico, difficolta', punti show, moduli fx, scenario.
- `dati/frasi.json`: frasi dei personaggi (nessun limite artificiale al numero).
- `dati/personaggi/<nome>/manifest.json`: id, ruolo, file, palette, crediti, licenza, stato.
- `dati/audio/playlist.json`: radio -> elenco brani (.xm).
- `dati/ui/menu.json`, `dati/ui/hud.json`: layout UI (font e grafica: @grafico).
- `dati/config.json`: seed, moduli fx on/off, target fps.

Immagini: PNG. Audio: .xm (riferimento di Andrea, sostituibile via playlist).

---

## 7. Piano per blocchi (costruito per blocchi)

| Blocco | Contenuto | Dipende da | Stato (RICHIESTA.md) |
|---|---|---|---|
| C | nucleo (core, road, render, input, audio base), runner SDL2, core libretro (stub se non fattibile), strada con curve e salite, auto giocatore, 50 fps | B (questi documenti) | [x] (consegna @developer; build/fps da verificare su Pi 400, blocco H) |
| D | personaggi/gameplay: Gianni, Gesu', UFO, fumetti, escalation, inattivita' | C | [x] |
| K/L/M | 3 livelli di prova (dati/livelli), scenari (assets_originali/scenari), UI (font, menu, HUD) | B, D | [x] / [~] vedi RICHIESTA.md |
| fx | razzo B (tasto B), sollevamento auto ad alta velocita', effetti colore crash | C | [ ] |
| Replay/test | determinismo, hash ogni 60 frame, misura frame Pi 400 | C | [~] (blocco H, da Pi 400) |
| N (facoltativo, solo dopo C) | ROM Mega Drive originale .md tutta nuova (fattibilita' toolchain 68000) | C | [ ] |

Ogni passo chiude con: build, test di replay, misura frame, review (PIANO_TURNI.md, VISIONE.md).

---

## 8. Fatti verificati vs ipotesi

**Fatti (verificati in questa sessione su file locali):**
- Vincoli duri da VISIONE.md (render 320x224, 50 Hz, int16 stereo, RetroPad, frame counter, RNG seme, replay, hash ogni 60, zero alloc, Pi 400).
- Pista C decisa da Andrea (2026-10-15); A2/Cannonball no-go (FONTI.md).
- D1 chiusa da Andrea (2026-10-16): codice MIT, grafiche/testi CC-BY 4.0; DEFAULT "nessuna licenza" decade.
- D3 chiusa da Andrea (2026-10-16): albero 5 percorsi x 5 tappe = 15 tappe, 5 finali; prototipo 3 livelli di prova stili diversi.
- Moduli da MODULI_E_REPARTI.md; dati esistenti (frasi.json 156 fumetti: Gianni 97, Gesù 59; show_logica.md; manifest CC-BY-4.0).
- Codice blocco C consegnato e ispezionato da @debugger: 25/25 file src con SPDX MIT, include corretti, frameHash ogni 60 frame, determinismo (frame counter + xorshift32 seme fisso, nessun orologio nel core, core senza SDL).
- `check_catalogo.py` exit 0 (catalogo: 11 righe, 2 cartelle); `quota_open.py` 91.8% (247656/269832 byte, soglia 50%).

**Ipotesi (da verificare, non usate per decisioni):**
- Prestazioni effettive su Pi 400 (frame <= 20 ms, obiettivo <= 12 ms): da misurare in blocco H con toolchain su Pi 400.
- Disponibilità pacchetti build su RetroPie/Raspberry Pi OS ([C] in BUILD.md §2, da verificare prima dell'installazione).
- libxmp per musica .xm su Pi ([C], da verificare).
- Numero esatto di tappe/finali dell'originale OutRun: solo riferimento di analisi (FONTI.md), non codice né asset.

---

## 9. Nota di fine

Questo documento specifica l'architettura; non contiene codice C++. Il codice è compito di @developer (blocco C) in `motore_nuovo/src/` e `motore_nuovo/CMakeLists.txt`.
