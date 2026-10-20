#!/bin/sh
# SPDX-License-Identifier: MIT
# Verifica SDL_Init tramite il runner reale, senza CTest o finestra di gioco.
cd "$(dirname "$0")/../../.." || exit 9  # TOOL -> Officina -> UFFICIO -> radice

RUNNER=${1:-UFFICIO/Officina/build/sglavinated_runner}
if [ ! -x "$RUNNER" ]; then
  echo "Runner non trovato o non eseguibile: $RUNNER" >&2
  echo "Compilare prima il target sglavinated_runner." >&2
  exit 2
fi

OUTPUT=$("$RUNNER" --check-sdl-init 2>&1)
STATUS=$?
printf '%s\n' "$OUTPUT"
if [ "$STATUS" -ne 0 ]; then
  echo "Controllo SDL_Init fallito (exit $STATUS)." >&2
  exit "$STATUS"
fi
if ! printf '%s\n' "$OUTPUT" | grep -F "SDL_Init: OK" >/dev/null; then
  echo "Il runner e' terminato senza confermare SDL_Init." >&2
  exit 3
fi
echo "Controllo SDL_Init: OK"
