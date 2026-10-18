#!/bin/sh
# Giro parallelo: avvia direttamente ogni agente (senza toccare la corsa in atto). Uso: sh strumenti/giro.sh
# Si ferma con STOP/FINITO in sglavinated_src o dopo MAX_ORE (default 10).
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"; LOG="$P/giro.log"; MAX_ORE="${MAX_ORE:-10}"; t0=$(date +%s); r=0
oc() {
  (cd "$P" && exec opencode run --auto "$@") >> "$LOG" 2>&1 &
  pid=$!; ( sleep "${OC_TIMEOUT:-1500}"; kill "$pid" 2>/dev/null ) >/dev/null 2>&1 & w=$!
  wait "$pid"; rc=$?; kill "$w" 2>/dev/null; return $rc
}
cd "$P" || exit 1
while :; do
  [ -f STOP ] || [ -f FINITO ] && { echo "[giro] STOP/FINITO"; break; }
  [ $(( ($(date +%s) - t0) / 3600 )) -ge "$MAX_ORE" ] && { echo "[giro] limite ore"; break; }
  r=$((r+1)); echo "[giro] round $r $(date +%H:%M)" | tee -a "$LOG"
  for ag in ${AGENTI_GIRO:-sviluppatore collaudatore grafico ricercatore}; do
    [ -f STOP ] && break
    # Avvia l'agente solo se ha almeno un blocco [ ] o [~] in RICHIESTA.md (stessa regola di ciclo.sh):
    # un avvio a vuoto ricarica comunque le istruzioni e consuma crediti.
    awk -v ag="$ag" '/^## BLOCCO/ { a = ($0 ~ "@" ag "([^a-z]|$)") } /^\| *Stato/ { if (a && ($0 ~ /\[ \]/ || $0 ~ /\[~\]/)) t = 1 } END { exit !t }' RICHIESTA.md || { echo "[giro] $ag: nessun blocco aperto, salto" | tee -a "$LOG"; continue; }
    echo "[giro] avvio $ag $(date +%H:%M)" | tee -a "$LOG"
    oc --agent "$ag" "Round $r. Sei @$ag. Leggi AGENTS.md (unica fonte di regole) e RICHIESTA_APERTA.md. Lavora SOLO sui blocchi con Stato [ ] o [~] assegnati a te (max 15 minuti), scrivi solo nei percorsi del tuo ruolo. RICHIESTA.md e SCAMBIO.md sono di sola lettura: la voce va in VOCE_SCAMBIO_<n>.md nella tua cartella. Fine turno: UNA riga in bacheche/$ag/diario.md e risultati in out/$ag/. Se non hai blocchi assegnati, fermati senza inventare lavoro. Non delegare, non cambiare modello." || echo "[giro] $ag in errore" | tee -a "$LOG"
  done
  sleep 600
done
