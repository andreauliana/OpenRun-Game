#!/bin/sh
# Controllo completo OpenRun 4.6. Uso: sh strumenti/verifica.sh
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"; E=0
ok(){ echo "OK      $1"; }; ko(){ echo "ERRORE  $1"; E=1; }
for f in "$P"/.opencode/agents/*.md; do
  n=$(basename "$f" .md)
  [ "$(head -1 "$f")" = "---" ] || { ko "$n: manca '---' iniziale"; continue; }
  grep -q '^description:' "$f" && grep -q '^mode:' "$f" && grep -q '^model:' "$f" || { ko "$n: description/mode/model mancanti"; continue; }
  [ "$(grep -c '^---$' "$f")" -ge 2 ] || { ko "$n: manca '---' di chiusura"; continue; }
  ok "agente $n ($(grep -m1 '^model:' "$f" | sed 's/^model: *//'))"
done
if command -v python3 >/dev/null 2>&1; then
  python3 -c "import json,sys; json.load(open(sys.argv[1]))" "$P/opencode.json" 2>/dev/null && ok "opencode.json valido" || ko "opencode.json non valido"
else echo "SALTATO python3 non trovato (controllo JSON)"; fi
for r in AGENTS.md VISIONE.md RICHIESTA.md SCAMBIO.md opencode.json "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/DIREZIONE_ARTISTICA.md" "UFFICIO/Dipartimento Grafico/Libreria/assets_originali/CREDITI.md"; do
  [ -f "$P/$r" ] && ok "file $r" || ko "manca $r"
done
for r in SCALETTA.md COSA_FA_L_UMANO.md RAPPORTO_FATTIBILITA.md RISORSE_TECNICHE.md; do
  [ -f "$BASE/$r" ] && ok "file $r" || ko "manca $r"
done
[ -f "$BASE/TAPPE.md" ] && ok "file TAPPE.md (roadmap)" || ko "manca TAPPE.md (roadmap)"
for s in "$BASE"/strumenti/*.sh "$BASE"/run.sh; do sh -n "$s" 2>/dev/null && ok "sintassi $(basename "$s")" || ko "sintassi $(basename "$s")"; done
# nessun materiale Sega nella cartella
N=$(find "$P" -path "$P/outrun_src" -prune -o \( -name '*.gen' -o -name '*.smd' -o -name 'OutRun (USA, Europe).md' \) -print 2>/dev/null | wc -l | tr -d ' ')
[ "$N" = "0" ] && ok "nessuna ROM Sega presente" || ko "trovate $N ROM/gen in sglavinated_src (da togliere)"
# modelli presenti nella panchina
for f in "$P"/.opencode/agents/*.md; do
  m=$(grep -m1 '^model:' "$f" | sed 's/^model: *//'); grep -qx "$m" "$BASE/strumenti/panchina.txt" || echo "NOTA    $(basename "$f" .md): $m non e' in panchina.txt"
done
command -v opencode >/dev/null 2>&1 && ok "opencode installato" || echo "NOTA    opencode non trovato nel PATH"
command -v claude >/dev/null 2>&1 && ok "comando claude presente (review automatica possibile)" || echo "NOTA    comando claude assente: review a mano"
# chiavi duplicate nel frontmatter degli agenti
for f in "$BASE"/sglavinated_src/.opencode/agents/*.md; do
  python3 - "$f" <<'PY' || ko "$(basename "$f"): chiave duplicata nel frontmatter"
import re,sys,collections
fm=open(sys.argv[1]).read().split('---')[1]
c=collections.Counter(re.findall(r'^\s+"([^"]+)":',fm,re.M))
sys.exit(1 if [k for k,v in c.items() if v>1 and k!='*'] else 0)
PY
done
if [ $E -eq 0 ]; then echo "== TUTTO OK =="; else echo "== CI SONO ERRORI =="; fi
exit $E
