#!/bin/sh
# SPDX-License-Identifier: MIT
# Gate Tappa 1 in UN SOLO comando: configura, compila, lancia prova_gate sui 3 livelli.
#   sh tools/gate_t1.sh          smoke 10 s (500 frame) + replay 2x, 3 livelli
#   sh tools/gate_t1.sh soak     come sopra + 30000 frame (10 min simulati) sul livello 1
# Va lanciato COSI', senza pipe, senza "2>&1", senza "; echo": i permessi degli agenti
# confrontano il comando intero. Tutto l'output dettagliato va in out/collaudatore/gate_t1.log;
# a video escono solo le righe che servono. Exit code: 0 tutto OK, altrimenti codice del passo fallito.
cd "$(dirname "$0")/.." || exit 9
MODE="${1:-smoke}"
LOG=out/collaudatore/gate_t1.log
mkdir -p out/collaudatore
: > "$LOG"

echo "== configurazione (cmake)"
cmake -S motore_nuovo -B build_gate -DCMAKE_BUILD_TYPE=Release >> "$LOG" 2>&1 || {
  echo "CMAKE CONFIGURE FALLITO (exit 10). Ultime righe di $LOG:"; tail -n 15 "$LOG"; exit 10; }

echo "== compilazione (cmake --build)"
cmake --build build_gate --target prova_gate >> "$LOG" 2>&1 || {
  echo "BUILD FALLITO (exit 11). Errori:"; grep -n "error" "$LOG" | head -n 20; exit 11; }

RC=0
for LV in lv_01_neon lv_02_atomico lv_03_foglie; do
  echo "== prova_gate $LV"
  ./build_gate/prova_gate "out/collaudatore/$LV" "$LV" >> "$LOG" 2>&1
  R=$?
  echo "   exit $R"
  [ "$R" -ne 0 ] && RC=$R
done
if [ "$MODE" = "soak" ]; then
  echo "== soak 30000 frame lv_01_neon"
  ./build_gate/prova_gate out/collaudatore/soak lv_01_neon soak >> "$LOG" 2>&1
  R=$?
  echo "   exit $R"
  [ "$R" -ne 0 ] && RC=$R
fi
echo "== verifica accelerazione 10 s"
./build_gate/prova_gate out/collaudatore/accelerazione lv_01_neon accelerazione >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R
echo "== verifica ribaltamento e ripartenza"
./build_gate/prova_gate out/collaudatore/crash lv_01_neon crash >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R
echo "== verifica sterzo sinistra/destra"
./build_gate/prova_gate out/collaudatore/sterzo lv_01_neon sterzo >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R

echo "== riepilogo (da $LOG)"
grep -E "^(modo|speed@60|accelerazione|sterzo 30 frame|offroad|ms/frame|replay 2x|GATE .* OK|GATE OK|FALLITO)" "$LOG"
[ "$RC" -eq 0 ] && echo "GATE T1: TUTTO OK" || echo "GATE T1: FALLITO (exit $RC)"
exit "$RC"
