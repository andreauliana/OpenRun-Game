SPDX-License-Identifier: CC-BY-4.0

# Logica degli show (Gianni / Gesù / UFO) — riferimento v85c (hack v87)

Questo file descrive il COMPORTAMENTO del modulo gameplay (Gianni, Gesù, fumetti, escalation, inattività) da implementare nel motore nuovo in `src/gameplay/` (compito di @coder). I dati sono in `dati/frasi.json` e `dati/personaggi/{gianni,gesu}/fumetti.json`.

Origine (sola lettura, mai toccata): `HANDOVER.md`, `SPEC_personaggi_v74.md`, `OPENCODE_REFERENCE_v74.json`, `tools/gianni12.py`, `ARCHIVIO_V87.md`.

Valori numerici (24 frame cooldown, 260 frame durata, pr_tab, soglie escalation, tabelle a 32/4x32, 88 px) sono RIFERIMENTO di comportamento della v87: si portano come dati di partenza, non come limiti del nuovo motore. Il limite hardware "156 fumetti" NON si porta.

## 1. Personaggi e regole personaggio <-> frase

| Personaggio | Ruolo | Appare dopo | Fonte frasi |
|---|---|---|---|
| Gianni (la foglia di canapa) | tenta con slang (droghe leggere e pesanti) | uscita di strada (off-road), show periodico, inattività | solo id Gianni |
| Gesù (corrector) | corregge, messaggio anti-droga / sicurezza stradale | crash, show periodico di escalation | solo id Gesù |
| UFO | passaggio casuale, nessuna frase | timer frame | nessuna |

**Regole personaggio <-> frase (invarianti):**
- Gianni dice SOLO frasi del suo range: 0–26, 34–65, 85–104, 121–124, 134–140, 142–146, 152–153 (97 frasi).
- Gesù dice SOLO frasi del suo range: 27–33, 66–84, 105–120, 125–133, 141, 147–151, 154–155 (59 frasi).
- Crash → sempre Gesù.
- Uscita di strada → sempre Gianni.
- Totale: 156 frasi, id 0–155, 2 righe max, maiuscole, ≤88 px/riga (riferimento v87), no accenti.

## 2. Trigger (causa -> effetto)

| Evento | Rilevamento | Effetto | Note |
|---|---|---|---|
| Crash | segnale del gioco (collisione/capottamento) | Gesù, frase da `xtab` (o tabella escalation del livello) | ha sempre la priorità; interrompe Gianni se in mostra |
| Uscita di strada | ruote fuori strada per ≥ 8 frame (riferimento v87) | Gianni, frase da `otab` | cooldown 24 frame tra show; NON incrementa errori; aggiorna istante ultimo errore |
| Periodico (tempo) | intervallo `pr_tab[stage]` frame da ultimo show | frase periodica; chi parla dipende dal livello di escalation | `pr_tab` = [500, 440, 400, 360, 320] (riferimento v87) |
| Inattività | 12 s senza input in gara (soglia 500 frame a 50 fps, riferimento v87 PAL), senza crash/off-road/show/caricamento | Gianni, frase da `itab` (id 35–38) | una volta per pausa |
| UFO | contatore frame | passaggio a schermo, nessuna frase | esclude show in corso |

Guardie di partenza show (riferimento v87): gioco avviato da ≥ 250 frame; nessun UFO, caricamento o restore in corso.

## 3. Cooldown e timing (riferimento v87)

- Cooldown minimo tra due show: 24 frame.
- Durata show: 260 frame fissi nella v87 (prima: 220 + 30·stage); nel nuovo motore è un dato configurabile per livello.
- Bolla di testo: appare al frame ~30 (Gianni); nascosta prima dell'uscita (Gianni < 30 frame rimasti, Gesù < 55).
- Animazioni a frame fisso del gioco, senza jitter (pose ogni 16 frame, sway/bob su tabelle).

## 4. Escalation (livelli 0-3, decadimento a tempo)

Gli errori si accumulano nello stage e decadono col tempo.

- Errori: +1 a ogni crash; +1 ogni 2 uscite di strada (una uscita su due conta come errore).
- Livello = errori / 2: 0–1 → 0, 2–3 → 1, 4–5 → 2, ≥ 6 → 3.
- Decadimento: ogni 600 frame senza errori → errori −1 (≈ 12 s "puliti" a 50 fps → livello −1).
- Tutto azzerato al cambio stage.

Chi parla negli show periodici, per livello:
- Livello 0 → Gianni sempre.
- Livello 1 → Gesù 1 volta su 2.
- Livello 2 → Gesù 3 volte su 4.
- Livello 3 → Gesù sempre.

Frasi di escalation (6 per livello per personaggio, id che stanno in una sola tabella, mai in show periodico):
- Gianni: `g1t` [0,1,2,3,4,5], `g2t` [6,7,8,9,10,12], `g3t` [14,15,17,18,20,21]
- Gesù: `j1t` [27,28,29,30,31,32], `j2t` [66,67,68,69,70,71], `j3t` [72,105,110,111,112,113]

## 5. Tabelle show (dati v87, in dati/frasi.json e dati/personaggi/*/fumetti.json)

| Tabella | Uso | Contenuto (id frase) | Dove |
|---|---|---|---|
| `otab` | Gianni uscita di strada | 134,135,136,121,142,143,144,145,146,152,153,122,123,124,44,19,54,64,65 (ciclica x2) | personaggi/gianni/fumetti.json |
| `ph_tab` | Gianni periodico, per stage (riferimento v87: indice `stage*32 + (VAR178&31)`) | vedi JSON (32 entrate elencate piatte) | personaggi/gianni/fumetti.json |
| `g1t/g2t/g3t` | Gianni escalation L1/L2/L3 | 6 id ciascuno | personaggi/gianni/fumetti.json |
| `xtab` | Gesù dopo crash | 126,130,141,147,148,149,150,151,154,155,73,76,80,82,106-109 (ciclica x2) | personaggi/gesu/fumetti.json |
| `jtab` | Gesù periodico | vedi JSON | personaggi/gesu/fumetti.json |
| `j1t/j2t/j3t` | Gesù escalation L1/L2/L3 | 6 id ciascuno | personaggi/gesu/fumetti.json |
| `itab` | Inattività (Gianni) | 35,36,37,38 | personaggi/gianni/fumetti.json |
| `pr_tab` | Intervallo periodico per stage | [500,440,400,360,320] | dati/frasi.json |

Nella v87 esisteva anche `ctab` (32 id, codice morto, ramo Gesù-ma-frasi-Gianni = bug P2): nel nuovo motore è assorbita da `itab` (inattività, Gianni).

Tavole di animazione/timing (riferimento comportamentale v87, non da copiare come codice): `sq` (sequenza pose 32), `ez_tab` (entrata 24), `xo_tab` (uscita 24), `bz_tab` (rimbalzo bolla 12), `sw_tab` (sway 4), `bob` (hover 16), `gmask` (pezzi 64).

## 6. Interfaccia verso il nucleo (per @coder)

Il modulo gameplay espone, per frame:
- `show_event(tipo)`: registra crash / uscita-di-strada / inattività scaduta.
- `show_update(frame, stage, input_attivo)`: avanza cooldown, escalation, decadimento errori; restituisce se deve partire uno show, con personaggio, tabella e id frase scelti.
- `show_attivo()` / `dati_show()`: personaggio in mostra, frame rimanenti, id frase (per il renderer della bolla).
- `livello_escalation()`: 0–3 (per altri moduli, es. effetti colori dopo crash).

Tutto deterministico: tempo = contatore di frame, RNG con seed, nessun orologio di sistema.

## 7. Cosa NON si porta dalla v87

- Limite 156 fumetti / budget Mega Drive / VRAM 28–87 tile / bolla 88 px / codice 68000 / grafica e audio Sega.
- `ctab` come tabella separata (assorbita in `itab`).
- Tabella frase per tabella di provenienza: la frase da sola non distingue se lo show è nato da off-road o periodico (16 id sono in entrambe `otab` e `ph_tab`); la provenienza è gestita dal trigger, non dall'id.
