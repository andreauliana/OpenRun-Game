#!/bin/sh
# Edizione "Really Open" (motore C++): compila sul Raspberry Pi con RetroPie e aggiunge il gioco al menu Ports.
# Uso, dal terminale del Pi:  sh "$HOME/4.5 updated/pi/compila_e_installa.sh"
set -e
DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$DIR/sglavinated_src"
B="$SRC/motore_nuovo/build_pi"
echo "1/4 pacchetti necessari (chiede la password del Pi)"
sudo apt-get install -y cmake g++ libsdl2-dev libsdl2-image-dev
echo "2/4 compilazione (qualche minuto; -j2 per non scaldare il Pi)"
cmake -S "$SRC/motore_nuovo" -B "$B" -DCMAKE_BUILD_TYPE=Release
cmake --build "$B" -j2
echo "3/4 lanciatore nel menu Ports"
L="$HOME/RetroPie/roms/ports/Sglavinated Really Open.sh"
mkdir -p "$HOME/RetroPie/roms/ports"
printf '#!/bin/sh\nexec "%s/sglavinated_runner" --fullscreen --data "%s"\n' "$B" "$SRC" > "$L"
chmod +x "$L"
echo "4/4 fatto. Riavvia EmulationStation (Start > Quit > Restart EmulationStation): il gioco e' in Ports."
echo "Per uscire dal gioco: ESC sulla tastiera oppure Select+Start sul pad."
