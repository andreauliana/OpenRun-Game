#!/bin/sh
# Mostra chi usa quale modello, se esiste ancora in OpenCode, e i modelli free nuovi non ancora in panchina.
# Uso: sh comandi/v3.sh modelli
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
A="$BASE/sglavinated_src/.opencode/agents"; PAN="$BASE/strumenti/panchina.txt"; ESC="$BASE/strumenti/panchina_esclusi.txt"
echo "== Assegnazioni =="
for f in "$A"/*.md; do
  printf '%-17s %s\n' "$(basename "$f" .md)" "$(grep -m1 '^model:' "$f" | sed 's/^model: *//')"
done
echo "== Controllo con opencode models =="
LIST="$(cd "$BASE/sglavinated_src" && opencode models opencode 2>/dev/null)"
[ -n "$LIST" ] || LIST="$(cd "$BASE/sglavinated_src" && opencode models 2>/dev/null)"
[ -n "$LIST" ] || { echo "opencode non risponde: controlla l'installazione"; exit 1; }
for f in "$A"/*.md; do
  m="$(grep -m1 '^model:' "$f" | sed 's/^model: *//')"; n="$(basename "$f" .md)"
  if grep -qxF "$m" "$ESC" 2>/dev/null; then echo "ESCLUSO $n: $m  -> sh strumenti/cambia_modello_auto.sh $n"
  elif echo "$LIST" | grep -qxF "$m"; then echo "OK      $n: $m"
  else echo "MANCA   $n: $m  -> sh strumenti/cambia_modello_auto.sh $n"; fi
done
echo "== Panchina (strumenti/panchina.txt) =="
grep -v '^#' "$PAN" | grep . | while read -r m; do
  if grep -qxF "$m" "$ESC" 2>/dev/null; then s="escluso"; elif echo "$LIST" | grep -qxF "$m"; then s="ok"; else s="NON PIU' IN OPENCODE: toglilo"; fi
  printf '  %-40s %s\n' "$m" "$s"
done
echo "== Modelli free di OpenCode non in panchina (valuta se aggiungerli) =="
echo "$LIST" | grep -E '^opencode/.*(-free|big-pickle)$' | while read -r m; do
  grep -qxF "$m" "$PAN" || grep -qxF "$m" "$ESC" 2>/dev/null || echo "  $m"
done
