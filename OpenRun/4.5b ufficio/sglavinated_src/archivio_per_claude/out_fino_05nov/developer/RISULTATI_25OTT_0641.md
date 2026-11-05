# Risultati turno developer — 25/10/2026 06:41

Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh` (default, non soak) dalla radice sglavinated_src.
Exit code: 0.
Esito: GATE T1: TUTTO OK.

Misure (macOS/darwin, headless core):
- 6/6 percorsi GATE PERCORSO OK: lv_01_neon rett 63% cur 0.85/0.85; lv_02_atomico 65% 0.72/0.98; lv_03_foglie 57% 0.75/0.95; lv_04_cromo 71% 0.85/0.85; lv_05_spray 55% 0.86/0.84; lv_06_desktop 68% 0.85/0.85 (rettilineo in [25%,75%], curva max <= 1.0, dx/sx >= 0.3, soglie non abbassate).
- Giro guidato finito su tutti e 6, fuori strada 0 frame.
- Replay 2x IDENTICO. ms/frame medio 0.21 max 4.9 (limite 20).
- Accelerazione, ribaltamento/ripartenza, sterzo, continuita' curve, gara 4 bivi (arrivo, posizione, incidenti, replay IDENTICO), frequenza 60 Hz e 50 Hz, screenshot P1: OK.

Risolve ANDREA_REVIEW T1-Q3/T1-Q5: il giro guidato (R1) ora passa su tutti e 6 i livelli su Mac (era fallito 5/6 il 19/10, exit 13).
Nessuna modifica di codice in questo turno (gate già verde). Blocchi futuri: DECO1 (asset @grafico), PIST1, RIV, R2. v91 runner Mac e fps/temperatura Pi 400 restano a Andrea.
Log: UFFICIO/Officina/Test area/gate_t1.log.
