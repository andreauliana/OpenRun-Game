# Review incrociata DECO1 punto 2 — @developer-junior rivede @developer — 27/10/2026 19:34

## Blocco
DECO1 [ ] — punto 2 (motore: skyline, oggetti bordo strada, densita cartelloni) di @developer; review di @developer-junior (gate decorazioni e' punto 3, gia' mio e passato).

## Oggetto
motore_nuovo/src/game/game.cpp, Game::caricaOggettiStrada() (DECO1 2f, punti 1-3, esito out/developer/ESITO_R2_DECO1_P2F_27OTT.md):
- passoZ 4000 -> 2000 unita' mondo (una istanza ogni 2000 anziche 4000)
- scala oggetto default 1.0 -> 4.0 (senza campo altezza_mondo/scala nel manifest della variante)
- nessun oggetto negli ultimi BIVIO_SEG segmenti (break a road_.bivioInizioZ(); bivio pulito, niente elementi in mezzo alla carreggiata)

## Build (verifica mia in questo turno)
`cmake --build UFFICIO/Officina/build --target prova_gate` — exit 0 (sglavinated_core + prova_gate compilati, core senza SDL).

## Gate (eseguito da me in questo turno)
`UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/build/test-results/decorazioni_review" lv_01_neon decorazioni` — exit 0, 27/10/2026 19:34.
- 6/6 livelli: catalogo id skyline/oggetti risolto (lv_01 6/6, lv_02 7/7, lv_03 6/6, lv_04 5/5, lv_05 5/5, lv_06 5/5)
- cartelloni 35-61 per livello, densita ~26.6-27.0% del percorso (target D2b 20-30%)
- ms/frame medio 0.546-0.860, max 1.120 (limite 20)
- screenshot PPM per livello in UFFICIO/Officina/build/test-results/decorazioni_review/; cielo per livello risolto (es. lv_01 0x0A0A1A, 16292 px cielo)
- replay/determinismo invariato: core senza SDL, nessun tocca a src/runner/, misure in secondi

## Esito
POSITIVO. DECO1 punto 2 conforme alla richiesta Andrea (compito 2: skyline/oggetti da tema_grafico, densita cartelloni ~20-30%, seme fisso, nessuna allocazione nel ciclo) e a ESITO_R2_DECO1_P2F_27OTT.md. Nessuna regressione di ms/frame.

## Rimanenti (non mie, non bloccano questo turno)
- G1 @grafico: catalogo 100 PNG fuori elenco, check_catalogo_grafico.py exit 0, "PNG non catalogati: 0"; motore usa solo pronto/approvato.
- @capo: apre e chiude il checkpoint DECO1.
- RIV [~]: gate urto (mio) in attesa del bump urto-rivale di @developer (RIV 2a, non ancora fatto).
- T1-RUN [~] (D19 = B: attende Andrea; nessun lavoro Pi 400/RetroPie prima dell'ok Mac).
