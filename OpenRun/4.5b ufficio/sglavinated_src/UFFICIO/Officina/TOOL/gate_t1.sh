#!/bin/sh
# SPDX-License-Identifier: MIT
# Gate Tappa 1 in UN SOLO comando: configura, compila, lancia prova_gate sui 3 livelli.
#   sh UFFICIO/Officina/TOOL/gate_t1.sh          smoke 10 s (500 frame) + replay 2x, 3 livelli
#   sh UFFICIO/Officina/TOOL/gate_t1.sh soak     come sopra + 30000 frame (10 min simulati) su tutte e 6 le piste
# Va lanciato COSI', senza pipe, senza "2>&1", senza "; echo": i permessi degli agenti
# confrontano il comando intero. Tutto l'output dettagliato va in UFFICIO/Officina/Test area/gate_t1.log;
# a video escono solo le righe che servono. Exit code: 0 tutto OK, altrimenti codice del passo fallito.
cd "$(dirname "$0")/../../.." || exit 9  # TOOL -> Officina -> UFFICIO -> radice
MODE="${1:-smoke}"
LOG="UFFICIO/Officina/Test area/gate_t1.log"
mkdir -p "UFFICIO/Officina/Test area"
: > "$LOG"

# Non cancellare mai la build esistente: gli ZIP possono essere spostati su un'altra
# macchina e contenere una CMakeCache con percorsi assoluti non piu' validi.
BUILD_DIR="UFFICIO/Officina/build"
EXPECTED_SOURCE=$(cd motore_nuovo && pwd -P) || exit 9
if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
  CACHED_SOURCE=$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$BUILD_DIR/CMakeCache.txt" | head -n 1)
  if [ -n "$CACHED_SOURCE" ] && [ "$CACHED_SOURCE" != "$EXPECTED_SOURCE" ]; then
    BUILD_DIR="UFFICIO/Officina/build_relocated_$(printf '%s' "$EXPECTED_SOURCE" | cksum | awk '{print $1}')"
    echo "Cache CMake non portabile: uso $BUILD_DIR senza cancellare la build precedente." >> "$LOG"
  fi
fi

echo "== configurazione (cmake)"
cmake -S motore_nuovo -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release >> "$LOG" 2>&1 || {
  echo "CMAKE CONFIGURE FALLITO (exit 10). Ultime righe di $LOG:"; tail -n 15 "$LOG"; exit 10; }

echo "== compilazione (cmake --build)"
cmake --build "$BUILD_DIR" --target prova_gate >> "$LOG" 2>&1 || {
  echo "BUILD FALLITO (exit 11). Errori:"; grep -n "error" "$LOG" | head -n 20; exit 11; }

RC=0
LIVELLI="lv_01_neon lv_02_atomico lv_03_foglie lv_04_cromo lv_05_spray lv_06_desktop"
for LV in $LIVELLI; do
  echo "== prova_gate $LV"
  ./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/$LV" "$LV" >> "$LOG" 2>&1
  R=$?
  echo "   exit $R"
  [ "$R" -ne 0 ] && RC=$R
  echo "== percorso $LV (profilo + giro guidato)"
  ./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/percorso_$LV" "$LV" percorso >> "$LOG" 2>&1
  R=$?
  echo "   exit $R"
  [ "$R" -ne 0 ] && RC=$R
done
if [ "$MODE" = "soak" ]; then
  for LV in $LIVELLI; do
    echo "== soak 30000 frame $LV"
    ./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/soak_$LV" "$LV" soak >> "$LOG" 2>&1
    R=$?
    echo "   exit $R"
    [ "$R" -ne 0 ] && RC=$R
  done
fi
echo "== verifica accelerazione 10 s"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/accelerazione" lv_01_neon accelerazione >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R
echo "== verifica ribaltamento e ripartenza"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/crash" lv_01_neon crash >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R
echo "== verifica sterzo sinistra/destra"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/sterzo" lv_01_neon sterzo >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R
echo "== verifica continuita' curve e corsie"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/curve" lv_01_neon curve >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R

echo "== gara a tappe (bivi, 4 combinazioni di scelte, arrivo)"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/gara" lv_01_neon gara >> "$LOG" 2>&1
R=$?
echo "   exit $R"
[ "$R" -ne 0 ] && RC=$R

echo "== screenshot P1 (curva, cartelloni sponsor, personaggio con fumetto)"
./"$BUILD_DIR"/prova_gate "UFFICIO/Officina/Test area/screenshot_p1" lv_01_neon screenshot >> "$LOG" 2>&1
R=$?
echo "   exit $R  (immagini in UFFICIO/Officina/Test area/screenshot_p1/)"
[ "$R" -ne 0 ] && RC=$R

echo "== riepilogo (da $LOG)"
grep -E "^(modo|speed@60|accelerazione|sterzo 30 frame|continuita' curve|offroad|ms/frame|replay 2x|screenshot P1|percorso |giro guidato|gara |GATE .* OK|GATE OK|FALLITO)" "$LOG"
[ "$RC" -eq 0 ] && echo "GATE T1: TUTTO OK" || echo "GATE T1: FALLITO (exit $RC)"
exit "$RC"
