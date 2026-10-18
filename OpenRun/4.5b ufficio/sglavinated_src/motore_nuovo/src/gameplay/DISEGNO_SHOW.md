SPDX-License-Identifier: CC-BY-4.0

# DISEGNO_SHOW — interfaccia di disegno degli show (DatiShow -> renderer)

Documento d'interfaccia del BLOCCO P3: **cosa disegnare a ogni frame** a
partire da `DatiShow` (personaggio, posa, posizione, fumetto, UFO) e come
collegarlo al renderer. Il collegamento dati->scena->framebuffer e' presente
in `src/game/game.cpp`; la resa finale di sprite e testo resta incompleta.

Moduli (codice MIT, SPDX in testa a ogni file):

| File | Cosa fa |
|---|---|
| `disegno_show.h/.cpp` | `DisegnoShow::aggiorna(frame, showManager_)` -> `ScenaShow`: valori gia' pronti in px |
| `testi_frasi.h/.cpp` | `TestiFrasi::carica("motore_nuovo/dati/frasi.json")`: id frase -> `linea1`/`linea2` (156 fumetti, CC-BY-4.0) |

Il modulo **non disegna**: non include `render/` ne' SDL, non tocca il
framebuffer. Restituisce solo istruzioni (`ScenaShow`) che il renderer
consuma. Nessun limite hardware della v87 (156 fumetti, VRAM, bolla 96x24 e
88 px di riga): la larghezza del fumetto la decide il testo, con un solo cap
di sicurezza sullo schermo.

## 1. Cosa c'e' in `ScenaShow` (ogni frame)

| Campo | Significato |
|---|---|
| `personaggioVisibile`, `personaggio`, `origine` | chi e perche' (crash/uscita di strada/periodico/inattivita') |
| `pose` | indice della pose (Gianni 0-7, Gesu' 0-3); chiave asset `gianni/pose_07` via `DisegnoShow::chiaveSprite()` |
| `persX`, `persY`, `persW`, `persH` | box dello sprite sullo schermo 320x224 (angolo alto-sx + ingombro da coprire; gia' comprensivo di entrata, uscita, sway e bob) |
| `frameShow`, `frameRimasti` | avanzamento dello show (per animazioni del renderer o transizioni) |
| `bollaVisibile` | tempi **gia' decisi** da `ShowManager::bollaVisibile()` (appare al ~30 frame, sparisce prima dell'uscita: Gianni < 30 frame rimasti, Gesu' < 55). Il renderer non ricalcola nulla |
| `bollaX/Y/W/H` | scatola del fumetto, calcolata sul testo (max 2 righe), centrata sopra la testa del personaggio e tenuta dentro lo schermo |
| `testoX`, `rigaY[2]` | posizione della riga 1 e della riga 2 (y della seconda utile anche con 1 riga) |
| `codaX`, `codaY` | punto della coda del fumetto (sulla testa del personaggio) |
| `idFrase`, `righe`, `linea1`, `linea2` | testo: id in `dati/frasi.json` + i caratteri gi' letti (nullptr se `TestiFrasi` non caricato: resta solo la scatola) |
| `ufoVisibile`, `ufoX`, `ufoY` | UFO: passaggio da destra a sinistra con ondulazione verticale, **mai una frase** |

Ordine di disegno (dopo `drawPlayerCar`): **1) UFO, 2) personaggio, 3)
fumetto** (scatola poi testo). Il personaggio sta sopra la strada e sotto il
fumetto. Con uno show a schermo l'UFO non viene disegnato (e' in fuga):
evita il salto di posa della fuga a 24 frame.

## 2. Timing (tutto dato in `ShowLayout`, nessun codice da toccare)

Fasi di uno show (durata da `DatiShow::durata`, 260 frame di default):

- **entra** (`entraFrame`, 24): scivola da fuori schermo lungo `entraDa`
  (Gianni da sinistra; **Gesu' dall'alto**: "cade dall'alto e risale" dal
  manifest del personaggio).
- **tenuta**: pose ogni `posePasso` frame (`pose = (t / posePasso) % poseNumero`);
  deriva lenta da tabelle intere: `SWAY` (orizzontale, 4 valori) e `BOB`
  (verticale, 16 valori) moltiplicati per `swayAmp`/`bobAmp` — stessa tecnica
  a tavole della v87 (sw/bob), valori nostri, **nessun seno/libm** (identico
  su ogni piattaforma, serve ai test di replay).
- **esce** (`esceFrame`, 24): scivola via verso `esceVerso` (Gesu' risale in
  alto). Il fumetto e' gia' stato nascosto prima dall'`ShowManager`.

UFO: `DatiUfo::progresso` (0 = appena fuori a destra, 1 = appena fuori a
sinistra) + `yOffset` (ondulazione, tabella tw) -> `ufoX`/`ufoY`.

Default di `ShowLayout`: personaggi a destra (x = 240), per non andare
sull'HUD in alto a sinistra (`dati/ui/hud.json`); fumetto centrato sopra la
testa; larghezza = max caratteri x `carattereLarghezza` + padding, con
`larghezzaMin`/`larghezzaMax` in dati. La metrica del font
(`carattereLarghezza`, `rigaAltezza`) va allineata al font di @tipografo
quando esiste: `disegnoShow_.setLayout(...)`.

## 3. Stato del collegamento nel progetto

- `motore_nuovo/CMakeLists.txt` include `disegno_show.cpp` e `testi_frasi.cpp`
  nel core.
- `Game::init()` carica e collega i testi e configura `DisegnoShow`.
- `Game::update()` aggiorna `ShowManager` e `DisegnoShow` sullo stesso frame.
- `Game::render()` consuma `ScenaShow` dopo l'auto. Attualmente UFO,
  personaggio e fumetto sono rappresentati da rettangoli di debug; il testo
  delle frasi e gli sprite non sono ancora disegnati dal renderer.

## 4. Lavoro ancora necessario

1. Collegare le chiavi sprite `gianni/pose_00..07`, `gesu/pose_00..03` e `ufo`
   agli asset approvati nel catalogo; non usare asset in bozza come risorse di
   produzione.
2. Disegnare le pose e il fumetto (contorno, coda e testo letto da
   `ScenaShow`) mantenendo l'ordine UFO, personaggio, fumetto.
3. Aggiungere o aggiornare test per timing, visibilita', layout entro 320x224
   e replay deterministico; registrare prove nel checkpoint attivo.

Non aggiungere nuovamente i file al CMake o ripetere il wiring gia' presente.
Per la logica e le invarianti consultare `dati/SPEC_GAMEPLAY.md`,
`dati/show_logica.md`, `src/gameplay/show.h` e `src/gameplay/testi_frasi.h`.

## 5. Provenienza e licenza

I dettagli di timing e posa derivano dai dati del progetto e dalle regole
documentate; il codice e gli asset devono mantenere le licenze registrate nei
rispettivi manifest. Questo documento e' CC-BY-4.0. Nessun codice 68000 o
asset/audio Sega e' parte dell'implementazione.
