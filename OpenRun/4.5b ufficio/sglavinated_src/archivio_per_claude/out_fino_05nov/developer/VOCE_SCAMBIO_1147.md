# VOCE_SCAMBIO_1147 — @sviluppatore -> ciclo | 17/10/2026 17:55

## Blocco
T1-RUN punto 4 / P1 punto 4 — sprite veri nel gate headless (cartelloni sponsor negli screenshot del gate). Stato proposto: [~] (non chiuso).

## Fatto in questo turno
- `motore_nuovo/src/render/sprite_raw.h` e `sprite_raw.cpp` (SPDX MIT): caricatore raw per il core ("SGLR" + u32 LE larghezza + u32 LE altezza + RGBA8888, quantizzazione a palette max 256 colori con indice 0 trasparente, identica al decoder del runner sponsor_loader.cpp). Zero dipendenze esterne: solo <cstdio>/<cstdint>.
- `motore_nuovo/CMakeLists.txt`: sprite_raw.h/.cpp aggiunti a `sglavinated_core`; il target compila (gate build OK).
- `out/sviluppatore/png_a_raw.py` (SPDX MIT): convertitore PNG -> raw, solo libreria standard Python 3 (zlib), supporta PNG 8-bit RGB/RGBA senza interlaccio e filtri 0-4. Legge `assets_originali/sponsor/<id>/cartellone_<stile>.png` (7 sponsor x 3 stili neon/atomico/foglie, originali del progetto) e scrive `motore_nuovo/dati/sprite_raw/sponsor/<id>_<stile>.sglraw`.
- `sh tools/gate_t1.sh`: exit 0, `GATE T1: TUTTO OK` sui 3 livelli. speed@60=33.76, ms/frame medio 0.076 (max 0.189, limite 20), replay 2x IDENTICO, offroad raggiunto, avvolgimento e inattivita OK. Log in `out/collaudatore/gate_t1.log`.

## Mancante (esterno al mio turno)
1. I 21 file `.sglraw` non sono stati generati: `png_a_raw.py` richiede python3, che in questo ambiente shell non e' eseguibile (permission denied). Da generare non appena python3 e' disponibile.
2. `motore_nuovo/test/prova_gate.cpp` e' di @collaudatore (regola "un foglio, uno scrittore"): va modificato per chiamare `game.setSponsorSprites(...)` con i sprite caricati da `caricaSpriteRaw` PRIMA di `game.init()`, quindi rigenerare i 3 screenshot (partenza/curva/fuori strada) con cartelloni veri, come richiesto da FEEDBACK_ANDREA v90 (gate T1).
3. `v91` di Andrea (runner SDL2 su Mac, fps/temperatura su Pi 400): T1-RUN punto 8, in capo ad Andrea.

## Prossimo
Generare i `.sglraw`, integrare `setSponsorSprites` in `prova_gate` (@collaudatore), rilanciare `sh tools/gate_t1.sh` e attendere v91 di Andrea per chiudere T1-RUN.
