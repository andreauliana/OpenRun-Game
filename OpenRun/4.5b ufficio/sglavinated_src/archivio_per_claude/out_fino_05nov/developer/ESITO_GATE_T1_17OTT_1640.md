# Esito Gate T1 — 17 ott 2026 16:40 — @debugger

Comando: `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak` (singoli, regola 26).
Esito: **GATE T1: TUTTO OK, exit 0**. Log completo: `out/debugger/gate_t1.log`.

## Smoke — 3 livelli (500 frame ciascuno, replay 2x)

| Livello | speed@60 | offroad (frame) | ms/frame medio | ms/frame max | replay 2x | Avvolgimento | Inattività |
|---|---|---|---|---|---|---|---|
| lv_01_neon | 33.76 | 476 | 0.071 | 0.109 | IDENTICO | wrapCount=4, len~56398, IDENTICO | bolla 4655 (>=100), IDENTICO |
| lv_02_atomico | 33.76 | 476 | 0.071 | 0.139 | IDENTICO | wrapCount=4, len~57599, IDENTICO | bolla 4655, IDENTICO |
| lv_03_foglie | 33.76 | 475 | 0.072 | 0.157 | IDENTICO | wrapCount=4, len~55496, IDENTICO | bolla 4655, IDENTICO |

Limite ms/frame: 20 (obiettivo 12). Tutti i livelli ampiamente sotto soglia.
Casi estesi T1-RUN punto 5 (già in `prova_gate.cpp`): avvolgimento playerZ oltre totalLen_ (wrapCount=4 su tutti i livelli, nessuna discesa intermedia) e inattività 900 frame (show/fumetto visibile: bolla blu 4655 px, personaggio 6497 px), replay deterministico in entrambi.

## Soak — 30000 frame lv_01_neon

speed@60=33.76, X[min,max]=[-164.8,900.0], zFinale=28814.2, offroad raggiunto frame 476,
ms/frame medio=0.071 max=0.713 (limite 20), replay 2x IDENTICO → GATE OK.

## Indice asset (punto 6 T1-RUN)

`python3 motore_nuovo/strumenti_py/indice_assets.py`: 293 file, lacune 4/11 note (traffico/altri veicoli, Gianni, Gesù, UFO).

## Aperti (non eseguibili qui, regole 9/21)

- v91 di Andrea su `sglavinated_runner` (Mac) → FEEDBACK_ANDREA.md (T1-RUN punto 8).
- fps reali e temperatura su Pi 400 (`vcgencmd measure_temp`, `get_throttled`) → chiude H.

Nessun file di prodotto toccato. Nessun comando negato in questo turno.
