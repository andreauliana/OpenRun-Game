# REVIEW FORK1 — revisore @developer — 26/10/2026 07:20

Blocco: FORK1 [~] (RICHIESTA_APERTA.md). Compito assegnato: review incrociata (1 turno) delle gate 4 (`bivio_uscita`), 5 (`bivio_bordi`), 7 (`tema_transizione`) di `motore_nuovo/test/prova_gate.cpp`. Esito in `out/developer/REVIEW_FORK1_<data>.md`. L'autore del codice non e' il revisore: il codice e' di @developer-junior (commit b6eba6f e precedenti); io verifico diff, requisiti, test, log e regressioni.

## File esaminati (sola lettura, nessuna modifica)
- `motore_nuovo/test/prova_gate.cpp`: `verificaBivioUscita` (gate 4), `verificaBivioBordi` (gate 5), `verificaTemaTransizione` (gate 7).
- `motore_nuovo/src/game/game.cpp`: `fineTappa` (raccordo curva/collina dall'ultimo segmento precedente + `iniziaTransizioneTema`), corsia a scelta aperta = ramo piu' vicino (`centroCorsia = carr.centro[vicino]`, niente centro fisso a 0), `assistenzaBivio_` (spinta di curva attenuata `(1.0f - assistenzaBivio_)`, richiamo morbido max `ASSIST_RICHIAMO_MAX` unita'/s, offroad/conteggio ribaltamento sospesi durante assistenza), ripartenza post-incidente su `playerXIncidente_` (ramo piu' vicino, non spartitraffico).
- `motore_nuovo/src/render/renderer.cpp`: `iniziaTransizioneTema` fissa `temaDa_` (tappa che finisce) e `temaA_` (tema di arrivo da `impostaTema`, resta fisso); `aggiornaTransizioneTema` interpola `cielo`, `erba`, `erbaAlt`, `rumbleAlt` da `temaDa_` a `temaA_` in `transizioneDurataS_` secondi; nessuna interpolazione verso `tema_` sovrascritto a ogni frame.
- `motore_nuovo/src/road/road.h`: `BIVIO_SEG` 1200 (raggio apertura bivio piu' dolce).
- `motore_nuovo/dati/tracciati/regole.json`: blocchi "bivio" (finestra +-60, delta_larghezza_max 400, delta_centro_max 400, delta_x_rel_max 40, delta_curva_max 0.05, delta_collina_max 0.05), "tema_transizione" (delta_canale_max_per_frame 12), "bivio_bordi" (offroad_max_consecutivi 60).
- `motore_nuovo/CMakeLists.txt`: 3 gate FORK1 registrati in ctest (`gate_bivio_uscita`, `gate_bivio_bordi`, `gate_tema_transizione`).
- `motore_nuovo/dati/config.json`: `transizione_tema_s`: 2.0.

## Confronto requisiti (RICHIESTA_APERTA.md FORK1) vs implementazione
Punto 4 (gate `bivio_uscita`): 4 combinazioni SS/SD/DS/DD, guida fino al primo cambio tappa, finestra +-finestra attorno al cambio con larghezza/centro/x_rel/curva/collina, fallisce se scarto tra frame consecutivi > soglie di regole.json, screenshot PPM (5 prima, cambio, 5 dopo). OK: `campionaBivio` usa centro = ramo piu' vicino anche a scelta aperta (no salto fittizio di BIVIO_DIST); `verificaBivioUscita` corregge il centro al blocco scelta con riferimento lato*BIVIO_DIST; soglie lette da regole.json, non abbassate.
Punto 5 (gate `bivio_bordi`): sterzo verso ramo scelto dentro e fuori dal bivio sul livello piu' curvo (lv_03_foglie), arriva al cambio tappa, nessun ribaltamento, offroad consecutivi <= N=60. OK: obiettivo = centro del ramo scelto, seconda scelta dello script rispettata, crash se speed < 1 per >60 frame dentro il bivio.
Punto 7 (gate `tema_transizione`): tema interpolato gradualmente in `transizione_tema_s` (config.json, 2.0 s), cielo campionato sopra l'orizzonte, variazione per canale <= 12/frame, cielo finale != iniziale, transizione > 1 frame ed entro il doppio della durata attesa. OK.

## Test e log (esecuzioni gia' avvenute, non ripetute in questo turno)
- `sh UFFICIO/Officina/TOOL/gate_t1.sh` (26/10/2026 07:00): exit 0, GATE T1: TUTTO OK, 6/6 percorsi OK (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%), ms/frame medio 0.20 max 0.95 (limite 20), replay 2x IDENTICO, gara 4 combinazioni OK, frequenza 60/50 Hz OK, screenshot P1 OK. Log: `UFFICIO/Officina/Test area/gate_t1.log`.
- `sh UFFICIO/Officina/TOOL/run_tests.sh` (26/10/2026 07:00): ctest 20/20 verde, inclusi `gate_bivio_uscita`, `gate_bivio_bordi`, `gate_tema_transizione`.
- `UFFICIO/Officina/build/prova_gate ... lv_01_neon bivio_uscita` (26/10/2026 07:14, revisore junior): exit 0, `GATE BIVIO USCITA OK` sulle 4 combinazioni; maxDelta larghezza 16.0, centro 6.3, x_rel 12.7, curva 0.00, collina 0.00 (soglie 400/400/40/0.05/0.05).
- `gate_bivio_bordi` exit 0 (25/10 08:46), `gate_tema_transizione` exit 0 (25/10 07:47): invariati, confermati da ctest 20/20 di oggi.

## Regressioni
- Determinismo invariato: replay 2x IDENTICO su smoke/soak/gara; `impostaFrequenzaDaConfig` riscrive solo `frequenza_hz` e non cancella più `transizione_tema_s` (regressione corretta in b6eba6f).
- Soglie gate non abbassate: `regole.json` default (rettilineo 25-75%, curva_max 1.0, dx/sx >= 0.3, fuori_strada_max 0.01) coerenti con lv_02_atomico di riferimento (ANDREA_REVIEW T1-Q5).
- Nessun file `src/runner/` toccato; core senza SDL; determinismo a 50/60 Hz preservato (ANDREA_REVIEW nota frequenza 19/10).

## Esito review
REVISIONE POSITIVA: gate 4, 5, 7 di `prova_gate.cpp` soddisfano i vincoli FORK1 punti 4, 5, 7 di RICHIESTA_APERTA.md; codice coerente con `game.cpp` (corsia a scelta aperta) e `renderer.cpp` (transizione tema graduale); test verdi e log coerenti; nessun rilievo bloccante.

## Rilievi / prossimi passi
- Le soglie "bivio" in `regole.json` sono proposte da validare in tuning con @developer (non abbassare per far passare i gate).
- Mancante per chiusura FORK1: @capo apre CP-007 quando entrambe le review (junior fatta alle 07:14, mia appena registrata) sono positive. Elemento di passaggio al bivio @grafico: facoltativo, non blocca.
- In coda dopo FORK1: DECO1 [ ] (in attesa @grafico/G1: raw skyline e oggetti bordo strada 6 livelli), poi RIV [ ], R2 [ ]; T1-RUN [~] e H [~] in attesa Andrea (D19=B: ok runner Mac v91 mancante, poi fps/temperatura Pi 400); nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Mac.
- gate non eseguito in questo turno: esiti citati dai log sopra (ultimo gate `gate_t1.sh` 26/10/2026 07:00 exit 0).
