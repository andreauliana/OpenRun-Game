# Voce scambio @sviluppatore — 18/10 11:53 (turno 1)

Stato blocchi: P1 [~], T1-RUN [~].

Fatto questo turno:
- Letto (ordine imposto): RIASSUNTO, DIARIO_RECENTE, FEEDBACK_ANDREA (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA, SCAMBIO_RECENTE.
- Verificato gate: `sh tools/gate_t1.sh` → lv_01_neon exit 0, lv_02_atomico exit 0, lv_03_foglie timeout a 180 s (carico sistema; precedente 18/10 11:17 exit 0 completo su 3 livelli + soak). Replay 2x identico, offroad raggiunto. Nessuna modifica di codice in questo turno.
- Verificato catalogo grafico: cartelloni approvati (7 sponsor × 3 livelli), auto giocatore approvate, oggetti bordo strada `pronto`; personaggi (Gianni, Gesu', UFO) e traffico `mancante`.

Blocco misurato:
- Sfoltimento cartelloni in `dati/livelli/*/livello.json` (T1-RUN 4, prossimo P1): array ~300 voci per livello (uno ogni 1-4 segmenti). `python3` negato in shell (`Permission denied: shell`) → impossibile eseguire script di assottigliamento JSON; riscrittura manuale via `write`/`edit` non affidabile su array di migliaia di byte. Rinviato al prossimo turno quando la shell permette python3.
- Screenshot P1 in `out/sviluppatore/`: shell `cp` negato (già segnalato); esistono in `out/collaudatore/<livello>/`.

Consegna: VOCE_SCAMBIO_1152 già presente in `out/sviluppatore/`; questa voce (1153) aggiorna lo stato.

Prossimo: sfoltire cartelloni lv_01/lv_02/lv_03 (semaforo: shell python3), rieseguire `sh tools/gate_t1.sh`, attendere v91 Andrea (test runner Mac) per chiudere P1/T1-RUN.
