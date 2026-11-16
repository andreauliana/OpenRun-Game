# Inventario grafica (16/11/2026)

Controllo fatto su `main` f2f3581. Come:

1. Il gioco (core senza SDL, `prova_gate`) e' stato avviato su tutti e 6 i livelli
   (modi smoke, decorazioni, gara, ciclo, urto, sponsor_raw, percorso, espansione)
   con un registro di ogni `.sglraw` aperto da `caricaSpriteRaw`.
2. Per i file che non risultavano aperti si e' letto il codice che li carica
   (`game.cpp`, `runner/main.cpp`, `sponsor_loader.cpp`, `libretro`), i JSON dei
   livelli e i convertitori (`converti_oggetti_raw.py`, `png_a_raw.py`,
   `font_a_header.py`, `trama_scenari.py`, `CATALOGO_GRAFICO.json`).

Il gioco legge solo i `.sglraw` in `motore_nuovo/dati/sprite_raw/`. I PNG di
`Libreria/assets_originali/` sono gli originali da cui i convertitori fanno i raw
(eccezione: i cartelloni sponsor, che il runner SDL legge anche come PNG).

## Cosa usa il gioco (143 raw, ognuno col suo PNG originale)

| Cartella in `sprite_raw/` | N. | File |
|---|---|---|
| `giocatore/pose/` | 10 | auto_dritta, auto_sx1..sx4, auto_stop_dritta, auto_stop_sx1..sx4 (pose di Andrea; destra = specchio) |
| `giocatore/` | 7 | ruota_1..3, fumo_1..3, luci_stop |
| `pubblico/` | 4 (+6) | pubblico_a/b_su/giu; supporto_a/b_relax/parla/esulta = partenza cinematografica (solo runner) |
| `rivali/` | 7 | auto_rivale_<colore>_retro |
| `traffico/` | 4 | traffico_*_retro |
| `traffico_incrocio/` | 4 | lato_berlina_gialla/verde, lato_bus_rosso, lato_furgone_bianco |
| `sponsor/` | 21 | 7 sponsor x 3 stili (li ridisegna solo Andrea) |
| `ui/` | 1 | logo_openrun (segnaposto) |
| `scenari/comuni/` | 12 | barca, boa, faro, scoglio, ombrellone, motoscafo, freccia_curva_dx/sx, freccia_cresta_dx/sx, parete_galleria, luce_galleria |
| `scenari/lv_01_neon/` | 12 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bidone, cassetta, lampione, palina, palo_neon |
| `scenari/lv_02_atomico/` | 13 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bancone_drive_in, cupola, freccia, panchina_cromata, segnale_atomico, torre_razzo |
| `scenari/lv_03_foglie/` | 12 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, foglia_gigante, fungo_psico, insegna_fumosa, panchina_curva, panchina_fumo |
| `scenari/lv_04_cromo/` | 11 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, guard_rail_cromo, lampione_lama, pilone_cromo, robot_servizio |
| `scenari/lv_05_spray/` | 11 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bidone, cassonetto, serranda_spray, transenna |
| `scenari/lv_06_desktop/` | 14 | skyline, albero, albero_fiori, albero_pino, cespuglio, cespuglio_fiori, ciuffo, edificio_alto/medio/basso, case_computer, delfino_cartone, icona_cartella, pali_cavi |

Pose che il gioco cerca ma non ci sono ancora (va bene, sono facoltative):
`auto_dx1..4` (si usa lo specchio delle sx), `auto_salita`, `auto_discesa` e i loro `auto_stop_*`.

Altri originali tenuti perche' servono a un convertitore:
- `ui/font/varianteB/font.png` -> `font_a_header.py` -> `src/render/font_hud.h` (font HUD compilato).
- `sprite/auto_gt/auto_rosso_corsa.png` -> `png_a_raw.py` -> `giocatore/auto_giocatore.sglraw` (core libretro).
- `skyline/varianteA_futurista/` -> skyline di lv_04_cromo (+ `skyline_trama.png` scritto da `trama_scenari.py`).
- `font/born2bsporty/Born2bSportyV2.ttf` (titoli e numeri HUD).

## Tolti in questo giro (57 file, tutti mai caricati e mai convertiti in un raw usato)

Per riaverli: `git checkout f2f3581 -- "<percorso>"` (f2f3581 = main prima della pulizia).

| Cosa | File | Perche' |
|---|---|---|
| Rivali visti di lato | `sprite/rivali/auto_rivale_<7 colori>_lato.png` + 7 `.sglraw` | nessun codice li carica (si usano solo i `_retro`) |
| Parapetto del ponte | `scenari/comuni/oggetti/parapetto.png` + `.sglraw` | parapetti cancellati da Andrea (ponte aperto) |
| Sbandata auto giocatore | `sprite/rivali/auto_giocatore_sbandata.png` | mai convertito, mai usato |
| Auto varianti D5 | `sprite/auto_gt/auto_argento_lunare.png`, `auto_neon_cyan.png` | mai convertite; l'auto e' quella disegnata da Andrea |
| Auto varianti D6 | `sprite/auto/varianteA/B/C/` (3 PNG + manifest) | gia' "scartato" in catalogo |
| Palette livello 1 (blocco F, ottobre) | `palette/` (3 PNG + json + manifest) | il motore non le legge |
| Skyline B e C (blocco F) | `skyline/varianteB_synthwave/`, `skyline/varianteC_psichedelico/` | nessun livello le usa |
| Anteprime UI bianco e nero (D7) | `ui/menu/` (A,B,C), `ui/hud/`, `ui/risultati/`, `ui/splash/`, `ui/titolo/` | solo immagini di prova; menu, HUD e risultati sono disegnati dal codice |
| Font A e C | `ui/font/varianteA/`, `ui/font/varianteC/` | il gioco usa solo la variante B |

Tolte anche le 21 voci corrispondenti da `CATALOGO_GRAFICO.json`
(`check_catalogo_grafico.py`: 0 errori) e rigenerato `INDICE_ASSET.json/.md`
con `strumenti_py/indice_assets.py`.

## Cartelle tolte (Andrea 16/11: "puoi togliere folder non usate")

Nessuno script, convertitore o codice le leggeva (controllato: `converti_oggetti_raw.py` rifatto = 0 raw cambiati, 0 errori).

- `scenari/lv_01..03/`: le cartelle di scenario senza immagini (`*_varianteB`, `*_varianteC`,
  `scenario_lv_0x_0y`, `scenario_0x_01_varianteA`): solo `manifest.json` e `palette.json` di ottobre.
  Restano solo `scenario_<tema>_0x_varianteA/` (skyline + `oggetti/obj_*.png`).
- `sglavinated_src/bacheche/`: copia vecchia dei diari; le bacheche vere sono in `UFFICIO/bacheche/`.
- `assets_originali/genera_bloccoF.py`: generatore delle palette e skyline B/C tolte.

## Tenuti, ma da decidere (Andrea)

- `scenari/comuni/oggetti/guardrail.png` + `.sglraw`: disegno di Andrea (PR #93), oggi il gioco non lo usa.
- `sprite/auto_gt/auto_giocatore_*.png` (7) + i loro `.sglraw` in `giocatore/`: auto degli agenti,
  il codice le carica solo se manca la `auto_dritta` di Andrea (riserva). Si possono togliere
  insieme a quelle righe di codice in `game.cpp` e `prova_gate.cpp`.
- `motore_nuovo/docs/*.png` (3 immagini di documentazione, fuori dalla grafica del gioco).

## Musica: dove sono i progetti tracker

I progetti audio sono i file `.xm` stessi (formato FastTracker 2, si aprono e si
modificano con MilkyTracker o OpenMPT): non c'e' un file sorgente separato.

`sglavinated_src/motore_nuovo/dati/audio/musica/`
- `beatmastaz.xm` = menu
- `asplajungle4.xm` = gara
- `SepteJungle2.xm` = non usato (tolto dalla playlist da Andrea il 29/10, escluso dalla build web)
- quale brano va dove: `motore_nuovo/dati/audio/playlist.json`
- suoni del motore (non tracker): `motore_nuovo/dati/audio/motore/*.wav`

Nel repo non e' mai esistito altro formato tracker (.mod, .it, .s3m, .xrns...).
Se Andrea ha versioni di lavoro (es. con campioni o progetti di un altro programma), sono solo sul suo computer.

## Proposta: cartelle piu' semplici (spostamenti da scegliere, NON fatti in questa PR)

Le cartelle vuote o inutili sono gia' tolte (sopra). Qui sotto solo gli spostamenti,
che cambiano percorsi e quindi vanno decisi.

Oggi un PNG da ridisegnare sta 9 livelli sotto la radice, per esempio
`OpenRun/4.5b ufficio/sglavinated_src/UFFICIO/Dipartimento Grafico/Libreria/assets_originali/scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_albero.png`.

**Opzione A (consigliata, rischio basso).** Spostare solo gli originali, il motore non cambia:

```
sglavinated_src/
  GRAFICA/                 <- ex Libreria/assets_originali
    auto/                  pose di Andrea + ruote, fumo, stop
    rivali/  traffico/  pubblico/  ui/  font/  sponsor/
    livelli/lv_01_neon/    skyline.png + obj_*.png (senza scenario_*_varianteA/oggetti/)
    livelli/comuni/
  MUSICA/                  <- solo un LEGGIMI che punta a motore_nuovo/dati/audio/musica
  motore_nuovo/            (invariato: il gioco legge da qui)
  UFFICIO/                 (agenti, strumenti, archivio)
```

Da aggiornare: convertitori in `TOOLS/`, `CATALOGO_GRAFICO.json`, `check_catalogo_grafico.py`,
il caricatore PNG degli sponsor nel runner (1 riga), i testi per gli agenti. Una PR, poi si
ricompila e si prova.

**Opzione B (piu' pulita, piu' lavoro).** Come A, e in piu' togliere i livelli
`OpenRun/4.5b ufficio/sglavinated_src/` portando tutto in `OpenRun/`.
Cambiano anche gli script (`comandi/`, `pi/`, `run.sh`, ciclo agenti) e la tua cartella locale:
meglio farla quando gli agenti sono fermi.

**Opzione C.** Lasciare tutto com'e', solo questo inventario.
