# Riassunto developer — 09/11/2026 08:51 (turno 1)
- MIG1 D1 [x] paletti abbattuti in coordinate mondo: volano di lato, atterrano sull'erba a fianco della strada, restano lì scorrendo via con la strada; base mai sotto la riga delle ruote (clamp ~208 px), mai fuori area (clipping framebuffer).
- File D1: `motore_nuovo/src/game/game.h` (Detrito {t,lato,v,z,x0,x1}, 12 slot), `motore_nuovo/src/game/game.cpp` (abbattiPali imposta z/x mondo, update: resta a terra finché non esce dietro, render: proiezione via Renderer::drawDetriti), `motore_nuovo/src/render/renderer.h` + `renderer.cpp` (DetritoRender + drawDetriti, proiezione identica a drawSprites; saltato in galleria/ponte/doppia).
- Gameplay `abbattiPali` invariato (PALO_X, -3% speed*0.97, rientro, urto audio).
- CTest 09/11/2026 08:51: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, 27/27 OK (gate_tratti ora verde, era KO pre-esistente).
- Gate 09/11/2026 08:51: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK, replay IDENTICI, ms/frame medio 1.07-1.66 (max 10.47 unico spike, limite 20), screenshot P1 OK.
- Prossimo @developer: MIG1 D2 sportellata efficace (solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, recupero rivale ~5 s).
- Blocchi aperti [~]: MIG1 (D2-D5), V47 (V1, V3-V5), H (Pi 400 pendente Andrea), DECO1, ENG1+ARC1.
- Perimeteri: niente toccate a HUD, auto/pose, musica, tracciati, `src/runner/`, `REGOLE_AUTO_GIOCATORE.md`.
- J2 (@developer-junior) può provarare D1 con screenshot dopo questo turno.
