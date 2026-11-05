# Esito gate T1 — 20/10/2026 19:50 (turno 2, @developer)

Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh soak
Exit code: 0
Esito: GATE T1: TUTTO OK

- 6/6 GATE PERCORSO OK:
  - lv_01_neon: rettilineo 63% (limiti 25%-75%), curva max dx/sx 0.85/0.85
  - lv_02_atomico: rettilineo 65%, curva max dx/sx 0.72/0.98
  - lv_03_foglie: rettilineo 57%, curva max dx/sx 0.75/0.95
  - lv_04_cromo: rettilineo 71%, curva max dx/sx 0.85/0.85
  - lv_05_spray: rettilineo 55%, curva max dx/sx 0.86/0.84
  - lv_06_desktop: rettilineo 68%, curva max dx/sx 0.85/0.85
  - Giro guidato finito su tutti e 6 i livelli, fuori strada 0 frame (0.0%, max 1%).
- Soak: 6/6 livelli, 30000 frame ciascuno, exit 0; ms/frame medio ~0.19, max 7.252 (limite 20).
- Replay 2x: IDENTICO su tutti i livelli e nelle 4 gare a tappe (bivi).
- Accelerazione, ribaltamento/ripartenza, sterzo, curve continue, gara a tappe (4 bivi), screenshot P1: OK.
Log: UFFICIO/Officina/Test area/gate_t1.log

Blocco R1 compiti @developer 1-3 conclusi (road.cpp raccordo entrata/uscita min(n/3,60), deriva solo curve != 0; dati/tracciati/tratti.json 8 tratti CC-BY-4.0; dati/tracciati/regole.json 6 livelli + default lette da prova_gate; 6 dati/livelli/*/livello.json con campo tracciato).
In attesa di terzi: review Junior punti 1-3, chiusura CP-005 Capo, ri-verifica runner Mac Andrea (D12=A) prima di R2 / 60 Hz. G1/P1 punto 4 sprite in capo a @grafico.
