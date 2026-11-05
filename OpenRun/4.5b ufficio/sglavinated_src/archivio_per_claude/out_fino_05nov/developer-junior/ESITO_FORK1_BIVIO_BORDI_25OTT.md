# Esito FORK1 punto 5 `bivio_bordi` — @developer-junior — 25/10/2026 08:46

## Blocco
FORK1 [ ] — bordo strada attraverso il bivio sul livello più curvo (Andrea 22 ott, RICHIESTA_APERTA.md). Parte @developer-junior: gate `bivio_bordi` per verificare che l'auto attraversi il bivio sul livello più curvo senza ribaltamento e senza più di N frame consecutivi offroad.

## Modifiche (percorso assegnato al ruolo, nessun src/)
- `motore_nuovo/test/prova_gate.cpp`:
  - Header modo: aggiunto `bivio_bordi` alla lista dei modi.
  - `struct BordoRegole` + `leggiRegoleBordoBivio()`: legge `offroad_max_consecutivi` da `regole.json` ("bivio_bordi"), default 60.
  - `kScelteBordi[] = { "DS", "DD" }`: lv_01_neon -> destra (lv_03_foglie, il più curvo: curva max 1.02) -> uscita sinistra o destra.
  - `guidaVersoRamo()`: gas pieno, sterzo verso il ramo scelto sia dentro che fuori dal bivio; il ramo non scelto non spinge (obiettivo = latoBivio * BIVIO_DIST dopo la scelta).
  - `verificaBivioBordi()`: rigioca deterministico (seed 12345), conta cambi di tappa (2 = arrivo al cambio tappa uscendo dal bivio sul livello più curvo), rileva crash (playerSpeed < 1 per >60 frame dentro il bivio), conta offroad consecutivi, salva screenshot PPM attorno al cambio (cambio, -5..+5).
  - Dispatch `modo == "bivio_bordi"` in `main()` (exit 18 su fallimento).
- `motore_nuovo/dati/tracciati/regole.json`: aggiunta sezione "bivio_bordi" con `offroad_max_consecutivi`: 60 e nota (soglia di allarme a metà del ribaltamento OFFROAD_CRASH_S=2.0 s = 120 frame @ 60 Hz; valori da validare con @developer nel tuning FORK1, non abbassare le soglie del motore).

## Verifica
- Comando di build: `cmake --build UFFICIO/Officina/build --target prova_gate` — exit 0.
- Comando di gate: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/bivio_bordi" lv_01_neon bivio_bordi` — exit 0.
- Esito `bivio_bordi DS`: cambi=2, cambio frame 4027 (lv_05_spray), maxOffroad=33 <= N=60, crash=no, max|xRel|=2800@1817 (escursione reportata, non condizione di fallimento) -> OK.
- Esito `bivio_bordi DD`: stessi valori -> OK.
- Stampa console: "GATE BIVIO BORDI OK" (exit 0).
- Screenshot PPM: UFFICIO/Officina/Test area/bivio_bordi/p_bivio_bordi_DS_* e p_bivio_bordi_DD_* (cambio, prima/dopo 1..5 frame).
- Nessun file `src/` toccato (vincolo: non modificare `src/runner/` né `game.cpp` di @developer).

## Problemi residui
- Lo stop rilevato al frame 435 (speed=0 prima del bivio) è materia RIV (traffico/rivali), ignorato da questo gate come concordato; non blocca FORK1 punto 5.
- FORK1 punto 4 `bivio_uscita` [x] resta exit 16 atteso fino al fix game.cpp (teleport al blocco scelta game.cpp:790).
- D19 (chiusura formale T1-RUN) [APERTA] senza risposta Andrea in ANDREA_REVIEW.md.
- Gate eseguito in questo turno: sì (`bivio_bordi`, exit 0, 25/10 08:46).
