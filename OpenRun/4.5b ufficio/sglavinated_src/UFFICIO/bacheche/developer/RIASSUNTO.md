# Riassunto developer — 09/11/2026 (turno 11:23)
- MIG1 D1-D5 [x], V47 V1,V3,V4 [x]. V47 V5 [~]: testi fasi/risultati/menu chiariti in inglese semplice (`motore_nuovo/src/game/fasi.cpp`: aiuti menu/nome). **Blocco:** `motore_nuovo/dati/frasi.json` (156 frasi, oggi in italiano) e' assegnato a @grafico (AGENTS.md) e write negato; traduzione frasi richiede @grafico o via capo.
- V47 V3 ms/frame smoke 6 livelli (10:34): lv_01 1.062, lv_02 0.988, lv_03 1.052, lv_04 1.427, lv_05 1.229, lv_06 1.110 (max 3.385-4.037).
- Verifica 09/11 11:23: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 27/27. gate_t1.sh non eseguito in questo turno.
- Prossimo @developer: sbloccare V47 V5 frasi (coordinare con @capo/@grafico) oppure passare a DECO1/ENG1+ARC1 se aperti e indipendenti.
- Perimetri: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi bloccati; solo costanti MIG1 aperte. gate_t1.sh exit 0 dopo codice/dati.