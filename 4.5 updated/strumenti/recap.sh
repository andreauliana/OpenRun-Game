#!/bin/sh
# Recap del mattino: tutto quello che serve in un colpo solo.
# Uso: sh strumenti/recap.sh
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"; cd "$P" || exit 1
echo "=== LOG (ultime 25 righe) ==="; tail -25 notte.log 2>/dev/null || echo "nessun notte.log"
echo; echo "=== STATO BLOCCHI ==="; grep -n '^## BLOCCO\|^| Stato' RICHIESTA.md | paste - - 2>/dev/null | head -30
echo; echo "=== RIEPILOGO_MATTINA ==="; cat RIEPILOGO_MATTINA.md 2>/dev/null || echo "non ancora scritto (guarda notte.log)"
echo; echo "=== DECISIONI APERTE ==="; grep -n '^## D[0-9].*APERTA' DECISIONI_ANDREA.md
echo; echo "=== CHECKPOINT (verdetti) ==="; grep -n 'CHECKPOINT\|VERDETTO' SCAMBIO.md | tail -12
echo; echo "=== QUOTE ==="; python3 motore_nuovo/strumenti_py/quota_open.py | tail -3; python3 motore_nuovo/strumenti_py/quota_bacheche.py | tail -3
echo; echo "=== BACHECA PUBBLICA (ultime righe) ==="; tail -24 BACHECA_PUBBLICA.md; echo "Righe totali: $(wc -l < BACHECA_PUBBLICA.md | tr -d ' ')"
echo; echo "Per Claude app: apri PACCHETTO_PER_CLAUDE_APP.md"
