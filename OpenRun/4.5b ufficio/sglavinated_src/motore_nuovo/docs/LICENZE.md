<!-- SPDX-License-Identifier: MIT -->
# LICENZE — motore nuovo "Outrun Sglavinated Edition v3 (open)" (pista C)

SPDX-License-Identifier: MIT (questo documento)

Autore: @architetto. Data: 16 ott 2026. Aggiornato: 16 ott 2026 (D1 chiusa).
Percorso: motore_nuovo/docs/LICENZE.md

---

## 1. Licenza del progetto (D1 CHIUSA — confermata da Andrea 2026-10-16)

**Codice sorgente: MIT** (`motore_nuovo/LICENSE`).
**Grafiche, testi, dati creativi: CC-BY 4.0** (`manifest.json` di ogni cartella).

Il DEFAULT precedente ("nessuna licenza dichiarata") decade.
Tutti i file `src/` hanno intestazione `// SPDX-License-Identifier: MIT`.
Il file `motore_nuovo/LICENSE` (testo completo MIT) copre tutto il codice.

---

## 2. Documenti e dati consegnati

| File | Percorso | Licenza | Nota |
|---|---|---|---|
| FONTI.md | motore_nuovo/docs/FONTI.md | MIT (SPDX) | blocco B0 |
| ARCHITETTURA.md | motore_nuovo/docs/ARCHITETTURA.md | MIT (SPDX) | blocco B |
| FORMATI_DATI.md | motore_nuovo/docs/FORMATI_DATI.md | MIT (SPDX) | blocco B |
| BUILD.md | motore_nuovo/docs/BUILD.md | MIT (SPDX) | blocco B |
| LICENZE.md | motore_nuovo/docs/LICENZE.md | MIT (SPDX) | blocco B |
| FEDELTA.md | motore_nuovo/docs/FEDELTA.md | MIT (SPDX) | blocco B + BLOCCO FED |
| show_logica.md | motore_nuovo/dati/show_logica.md | CC-BY-4.0 (SPDX) | logica show v87 |
| frasi.json + personaggi/ | motore_nuovo/dati/ | CC-BY-4.0 (manifest) | CONS, @traduttore |
| SPEC_GAMEPLAY.md | motore_nuovo/dati/SPEC_GAMEPLAY.md | CC-BY-4.0 (SPDX) | blocco D |
| gameplay/*.h/*.cpp | motore_nuovo/src/gameplay/ | MIT (SPDX) | blocco D |
| livelli/*.json | motore_nuovo/dati/livelli/ | CC-BY-4.0 (manifest) | blocco K |
| ui/*.json + assets | assets_originali/ui/ | CC-BY-4.0 / MIT (manifest) | blocco M |
| asset varianti | assets_originali/sprite, skyline, palette/ | CC0-1.0 (manifest) | blocco F |
| Brani radio .bin | shots/pcmr_M.bin, pcmr_A.bin | CC-BY-4.0 (Andrea) | riferimento, sostituibili |

---

## 3. Librerie e strumenti di build (verificate o da verificare)

| Strumento | Licenza | Uso nel progetto | Stato |
|---|---|---|---|
| CMake | BSD-like (Kitware) | generazione build | [C] |
| SDL2 | zlib | finestra, input, runner | [C] |
| SDL2_image | zlib | caricamento immagini | [C] |
| SDL2_mixer | zlib | audio runner | [C] |
| nlohmann/json | MIT (single header) | parsing JSON dati | [C] |
| libxmp | LGPL | musica tracker .xm (futuro, opzionale) | [C] |
| Python 3 | PSFL | strumenti build/controlli | [C] |
| GCC / Clang | GPL / BSD-like | compilatore C++17 | [C] |
| RetroArch / libretro API | varie per core | core libretro target | [C] |

Nota: `[C]` = verificata dalla conoscenza, da controllare prima dell'uso. Prima di installare un pacchetto, riportarne la licenza in questa tabella.

---

## 4. Fonti tecniche (solo idee, mai codice o asset)

| Fonte | Cosa si prende | Licenza | Uso consentito |
|---|---|---|---|
| Lou's Pseudo 3d Page | matematica strada a segmenti | non dichiarata | solo riferimento |
| JavaScript Racer (J. Gordon) | struttura ciclo di gioco | non dichiarata | solo riferimento |
| Cannonball / OutRun originale | solo numeri di gameplay come analisi (velocità, feeling, 50 Hz, ritmo tappe) | Cannonball: custom no-commercial; Sega: proprietario | solo analisi |
| ZgzInfinity/OutRun | organizzazione moduli C++ | GPL-3.0 | solo se GPL accettata (non è il caso: D1 = MIT) |
| Hack Sglavinated v87 | personaggi, fumetti, ritmo show (opere di Andrea) | non dichiarata; derivato ROM Sega | dati portati, mai codice/asset Sega |

Nessun codice, asset, sprite, tracciato o musica della ROM originale Sega entra nel motore nuovo.

---

## 5. Obblighi per ogni nuovo file

1. Dichiarare la licenza: `SPDX-License-Identifier: MIT` (codice) o `CC-BY-4.0` (dati/grafiche) in testata o `manifest.json`.
2. Ogni risorsa esterna in `assets_originali/CREDITI.md` (autore, licenza, URL).
3. Prima di consegnare: `python3 motore_nuovo/strumenti_py/quota_open.py` (soglia 50%) e, se ci sono personaggi, `python3 motore_nuovo/strumenti_py/check_catalogo.py`.

---

## 6. Quota open source

Valore ultimo eseguito (prima di questa consegna): **91.8%** (247656 di 269832 byte, soglia 50.0%). Da rieseguire prima della consegna ufficiale di questo blocco e riportato in SCAMBIO.md.
