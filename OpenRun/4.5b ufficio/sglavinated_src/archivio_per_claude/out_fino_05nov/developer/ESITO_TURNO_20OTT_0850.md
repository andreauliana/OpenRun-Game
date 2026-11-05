# Esito turno developer — 20/10/2026 08:50

Blocco attivo: R1 (tracciati 6 piste), stato [~].

Gate: NON eseguito. Comando `sh tools/gate_t1.sh soak` non eseguibile in questo ambiente (shell permission.rejected). Esito riportato: 'gate non eseguito'. Ultimo gate noto: 19/10 09:39, exit 13, giro guidato KO 5/6 (solo lv_02_atomico OK: rettilineo 40%, curva max 0.86); core OK ms/frame ~0.19 max 0.94, replay IDENTICO su tutti i 6 livelli.

Blocco scrittura: scrittura nuovi file in `motore_nuovo/dati/` negata (tentativo `dati/tracciati/tratti.json` e `regole.json`: Permission denied). Compiti R1-2 e R1-3 bloccati in attesa di sblocco scrittura. `prova_gate.cpp` non scrivibile al ruolo.

Stato compiti R1 (@developer):
- R1-1 road.cpp raccordo entrata/uscita + correzione deriva solo su curve != 0: FATTO (già presente in sorgente).
- R1-2 dati/tracciati/tratti.json (libreria tratti con curva/collina/lunghezza/entrata/uscita): BLOCCATO.
- R1-3 dati/tracciati/regole.json (quota rettilinei min/max, rettilineo più lungo, curva max, fuoristrada max per pista): BLOCCATO.
- R1-4/R1-5 (gate che stampa il tratto violato + riscrittura 6 livello.json con tracciato, test espansione): assegnati a @developer-junior.

Prossima azione (T1-Q5 DEFAULT A, Andrea): allineare i 6 tracciati al profilo guidabile lv_02_atomico (rettilineo ~35-40%, curva max coerente 0.6-1.0, rettilineo più lungo >= 300 segmenti) e rilanciare `sh tools/gate_t1.sh soak` su tutte e 6 le piste. Richiede sblocco scrittura dati/ e shell. @capo apre CP-005 dopo gate verde e review incrociate (Builder rivede 4-5, Junior rivede 1-3).

Ordine test (Andrea, ANDREA_REVIEW T1-Q5): prima ok su Mac (runner SDL2), poi Raspberry Pi 400/RetroPie. Nessun lavoro dedicato a Pi 400 finché non c'è l'ok di Andrea su Mac.

Scritture questo turno: solo bacheche/developer/ e out/developer/ (scrivibili). Zero scritture in motore_nuovo/ e tools/.
