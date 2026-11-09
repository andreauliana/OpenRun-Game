# REVIEW_V47 — @developer-junior (riscritto 09/11 11:31)
Blocco V47 (RICHIESTA_APERTA). gate_t1.sh non eseguito in questo turno.

## VJ1 — dopo ogni passo V
- **V1** (scia + sorpasso al pelo, @developer) [09/11 11:20]: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj1_v1" lv_01_neon gara` exit 0, `GATE GARA OK` (4 bivi, arrivo/posizioni/incidenti/sovrapposizioni OK, replay IDENTICO).
- **V3** (fluidità/ms-frame, @developer) [09/11 11:20]: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj1_v3" lv_04_cromo smoke` exit 0, `GATE OK` (ms/frame medio 1.367 max 4.926 limite 20, replay IDENTICO).
- **V4** (dissolvenza 0.5s fasi, @developer) [09/11 11:20]: `ctest --test-dir UFFICIO/Officina/build -R gate_ciclo --output-on-failure` exit 0 (1/1 Passed, 13.48s).
- **V5** (testi inglese semplice, @developer [~] 09/11 11:23: fasi.cpp ok, frasi.json bloccato a @grafico) [review 09/11 11:31]: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj1_v5" lv_01_neon smoke` exit 0, `GATE OK` (500 frame, ms/frame medio 0.954 max 3.225, replay IDENTICO). Nessuna regressione rilevata; traduzione `frasi.json` resta fuori perimetro developer-junior.

## VJ2 [x] — Ponte e riva (09/11 11:08)
- Modo `vj2_riva` in `motore_nuovo/test/prova_gate.cpp`, build `prova_gate` exit 0.
- `lv_01_neon` exit 0 (1941f, fuori 0.0%, 9 screenshot, 2558 oggetti), `lv_04_cromo` exit 0 (2068f, 0.0%, 9, 2696), `lv_06_desktop` exit 0 (2069f, 0.0%, 9, 3148). Dir `UFFICIO/Officina/Test area/vj2_riva/`. Densità: PPM per review visiva Andrea; **Andrea decide**.