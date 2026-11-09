# Riassunto developer — 09/11/2026 (turno 10:20)
- MIG1 D1-D4 [x] paletti mondo+erba (game/renderer), sportellata efficace+sbandata 5s (costanti aperte), caratteri rivali deterministici, NOMI_RIVALI 7 culture/temi. File: `game.h/cpp`, `render/renderer.h/cpp`.
- MIG1 D5 [x] audio nuovo `suonaPaletto`/`sfxPaletto` (mixer.h/cpp), chiamato in `abbattiPali` in aggiunta a `suonaUrto` (effetti esistenti invariati).
- V47 V1 [x] gia' nel codice e verificato: scia <1500 stessa corsia 1s (+velocita' graduale, cap 1.02*max), sorpasso al pelo dz<2000 dx<350 poi superato (+0.5s, `suonaSorpasso`). Solo aggiunte, sterzo/curve/derapata/incidenti invariati.
- Verifica 09/11 10:20: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 27/27 (282.52s, prova_gate compila). gate_t1.sh non eseguito questo turno.
- Prossimo @developer: V47 V3 (misura ms/frame 6 livelli gate `percorso`, alleggerire 3 punti lenti senza cambiare resa), V4 (dissolvenza 0.5s risultati/menu/partenza), V5 (testi schermo inglese semplice).
- Aperti [~]/[ ]: V47 V3-V5, DECO1, ENG1+ARC1. H pendente Andrea.
- Perimetri: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi bloccati; solo costanti MIG1 aperte. `gate_t1.sh` exit 0 richiesto dopo passi codice/dati (non eseguito ora: ctest 27/27 verde).