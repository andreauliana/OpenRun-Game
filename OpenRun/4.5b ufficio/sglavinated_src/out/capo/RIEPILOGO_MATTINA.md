# RIEPILOGO MATTINA — OpenRun 4.6 (18/10/2026)

## Fatto per blocco/agente (18/10)
- Officina / Builder (Copilot): STATUS_OFFICINA.md come unico handoff tecnico; wrapper CMake/test e allowlist aggiornati. Gate T1 `sh tools/gate_t1.sh` exit 0: 3 livelli, replay identico, acc/ribaltamento/sterzo/curve OK; 0,19-0,20 ms/frame, max 0,393 ms/frame (report 1155). Log: gate_t1.log; exit code del processo non registrato nel log. CP-003 aperto.
- Designer / Grafico (audit Copilot): STATUS_GRAFICO_PER_CAPO.md unico handoff; catalogo 52 voci (30 app, 16 pronte, 2 in costruzione, 4 mancanti), 100 PNG non catalogati; 3 livelli, 3 auto 64x32, 21 banner sponsor, testi fumetti 156. Mancano sprite concorrenti/personaggi, 6 percorsi, 3 finali animati. CP-002 aperto.
- Capo / Andrea + archivio v4 (Copilot): ANDREA_REVIEW.md fonte unica canonica; D1-D11 e feedback v90 conservati, T1-Q1..Q4 rinominate; archivio v4 (126 file) congelato e classificato. CP-004 aperto.

## Non funziona / blocchi
- Nessun checkpoint (CP-002, CP-003, CP-004) ha revisore indipendente assegnato: esiti IN ATTESA.
- CP-003: `ctest -N` scopre 7 test ma non li esegue; CMake Tools VS Code senza progetto attivo; exit code del gate non nel log.
- Runner SDL2 senza SDL2_image: nel gate headless prova_gate disegna rettangoli, non sprite PNG (D10: scelta B in attesa di verifica tecnica).
- Nessuna prova su Raspberry Pi 400; solo macOS.

## Decisioni aperte in ANDREA_REVIEW.md (3 più importanti)
1. T1-Q1 chiusura gate T1: risposta non registrata (DEFAULT sì se soak exit 0, replay 2x identico, <=20 ms/frame).
2. T1-Q2 promozione P1: risposta non registrata (DEFAULT sì, screenshot differiti).
3. T1-Q3 prova manuale Mac + Pi 400: esito non registrato (DEFAULT sì appena possibile).

## Prossimo passo consigliato
Capo assegna revisori indipendenti per CP-002, CP-003, CP-004; richiede `sh tools/gate_t1.sh soak` + `ctest` con exit code e misure registrate, poi sollecita ad Andrea le risposte T1-Q1..Q3.
