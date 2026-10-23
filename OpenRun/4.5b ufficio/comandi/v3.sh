#!/bin/sh
# Comando unico della build (nomi interni V3). Uso: sh v3.sh <start|stop|stop-subito|stato|vivo|pannello|aggiorna>
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
export V3_BASE="$BASE"
S="$BASE/sglavinated_src"; T="$BASE/strumenti"
pid_del_progetto() {
  check_pid="$1"; check_path="$2"
  case "$check_pid" in ''|*[!0-9]*) return 1 ;; esac
  kill -0 "$check_pid" 2>/dev/null || return 1
  check_command="$(ps -p "$check_pid" -o command= 2>/dev/null)"
  case "$check_command" in *"$check_path"*) return 0 ;; *) return 1 ;; esac
}
ciclo_attivo() {
  [ -f "$S/.ciclo.pid" ] || return 1
  pid="$(cat "$S/.ciclo.pid")"
  pid_del_progetto "$pid" "$T/ciclo.sh"
}
PANNELLO_PORT=8090
PANNELLO_PID="$S/.pannello.pid"
pannello_attivo() {
  [ -f "$PANNELLO_PID" ] || return 1
  pid="$(cat "$PANNELLO_PID")"
  pid_del_progetto "$pid" "$T/pannello.py"
}
apri_pannello() {
  if ! pannello_attivo; then
    nohup python3 "$T/pannello.py" --porta "$PANNELLO_PORT" > "$S/pannello.log" 2>&1 &
    echo "$!" > "$PANNELLO_PID"
    sleep 1
  fi
  pid="$(cat "$PANNELLO_PID" 2>/dev/null)"
  for _ in 1 2 3 4 5; do
    if curl -fsS "http://localhost:$PANNELLO_PORT/" >/dev/null 2>&1; then
      command -v open >/dev/null 2>&1 && open "http://localhost:$PANNELLO_PORT/"
      echo "Pannello: http://localhost:$PANNELLO_PORT/"
      return 0
    fi
    sleep 1
  done
  pid_del_progetto "$pid" "$T/pannello.py" && kill "$pid" 2>/dev/null
  rm -f "$PANNELLO_PID"
  echo "ERRORE: pannello non disponibile su http://localhost:$PANNELLO_PORT/; vedi sglavinated_src/pannello.log." >&2
  return 1
}
ferma_tutto() {
  touch "$S/STOP"
  if [ -f "$S/.ciclo.pid" ]; then
    pid="$(cat "$S/.ciclo.pid")"
    case "$pid" in
      ''|*[!0-9]*) echo "ERRORE: PID del ciclo non valido; non termino altri processi." >&2; return 1 ;;
      *)
        if pid_del_progetto "$pid" "$T/ciclo.sh"; then
          kill -TERM "$pid" 2>/dev/null || { echo "ERRORE: impossibile fermare il ciclo $pid." >&2; return 1; }
          sleep 2
          if kill -0 "$pid" 2>/dev/null; then
            echo "ERRORE: il ciclo non si e' fermato; nessun processo esterno e' stato terminato." >&2
            return 1
          fi
        else
          rm -f "$S/.ciclo.pid"
        fi
    esac
  fi
}
case "${1:-stato}" in
  start)
    if ciclo_attivo; then echo "Il ciclo e' gia' attivo: non ne avvio un secondo. Usa: v3 stato"; exit 0; fi
    rm -f "$S/STOP"; apri_pannello || exit 1; cd "$T" && exec sh start_process_brains.sh ;;
  stop)       touch "$S/STOP"; echo "STOP creato: il ciclo si ferma a fine giro (puo' richiedere qualche minuto)." ;;
  stop-subito) ferma_tutto || exit 1; echo "Ciclo e richiesta OpenCode corrente fermati." ;;
  aggiorna)
    ferma_tutto || exit 1
    # Anche il pannello si riavvia: Python legge pannello.py solo all'avvio.
    if pannello_attivo; then kill "$(cat "$PANNELLO_PID")" 2>/dev/null; fi; rm -f "$PANNELLO_PID"
    rm -f "$S/STOP"; apri_pannello || exit 1; cd "$T" && exec sh start_process_brains.sh ;;
  stato)
    echo "== ciclo"; if ciclo_attivo; then ps -p "$(cat "$S/.ciclo.pid")" -o pid,etime,command; echo "ciclo: ATTIVO"; else echo "ciclo: fermo"; fi
    echo "== pannello"; if pannello_attivo; then ps -p "$(cat "$PANNELLO_PID")" -o pid,etime,command; else echo "pannello: fermo"; fi
    echo "== ultime righe del ciclo"; grep '^\[ciclo\]' "$S/notte.log" 2>/dev/null | tail -8 | cut -c1-150
    if [ -f "$S/notte.log" ]; then
      if [ "$(uname)" = Darwin ]; then mt=$(stat -f %m "$S/notte.log"); else mt=$(stat -c %Y "$S/notte.log"); fi
      echo "== ultimo aggiornamento log: $(( ($(date +%s) - mt) / 60 )) min fa"
    else echo "== log non ancora creato"; fi
    sh "$T/blocchi.sh" ;;
  vivo)     exec sh "$T/vivo.sh" ;;
  pannello) apri_pannello ;;
  *) echo "Uso: v3.sh start | stop | stop-subito | stato | vivo | pannello | aggiorna" ;;
esac
