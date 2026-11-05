# Esito gate FORK1 — 25/10/2026 09:59

## Comando eseguito
`sh UFFICIO/Officina/TOOL/gate_t1.sh`

## Exit code
0

## Risultato
GATE T1: TUTTO OK

- 6/6 percorsi GATE PERCORSO OK:
  - lv_01_neon: rettilineo 63%, curva max 0.85, giro 30.4 s @60Hz, fuori strada 0%
  - lv_02_atomico: rettilineo 65%, curva max 0.98, giro 39.8 s @60Hz, fuori strada 0%
  - lv_03_foglie: rettilineo 57%, curva max 0.95, giro 33.4 s @60Hz, fuori strada 0%
  - lv_04_cromo: rettilineo 71%, curva max 0.85, giro 37.5 s @60Hz, fuori strada 0%
  - lv_05_spray: rettilineo 55%, curva max 0.86, giro 33.4 s @60Hz, fuori strada 0%
  - lv_06_desktop: rettilineo 68%, curva max 0.85, giro 34.4 s @60Hz, fuori strada 0%
- ms/frame medio 0.20, max 0.66 (limite 20)
- replay 2x: IDENTICO (tutti i livelli)
- GATE ACCELERAZIONE OK, GATE RIBALTAMENTO OK, GATE STERZO OK, GATE CURVE CONTINUE OK
- GATE GARA OK (4 combinazioni: SS, SD, DS, DD, replay IDENTICO)
- GATE FREQUENZA 60 Hz OK, GATE FREQUENZA 50 Hz OK
- GATE SCREENSHOT P1 OK

## Modifica codice verificata
`motore_nuovo/src/road/road.h`: BIVIO_SEG 800 -> 1200 (raggio piu' ampio nell'apertura del bivio, rami che si separano piu' dolcemente; gia' applicato in turno precedente, rieseguito gate per verifica).

## Residui FORK1
- gate bivio_uscita, bivio_bordi, tema_transizione -> developer-junior
- elemento passaggio bivio -> @grafico

## Blocco
T1-RUN [~] attende risposta Andrea su D19 (APERTA, ANDREA_REVIEW.md senza RISPOSTA).