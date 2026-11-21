SPDX-License-Identifier: CC-BY-4.0

# SPEC_GAMEPLAY — compagni di viaggio, fumetti reattivi, escalation

Specifica breve del modulo gameplay del motore nuovo (blocco D, SCAMBIO voce 23).
Traduce le regole gia' verificate nella hack v87 (`HANDOVER.md`, `SPEC_personaggi_v74.md`,
`OPENCODE_REFERENCE_v74.json`, `tools/gianni12.py`) in specifica per `src/gameplay/`.
I testi/id sono in `dati/frasi.json` (CC-BY-4.0); la logica di riferimento e' in
`dati/show_logica.md`. Nessun codice 68000, nessun asset/grafica/audio Sega.

## 0. Trama (non Sega)

Il giocatore e' accompagnato in strada da due **compagni di viaggio**:

- **Gianni** (la foglia): il tentatore. Parla quando il giocatore sbaglia "lieve"
  (uscita di strada), quando si annoia (inattività) e negli show periodici.
- **Gesù**: il correttore. Interviene ai crash (sempre con priorita') e, man mano che
  gli errori salgono, prende sempre piu' la parola.
- **UFO**: passa e basta. Nessuna frase, mai.

Fumetti reattivi: ogni intervento e' un fumetto con 1-2 righe, scelto da tabelle di id
(`frasi.json`). L'**escalation** (livelli 0-3) misura quanti errori il giocatore sta
facendo nello stage e alza la frequenza e la durezza dei messaggi; se guida pulito per
un po', il livello scende da solo.

Novita' del motore nuovo (moduli fx, fuori da questo modulo ma collegati):

- **Razzo** (tasto B): modulo `src/fx/razzo_b/`, accessoria alla guida. Non genera show.
- **Salto auto** (ad massima velocita'): modulo `src/fx/sollevamento/`. Un salto non e' un
  crash: solo una collisione o un capottamento lo e'.
- **Effetti colore post-crash**: leggono `livello_escalation()` (0-3) per dosare
  l'effetto, senza toccare le regole dei fumetti.

## 1. Trigger e priorita' (causa -> effetto)

| Priorita' | Evento | Chi parla | Frasi | Note |
|---|---|---|---|---|
| 1 (massima) | **Crash** (collisione / capottamento, segnale del nucleo) | Gesu' | `xtab` (L0) o `j1t/j2t/j3t` (L1-L3) | sostituisce qualsiasi show in corso; +1 errore |
| 2 | **Uscita di strada** (ruote fuori strada per ≥ 8 frame) | Gianni | `otab` | cooldown 24 frame; NON conta come errore (1 su 2 conta, vedi §2); aggiorna l'istante dell'ultimo errore; se uno show periodico e' a schermo da ≥ 100 frame fa la sua uscita, altrimenti l'evento resta in attesa 250 frame |
| 3 | **Periodico** (intervallo `pr_tab[stage]` da ultimo show) | a rampa: vedi §3 | `ph_tab`/`jtab` (L0), tabelle livello (L1-L3) | solo se la gara e' avviata da ≥ 250 frame |
| 4 | **Inattivita'** | Gianni | `itab` (id 35-38) | 10 s senza tasti, una volta per pausa, vedi §4 |
| — | **UFO** | nessuno | nessuna | passaggio da destra a sinistra; esclude show in corso (e viene interrotto se ne parte uno) |

Guardie di partenza show: gara in corso (mai in menu, demo/attract, pausa o
caricamento), gara avviata da ≥ 250 frame, ≥ 24 frame dall'ultimo show, nessun UFO a
schermo. Con un evento crash/off-road in attesa il cooldown di 24 frame **non** vale.

## 2. Escalation (livelli 0-3, decadimento a tempo)

- Errori: **+1 a ogni crash**; **+1 ogni 2 uscite di strada** (una su due conta,
  alternata). L'uscita di strada aggiorna comunque l'istante dell'ultimo errore.
- Livello = `errori / 2`: 0-1 → 0, 2-3 → 1, 4-5 → 2, ≥ 6 → 3 (massimo 3).
- Decadimento: ogni **600 frame senza errori** (~12 s puliti a 50 fps) → errori −1.
- Tutto azzerato al cambio stage.
- `livello_escalation()` e' pubblico: altri moduli (es. effetti colore) lo leggono.

## 3. Show periodico e rampa del personaggio

Intervallo: `pr_tab[stage]` frame dall'ultimo show. Chi parla dipende dal livello:

| Livello | Chi parla | Frasi |
|---|---|---|
| 0 | Gianni sempre | `ph_tab` |
| 1 | Gesu' 1 volta su 2 (rotazione fissa) | Gianni `g1t`, Gesu' `j1t` |
| 2 | Gesu' 3 volte su 4 (rotazione fissa) | Gianni `g2t`, Gesu' `j2t` |
| 3 | Gesu' sempre | `g3t` / `j3t` |

Le rotazioni sono contatori, non casualita' (come la v87, VAR184/VAR178): a ciclo
chiuso le probabilita' sono esatte e il risultato e' riproducibile.

## 4. Inattivita'

- **10 s** senza tasti in gara (500 frame a 50 fps, la stessa soglia della v87)
  → Gianni dice una delle 4 frasi `itab` (id 35-38).
- Una volta per pausa: dopo aver parlato serve un nuovo input per riarmarsi.
- Non scatta con show attivo, evento off-road/crash pendente o fuori gara.

## 5. Show e bolla

- Durata show: **260 frame** fissi (dato configurabile per stage).
- Bolla di testo: appare al frame ~30 dello show; nascosta prima dell'uscita
  (Gianni con < 30 frame rimasti, Gesu' con < 55). Il renderer legge
  `bollaVisibile()` del modulo, non ricalcola tempi.
- Il testo va mostrato solo: l'id frase basta al renderer (nessuna logica sul testo).

## 6. Tabelle (id frase, fonte `dati/frasi.json`)

| Tabella | Uso | Chi |
|---|---|---|
| `otab` (32) | uscita di strada | Gianni |
| `xtab` (32) | crash, livello 0 | Gesu' |
| `ph_tab` (32) | periodico livello 0 | Gianni |
| `jtab` (32) | periodico livello 0 (rampa, quando parla Gesu') | Gesu' |
| `itab` (4) | inattivita' | Gianni |
| `g1t/g2t/g3t` (6 ciascuna) | escalation L1-L3 | Gianni |
| `j1t/j2t/j3t` (6 ciascuna) | escalation L1-L3 + crash L1-L3 | Gesu' |
| `pr_tab` (5) | intervallo periodico per stage | — |

Regole invariate: Gianni dice solo id Gianni (97), Gesu' solo id Gesu' (59); mai due
show consecutivi con la stessa frase (il modulo avanza la rotazione). Le frasi di
escalation stanno in una sola tabella: non si mescolano con `ph_tab`/`jtab`.

**Nota `pr_tab`**: `dati/frasi.json` e `dati/show_logica.md` riportano i valori v74
[500,440,400,360,320]. La v77 li ha misurati e ridotti a **[200,180,165,150,140]**
(`HANDOVER.md` §0: show 99 → 118 su 16000 frame; verificati anche in
`SPEC_personaggi_v74.md` §6). Il modulo usa questi ultimi come default, ma sono
dati: si cambiano in `ShowConfig::prTab` senza toccare il codice.

## 7. Interfaccia verso il nucleo (`src/gameplay/show.h`)

Per frame il nucleo passa i fatti e riceve lo stato dello show:

- `StatoGameplayInput`: `frame`, `stage`, `inGara`, `inputAttivo`, `offRoad`
  (ruote fuori strada). Il modulo conta da solo i ≥ 8 frame off-road e i 10 s.
- `ShowManager::crash()`: il nucleo lo chiama su collisione/capottamento.
- `ShowManager::aggiorna(input)` → `const DatiShow&`: personaggio, origine, id frase,
  frame inizio/durata, `frameRimasti()`, `progresso()`, `bollaVisibile()`.
- `ShowManager::livelloEscalation()` (0-3), `ShowManager::ufo()` (passaggio UFO:
  attivo, progresso, ondulazione y — nessuna frase).
- `cambiaStage()` al cambio stage; `reset(seed)` a ogni nuova partita.

Tutto deterministico: tempo = contatore di frame, tabelle a rotazione con offset dal
seed, nessun orologio di sistema, nessuna dipendenza da SDL.

## 8. Cosa NON si porta dalla v87

- Limite 156 fumetti, budget VRAM/tile, bolla 96×24 px e 88 px per riga, codice
  68000, grafica e audio Sega: nel nuovo motore i testi sono limitati solo dal
  layout del renderer.
- `ctab` (tabella morta della v87): assorbita da `itab`.
- UFO legato al contatore globale (`x = 128+340-(frame&0x7FF)`): nel nuovo motore
  l'UFO ha un periodo proprio (`UfoConfig`), non ruba blocchi e non serve la fuga
  a 12/24 frame della v86 — resta pero' l'interruzione quando parte uno show.
- La frase da sola non indica la provenienza: 16 id sono in sia `otab` sia `ph_tab`;
  la provenienza e' decisa dal trigger (`OrigineShow`), non dall'id.

## 9. Fonti e licenza

Fonti (sola lettura): `HANDOVER.md` §0/§2/§4, `SPEC_personaggi_v74.md` §4-§7,
`OPENCODE_REFERENCE_v74.json`, `tools/gianni12.py`, `dati/frasi.json`,
`dati/show_logica.md`. Testi e tabelle: Andrea (CC-BY-4.0 in `frasi.json`).
Questo documento: CC-BY-4.0. Codice del modulo: MIT (vedi `src/gameplay/`).
