# Risultati turno builder-developer — 19/10/2026 ~12:23

Blocco: R1 (tracciati 6 piste), stato [~].

Verifiche lettura (nessuna scrittura eseguita):
- compito 1 road.cpp: fatto e verificato (raccordo entrata/uscita a lunghezza propria, correzione deriva distribuita solo su curve != 0, rettilinei intatti).
- tutti e 6 i livello.json hanno gia' campo "tracciato" (nomi tratti usati: rettilineo_corto/lungo, curva_dolce_dx/sx, curva_stretta_dx/sx, collina_su/giu).

Bloccati:
- compiti 2-3: dati/tracciati/tratti.json e regole.json non creabili (scrittura nuovi file in dati/ negata, tentativo respinto); prova_gate.cpp non scrivibile (lettura regole.json non implementabile da questo ruolo).
- gate: sh tools/gate_t1.sh soak NON eseguito (shell negata in questa sessione) -> gate non eseguito.

Ultimo esito gate (Andrea, 19/10 09:39, Mac): exit 13, GATE T1 FALLITO.
- giro guidato KO 5/6: lv_01_neon rettilineo 13%, lv_03_foglie 18%, lv_04_cromo 23%, lv_05_spray 21%, lv_06_desktop 12% (soglia gate 25-75%).
- solo lv_02_atomico OK: rettilineo 40%, curva max 0.86 (riferimento T1-Q5 DEFAULT A).
- core OK: ms/frame medio ~0.19 max 0.94 (limite 20), replay 2x IDENTICO su 6 livelli, smoke/soak/accelerazione/ribaltamento/sterzo/curve/gara/screenshot P1 OK.

Azione T1-Q5 DEFAULT A (sblocco scrittura richiesto): allineare tutti i percorsi al profilo guidabile lv_02_atomico (rettilineo ~35-40%, curva max 0.6-1.0, soglie non si abbassano) e rilanciare sh tools/gate_t1.sh soak. Poi ctest 17/17.

Compiti 4-5 (Junior): gate stampa tratto violato, test espansione tracciati, riscrittura 6 livello.json con tracciato. Review incrociata: Junior rivede 1-3, Builder rivede 4-5; Capo apre CP-005 in Checkpoint Revision/ solo a gate verde con entrambe le review.

Ordine test (Andrea T1-Q5): prima Mac di Andrea (R1 + R2), poi RetroPie/Raspberry Pi 400 solo con ok di Andrea; non rompere la build Linux nel frattempo.
