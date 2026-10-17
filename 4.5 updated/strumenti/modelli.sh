#!/bin/sh
# Mostra chi usa quale modello e se esiste ancora in OpenCode.
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
A="$BASE/sglavinated_src/.opencode/agents"
echo "== Assegnazioni =="
for f in "$A"/*.md; do
  printf '%-12s %s\n' "$(basename "$f" .md)" "$(grep -m1 '^model:' "$f" | sed 's/^model: *//')"
done
echo "== Controllo con opencode models =="
LIST="$(opencode models 2>/dev/null)"
[ -n "$LIST" ] || { echo "opencode non risponde: controlla l'installazione"; exit 1; }
for f in "$A"/*.md; do
  m="$(grep -m1 '^model:' "$f" | sed 's/^model: *//')"; n="$(basename "$f" .md)"
  if echo "$LIST" | grep -qx "$m"; then echo "OK     $n: $m"
  else echo "MANCA  $n: $m  -> sh strumenti/cambia_modello_auto.sh $n"; fi
done
