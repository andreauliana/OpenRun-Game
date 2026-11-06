# CREDITI — Risorse grafiche e dati del progetto

## Font

- `assets_originali/font/antonio/Antonio-Bold.ttf` — Font vettoriale per titoli e fumetti.
  - Licenza: OFL-1.1 (Open Font License). Autore: Impallari Type.
  - URL: https://fonts.google.com/specimen/Antonio
  - Usato per: `ui/titolo/varianteB/title.png` (lettering "Sglavinated Edition").
  - Nota: La licenza OFL richiede di conservare la licenza e non rivendicare il nome del font per opere derivate.

## UI — Variante B (Lofi Jungle, bianco e nero)

Tutti gli asset UI della variante B sono stati disegnati da zero senza materiale Sega, rispettando la palette bianco/nero e il limite di 16 colori per elemento.

- `assets_originali/ui/font/varianteB/font.png` — Font pixel 5x7, 2 colori.
  - Licenza: CC-BY-4.0. Disegnato da zero per il progetto.

- `assets_originali/ui/titolo/varianteB/title.png` — Lettering "Sglavinated Edition".
  - Licenza: CC-BY-4.0. Disegnato da zero; utilizza Antonio Bold per la rasterizzazione.
  - Nota: Il font Antonio Bold è coperto da OFL-1.1 (vedi sopra); l'immagine risultante è CC-BY-4.0.

- `assets_originali/ui/menu/varianteB/menu_preview.png` — Anteprima menu 320x224, voci GUIDA/RADIO/SHOW/OPZIONI/ESCI.
  - Licenza: CC-BY-4.0. Disegnato da zero dai dati in `motore_nuovo/dati/ui/menu.json`.

- `assets_originali/ui/hud/varianteB/hud_preview.png` — Anteprima HUD 320x224, elementi: distanza, velocità, tempo, tappa, posizione, distanza, giri.
  - Licenza: MIT. Disegnato da zero dai dati in `motore_nuovo/dati/ui/hud.json`.

## Strumenti di generazione

- `assets_originali/ui/genera_ui_b.py` — Script Python (licenza MIT) che rigenera i 4 PNG della variante B leggendo `menu.json` e `hud.json`. Prodotto CC-BY-4.0.

## Licenza complessiva

- Quota open: 100% (tutti gli asset interni sono originali o con licenza compatibile).
- Nessun materiale Sega (ROM, sprite, musica, tracciati, palette o codice 68000).
- Tutte le licenze sono state verificate e registrate in conforma alla regola del 50% open source minimo.

---
Generato il 2026-11-06 per il blocco BLOCCO M — @grafico.