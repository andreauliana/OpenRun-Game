#!/bin/sh
# Riserva locale con Ollama: quando i crediti gratuiti di OpenCode finiscono su tutta la panchina,
# tutti gli agenti passano al modello locale (nessun credito, gira sul Mac) e tornano ai modelli free dopo RISERVA_ORE.
# Lavoro minimo: un agente alla volta, poi RIPOSO_LOCALE secondi di pausa (default 600) per non scaldare il Mac.
# Uso: sh strumenti/modo_locale.sh <prepara|disponibile|entra [fisso]|esci|attivo|scaduto|stato>
#   prepara     scarica il modello base e crea "openrun-locale" con contesto lungo (una volta sola, serve Ollama aperto)
#   disponibile exit 0 se Ollama risponde e openrun-locale esiste
#   entra       salva i modelli attuali degli agenti e li passa tutti a ollama/openrun-locale
#               (con "fisso" resta in locale finche' non lanci "esci", senza tornare ai free dopo RISERVA_ORE)
#   esci        rimette i modelli salvati (i modelli free) e chiude il modo locale
#   attivo      exit 0 se il modo locale e' acceso
#   scaduto     exit 0 se il modo locale e' acceso da piu' di RISERVA_ORE ore (default 6)
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"; HERE="$BASE/strumenti"
STATO="$P/.modo_locale"
URL="${OLLAMA_URL:-http://localhost:11434}"
NOME="openrun-locale"; MODELLO="ollama/$NOME"
# Mac 16 GB, carico leggero: qwen3.5:4b (circa 3,5 GB, supporta tools) con contesto 16k e 4 thread.
# Piu' qualita' ma piu' calore: OLLAMA_BASE=qwen3.5:9b OLLAMA_CTX=32768 sh strumenti/modo_locale.sh prepara
BASE_OLLAMA="${OLLAMA_BASE:-qwen3.5:4b}"; CTX="${OLLAMA_CTX:-16384}"; THREAD="${OLLAMA_THREAD:-4}"
case "${1:-stato}" in
  prepara)
    command -v ollama >/dev/null 2>&1 || { echo "ERRORE: ollama non installato. Scaricalo da https://ollama.com/download e aprilo."; exit 1; }
    curl -fsS "$URL/api/tags" >/dev/null 2>&1 || { echo "ERRORE: Ollama non risponde su $URL: apri l'app Ollama e riprova."; exit 1; }
    ollama pull "$BASE_OLLAMA" || exit 1
    # Ollama di default usa 4096 token di contesto: troppo pochi per gli agenti. Il Modelfile lo alza.
    MF="$(mktemp)"; printf 'FROM %s\nPARAMETER num_ctx %s\nPARAMETER num_thread %s\n' "$BASE_OLLAMA" "$CTX" "$THREAD" > "$MF"
    ollama create "$NOME" -f "$MF"; rc=$?; rm -f "$MF"; [ "$rc" -eq 0 ] || exit 1
    echo "Pronto: $MODELLO ($BASE_OLLAMA, contesto $CTX). Prova: sh strumenti/modo_locale.sh disponibile" ;;
  disponibile)
    curl -fsS "$URL/api/tags" 2>/dev/null | grep -q "\"$NOME:latest\"" ;;
  entra)
    [ -f "$STATO" ] && { echo "modo locale gia' attivo"; exit 0; }
    { echo "inizio $(date +%s)"; [ "$2" = fisso ] && echo "fisso"; for f in "$P"/.opencode/agents/*.md; do
        echo "$(basename "$f" .md) $(grep -m1 '^model:' "$f" | sed 's/^model: *//')"; done; } > "$STATO.tmp" && mv "$STATO.tmp" "$STATO" || exit 1
    for f in "$P"/.opencode/agents/*.md; do V3_BASE="$BASE" sh "$HERE/set_model.sh" "$(basename "$f" .md)" "$MODELLO" >/dev/null; done
    echo "modo locale ATTIVO: tutti gli agenti usano $MODELLO" ;;
  esci)
    [ -f "$STATO" ] || { echo "modo locale gia' spento"; exit 0; }
    grep -v '^inizio \|^fisso$' "$STATO" | while read -r ag m; do
      [ -f "$P/.opencode/agents/$ag.md" ] || continue
      grep -qx "$m" "$HERE/panchina.txt" || m="$(head -1 "$HERE/panchina.txt")"
      V3_BASE="$BASE" sh "$HERE/set_model.sh" "$ag" "$m" >/dev/null
    done
    rm -f "$STATO"; echo "modo locale spento: agenti di nuovo sui modelli free" ;;
  attivo) [ -f "$STATO" ] ;;
  scaduto)
    [ -f "$STATO" ] || exit 1
    grep -qx fisso "$STATO" && exit 1
    t=$(sed -n 's/^inizio //p' "$STATO"); [ -n "$t" ] || exit 0
    [ $(( $(date +%s) - t )) -ge $(( ${RISERVA_ORE:-6} * 3600 )) ] ;;
  stato)
    if [ -f "$STATO" ]; then grep -qx fisso "$STATO" && echo "(fisso: resta finche' non lanci esci)"; echo "modo locale: ATTIVO da $(( ($(date +%s) - $(sed -n 's/^inizio //p' "$STATO")) / 60 )) min ($MODELLO)"; else echo "modo locale: spento"; fi
    if sh "$0" disponibile; then echo "Ollama: pronto ($NOME)"; else echo "Ollama: non pronto (apri l'app e lancia: sh strumenti/modo_locale.sh prepara)"; fi ;;
  *) echo "Uso: $0 prepara | disponibile | entra | esci | attivo | scaduto | stato"; exit 1 ;;
esac
