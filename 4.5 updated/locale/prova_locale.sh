#!/bin/sh
# Modello locale a minimo consumo. Uso: sh prova_locale.sh start | prova | stop
# Il modello si scarica dalla RAM dopo ogni risposta (OLLAMA_KEEP_ALIVE=0), gira con priorita' bassa (nice 19) e un solo thread di richieste.
MODELLO="${MODELLO_LOCALE:-qwen3:4b}"
if ! command -v ollama >/dev/null 2>&1; then echo "Ollama non installato. Scaricalo da https://ollama.com (o: brew install ollama)."; exit 1; fi
case "${1:-prova}" in
  start)
    pgrep -f "ollama serve" >/dev/null 2>&1 && { echo "Ollama gia' attivo."; exit 0; }
    OLLAMA_KEEP_ALIVE=0 OLLAMA_NUM_PARALLEL=1 nohup nice -n 19 ollama serve >/dev/null 2>&1 &
    sleep 3; echo "Ollama avviato a priorita' bassa." ;;
  prova)
    sh "$0" start >/dev/null
    echo "Scarico/verifico $MODELLO (la prima volta puo' richiedere minuti)..."
    ollama pull "$MODELLO" || { echo "Pull fallito."; exit 1; }
    echo "Test: traduzione breve"
    time nice -n 19 ollama run "$MODELLO" "Traduci in inglese, rispondi solo con la traduzione: Il ponte di Udine e' sul fiume Torre." ;;
  stop)
    pkill -f "ollama serve" 2>/dev/null; echo "Ollama fermato: nessun consumo in background." ;;
  *) echo "Uso: sh prova_locale.sh start | prova | stop" ;;
esac
