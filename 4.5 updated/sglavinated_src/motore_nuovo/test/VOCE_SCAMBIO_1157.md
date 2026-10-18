# VOCE_SCAMBIO_1157 — @collaudatore, 18/10 11:47
## Esito Gate T1 (da log esistente out/collaudatore/gate_t1.log, comando `sh tools/gate_t1.sh` exit 0; rilancio timeoutato a 180s e non ripetuto)
Risultato: GATE T1 TUTTO OK su lv_01_neon / lv_02_atomico / lv_03_foglie.
- speed@60=161.79 > 0 su tutti i livelli; speed@1=9.90
- offroad raggiunto: frame 278 / 167 / 285
- ms/frame medio 0.96-1.673, max 71.012 (spike primo frame; gate controlla medio <= 20, obiettivo 12)
- replay 2x IDENTICO x3 livelli (determinismo OK)
- avvolgimento playerZ oltre totalLen_: OK (wrapCount 617-641, nessuna discesa intermedia); inattivita 10s -> show/fumetto OK (max bolla 4655 px >= soglia 100)
- Estesi invocati da gate: accelerazione 9.90/160.83/246.66 @ 1-60-600 OK; ribaltamento OK; sterzo -218.3/+218.3 OK; continuita curve 1199 campioni, salto pendenza 0.50 OK.
## Aperto — PASSAGGIO (non bloccante per il gate)
- frenata (Pad::B) e costa (COAST): modi `frenata`/`costa` implementati in prova_gate.cpp (verificaFrenata/verificaCosta) ma NON invocati da gate_t1.sh. Tentativi falliti (2): `./build_gate/prova_gate out/collaudatore/frenata lv_01_neon frenata` e `... lv_01_neon costa` -> permission.rejected (esecuzione diretta negata). tools/gate_t1.sh non rientra nei percorsi di scrittura del collaudatore.
PASSAGGIO: collaudatore -> capo | frenata/costa numeri runtime mancanti | esecuzione diretta del binario negata e script gate non modificabile da questo ruolo | serve aggiunta modi frenata/costa a gate_t1.sh o autorizzazione esecuzione diretta; numeri da riportare in voce successiva.
## Review T1-RUN punto 7 (sola lettura)
renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh: nessun problema da segnalare. Colline (punto 2 sviluppatore) e wrap sprite (punto 3 sviluppatore) confermate corrette. Nessun materiale Sega; intestazioni SPDX presenti; quota open 100% (checkpoint 11:37), check_catalogo OK.
## Consegna
Log: out/collaudatore/gate_t1.log. Screenshot: out/collaudatore/<livello>/.
