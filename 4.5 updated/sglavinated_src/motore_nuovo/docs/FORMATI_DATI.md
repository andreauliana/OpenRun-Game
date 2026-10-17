# FORMATI DATI — motore nuovo "Outrun Sglavinated Edition v3 (open)" (pista C)

SPDX-License-Identifier: MIT (documento); i dati delle cartelle hanno la licenza del loro manifest (CC-BY-4.0 salvo diversa indicazione).

Autore: @architetto. Data: 16 ott 2026. Stato: consegna BLOCCO B (Turno 1).
Percorso: motore_nuovo/docs/FORMATI_DATI.md

---

## 1. Principi

- Tutto ciò che può variare (livelli, frasi, personaggi, radio, UI, moduli on/off) vive in file di dati, non in codice C++.
- Formati: JSON per i dati strutturati, PNG per sprite/scenari, `.xm` (tracker) per l'audio di riferimento.
- Nessun limite artificiale: numero di frasi, personaggi, livelli, moduli fx non è bloccato da costanti nel codice. I limiti della hack v87 (156 fumetti, 4 MB ROM, 64 KB RAM, bolla 88 px) NON si portano.
- Determinismo: ogni dato che influisce sul gioco (seme livello, seed RNG, tabelle show) è leggibile e fissabile da file.
- Codifica file: UTF-8, JSON senza commenti.

---

## 2. Convenzioni generali

| Elemento | Convenzione |
|---|---|
| Directory dati | `motore_nuovo/dati/` |
| Livelli | `motore_nuovo/dati/livelli/<id>/livello.json` |
| Personaggi | `motore_nuovo/dati/personaggi/<id>/manifest.json` + `fumetti.json` |
| Audio | `motore_nuovo/dati/audio/playlist.json` (brani `.xm` di riferimento) |
| UI | `motore_nuovo/dati/ui/menu.json`, `motore_nuovo/dati/ui/hud.json` |
| Config | `motore_nuovo/dati/config.json` (seed, moduli on/off) |
| Immagini | PNG; sprite max 16 colori opachi per DIREZIONE_ARTISTICA.md; scenari/fondi possono usare gradienti (consentito) |
| Audio di gioco | .xm tracker (musica di Andrea, sostituibile senza toccare il codice) |
| Codifica file | UTF-8, JSON senza commenti |

Ogni personaggio ha cartella propria, `manifest.json` e riga in `docs/CATALOGO_PERSONAGGI.md` (verificato da `strumenti_py/check_catalogo.py` prima di ogni consegna).

---

## 3. livello.json — schema e esempio

Percorso: `dati/livelli/<id>/livello.json`. Un livello = una tappa con il suo scenario, tema e ritmo degli show.

Campi:

| Campo | Tipo | Obbligatorio | Descrizione |
|---|---|---|---|
| id | stringa | sì | identificativo livello (es. `lv_01_neon`) |
| tema | stringa | sì | tema del livello (da DIREZIONE_ARTISTICA.md: sci-fi anni '50, futurista, punk, synthwave/C-wave, Windows 95 vibe, parodia, psichedelico...) |
| stile | stringa | sì | stile dichiarato (regola 7: ogni livello ha stile diverso) |
| lunghezza_frame | numero | sì | lunghezza del tracciato in frame |
| seme | numero intero | sì | seme del tracciato generato da seed (tracciato da zero, mai tratto dall'originale) |
| difficolta | stringa | sì | facile/media/difficile (prototipo: facile) |
| traffico | oggetto | sì | densità e tipo veicoli di traffico (senza asset Sega) |
| punti_show | array di oggetti | no | frame o posizioni in cui partono show (Gianni/Gesu'/UFO) |
| moduli_fx | oggetto | no | flag on/off per moduli fx (razzo_b, sollevamento_auto, effetto_colore_crash) |
| scenario | stringa | no | riferimento allo scenario per @scenografo (assets_originali/scenari/<id>/) |
| tempo_limite_frame | numero (frame) | no | tempo limite con checkpoint che ridanno tempo |
| checkpoint | array | no | posizioni dei checkpoint |
| musica | oggetto | no | radio iniziale e brani (riferimento playlist.json) |

Esempio (livello 1 di prova):

```json
{
  "id": "lv_01_neon",
  "tema": "notte urbana sci-fi",
  "stile": "synthwave / C-wave",
  "lunghezza_frame": 15000,
  "seme": 4211,
  "difficolta": "facile",
  "traffico": {
    "densita": "bassa",
    "veicoli": ["auto_sci_fi_a", "auto_sci_fi_b"]
  },
  "punti_show": [
    { "frame": 500, "tipo": "uscita_strada", "personaggio": "gianni" },
    { "frame": 1200, "tipo": "crash", "personaggio": "gesu" },
    { "frame": 2500, "tipo": "periodico", "personaggio": "gianni" }
  ],
  "moduli_fx": {
    "razzo_b": true,
    "sollevamento_auto": true,
    "effetto_colore_crash": true
  },
  "scenario": "scenario_neon_01",
  "tempo_limite_frame": 6000,
  "checkpoint": [2000, 4000, 6000],
  "musica": { "radio_iniziale": "radio_lofi", "brani": ["brano1.xm", "brano2.xm"] }
}
```

---

## 4. Sprite e personaggi

Sprite: file PNG nelle cartelle di appartenenza (auto del giocatore in `assets_originali/sprite/`, scenari in `assets_originali/scenari/<id>/`). Ogni personaggio ha:

- `dati/personaggi/<id>/manifest.json` con campi obbligatori per `check_catalogo.py`: `id`, `ruolo`, `file`, `crediti`, `licenza`. Campi consigliati: `nome`, `descrizione`, `stato` (`nuovo`/`hack`/`idea`), `source_v87`, `frasi_n`, `id_frasi`, `tabelle_show`, `note_v87_non_riportate`.
- `dati/personaggi/<id>/fumetti.json`: elenco fumetti del personaggio.
- Riga in `docs/CATALOGO_PERSONAGGI.md`.

Esempio manifest:

```json
{
  "id": "gianni",
  "nome": "Gianni (la foglia, tempter)",
  "ruolo": "personaggio che tenta: appare dopo uscite di strada e show periodici",
  "stato": "nuovo",
  "file": { "fumetti": "fumetti.json" },
  "licenza": "CC-BY-4.0",
  "crediti": "Testi e disegni originali: Andrea Uliana (hack v87, sola lettura)."
}
```

---

## 5. frasi.json — schema

Percorso: `dati/frasi.json`. Contiene tutte le frasi dei personaggi (Gianni, Gesù, eventuali nuovi). Nessun limite artificiale sul numero.

Campi di primo livello: `licenza`, `versione`, `totale`, `id_min`, `id_max`, `fonte_v87`, `note`, `regole_testo_v87` (riferimento, non vincolo), `range_personaggio_v87`, `frasi` (array).

Ogni frase:

| Campo | Tipo | Descrizione |
|---|---|---|
| id | intero | identificativo frase (0..N, nessun tetto) |
| personaggio | stringa | `gianni` | `gesu` | altro |
| testo | stringa | testo maiuscolo, 2 righe max (riferimento v87) |
| tabella | stringa | tabella di appartenenza (es. `otab`, `ph_tab`, `itab`, `g1t`) |

Nota: i dati attuali (156 frasi: Gianni 97, Gesù 59) sono già in `dati/frasi.json` (consegna BLOCCO CONS, @traduttore). Nel nuovo motore sono sostituibili e ampliabili senza toccare il codice.

---

## 6. playlist.json — radio e brani

Percorso: `dati/audio/playlist.json`. Mappatura radio -> elenco brani. I brani si cambiano senza toccare il codice (regola 8).

Esempio:

```json
{
  "licenza": "riferimento (musica di Andrea, sostituibile)",
  "radio": {
    "radio_lofi": ["brano_lofi_1.xm", "brano_lofi_2.xm"],
    "radio_jungle": ["brano_jungle_1.xm"],
    "radio_atmosferica": ["brano_atmosf_1.xm"]
  },
  "radio_iniziale": "radio_lofi",
  "note": "Solo tracker .xm di Andrea. Nessun brano definitivo."
}
```

---

## 7. UI — ui/*.json

Percorso: `dati/ui/menu.json` e `dati/ui/hud.json`. Layout, font e posizioni (grafica e font: @grafico, BLOCCO M), risoluzione target 320x224.

`dati/ui/hud.json` (esempio): campi `tachimetro`, `tempo`, `punteggio`, `indicatore_tappa` con posizione e dimensione su framebuffer 320x224.

`dati/ui/menu.json` (esempio): voci menu principale, titolo "Sglavinated Edition", opzioni radio.

---

## 8. config.json — moduli on/off

Percorso: `dati/config.json`. Flag per attivare/disattivare moduli extra (`fx/<nome>/`) e parametri globali (seed RNG, target fps 50).

Esempio:

```json
{
  "seed_globale": 12345,
  "fps_target": 50,
  "render_w": 320,
  "render_h": 224,
  "audio": { "canali": 2, "formato": "int16_stereo" },
  "moduli": {
    "razzo_b": true,
    "sollevamento_auto": true,
    "effetto_colore_crash": true
  }
}
```

Spegnendo tutti i moduli extra il nucleo gira uguale (regola dei moduli extra, MODULI_E_REPARTI.md).

---

## 9. Cosa NON è in questi formati

- Nessun dato della ROM originale Sega (tracciati, sprite, musica, tile).
- Nessun limite hardware v87 (156 frasi, budget VRAM/tile, bolla 88 px).
- Nessun percorso o nome derivato da giochi esistenti (DIREZIONE_ARTISTICA.md).
