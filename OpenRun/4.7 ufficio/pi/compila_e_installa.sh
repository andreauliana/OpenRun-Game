#!/bin/sh
# OpenRun (motore C++): compila sul Raspberry Pi con RetroPie e aggiunge il gioco al menu Ports.
# Non tocca gli emulatori gia' installati: aggiunge solo un file in ~/RetroPie/roms/ports.
# Uso, dal terminale del Pi (F4 da EmulationStation, oppure ssh):
#   sh ~/OpenRun-Game/OpenRun/"4.7 ufficio"/pi/compila_e_installa.sh
# Dopo un aggiornamento (git pull) basta rilanciarlo.
set -e
DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$DIR/sglavinated_src"
B="$SRC/motore_nuovo/build_pi"
PORTS="$HOME/RetroPie/roms/ports"
echo "1/4 pacchetti necessari (chiede la password del Pi, di solito: raspberry)"
# Installa solo i pacchetti che mancano, senza aggiornare quelli presenti:
# RetroPie ha la sua SDL2 e non va sostituita (gli emulatori restano come sono).
manca() { for p in "$@"; do dpkg -s "$p" >/dev/null 2>&1 || printf '%s ' "$p"; done; }
M="$(manca cmake g++ pkg-config libsdl2-dev)"
if [ -n "$M" ]; then sudo apt-get update && sudo apt-get install -y --no-upgrade $M; fi
M="$(manca libsdl2-image-dev)"   # facoltativa: solo cartelloni sponsor PNG
if [ -n "$M" ]; then sudo apt-get install -y --no-upgrade $M || echo "avviso: SDL2_image non installata, il gioco parte lo stesso"; fi
echo "2/4 compilazione (qualche minuto; -j2 per non scaldare il Pi)"
cmake -S "$SRC/motore_nuovo" -B "$B" -DCMAKE_BUILD_TYPE=Release
cmake --build "$B" --target sglavinated_runner -j2
if [ ! -x "$B/sglavinated_runner" ]; then
    echo "ERRORE: il gioco non e' stato compilato (manca SDL2? sudo apt-get install libsdl2-dev)"
    exit 1
fi
echo "3/4 lanciatore nel menu Ports"
mkdir -p "$PORTS"
rm -f "$PORTS/Sglavinated Really Open.sh"   # nome vecchio, sostituito da OpenRun.sh
L="$PORTS/OpenRun.sh"
printf '#!/bin/sh\nexec "%s/sglavinated_runner" --fullscreen --data "%s"\n' "$B" "$SRC" > "$L"
chmod +x "$L"
echo "4/4 fatto. Riavvia EmulationStation (Start > Quit > Restart EmulationStation): OpenRun e' in Ports."
echo "Per uscire dal gioco: Select+Start sul pad oppure ESC sulla tastiera."
