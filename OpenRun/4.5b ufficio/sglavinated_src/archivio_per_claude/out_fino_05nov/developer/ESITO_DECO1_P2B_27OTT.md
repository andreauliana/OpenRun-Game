# Esito DECO1 passo 2b — 27/10/2026 06:37 — @developer

## Blocco
DECO1 passo 2b: skyline sopra l'orizzonte per tutti e 6 i livelli, letto da `tema_grafico.skyline` di ogni `livello.json`, con cambio a ogni tappa (no scatto visivo).

## Modifiche
- `motore_nuovo/src/game/game.cpp`: `caricaLivello` carica `dati/sprite_raw/scenari/<livello>/skyline.sglraw` a ogni carica tappa (init e fineTappa); `fineTappa` fotografa lo skyline prima del cambio e avvia `iniziaTransizioneTema` con lo skyline della tappa che finisce.
- `motore_nuovo/src/render/renderer.{h,cpp}`: `impostaSkyline` mantiene `skylineSprite_` + `skylineA_`; `iniziaTransizioneTema` accetta lo skyline di partenza; nuovo `drawSkylineTransizione` crossfade da->a sincronizzato con l'interpolazione di `tema_` (per-frame delta <= canale soglia); fine transizione fissa lo skyline della tappa nuova.
- `motore_nuovo/src/runner/main.cpp`: rimosso pre-caricamento hardcoded di lv_01_neon (ridondante: il core carica per livello).
- `motore_nuovo/test/prova_gate.cpp`: `caricaSkyline` carica per ogni livello; `verificaDecorazioni` controlla lo skyline sopra l'orizzonte su tutti e 6 i livelli.

## Gate eseguiti (questo turno)
- `sh UFFICIO/Officina/TOOL/gate_t1.sh` — exit 0: GATE T1: TUTTO OK. 6/6 smoke + percorso OK, gara 4 combinazioni replay IDENTICO, frequenza 60/50 Hz OK, ms/frame medio 0.28-0.56 (limite 20).
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — exit 0: ctest 21/21 verde, inclusi `gate_tema_transizione` (maxDeltaCanale 2.0 <= 12, transizione graduale 151 frame) e `gate_decorazioni` (skyline disegnato su tutti e 6 i livelli, catalogo risolto, ms/frame <= 20).

## Residui
- CP-007 (@capo, review FORK1), elemento passaggio bivio (@grafico, facoltativo, non blocca).
- T1-RUN [~] e H [~] in attesa Andrea (ok runner Mac v91, poi Pi 400).
- DECO1 passi 2c-2e post-2b: oggetti bordo strada, densita' cartelloni 20-30%, parallasse.
