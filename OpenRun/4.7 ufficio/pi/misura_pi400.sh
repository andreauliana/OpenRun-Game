#!/bin/sh
# Build di misura per Raspberry Pi 400: compila in Release, misura i tempi per
# frame e salva un riepilogo da mandare a Claude. Non installa nulla nel menu
# Ports e non cambia il gioco.
#
# Uso, dal terminale del Pi (meglio dal desktop del Pi, non da SSH):
#   sh "<cartella del repo>/OpenRun/4.7 ufficio/pi/misura_pi400.sh"            (60 s per pista)
#   sh ".../pi/misura_pi400.sh" 20                                              (prova veloce, 20 s)
#
# Passi:
#   1. controlla i programmi necessari (non usa sudo: se manca qualcosa dice il comando)
#   2. compila in motore_nuovo/build_misura (Release, -j2)
#   3. misura_pi: nucleo senza finestra, 6 piste + gara a tappe, guida automatica
#   4. runner vero a schermo intero (solo se c'e' un desktop): gara con guida
#      automatica, filtro CRT spento e poi acceso
#   5. stato del Pi (temperatura, frequenza, limitazioni) prima e dopo
# Risultato: misure_pi400/<data_ora>/riepilogo.txt
set -e
SECONDI="${1:-60}"
DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$DIR/sglavinated_src"
B="$SRC/motore_nuovo/build_misura"
OUT="$DIR/misure_pi400/$(date +%Y-%m-%d_%H%M)"
mkdir -p "$OUT"
R="$OUT/riepilogo.txt"

statoPi() {
    echo "--- stato Pi ($1) ---"
    if command -v vcgencmd >/dev/null 2>&1; then
        vcgencmd measure_temp
        vcgencmd measure_clock arm
        vcgencmd get_throttled
        echo "(get_throttled: 0x0 = tutto ok; 0x50000 o 0x50005 = alimentatore debole o surriscaldamento)"
    else
        echo "vcgencmd non disponibile (non e' un Pi?)"
        if [ -r /sys/class/thermal/thermal_zone0/temp ]; then
            echo "temp: $(($(cat /sys/class/thermal/thermal_zone0/temp) / 1000)) C"
        fi
    fi
    if [ -r /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor ]; then
        echo "governor: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)"
    fi
    echo
}

echo "1/5 controllo programmi"
MANCA=""
for p in cmake g++ pkg-config; do command -v "$p" >/dev/null 2>&1 || MANCA="$MANCA $p"; done
pkg-config --exists sdl2 2>/dev/null || MANCA="$MANCA libsdl2-dev"
if [ -n "$MANCA" ]; then
    echo "Mancano:$MANCA"
    echo "Installa con:  sudo apt update && sudo apt install -y build-essential cmake pkg-config libsdl2-dev libsdl2-image-dev"
    echo "poi rilancia questo script."
    exit 1
fi

{
    echo "=== MISURA PI 400 — $(date '+%d/%m/%Y %H:%M') ==="
    echo "macchina: $(tr -d '\0' < /proc/device-tree/model 2>/dev/null || uname -m)"
    echo "sistema : $(uname -srm)"
    echo "commit  : $(git -C "$DIR" rev-parse --short HEAD 2>/dev/null || echo '?')"
    echo "SDL2    : $(pkg-config --modversion sdl2 2>/dev/null)"
    echo "memoria : $(free -m 2>/dev/null | awk '/^Mem:/ {print $2 " MB totali, " $7 " MB liberi"}')"
    echo
    statoPi "prima"
} > "$R"

echo "2/5 compilazione Release in motore_nuovo/build_misura (qualche minuto la prima volta)"
cmake -S "$SRC/motore_nuovo" -B "$B" -DCMAKE_BUILD_TYPE=Release > "$OUT/cmake.log" 2>&1
T0=$(date +%s)
cmake --build "$B" --target misura_pi sglavinated_runner -j2 >> "$OUT/cmake.log" 2>&1 || {
    echo "Compilazione fallita: guarda $OUT/cmake.log"; exit 1; }
echo "compilazione: $(( $(date +%s) - T0 )) s (0 = gia' compilato)" >> "$R"
echo >> "$R"

echo "3/5 misura del nucleo: 7 prove da $SECONDI s (circa $(( SECONDI * 7 / 60 + 1 )) minuti se il Pi regge i 60 fps)"
cd "$SRC"
"$B/misura_pi" "$OUT" "$SECONDI" > "$OUT/misura_pi.log" 2>&1
cat "$OUT/misura.txt" >> "$R"
echo >> "$R"

echo "4/5 runner a schermo intero"
if [ -n "$SSH_CONNECTION" ] && [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; then
    echo "Da SSH senza schermo: salto il runner. Lancia lo script dal terminale del desktop del Pi" | tee -a "$R"
    echo "(oppure da console:  SDL_VIDEODRIVER=kmsdrm sh .../misura_pi400.sh)." | tee -a "$R"
else
    for modo in --no-crt --crt; do
        echo "   gara con guida automatica, $modo ($SECONDI s; Esc per interrompere)"
        "$B/sglavinated_runner" --fullscreen --subito --pilota-auto $modo \
            --misura-secondi "$SECONDI" > "$OUT/runner$modo.log" 2>&1 || true
        {
            echo "--- runner $modo ---"
            grep -E "renderer:|misura: schermo" "$OUT/runner$modo.log" | sed 's/^INFO: //' || true
            sed -n '/=== MISURA RUNNER/,$p' "$OUT/runner$modo.log"
            echo
        } >> "$R"
    done
fi

echo "5/5 stato del Pi dopo le prove"
statoPi "dopo" >> "$R"

echo
cat "$R"
echo
echo "Fatto. Riepilogo: $R"
echo "Mandalo a Claude (o fai commit della cartella misure_pi400 con GitHub Desktop)."
