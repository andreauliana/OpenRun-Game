# Esito gate T1 — 21/10 10:44 (turno 3, @developer)

Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh (smoke: cmake configure + build prova_gate, poi prova_gate + percorso su tutti e 6 i livelli, accelerazione/crash/sterzo/curve/gara/screenshot P1 su lv_01_neon)
Exit code: 0
Esito: GATE T1: TUTTO OK

Misure:
- smoke ms/frame medio=0.291 max=16.094 (limite 20) — prove lv_01..lv_06 exit 0
- replay 2x IDENTICO su tutti i livelli; speed@60=161.79, speed@1=9.90
- accelerazione: reattiva (speed@1>=8.0, speed@60>=150.0) — GATE ACCELERAZIONE OK
- ribaltamento/ripartenza — GATE RIBALTAMENTO OK
- sterzo 30 frame: sinistra=-218.3 destra=218.3 — GATE STERZO OK
- continuita' curve: campioni=1199, salto pendenza max=0.02 — GATE CURVE CONTINUE OK
- gara 4 bivi: 4 combinazioni (SS/SD/DS/DD), arrivo OK, posizione 1/8 o 2/8, replay IDENTICO — GATE GARA OK
- screenshot P1: curva frame 1232 (scarto 182 px), sponsor frame 1353 (3463 px), personaggio+fumetto frame 372 — GATE SCREENSHOT P1 OK

Percorso (giro guidato, soglie da dati/tracciati/regole.json, non abbassate):
- lv_01_neon: segmenti=4250 rettilineo=63% (limiti 25%-75%), rett piu' lungo 659 (min 300), curva dx=0.85 sx=0.85 (min 0.30 max 1.00), giro finito 1527 frame (30.5 s), fuori strada 0 frame (0.0% max 1%) — GATE PERCORSO OK
- lv_02_atomico: segmenti=5650 rettilineo=65%, rett 1440, curva 0.72/0.98, giro finito 1999 frame (40.0 s), fuori 0% — GATE PERCORSO OK
- lv_03_foglie: segmenti=4700 rettilineo=57%, rett 590, curva 0.75/0.95, giro finito 1679 frame (33.6 s), fuori 0% — GATE PERCORSO OK
- lv_04_cromo: segmenti=5300 rettilineo=71%, rett 1457, curva 0.85/0.85, giro finito 1881 frame (37.6 s), fuori 0% — GATE PERCORSO OK
- lv_05_spray: segmenti=4700 rettilineo=55%, rett 607, curva 0.86/0.84, giro finito 1679 frame (33.6 s), fuori 0% — GATE PERCORSO OK
- lv_06_desktop: segmenti=4850 rettilineo=68%, rett 813, curva 0.85/0.85, giro finito 1729 frame (34.6 s), fuori 0% — GATE PERCORSO OK

Provenienza/licenza: dati/tracciati/tratti.json CC-BY-4.0 (autore @developer, tracciati originali, nessun materiale Sega); dati/livelli/*/livello.json CC-BY-4.0. Nessuna modifica di codice in questo turno (zero scritture in motore_nuovo/ e tools/).
Note: gate smoke (non soak). Soak 30000 frame non eseguito questo turno. Ultimo soak completo 20/10 19:50 exit 0.
Log: UFFICIO/Officina/Test area/gate_t1.log.
Attesa: ri-verifica runner Mac Andrea (D12=A, D14=A) prima di 60 Hz/R2; chiusura CP-005 Capo; G1/P1 punto 4 sprite @grafico.
