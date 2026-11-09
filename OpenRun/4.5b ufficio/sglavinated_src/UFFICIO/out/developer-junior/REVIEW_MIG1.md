# REVIEW_MIG1 — @developer-junior (J4, riscritto 09/11 10:49)
Fonti: `RICHIESTA_APERTA.md` MIG1, `UFFICIO/out/developer/ULTIMO_TURNO.md`/stato repo, `UFFICIO/Officina/STATUS_OFFICINA.md` (storico), prove locali 09/11 10:15 e 10:45. Gate_t1.sh non eseguito in questo turno.

## Cosa va
- **D1 [x]** (letto): `abbattiPali` in coord. mondo; detriti proiettati con `projs_`, clamp `baseY <= 208.0f` (mai sotto riga ruote/area visibile). **J2 [x]**: prova `pali` migliorata in `motore_nuovo/test/prova_gate.cpp` (sterzo controllato), build `prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/mig1_j2_test4" lv_01_neon pali` exit 0 (09/11 10:45), hit_rilevati=17, screenshot=3 in `UFFICIO/Officina/Test area/mig1_j2_test4/mig1_j2_pali/`.
- **D2 [x]** (letto): colpo sterzo rapido affiancato (`dSterzo>0.22`, `AFFIANCATA_DX`), `Rivale::sbandataS=5s`, recupero graduale, memoria danni/KO mantenuta.
- **D3 [x]** (letto): `CarattereRivale` seed fisso deterministico (aggressivo/prudente/vendicativo/sbaglia-curva), guardie anti-tamponamento/sovrapposizione.
- **J3 [~]**: `sportellata` exit 0 (09/11 10:15), `GATE SPORTELLATA OK`, colpo_a_segno=SI a 0.52s (frame 31), urti_gara=8. Proxy ripresa Z: 0-2s=30168, 2-4s=10325, 4-7s=-40493 (inversione coerente con recupero, non prova diretta).
- **J1 [x], J4 [x]**: review aggiornata al passo corrente.

## Cosa non va / limiti
- **J3**: recupero pieno `sbandataS` ~5s non misurato direttamente (`sbandataS` privato). Serve strumentazione/API dedicata o estensione prova per evidenza forte.
- **Nota D1**: rotazione estrema del detrito può eccedere `baseY` di ~1px; da confermare visivamente sugli screenshot J2 se critico.
- **Aperti MIG1**: D4 nomi 7 rivali [ ]; D5 audio extra facoltativo [ ]; G2 screenshot/3 osservazioni dopo D1-D3 [ ].

## Miglioria piccola
- Estendere modo `sportellata`/supporto test per esporre o campionare stato recupero rivale (sbandata residua o metrica pubblica), mantenendo determinismo e senza toccare gameplay legacy.