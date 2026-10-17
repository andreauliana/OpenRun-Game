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
    echo "[giro] avvio $ag $(date +%H:%M)" | tee -a "$LOG"
    oc --agent "$ag" "Round $r. Sei @$ag. Leggi AGENTS.md e RICHIESTA.md. Lavora SOLO sui blocchi con Stato [ ] o [~] assegnati a te (max 20 minuti), scrivi nelle tue cartelle (regola 15). Aggiorna Stato e una riga in SCAMBIO.md. A fine lavoro scrivi 3-5 righe in bacheche/$ag/diario.md e metti i risultati condivisi in out/$ag/. Se non hai blocchi assegnati, fermati senza inventare lavoro. BACHECA_PUBBLICA.md e' facoltativa e non richiede risposte. Non delegare, non cambiare modello." || echo "[giro] $ag in errore" | tee -a "$LOG"
  done
  sleep 600
done
