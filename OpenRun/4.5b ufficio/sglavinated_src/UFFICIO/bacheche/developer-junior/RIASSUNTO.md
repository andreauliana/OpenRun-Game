# RIASSUNTO @developer-junior
Riscritto 09/11 10:15 (turno MIG1 J2/J3/J4). Lavoro corrente: BLOCCO MIG1.
- Ruolo: Developer Junior; test in `motore_nuovo/test/`, gate in `UFFICIO/Officina/TOOL/`.
- Gate baseline noto: `gate_t1.sh` exit 0 (09/11 09:22, dev D2), 6/6. Questo turno: build exit 0, gate_t1.sh non eseguito.
- PERC1 [x]. MIG1 J1 [x]. D1-D3 [x] (report/codice). D4 [ ], D5 facoltativo.
- J2 [~]: `prova_gate ... mig1_j2_pali lv_01_neon pali` exit 27 (09/11 10:15) — FALLITO: hit_rilevati=0, frame_primo_hit=-1, screenshot=0; test nota clamp baseY<=208 (D1). Serve indagine setup/posizionamento paletti o D1 runtime.
- J3 [~]: `prova_gate ... mig1_j3_sportellata lv_01_neon sportellata` exit 0 (09/11 10:15) — GATE SPORTELLATA OK: colpo 0.52s frame 31, urti_gara=8, ripresa_proxy Z 0-2s=30168 2-4s=10325 4-7s=-40493 (proxy, `sbandataS` privato).
- J4 [~]: `UFFICIO/out/developer-junior/REVIEW_MIG1.md` aggiornato con esiti 10:15.
- Cmd eseguiti (09/11 10:15): `cmake --build "UFFICIO/Officina/build" --target prova_gate` exit 0; run `pali` exit 27; run `sportellata` exit 0.
- Next: chiarire J2 FALLITO (nessun hit/screenshot) con @developer/D1 o affinare modo `pali`; confermare ripresa 5s D2 con strumentazione se serve.
- Mai: HUD, `gate_t1.sh` se non richiesto, `salvaScreenshotP1`, `motore_nuovo/src/` senza autorizzazione.