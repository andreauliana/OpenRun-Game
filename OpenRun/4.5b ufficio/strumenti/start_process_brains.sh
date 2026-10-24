#!/bin/sh
# START PROCESS BRAINS: preflight, avvia il ciclo degli agenti in background e mostra il cruscotto dal vivo (vivo.sh).
# Uso: sh comandi/v3.sh start
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
if ! command -v opencode >/dev/null 2>&1; then echo "ERRORE: opencode non installato/non nel PATH. Non parto."; exit 1; fi
if ! (cd "$BASE/sglavinated_src" && opencode models >/dev/null 2>&1); then echo "ERRORE: opencode non carica la configurazione (opencode.json). Non parto, per non lasciare gli agenti senza limiti di cartella. Mandami l'output di: cd \"$BASE/sglavinated_src\" && opencode models"; exit 1; fi
# prova reale: OpenCode deve trovare l'agente capo (provo prima normale, poi con --standalone)
P="$BASE/sglavinated_src"; OC_STANDALONE=0
if [ -f "$P/.ciclo.pid" ]; then
  C_PID="$(cat "$P/.ciclo.pid")"
  case "$C_PID" in
    ''|*[!0-9]*) ;;
    *)
      C_CMD="$(ps -p "$C_PID" -o command= 2>/dev/null)"
      case "$C_CMD" in *"$BASE/strumenti/ciclo.sh"*) echo "[brains] il ciclo di questa build e gia attivo (pid $C_PID): mostro solo il cruscotto."; exec sh "$BASE/strumenti/vivo.sh" ;; esac
      ;;
  esac
fi
# se il modello free e' al limite di richieste, aspetto (5 min x fino a 24 volte) invece di rinunciare
T=""; RC=1
for i in $(seq 1 24); do
  T="$(cd "$P" && opencode run --agent capo "Rispondi solo con la parola PRONTO." 2>&1)"; RC=$?
  if echo "$T" | grep -qi "usage limit"; then
    # Limite gratuito sul modello del capo: provo gli altri modelli della panchina (come fa il ciclo).
    CUR="$(grep -m1 '^model:' "$P/.opencode/agents/capo.md" | sed 's/^model: *//')"; OK_PAN=0
    while read -r m; do
      [ -n "$m" ] && [ "$m" != "$CUR" ] || continue
      V3_BASE="$BASE" sh "$BASE/strumenti/set_model.sh" capo "$m" >/dev/null
      T="$(cd "$P" && opencode run --agent capo "Rispondi solo con la parola PRONTO." 2>&1 < /dev/null)"; RC=$?
      if ! echo "$T" | grep -qiE "usage limit|rate limit"; then echo "[brains] limite gratuito su $CUR: il capo passa a $m"; OK_PAN=1; break; fi
    done < "$BASE/strumenti/panchina.txt"
    [ "$OK_PAN" -eq 1 ] && break
    V3_BASE="$BASE" sh "$BASE/strumenti/set_model.sh" capo "$CUR" >/dev/null
    echo "LIMITE RAGGIUNTO: i crediti gratuiti di OpenCode sono finiti su tutti i modelli della panchina (strumenti/panchina.txt). Non parto: riprova piu' tardi (o domani)." >&2; exit 1
  fi
  if ! echo "$T" | grep -qi "rate limit"; then break; fi
  if [ "$i" -eq 24 ]; then echo "ERRORE: rate limit persistente; non avvio il ciclo." >&2; exit 1; fi
  echo "[brains] limite di richieste raggiunto, attendo 5 minuti ($i/24) - $(date +%H:%M)"; sleep 300
done
# Riusa la risposta del controllo: una chiamata basta a provare che capo risponde.
if [ "$RC" -ne 0 ] || echo "$T" | grep -qi "not found"; then
  T2="$(cd "$P" && opencode run --standalone --agent capo "Rispondi solo con la parola PRONTO." 2>&1)"; RC2=$?
  if [ "$RC2" -eq 0 ] && ! echo "$T2" | grep -qi "not found"; then OC_STANDALONE=1; T="$T2"
  else echo "ERRORE: OpenCode non esegue l'agente 'capo'. Non parto."; echo "--- prova 1 ---"; echo "$T" | head -8; echo "--- prova 2 (--standalone) ---"; echo "$T2" | head -8; echo "Diagnosi: cd \"$P\" && opencode debug agents && opencode debug config"; echo "Mandami questo output."; exit 1; fi
fi
export OC_STANDALONE
echo "OK OpenCode esegue l'agente capo (standalone=$OC_STANDALONE)"
sh "$BASE/strumenti/verifica.sh" | tail -15
LOG="$BASE/sglavinated_src/notte.log"
echo "[brains] avvio $(date)" >> "$LOG"
export MAX_CICLI="${MAX_CICLI:-1000}"
if command -v caffeinate >/dev/null 2>&1; then
  caffeinate -i sh "$BASE/strumenti/ciclo.sh" >> "$LOG" 2>&1 &
else
  sh "$BASE/strumenti/ciclo.sh" >> "$LOG" 2>&1 &
fi
echo "[brains] ciclo avviato in background (pid $!). Ctrl+C chiude solo il cruscotto: il ciclo continua. Per fermarlo: touch sglavinated_src/STOP"
sleep 2
exec sh "$BASE/strumenti/vivo.sh"
