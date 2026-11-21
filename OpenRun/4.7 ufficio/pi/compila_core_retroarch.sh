#!/bin/sh
# Core libretro (motore C++): compila sul Raspberry Pi con RetroPie e lo avvia dentro RetroArch dal menu Ports.
# Build separata da compila_e_installa.sh (cartella build_libretro, niente SDL).
# Uso, dal terminale del Pi:  sh "$HOME/4.5 updated/pi/compila_core_retroarch.sh"
set -e
DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$DIR/sglavinated_src"
B="$SRC/motore_nuovo/build_libretro"
RA=/opt/retropie/emulators/retroarch/bin/retroarch
CFG=/opt/retropie/configs/all/retroarch.cfg
echo "1/4 pacchetti necessari (chiede la password del Pi)"
sudo apt-get install -y cmake g++
echo "2/4 compilazione del core (qualche minuto; -j2 per non scaldare il Pi)"
cmake -S "$SRC/motore_nuovo" -B "$B" -DCMAKE_BUILD_TYPE=Release -DBUILD_LIBRETRO=ON
cmake --build "$B" --target sglavinated_libretro -j2
echo "3/4 lanciatore nel menu Ports"
L="$HOME/RetroPie/roms/ports/Sglavinated RetroArch.sh"
mkdir -p "$HOME/RetroPie/roms/ports"
printf '#!/bin/sh\nexec "%s" --config "%s" -L "%s/sglavinated_libretro.so" "%s/motore_nuovo/dati/percorso_gara.json"\n' \
    "$RA" "$CFG" "$B" "$SRC" > "$L"
chmod +x "$L"
echo "4/4 fatto. Riavvia EmulationStation (Start > Quit > Restart EmulationStation): il gioco e' in Ports."
echo "Core: $B/sglavinated_libretro.so"
echo "Per uscire: Hotkey+Start sul pad (di solito Select+Start) oppure ESC sulla tastiera."
