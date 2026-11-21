#!/bin/sh
# Uso: sh strumenti/set_model.sh <agente> <provider/modello>
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"
[ $# -eq 2 ] || { echo "Uso: $0 <agente> <provider/modello>"; exit 1; }
F="$P/.opencode/agents/$1.md"
[ -f "$F" ] || { echo "Agente sconosciuto: $1"; ls "$P/.opencode/agents" | sed 's/\.md$//'; exit 1; }
sed "s#^model:.*#model: $2#" "$F" > "$F.tmp" && mv "$F.tmp" "$F"
if [ "$1" = "capo" ]; then
  sed "s#\"model\": *\"[^\"]*\"#\"model\": \"$2\"#" "$P/opencode.json" > "$P/opencode.json.tmp" && mv "$P/opencode.json.tmp" "$P/opencode.json"
fi
echo "$1 -> $2"
