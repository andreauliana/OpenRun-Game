#!/bin/sh
# Versione browser di Open Run (Emscripten). Prepara web/sito/ da copiare sul sito:
#   sito/404.html          -> pagina 404 per un sito normale (radice del sito)
#   sito/404.php           -> 404 del tema WordPress di andreauliana.com (va nel tema al posto di 404.php)
#   sito/openrun/          -> gioco (openrun.js, .wasm, .data, font, css, js): radice del sito, o dentro il tema per WordPress
# Serve emsdk attivo (emcc nel PATH): vedi web/LEGGIMI.md.
set -e
QUI="$(cd "$(dirname "$0")" && pwd)"
MOTORE="$(cd "$QUI/.." && pwd)"
BUILD="$MOTORE/../build_web"
SITO="$QUI/sito"
FONT="$MOTORE/../UFFICIO/Dipartimento Grafico/Libreria/assets_originali/font/born2bsporty/Born2bSportyV2.ttf"

if ! command -v emcmake >/dev/null 2>&1; then
    echo "emcmake non trovato: attiva emsdk prima (source ~/emsdk/emsdk_env.sh)" >&2
    exit 1
fi

emcmake cmake -S "$MOTORE" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build "$BUILD" --target openrun -j 4

rm -rf "$SITO"
mkdir -p "$SITO/openrun"
cp "$BUILD/openrun.js" "$BUILD/openrun.wasm" "$BUILD/openrun.data" "$SITO/openrun/"
cp "$FONT" "$SITO/openrun/"
cp "$QUI/openrun.css" "$QUI/avvio.js" "$SITO/openrun/"
# Le due pagine: il blocco del gioco (blocco.html) va al posto di <!-- OPENRUN_BLOCCO -->.
metti_blocco() { awk -v b="$QUI/blocco.html" '/<!-- OPENRUN_BLOCCO -->/ { while ((getline r < b) > 0) print r; next } { print }' "$1" > "$2"; }
metti_blocco "$QUI/404.html" "$SITO/404.html"
metti_blocco "$QUI/wordpress/404.php" "$SITO/404.php"
echo "Pronto: $SITO"
du -sh "$SITO/openrun"
echo "Prova in locale: cd \"$SITO\" && python3 -m http.server 8000, poi apri http://localhost:8000/404.html"
