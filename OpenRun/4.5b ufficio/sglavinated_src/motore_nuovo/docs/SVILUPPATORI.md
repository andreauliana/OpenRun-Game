# Guida per sviluppatori (18 ott)

Tutti i comandi partono da `sglavinated_src/`, senza pipe (i permessi confrontano il comando intero).

## Build, test, review
| Cosa | Comando |
|---|---|
| Gate completo (build + tutti i test) | `sh UFFICIO/Officina/TOOL/gate_t1.sh` (log: `UFFICIO/Officina/Test area/gate_t1.log`) |
| CTest (17 test) | `sh UFFICIO/Officina/TOOL/run_tests.sh` |
| Un solo test | `./UFFICIO/Officina/build/prova_gate <out> <livello> <modo>`; modi: smoke, percorso, gara, curve, sterzo, crash, accelerazione, screenshot, soak |
| Runner SDL | `cmake --build UFFICIO/Officina/build --target sglavinated_runner`, poi `sh UFFICIO/Officina/TOOL/check_sdl_init.sh`; `--livello lv_03_foglie` per una pista sola, senza = gara |
| Review | `git status`, `git diff` (sola lettura) |

Il checkpoint di `strumenti/ciclo.sh` lancia `UFFICIO/Officina/TOOL/gate_t1.sh`: se è rosso la review del modello parte e manda la correzione a @developer.

## Piste e percorsi
- `dati/livelli/<id>/livello.json`: `segmenti` (curva, collina, lunghezza in segmenti da 200 unità), `colori`, `tema_grafico`, `cartelloni`, `tempo_limite_s`. Formato: `docs/FORMATI_DATI.md`, tabella piste: `docs/LIVELLI.md`.
- Regole del gate percorso: rettilinei 25-75%, un rettilineo ≥300 segmenti, curve a sinistra e a destra, giro guidato con fuori strada ≤1%.

## Tragitti (gara a tappe)
- `dati/percorso_gara.json`: piramide lv_01 → lv_02/lv_03 → lv_04/05/06 (arrivi). Il bivio occupa gli ultimi 800 segmenti di ogni tappa (`Road::BIVIO_*`).
- Tempo: `tempo_limite_s` della prima tappa, poi ogni tappa nuova aggiunge il suo (checkpoint). A zero: TEMPO SCADUTO, l'auto va per inerzia.

## Altri giocatori
- 7 avversari (`Game::inizializzaRivali/aggiornaRivali`): griglia davanti, elastico 0.85/1.05 oltre 15000 unità, cambio corsia, niente tamponamenti.

## HUD
- `dati/ui/hud.json` (variante B, bianco e nero): posizioni ed elementi; font `render/font_hud.h` generato da `strumenti_py/font_a_header.py`.

## RetroPie (libretro)
- `src/libretro/sglavinated_libretro.cpp` avvia la gara. Il core NON compila ancora: manca `libretro.h` (`-DBUILD_LIBRETRO=ON` dà errore). Prossimo passo: aggiungere l'header ufficiale (licenza MIT) in `libreria/libretro/` e collegarlo in CMake.

## Da fare nel motore
- Skyline e oggetti di bordo strada: sono nel catalogo (`assets_originali/CATALOGO_GRAFICO.json`, `tema_grafico` del livello) ma il renderer non li disegna ancora.
