# Inventario grafica v4.7 (riferimento legacy, 16/11/2026)

Questo e' l'elenco di riferimento: in `GRAFICA/` c'e' solo cio' che il gioco usa.
Un asset nuovo entra solo se il gioco lo carica, con PNG in `GRAFICA/` + `.sglraw` in
`motore_nuovo/dati/sprite_raw/` + voce in `GRAFICA/CATALOGO_GRAFICO.json`.

## Dove sono le cose

```
sglavinated_src/
  GRAFICA/                         originali da ridisegnare (stesso nome, stessa misura)
    scenari/<livello>/skyline.png  cielo e citta' del livello
    scenari/<livello>/oggetti/     obj_<nome>.png = oggetti a bordo strada
    scenari/comuni/oggetti/        frecce, galleria, ponte e mare (tutti i livelli)
    sprite/giocatore_pose/         la tua auto (dritta, sx4; 50x30, 17/11)
    sprite/giocatore/              ruote, fumo, luci stop
    sprite/rivali/  traffico/  traffico_incrocio/  pubblico/
    sponsor/<sponsor>/             cartelloni (solo Andrea)
    ui/logo/  ui/font/varianteB/   logo segnaposto, font HUD
    font/born2bsporty/             font dei titoli
    CATALOGO_GRAFICO.json          PNG -> raw
  motore_nuovo/dati/sprite_raw/    i raw che il gioco legge (generati, non a mano)
  motore_nuovo/dati/audio/musica/  la musica (.xm)
  UFFICIO/Dipartimento Grafico/    TOOLS/ (convertitori) + questo inventario
```

Dopo aver ridisegnato un PNG, da `sglavinated_src/`:
- oggetti dei livelli e comuni: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_oggetti_raw.py"`
- pose dell'auto: `python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_pose_auto.py"`
- skyline (con la trama Bayer): `python3 "UFFICIO/Dipartimento Grafico/TOOLS/trama_scenari.py"`
- sponsor, rivali, traffico, pubblico: `python3 motore_nuovo/strumenti_py/png_a_raw.py`

## Come e' stato controllato

Il gioco (core, `prova_gate`) e' stato avviato su tutti e 6 i livelli (smoke, decorazioni, gara,
ciclo, urto, sponsor_raw, percorso, espansione) con un registro di ogni `.sglraw` aperto; per il
resto si sono letti codice, JSON dei livelli e convertitori. Dopo lo spostamento i convertitori
rifatti danno raw identici a prima.

## Cosa usa il gioco (149 raw, ognuno col suo PNG in `GRAFICA/`)

| `sprite_raw/` | N. | File |
|---|---|---|
| `giocatore/pose/` | 15 | auto_dritta, auto_sx4, auto_dx4 (car-custom di Andrea, 52x30, 18/11; niente specchio); auto_sx2/dx2 passaggio di mezzo, auto_<posa>_b capelli al vento, auto_stop_<posa> stop accesi (Claude 18/11 dai pixel di Andrea; sx1, sx3, dx1, dx3, salita, discesa da disegnare) |
| `giocatore/` | 7 | ruota_1..3, fumo_1..3, luci_stop (+ `auto_giocatore.json`, punti per ruote e fumo) |
| `pubblico/` | 10 | pubblico_a/b_su/giu (arrivo); supporto_a/b_relax/parla/esulta (partenza) |
| `rivali/` | 7 | auto_rivale_<colore>_retro |
| `traffico/` | 4 | traffico_*_retro |
| `traffico_incrocio/` | 4 | lato_berlina_gialla/verde, lato_bus_rosso, lato_furgone_bianco |
| `sponsor/` | 21 | 7 sponsor x 3 stili (li ridisegna solo Andrea) |
| `ui/` | 1 | logo_openrun (segnaposto) |
| `scenari/comuni/` | 12 (+1) | barca, boa, faro, scoglio, ombrellone, motoscafo, freccia_curva_dx/sx, freccia_cresta_dx/sx, parete_galleria, luce_galleria; (+ guardrail: disegno di Andrea, oggi non usato) |
| `scenari/lv_01_neon/` | 12 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bidone, cassetta, lampione, palina, palo_neon |
| `scenari/lv_02_atomico/` | 13 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bancone_drive_in, cupola, freccia, panchina_cromata, segnale_atomico, torre_razzo |
| `scenari/lv_03_foglie/` | 12 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, foglia_gigante, fungo_psico, insegna_fumosa, panchina_curva, panchina_fumo |
| `scenari/lv_04_cromo/` | 11 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, guard_rail_cromo, lampione_lama, pilone_cromo, robot_servizio |
| `scenari/lv_05_spray/` | 11 | skyline, albero, cespuglio, ciuffo, edificio_alto/medio/basso, bidone, cassonetto, serranda_spray, transenna |
| `scenari/lv_06_desktop/` | 14 | skyline, albero, albero_fiori, albero_pino, cespuglio, cespuglio_fiori, ciuffo, edificio_alto/medio/basso, case_computer, delfino_cartone, icona_cartella, pali_cavi |

Pose che il gioco cerca ma non ci sono ancora (facoltative): `auto_dx1..4` (si usa lo specchio),
`auto_salita`, `auto_discesa` e i loro `auto_stop_*`.

Altri file tenuti perche' servono: `ui/font/varianteB/font.png` (-> `font_hud.h`, font HUD),
`font/born2bsporty/Born2bSportyV2.ttf` (-> `font_titolo.h`), `scenari/lv_04_cromo/skyline_trama.png`.

## Musica

I progetti tracker sono i `.xm` stessi (FastTracker 2: si aprono con MilkyTracker o OpenMPT),
in `motore_nuovo/dati/audio/musica/`; chi suona dove sta in `motore_nuovo/dati/audio/playlist.json`:
- `Menu.xm` = menu
- `MainRace.xm` = gara (prima e seconda tappa, e corsa singola)
- `Final-Level.xm` = ultima tappa della gara a tappe (Andrea 16/11)
- suoni del motore (non tracker): `motore_nuovo/dati/audio/motore/*.wav`

Nel repo non c'e' mai stato altro formato tracker.

## Tolto il 16/11 (pulizia v4.7)

Per riavere un file: `git checkout f2f3581 -- "<vecchio percorso>"` (f2f3581 = main prima della pulizia;
i vecchi percorsi iniziano con `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/`).

- Auto vecchia degli agenti (7 pose + `auto_rosso_corsa`, `auto_argento_lunare`, `auto_neon_cyan`) e
  `giocatore/auto_giocatore*.sglraw`, con le righe di codice che la caricavano come riserva
  (`game.cpp`, `prova_gate.cpp`, core libretro, `png_a_raw.py`) e `disegna_auto_giocatore.py`.
- Auto varianti D6 (anni 50, futurista, punk), `auto_giocatore_sbandata.png`.
- Rivali visti di lato (7 PNG + 7 raw), parapetto del ponte (PNG + raw).
- Palette e skyline B/C del blocco F (+ `genera_bloccoF.py`).
- Anteprime UI in bianco e nero (menu A/B/C, HUD, risultati, splash, titolo) e font A/C
  (+ `genera_splash.py`, `disegna_risultati.py`).
- Cartelle di scenario senza immagini (varianti B/C, `scenario_lv_*`), la vecchia
  `sglavinated_src/bacheche/`, la `Libreria/` (README, `CATALOGO_RISORSE.md`, `CREDITI.md` vecchio,
  `Shared asset/` vuota dal 18/10), `OGGETTI_LIVELLI.md` (sostituito da questo file).
- Script una tantum del grafico: `aggiorna_catalogo_g1/lv456.py`, `completa_deco1_*.py`,
  `genera_bloccoL.py`, `genera_scenari_lv456.py`, `genera_v47_vg1_quadro.py`,
  `_scrivi_ultimo_turno_vg3_32.py`, `_riepilogo_varianti.json`.
- Spostati: `assets_originali/` -> `GRAFICA/`; `scenari/<livello>/scenario_<tema>_0N_varianteA/*` ->
  `scenari/<livello>/*`; `skyline/varianteA_futurista/` -> `scenari/lv_04_cromo/`.
