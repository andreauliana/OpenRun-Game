# Riassunto developer — 09/11/2026 (turno 09:39)
- MIG1 D1 [x] paletti in coordinate mondo, atterrano sull'erba a fianco strada, scorrono via; base >= riga ruote, clipping ok. File: `game.h/cpp`, `render/renderer.h/cpp`.
- MIG1 D2 [x] sportellata efficace: sterzo rapido verso auto affiancata (`dSterzo>0.22`) chiama urto laterale; rivale colpito `sbandataS=5s`, recupero graduale; danni/KO invariati (-3%/botta, KO 4a). Costanti aperte: `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`.
- MIG1 D3 [x] rivali con carattere (seme fisso, deterministico, shuffle seed): `CarattereRivale` + `carattere`/`vendettaS` in `game.h`; init assegna aggressivo/prudente/vendicativo/sbaglia-curva + 3 normali; `aggiornaRivali` applica comportamenti (chiude traiettoria, prudente si scosta, vendicativo cerca il giocatore 8s dopo sportellata laterale in `urto`, errore deterministico in curva); obiettivi clampati, movimento liscio, separazione/anti-teletrasporto esistente invariata.
- File D3: `motore_nuovo/src/game/game.h`, `motore_nuovo/src/game/game.cpp`.
- Verifica questo turno: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 27/27. `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK, replay IDENTICI.
- Prossimo @developer: MIG1 D4 nomi dei 7 rivali (`NOMI_RIVALI`, max 6 lettere, culture/temi) con origine/tema in `ULTIMO_TURNO.md`; poi D5 facoltativo, poi V47 V1/V3-V5.
- Aperti [~]: MIG1 D4-D5, V47 V1,V3-V5, DECO1, ENG1+ARC1. H pendente Andrea.
- Perimetri: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi bloccati; solo costanti MIG1 aperte.
