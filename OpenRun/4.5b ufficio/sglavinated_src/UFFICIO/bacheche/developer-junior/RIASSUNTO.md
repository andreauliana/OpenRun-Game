# RIASSUNTO @developer-junior
Riscritto 09/11 10:02 (turno MIG1 J3/J4). Lavoro corrente: BLOCCO MIG1.
- Ruolo: Developer Junior; test in `motore_nuovo/test/`, gate in `UFFICIO/Officina/TOOL/`.
- Gate baseline noto: `gate_t1.sh` exit 0 (09/11 09:22, dev D2), 6/6. Questo turno: gate non eseguito; build non eseguita (shell permission.rejected). Binario `prova_gate` 09:44 obsoleto.
- PERC1 [x]. MIG1 J1 [x]. J2 [~]: modo `pali` in `motore_nuovo/test/prova_gate.cpp` (screenshot `mig1_j2_pali/`, clamp riga ruote D1). J3 [~]: modo `sportellata` aggiunto (colpo sterzo rapido affiancato, misura t colpo via `urtiGara()`, proxy ripresa Z 0-2/2-4/4-7s, screenshot `mig1_j3_sportellata/`; nota: `sbandataS` privato).
- J4 [~]: `UFFICIO/out/developer-junior/REVIEW_MIG1.md` aggiornato (D1/D2/D3 [x] confermati codice/report; J2/J3 codice pronti, esecuzione bloccata).
- Cmd esatti negati: `cmake --build "UFFICIO/Officina/build" --target prova_gate` → permission.rejected (09:55, 10:02). `./.../prova_gate ... pali` su binario vecchio → ha eseguito smoke/avvolgimento/inattività (modo assente).
- Next quando shell ok: build `prova_gate`, run `pali` e `sportellata`, riportare exit code/misure/screenshot in REVIEW+ULTIMO_TURNO.
- File turno: `motore_nuovo/test/prova_gate.cpp`, `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `UFFICIO/bacheche/developer-junior/{RIASSUNTO,diario}.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`.
- Mai: HUD, `gate_t1.sh` se non richiesto, `salvaScreenshotP1`, `motore_nuovo/src/` senza autorizzazione.
