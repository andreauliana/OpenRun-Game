#!/bin/sh
# Cruscotto dal vivo (si aggiorna ogni 15 s). Uso: sh strumenti/vivo.sh   (Ctrl+C per uscire)
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
S="$BASE/sglavinated_src"
while :; do
  ciclo="FERMO"; agente=""
  if [ -f "$S/.ciclo.pid" ]; then
    pid="$(cat "$S/.ciclo.pid")"
    case "$pid" in
      ''|*[!0-9]*) ;;
      *)
        cmd="$(ps -p "$pid" -o command= 2>/dev/null)"
        case "$cmd" in *"$BASE/strumenti/ciclo.sh"*) ciclo="ATTIVO" ;; esac
        ;;
    esac
  fi
  if [ "$ciclo" = "ATTIVO" ] && [ -f "$S/.agente_corrente" ]; then
    read agente _ < "$S/.agente_corrente"
  fi
  clear
  echo "=== $(date '+%H:%M:%S')  ciclo: $ciclo  agente: ${agente:-nessuno} ==="
  echo; echo "--- ultime righe del log ---"
  tail -n 14 "$S/notte.log" 2>/dev/null | sed 's/\x1b\[[0-9;]*m//g' | cut -c1-110
  echo; echo "--- spazi agenti (file / ultimo aggiornamento) ---"
  for d in "$S"/bacheche/*/; do n=$(basename "$d"); c=$(find "$d" -type f | wc -l | tr -d ' '); l=$(find "$d" -type f -exec stat -f '%m' {} + 2>/dev/null | sort -n | tail -1); printf "%-12s %3s file  %s\n" "$n" "$c" "$( [ -n "$l" ] && date -r "$l" '+%H:%M' || echo - )"; done
  echo "out/: $(find "$S/out" -type f | wc -l | tr -d ' ') file   Libreria: $(find "$S/UFFICIO/Dipartimento Grafico/Libreria" -type f 2>/dev/null | wc -l | tr -d ' ') file"
  echo; echo "--- STATUS_OFFICINA (ultime righe) ---"; grep -A1 "^| Gate\|^| Ultimo exit\|^| Performance" "$S/UFFICIO/Officina/STATUS_OFFICINA.md" 2>/dev/null | head -6 | cut -c1-110 || echo "(non trovato)"
  echo; V3_BASE="$BASE" sh "$BASE/strumenti/blocchi.sh"
  sleep 15
done
