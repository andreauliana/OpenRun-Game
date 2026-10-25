# Esito FORK1 parte @developer — 25/10/2026 07:32

## Blocco
FORK1 [ ] — bivio e cambio percorso fluidi, transizione di tema, safe area nei bordi strada durante bivio. Parte @developer rimasta: "raggio piu' ampio nell'apertura del bivio (rami che si separano piu' ampiamente, raccordo in Road::carreggiate, BIVIO_SEG), senza scatti/errori di cambi prospettiva" (RICHIESTA_APERTA.md).

## Modifica
- `motore_nuovo/src/road/road.h`: `BIVIO_SEG` 800 -> 1200 (zona del bivio da 160000 a 240000 unita' mondo). L'apertura 0 -> BIVIO_DIST (1900) ora si distribuisce su 720 segmenti invece di 480: i due rami si separano piu' dolcemente (raggio piu' ampio). Alla scelta bloccata (`bivioBloccoZ()`) resta `d == BIVIO_DIST` e centro del ramo scelto == 0, coerente con il ricentramento in `game.cpp` (`player_.x -= latoBivio_ * BIVIO_DIST`): nessuno scatto di prospettiva. `BIVIO_SVOLTA` (4.0e-4) e `BIVIO_DIST` invariati.
- Nessun altro file toccato (niente `src/runner/`, niente dati livello, niente test).

## Verifica
- Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh` — exit 0.
- Esito: `GATE T1: TUTTO OK`.
- 6/6 percorsi `GATE PERCORSO OK` (lv_01_neon..lv_06_desktop; rettilineo 55-71%, curva max 0.72-0.98, giro guidato finito su tutti, fuori strada 0 frame).
- Replay 2x IDENTICO su tutti i livelli e nelle 4 gare.
- ms/frame medio 0.213, max 2.844 (limite 20).
- Gara a tappe 4 combinazioni (SS, SD, DS, DD) OK, arrivo raggiunto, replay IDENTICO.
- Frequenza 60 Hz e 50 Hz OK (giro guidato stessi secondi).
- Screenshot P1 OK (curva, cartelloni sponsor, personaggio con fumetto).
- Log: `UFFICIO/Officina/Test area/gate_t1.log`.

## Residui del blocco (non di questo turno)
- developer-junior: gate `bivio_uscita`, `bivio_bordi`, `tema_transizione`.
- @grafico: elemento di passaggio al bivio.
- T1-RUN [~] resta in attesa della decisione di Andrea su D19 (ANDREA_REVIEW.md senza risposta).
