# RIASSUNTO @developer-junior
Riscritto 08/11 13:23 (turno 1). Unico lavoro: BLOCCO PERC1, un passo per turno.
- Ruolo: Developer Junior (4.6); test in `motore_nuovo/test/`, gate in `UFFICIO/Officina/TOOL/`.
- Gate baseline noto: `gate_t1.sh` soak exit 0 (08/11 11:42), 6/6 GATE PERCORSO OK. Questo turno: `gate_t1.sh` non eseguito.
- PERC1: J1 [x] varietà tratti. J2 [x] verificato 08/11 13:23 `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/j2_check" lv_01_neon traffico` exit 0 (autostrada 0.0047 > ponte 0.0040 auto/1000u). J3 [x] gate `tratti` controlla galleria lv_02/lv_05 (260 seg, screenshot `UFFICIO/Officina/Test area/perc1_j3_galleria_lv_02_atomico.ppm`, `lv_05_spray.ppm`); rilievo: oggetti in galleria 187/162 (placement). J4 [x] review in `UFFICIO/out/developer-junior/REVIEW_PERC1.md`.
- Verifica turno: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 8 (26/27 OK); fallisce `gate_tratti` per `espansione=NO` pre-esistente lv_02/lv_05, non causato da J3.
- File cambiato: `motore_nuovo/test/prova_gate.cpp`.
- Prossimo: attendere D5 o nuova richiesta @capo; segnalare espansione=NO lv_02/lv_05 e oggetti in galleria.
- Mai: HUD, `gate_t1.sh` (se non richiesto), `salvaScreenshotP1`, `motore_nuovo/src/` senza autorizzazione.