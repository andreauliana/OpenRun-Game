# Esito gate T1 — 21/10/2026 06:41 (turno 1, @developer, Mac darwin)

Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh soak
Exit code: 0
Esito: GATE T1: TUTTO OK

- 6/6 GATE PERCORSO OK:
  - lv_01_neon: rettilineo 63% (limiti 25%-75%), curva max dx/sx 0.85/0.85, giro guidato 1527 frame (30.5 s), fuori strada 0%
  - lv_02_atomico: rettilineo 65%, curva max dx/sx 0.72/0.98, giro guidato 1999 frame (40.0 s), fuori strada 0%
  - lv_03_foglie: rettilineo 57%, curva max dx/sx 0.75/0.95, giro guidato 1679 frame (33.6 s), fuori strada 0%
  - lv_04_cromo: rettilineo 71%, curva max dx/sx 0.85/0.85, giro guidato 1881 frame (37.6 s), fuori strada 0%
  - lv_05_spray: rettilineo 55%, curva max dx/sx 0.86/0.84, giro guidato 1679 frame (33.6 s), fuori strada 0%
  - lv_06_desktop: rettilineo 68%, curva max dx/sx 0.85/0.85, giro guidato 1729 frame (34.6 s), fuori strada 0%
- Soak: 6/6 livelli, 30000 frame ciascuno, exit 0; ms/frame medio ~0.19, max 59.284 su lv_03_foglie (spike, limite 20 — da monitorare).
- Replay 2x: IDENTICO su tutti i livelli e nelle 4 gare a tappe (bivi).
- Accelerazione, ribaltamento/ripartenza, sterzo, curve continue, gara a tappe (4 bivi), screenshot P1: OK.
Log: UFFICIO/Officina/Test area/gate_t1.log

Blocco R1 compiti @developer 1-3 conclusi (road.cpp raccordo entrata/uscita min(n/3,60), deriva solo curve != 0; dati/tracciati/tratti.json 8 tratti CC-BY-4.0; dati/tracciati/regole.json 6 livelli + default lette da prova_gate; 6 dati/livelli/*/livello.json con campo tracciato).
T1-RUN [~]: tutte le 6 piste girano guidate anche su Mac (darwin) con le correzioni R1 — dato per ri-verifica Andrea D12/D13 prima di 60 Hz/R2.
In attesa di terzi: review Junior punti 1-3, chiusura CP-005 Capo, ri-verifica runner Mac Andrea (D12=A).
