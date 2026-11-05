# Esito FORK1 punto 4 — gate `bivio_uscita` — 23/10 11:10

Autore: @developer-junior. Blocco: FORK1 [ ] (percorso di @developer per i punti 1-3; punti 4-5 di @developer-junior).

## Comandi ed exit code
- `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0.
- `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/bivio_uscita" lv_01_neon bivio_uscita` -> exit 16.

## Cosa è stato aggiunto
- `motore_nuovo/test/prova_gate.cpp`: nuovo modo `bivio_uscita` (FORK1 punto 4): guida la gara su lv_01_neon per le 4 combinazioni di bivi (SS, SD, DS, DD), rileva il blocco della scelta (`latoBivio` 0->+/-1) e il primo cambio tappa (`fineTappa`), registra larghezza strada, centro corsia, `x` dell'auto (relativa alla corsia), curva e collina in finestra +-60 frame attorno agli eventi, fallisce se uno scarto tra frame consecutivi supera le soglie, salva screenshot PPM (blocco, cambio, 5 prima e 5 dopo il cambio).
- `motore_nuovo/dati/tracciati/regole.json`: sezione "bivio" proposta (finestra_frame 60, delta_larghezza_max 400, delta_centro_max 400, delta_x_rel_max 40, delta_curva_max 0.05, delta_collina_max 0.05) da validare con @developer.

## Misure su lv_01_neon
| Combinazione | Blocco scelta | Cambio tappa 1->2 | maxDelta larghezza | maxDelta centro | maxDelta x_rel | maxDelta curva | maxDelta collina |
|---|---|---|---|---|---|---|---|
| SS | 1919 | 2235 | 16.0 | 0.0 | 1800.0 | 0.00 | 0.00 |
| SD | 1919 | 2235 | 16.0 | 0.0 | 1800.0 | 0.00 | 0.00 |
| DS | 1857 | 2055 | 16.0 | 0.0 | 1190.8 | 0.00 | 0.00 |
| DD | 1857 | 2055 | 16.0 | 0.0 | 1190.8 | 0.00 | 0.00 |

Curva/collina a 0 perché il cambio tappa cade dentro un rettilineo. Larghezza a 16.0/frame: ramp atteso (0.6->1.0 in ~1 s), sotto soglia.
x_rel salta 1190-1800 al blocco scelta: teleport di `player.x` di `BIVIO_DIST` in `game.cpp:790` senza continuità/isteresi (FORK1 punti 1-3).

## Esito
GATE FALLITO (exit 16) — atteso: il blocco FORK1 non è ancora tuningato in `src/`. Il gate resta fallente finché `game.cpp` non smussa il teleport al blocco scelta. Screenshot PPM: 48 file in `UFFICIO/Officina/Test area/bivio_uscita/`.

## Regressione
Nessuna: `sh UFFICIO/Officina/TOOL/gate_t1.sh` smoke exit 0 (6/6 percorsi OK, gara 4 combinazioni OK, frequenza 60/50 Hz OK, ms/frame <= 0.6).

## Prossimo
Punto 5 `bivio_bordi` (@developer-junior) dopo il tuning @developer di FORK1 punti 1-3.
