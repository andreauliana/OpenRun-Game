#!/bin/sh
# Chi e' attivo e blocchi aperti con ruolo. Usato da `v3.sh stato` e da `vivo.sh`.
# Legenda: > = agente in turno adesso, * = agente della squadra del ciclo.
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
S="$BASE/sglavinated_src"
cur=""; ini=""
if [ -f "$S/.ciclo.pid" ] && [ -f "$S/.agente_corrente" ]; then
  pid="$(cat "$S/.ciclo.pid")"
  case "$pid" in ''|*[!0-9]*) ;; *)
    case "$(ps -p "$pid" -o command= 2>/dev/null)" in *"$BASE/strumenti/ciclo.sh"*) read cur ini < "$S/.agente_corrente" ;; esac ;;
  esac
fi
sq="$(cat "$S/.agenti_attivi" 2>/dev/null)"
echo "== agenti"
if [ -n "$cur" ]; then
  case "$ini" in ''|*[!0-9]*) m="?" ;; *) m=$(( ($(date +%s) - ini) / 60 )) ;; esac
  echo "  in turno ora: @$cur (da $m min)"
else
  echo "  in turno ora: nessuno"
fi
echo "  squadra del ciclo: ${sq:-nessuna}"
echo "== blocchi aperti (> in turno, * in squadra)"
awk -v cur="$cur" -v sq="$sq" '
BEGIN { n = split(sq, A, " "); for (i = 1; i <= n; i++) SQ[A[i]] = 1 }
/^## BLOCCO/ {
  s = $0; sub(/^## BLOCCO +/, "", s); id = s; sub(/[ \t].*/, "", id)
  r = ""; t = s; mark = " "
  while (match(t, /@[a-z]+/)) {
    a = substr(t, RSTART + 1, RLENGTH - 1)
    r = r (r == "" ? "" : "+") "@" a
    if (a == cur) mark = ">"; else if ((a in SQ) && mark != ">") mark = "*"
    t = substr(t, RSTART + RLENGTH)
  }
  if (r == "") r = "-"
  next
}
/^\| *Stato *\|/ {
  if (match($0, /\[.\]/)) {
    m = substr($0, RSTART, 3)
    if (m != "[x]" && id != "0" && id != "X") printf "  %s %-8s %-20s %s\n", mark, id, r, m
  }
}' "$S/RICHIESTA.md"
