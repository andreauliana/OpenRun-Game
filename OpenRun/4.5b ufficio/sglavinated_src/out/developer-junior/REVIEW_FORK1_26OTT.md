# REVIEW FORK1 — revisore @developer-junior — 26/10/2026 07:14

Blocco: FORK1 [~] (RICHIESTA_APERTA.md). Compito assegnato: review incrociata (1 turno) su modifica `game.cpp` (corsia a scelta aperta) e transizione tema (Renderer). Esito in `out/developer-junior/REVIEW_FORK1_<data>.md`. L'autore del codice non e' il revisore.

## File esaminati (sola lettura, nessuna modifica)
- `motore_nuovo/src/game/game.cpp`: righe 650-672 (`fineTappa`: raccordo curva/collina da ultimo segmento della tappa precedente + `iniziaTransizioneTema`), righe 836-854 (ripartenza post-ribaltamento sul ramo piu' vicino `playerXIncidente_`), righe 857-876 (corsia a scelta aperta = ramo piu' vicino `carr.centro[vicino]`, niente centro fisso a 0), righe 911-926 (`assistenzaBivio_`: spinta di curva attenuata `(1.0f - assistenzaBivio_)`, richiamo morbido max `ASSIST_RICHIAMO_MAX` unità/s verso centro ramo, offroad/conteggio ribaltamento sospesi durante assistenza).
- `motore_nuovo/src/render/renderer.cpp`: righe 145-169 (`iniziaTransizioneTema` fissa `temaDa_` = tema tappa che finisce e `temaA_` = tema di arrivo da `impostaTema`; `aggiornaTransizioneTema` interpola `cielo`, `erba`, `erbaAlt`, `rumbleAlt` da `temaDa_` a `temaA_` in `transizioneDurataS_` secondi; nessun interpolazione verso `tema_` sovrascritto a ogni frame).
- `motore_nuovo/test/prova_gate.cpp`: gate `bivio_uscita` campiona centro = ramo piu' vicino anche a scelta aperta e `xRel` rispetto alla corsia (no salto fittizio di `BIVIO_DIST`); soglie da `dati/tracciati/regole.json` ("bivio").
- `motore_nuovo/dati/tracciati/regole.json`: blocco "bivio" (finestra ±60 frame, delta_larghezza_max 400, delta_centro_max 400, delta_x_rel_max 40, delta_curva_max 0.05, delta_collina_max 0.05); blocco "tema_transizione" (delta_canale_max_per_frame 12).

## Controllo punti FORK1 (RICHIESTA_APERTA.md)
1) Cambio tappa senza scatto di prospettiva: OK — `fineTappa` imposta curva/collina di inizio tappa dall'ultimo segmento precedente, tema in transizione graduale, `playerZ`/`player_.z`/`road_.playerX` ricalcolati nello stesso frame del cambio.
2) Prima della scelta: niente salto di `centroCorsia` al passaggio tra rami; il ramo non scelto non spinge l'auto (spinta di curva inibita durante assistenza). OK.
3) Bordi: l'auto non resta intrappolata; richiamo morbido verso centro del ramo piu' vicino; ribaltamento offroad solo se fuori strada oltre `OFFROAD_CRASH_S` con assistenza spenta; ripartenza post-incidente sul ramo piu' vicino. OK.
6) Transizione di tema graduale: OK — colori interpolati canale per canale in secondi (`transizione_tema_s` = 2.0 s in `dati/config.json`, Andrea 25/10), tema di arrivo fissato dopo la scelta, nessun salto in un singolo frame.

## Gate eseguito in questo turno
Comando: `UFFICIO/Officina/build/prova_gate UFFICIO/Officina/build/test-results/bivio_uscita lv_01_neon bivio_uscita` (working directory radice `sglavinated_src`).
Data/ora: 26/10/2026 07:14. Exit code: 0.
Esito: `GATE BIVIO USCITA OK` per tutte le 4 combinazioni SS/SD/DS/DD; blocco scelta frame 1629, cambio tappa 1->2 frame 1823; maxDelta larghezza 16.0 (<= 400), centro 6.3 (<= 400), x_rel 12.7 (<= 40), curva 0.00, collina 0.00 (<= 0.05). Screenshot PPM salvati in `UFFICIO/Officina/build/test-results/bivio_uscita/`.
Nota: gate `bivio_bordi` e `tema_transizione` gia' verificati OK nei turni precedenti (25/10 08:46 e 07:47); `ctest` 20/20 gia' verde su FORK1 (RICHIESTA_APERTA.md, Claude 25/10).

## Esito review
REVISIONE POSITIVA: i punti FORK1 1-3 (@developer) e 6 (transizione tema) soddisfano i vincoli di RICHIESTA_APERTA.md; determinismo invariato (replay 2x IDENTICO), nessun file `src/` toccato da me (review di sola lettura), soglie non abbassate.

## Rilievi / prossimi passi
- Aprire: nessun rilievo bloccante. Le soglie "bivio" in `regole.json` sono proposte da validare in tuning con @developer (non abbassare per far passare i gate).
- Mancante per chiusura FORK1: review @developer sui gate 4 (`bivio_uscita`), 5 (`bivio_bordi`), 7 (`tema_transizione`) di `prova_gate.cpp` (altro turno/ruolo), poi @capo apre CP-007. Elemento di passaggio al bivio @grafico: facoltativo, non blocca.
- In coda dopo FORK1: DECO1 [ ], RIV [ ], R2 [ ]; T1-RUN [~] (D19 = B, attesa prova/decisione Andrea su Mac); nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok Mac.
