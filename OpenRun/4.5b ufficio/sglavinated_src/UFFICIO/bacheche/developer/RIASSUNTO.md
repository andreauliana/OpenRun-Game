# Riassunto developer — 09/11/2026 (turno 10:00)
- MIG1 D1 [x] paletti in coordinate mondo, atterrano sull'erba a fianco strada, scorrono via; base >= riga ruote. File: `game.h/cpp`, `render/renderer.h/cpp`.
- MIG1 D2 [x] sportellata efficace (dSterzo>0.22 in affiancata) chiama urto laterale; rivale colpito `sbandataS=5s`, recupero graduale; danni/KO invariati. Costanti aperte: `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`.
- MIG1 D3 [x] rivali con carattere deterministico (seme fisso): aggressivo/prudente/vendicativo/sbaglia-curva + 3 normali; comportamenti in `aggiornaRivali`/`urto`, movimento liscio. File: `game.h`, `game.cpp`.
- MIG1 D4 [x] `NOMI_RIVALI` (7, max 6 MAIUSC, culture/temi) in `motore_nuovo/src/game/game.cpp`: BOOGIE(USA/lv_01 neon), SPUTN(RU/lv_02 atomico), RASTA(JM/lv_03 foglie), AKIRA(JP/lv_04 cromo), DJEMBE(WA/lv_05 spray), PINGU(meme-OS/lv_06 desktop), CHIDO(MX/wildcard). Dettaglio in `ULTIMO_TURNO.md`.
- Verifica 09/11 10:00: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 11 (build KO `prova_gate.cpp:2136` usa `!game.init()` ma init è void; pre-esistente, non modificato; `sglavinated_core` compilato OK). gate non eseguito.
- Prossimo @developer: MIG1 D5 (facoltativo, audio extra NUOVI) poi V47 V1 (scia/sorpasso al pelo, solo aggiunte), V3, V4, V5.
- Aperti [~]: MIG1 D5, V47 V1,V3-V5, DECO1, ENG1+ARC1. H pendente Andrea. Blocco: prova_gate.cpp:2136 impedisce ctest/gate.
- Perimetri: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi bloccati; solo costanti MIG1 aperte.
