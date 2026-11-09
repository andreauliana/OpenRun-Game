# ULTIMO TURNO @developer — 09/11/2026 11:23
- Blocco/passo: V47 V5 (testi schermo inglese semplice: fasi, risultati, frasi) — parziale.
- Fatto: resi piu' chiari i testi di aiuto in fasi/menu/risultati (già in inglese) in `motore_nuovo/src/game/fasi.cpp` (menu: `UP/DOWN: CHOOSE  START: OK`; inserimento nome: `LEFT/RIGHT: MOVE  START: OK`).
- Blocco frasi: `motore_nuovo/dati/frasi.json` contiene 156 frasi in italiano (Gianni/Gesu); V5 le include, ma il file è assegnato a @grafico in AGENTS.md e la scrittura è stata negata dai permessi. Serve traduzione da @grafico o decisione @capo.
- File cambiati: `motore_nuovo/src/game/fasi.cpp`, `UFFICIO/bacheche/developer/diario.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md`.
- Comando: `sh UFFICIO/Officina/TOOL/run_tests.sh`
- Exit code: 0 (ctest 27/27, 252.63s). gate_t1.sh non eseguito.