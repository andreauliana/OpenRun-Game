# Ultimo turno @developer — 09/11/2026 (turno 09:22)
- Blocco/passo: MIG1 D2 [x] — sportellata efficace (Andrea 08/11).
- File cambiati: `motore_nuovo/src/game/game.h`, `motore_nuovo/src/game/game.cpp`.
- Implementato: `Rivale::sbandataS` (recupero ~5 s). In `urto()` lato rivale imposta `sbandataS=5` (solo colpo laterale). In `aggiornaRivali` decadimento sbandata e `passo *= 1 - 0.12*(sbandataS/5)`. Fianco-a-fianco (dz tra URTO_Z_MIN e URTO_Z_DAVANTI-50) ora usa `spritesFonte_`; se sterzo rapido verso auto affiancata (`dx*sterzo>0`, `dx*dSterzo>0`, `|dSterzo|>0.22`, `sterzoPrec_`) chiama `urto(fonte, dx, false)`; altrimenti separa come prima. `sterzoPrec_` aggiornato a fine blocco.
- Costanti aperte usate/presenti: `AFFIANCATA_DX` (650), `URTO_SPINTA_RIVALE` (game.cpp), `RIVALE_SORPASSO_DX` (360, già usato in sorpasso rivali).
- Comando 1: `sh UFFICIO/Officina/TOOL/run_tests.sh` — exit 0, ctest 27/27 OK.
- Comando 2: `sh UFFICIO/Officina/TOOL/gate_t1.sh` — exit 0 (questo turno). GATE T1 TUTTO OK, 6/6 percorsi, replay IDENTICI, ms/frame < 20, screenshot P1 OK.
- Next: MIG1 D3 rivali con carattere (seme fisso deterministico).