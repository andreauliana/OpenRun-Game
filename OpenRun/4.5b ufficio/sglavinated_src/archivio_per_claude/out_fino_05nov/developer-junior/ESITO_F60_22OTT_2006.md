# Esito turno developer-junior — 22/10 20:06
## Blocco F60 (60 Hz, dt in secondi) — [x] completato e verificato
- Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh
- Exit code: 0
- Esito: GATE T1: TUTTO OK
- Frequenza: GATE FREQUENZA 60 Hz OK, GATE FREQUENZA 50 Hz OK (giro guidato stessi secondi, replay IDENTICO a ciascuna)
- Prestazioni: ms/frame medio 0.16-0.275 (limite 20), replay 2x IDENTICO su tutti i livelli
- Percorsi: 6/6 GATE PERCORSO OK (rettilineo 52-71%, curva max <= 1.0, fuori strada 0%)
- Altri test: accelerazione, ribaltamento, sterzo, curve continue, gara 4 bivi OK
- File modificati in questo turno: nessuno in src/ (percorso @developer); prova_gate.cpp e gate_t1.sh gia' contengono i test frequenza (modifiche precedenti)
- Blocchi successivi: H [~] (ok Andrea runner Mac v91), R2 [ ] dopo F60
