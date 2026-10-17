# Esito Gate Tappa 1 — @debugger — 17/10/2026 14:00

Eseguito: `sh tools/gate_t1.sh` (smoke 500 frame = 10s su 3 livelli) e `sh tools/gate_t1.sh soak` (30000 frame = 10 min simulati su lv_01_neon).
Toolchain: cmake + g++ (regola 26, build concessa a @debugger). Build dir: build_gate. Target: sglavinated_core + prova_gate.

Risultati MISURATI (exit code 0, non dedotti):

SMOKE lv_01_neon — speed@60=33.76, X[min,max]=[-164.8,751.8], zFinale=942.7, offroad raggiunto frame 476, ms/frame medio=0.092 max=0.480 (limite 20), replay 2x IDENTICO, GATE OK.
SMOKE lv_02_atomico — speed@60=33.76, X[min,max]=[-164.8,751.0], zFinale=942.7, offroad raggiunto frame 476, ms/frame medio=0.087 max=0.240, replay 2x IDENTICO, GATE OK.
SMOKE lv_03_foglie — speed@60=33.76, X[min,max]=[-164.8,752.2], zFinale=939.6, offroad raggiunto frame 475, ms/frame medio=0.095 max=0.332, replay 2x IDENTICO, GATE OK.
SOAK lv_01_neon (30000 frame) — speed@60=33.76, X[min,max]=[-164.8,900.0], zFinale=2958.7, offroad raggiunto frame 476, ms/frame medio=0.083 max=2.889 (limite 20), replay 2x IDENTICO, GATE OK.

Frame: 50 fps stabili (ms/frame ~0.08-0.09 << obiettivo <=12 ms, limite 20 ms). Temperatura: non misurabile (non Pi 400).
Sintassi nucleo: OK 34/34 file (clang++ -fsyntax-only, voce 76/78).
Tre rilievi reviewer CHECKPOINT 1 (13:26): gia' sanati su disco da @coder (include path, guard strada vuota, SPDX MIT). Nessun file di prodotto toccato da @debugger (regola debugger).

Blocco H: TUTTE LE MISURE PASSANO. Da promuovere a [x] se Andrea conferma sui screenshot/log. Report log: out/debugger/gate_t1.log.
