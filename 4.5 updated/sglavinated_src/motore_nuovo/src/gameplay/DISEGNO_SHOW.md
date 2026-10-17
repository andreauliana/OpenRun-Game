SPDX-License-Identifier: CC-BY-4.0

# DISEGNO_SHOW — interfaccia di disegno degli show (DatiShow -> renderer)

Traduzione del BLOCCO P3 (SCAMBIO voce 73): **cosa disegnare a ogni frame** a
partire da `DatiShow` (personaggio, posa, posizione, fumetto, UFO) e come
collegarla al renderer — oggi in `src/game/game.cpp` lo show viene scartato
con `(void)show;` (riga 219).

Nota di perimetro: il documento sta in `src/gameplay/` perche' a @traduttore
la scrittura e' stata negata sia in `motore_nuovo/docs/` sia in
`motore_nuovo/dati/*.md` (permission denied, regola 21: non si insiste;
segnalato a @capo in SCAMBIO voce 76). Se @capo lo preferisce, lo spostare in
`motore_nuovo/docs/DISEGNO_SHOW.md`.

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

## 3. Collegamento al nucleo (compito di @coder, `src/game/` + `src/render/` + CMakeLists)

1. **CMakeLists.txt**: aggiungere alla lista di `sglavinated_core` (sono elenco
   esplicito, non glob): `src/gameplay/disegno_show.cpp`,
   `src/gameplay/disegno_show.h`, `src/gameplay/testi_frasi.cpp`,
   `src/gameplay/testi_frasi.h`.
2. **game.h**: `#include "gameplay/disegno_show.h"` e
   `#include "gameplay/testi_frasi.h"`; due membri:
   `gameplay::TestiFrasi testiFrasi_;` e `gameplay::DisegnoShow disegnoShow_;`
   (dopo `showManager_`).
3. **init()** (una sola volta, tutte le alloc qui):
   ```cpp
   testiFrasi_.carica("motore_nuovo/dati/frasi.json"); // percorso relativo, come livello.json
   testiFrasi_.verifica();     // invariante personaggio<->frase (log su false)
   disegnoShow_.setTesti(&testiFrasi_);
   disegnoShow_.layout().schermoW = Framebuffer::WIDTH;   // opzionale, 320x224
   disegnoShow_.layout().schermoH = Framebuffer::HEIGHT;
   ```
4. **update()**: al posto di `(void)show;` (riga 219):
   ```cpp
   const DatiShow& show = showManager_.aggiorna(gi);
   disegnoShow_.aggiorna(clock_.frame, showManager_); // stesso frame!
   ```
   Va chiamato **dopo** `showManager_.aggiorna()` e con lo stesso contatore di
   frame, altrimenti bolla e posa arrivano di un frame indietro.
5. **render()**, dopo `ren_.drawPlayerCar(...)`:
   ```cpp
   const ScenaShow& sc = disegnoShow_.scena();
   if (sc.ufoVisibile)          { /* sprite UFO a sc.ufoX/ufoY */ }
   if (sc.personaggioVisibile)   { /* sprite pose sc.pose in box persX/persY/persW/persH */ }
   if (sc.bollaVisibile)         { /* scatola bollaX/Y/W/H, coda codaX/Y, testo testoX/rigaY[0..1] */ }
   ```
6. **Hook necessari nel `Renderer`** (sono di @coder, questo modulo non puo'
   toccare `src/render/`):
   - `void disegnaSpriteSchermo(const Sprite& s, i32 x, i32 y, i32 w, i32 h);`
     — disegna in coordinate schermo con scala nearest-neighbour sul box dato
     (oggi `drawSprite` e' privato e solo per il mondo). Serve anche per l'UFO.
   - Fumetto: `disegnaRettangolo(x, y, w, h, riempimento, bordo, spessore)` (o
     `Framebuffer::fillRect`, che e' gia' pubblico) + il testo col font di
     @tipografo (`assets_originali/ui/font/` + `dati/ui/`).
   - **Verifica immediata anche senza sprite**: con solo
     `fb().fillRect(bollaX, bollaY, bollaX+bollaW, bollaY+bollaH, ...)` e un
     rettangolo di prova per `persX/persY/persW/persH` si vede gia' l'intero
     comportamento (entrata/tenuta/uscita, posa che cambia come indice,
     fumetto che appare e sparisce, UFO che attraversa). Deterministico, zero
     asset.
7. **Asset delle pose** (quando @asset li produce, registro in
   `docs/CATALOGO_PERSONAGGI.md`): chiavi `gianni/pose_00..07`,
   `gesu/pose_00..03`, `ufo` (un solo sprite: il volo e' dato da ufoX/ufoY).
   Registrazione come gli sponsor (`Renderer::aggiungiSponsorSprite`); la
   mappatura chiave -> indice spetta al runner, il modulo fornisce solo la
   chiave (`DisegnoShow::chiaveSprite(personaggio, pose, buf, dim)`).

## 4. Riferimenti nel codice

- `dati/SPEC_GAMEPLAY.md` §5 (show e bolla) e §7 (interfaccia verso il nucleo):
  la sezione §9 "interfaccia di disegno" va aggiunta da @capo (scrittura
  negata a me su `dati/*.md`): il testo pronto e' il paragrafo qui sotto.
- `dati/show_logica.md` §3 (tavole pose/sway/bob) e §6 (interfaccia al nucleo).
- `src/gameplay/show.h`: `DatiShow`, `ShowManager::bollaVisibile()`, `ufo()`.
- `src/gameplay/testi_frasi.h`: invariante personaggio <-> frase (`verifica()`).

Testo pronto per SPEC_GAMEPLAY.md §9 (da incollare da @capo):
"§9 Interfaccia di disegno (DatiShow -> renderer) — BLOCCO P3. Oltre alla
logica, il modulo espone cosa disegnare a ogni frame
(`src/gameplay/disegno_show.h/.cpp`, MIT): `DisegnoShow::aggiorna(frame,
showManager_)` -> `ScenaShow` con box del personaggio in px
(`persX/persY/persW/persH`), `pose` (Gianni 0-7, Gesu' 0-3, chiave asset
`gianni/pose_07`), scatola del fumetto (`bollaX/Y/W/H`, `testoX`, `rigaY[2]`,
coda `codaX/codaY`), testo gi' letto da `dati/frasi.json` (`TestiFrasi`), UFO
(`ufoX/ufoY`, mai frase). I tempi del fumetto restano quelli di §5:
`bollaVisibile()` dell'`ShowManager` e' l'unica fonte. Entrata/uscita in dati
(`ShowLayout`): Gianni scivola da sinistra, Gesu' dall'alto e risale; pose
ogni 16 frame; deriva sway/bob da tabelle intere (tecnica della v87, valori
nostri, nessun libm -> determinismo replay). Tutto in px sul framebuffer
320x224; il modulo non include `render/` ne' SDL."

## 5. Fonti e licenza

Fonti (sola lettura): `HANDOVER.md` §0/§2/§4, `SPEC_personaggi_v74.md` §4-§7,
`OPENCODE_REFERENCE_v74.json`, `tools/gianni12.py`, `dati/frasi.json`,
`dati/show_logica.md`, `dati/SPEC_GAMEPLAY.md`, `dati/personaggi/*/manifest.json`.
Testi delle frasi: Andrea (CC-BY-4.0 in `frasi.json`). Questo documento:
CC-BY-4.0. Codice: MIT (SPDX in testa ai file).
Nessun codice 68000, nessun asset/grafica/audio Sega, percorsi relativi.
