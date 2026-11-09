# Riassunto developer — 09/11/2026 (turno 09:22)
- MIG1 D1 [x] paletti in coordinate mondo, atterrano sull'erba a fianco strada, scorrono via; base >= riga ruote, clipping ok. File: `game.h/cpp`, `render/renderer.h/cpp`.
- MIG1 D2 [x] sportellata efficace: fianco-a-fianco rileva sterzo rapido verso auto affiancata (`sterzoPrec_`, soglia dSterzo 0.22) e chiama `urto(..., false)`. Rivale colpito: `Rivale::sbandataS=5s`, recupero graduale in `aggiornaRivali` (`passo *= 1 - 0.12*k`), memoria danni/KO invariata (-3%/botta, KO 4a). Costanti aperte già presenti: `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`.
- File D2: `motore_nuovo/src/game/game.h`, `motore_nuovo/src/game/game.cpp`.
- Verifica questo turno: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 27/27. `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK, replay IDENTICI, ms/frame <= limite 20, screenshot P1 OK.
- Prossimo @developer: MIG1 D3 rivali con carattere (seme fisso, deterministico; aggressivo/prudente/vendicativo/sbaglia in curva; mai sovrapposti/teletrasporti).
- Aperti [~]: MIG1 D3-D5, V47 V1,V3-V5, DECO1, ENG1+ARC1. H pendente Andrea.
- Perimetri: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi bloccati; solo costanti MIG1 aperte. Gate non rieseguito oltre a quanto sopra.