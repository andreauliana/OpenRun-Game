#!/bin/sh
# Uso: sh strumenti/cambia_modello_auto.sh <agente>
# Passa l'agente al modello successivo della panchina (strumenti/panchina.txt).
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
HERE="$BASE/strumenti"
[ $# -eq 1 ] || { echo "Uso: $0 <agente>"; exit 1; }
F="$BASE/sglavinated_src/.opencode/agents/$1.md"
[ -f "$F" ] || { echo "Agente sconosciuto: $1"; exit 1; }
CUR="$(grep -m1 '^model:' "$F" | sed 's/^model: *//')"
# Panchina senza i modelli esclusi (panchina_esclusi.txt: bloccati nel paese o da evitare).
ESC="$HERE/panchina_esclusi.txt"; [ -f "$ESC" ] || : > "$ESC"
LISTA="$(grep -v '^#' "$HERE/panchina.txt" | grep . | grep -vxF -f "$ESC")"
[ -n "$LISTA" ] || { echo "Nessun modello utilizzabile in panchina.txt (tutti esclusi)"; exit 1; }
NEXT="$(printf '%s\n' "$LISTA" | awk -v cur="$CUR" '{a[NR]=$0;n=NR} END{for(i=1;i<=n;i++) if(a[i]==cur){print a[(i%n)+1]; exit} print a[1]}')"
[ -n "$NEXT" ] || NEXT="$(printf '%s\n' "$LISTA" | head -1)"
V3_BASE="$BASE" sh "$HERE/set_model.sh" "$1" "$NEXT"
echo "(era: $CUR)"
