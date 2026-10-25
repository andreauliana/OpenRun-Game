#!/bin/sh
# Riserva locale (Ollama, oppure llama.cpp): quando i crediti gratuiti di OpenCode finiscono su tutta la panchina,
# tutti gli agenti passano al modello locale (nessun credito, gira sul Mac) e tornano ai modelli free dopo RISERVA_ORE.
# Lavoro minimo: un agente alla volta, poi RIPOSO_LOCALE secondi di pausa (default 600) per non scaldare il Mac.
# Il server locale gira solo mentre il modo locale e' acceso: "esci" lo spegne (se l'ha acceso lo script) e libera la RAM.
# Motore: LOCALE_MOTORE=ollama|llamacpp (default: ollama se installato, altrimenti llama.cpp).
# Uso: sh strumenti/modo_locale.sh <prepara|disponibile|avvia|ferma|entra [fisso]|esci|attivo|scaduto|modello|stato>
#   prepara     scarica il modello (una volta sola, circa 2,5 GB) e prova che il server risponde
#   disponibile exit 0 se il motore e' installato e il modello e' gia' stato scaricato con prepara
#   avvia/ferma accende o spegne il server locale
#   entra       accende il server, salva i modelli attuali degli agenti e li passa tutti al modello locale
#               (con "fisso" resta in locale finche' non lanci "esci", senza tornare ai free dopo RISERVA_ORE)
#   esci        rimette i modelli salvati (i modelli free), spegne il server e chiude il modo locale
#   attivo      exit 0 se il modo locale e' acceso
#   scaduto     exit 0 se il modo locale e' acceso da piu' di RISERVA_ORE ore (default 6)
#   modello     stampa il modello locale per OpenCode (ollama/openrun-locale o locale/openrun-locale)
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
P="$BASE/sglavinated_src"; HERE="$BASE/strumenti"
STATO="$P/.modo_locale"; PID="$P/.locale.pid"; LOG="$P/locale.log"
NOME="openrun-locale"
# Mac 16 GB, carico leggero: Qwen3 4B Instruct 2507 (circa 2,5 GB, supporta tools) con contesto 16k e 4 thread.
# Versione "instruct" = senza ragionamento: su CPU Intel (circa 5 token/s) Qwen3.5 pensava per centinaia di token
# anche per una parola, e da opencode.json il ragionamento non si spegne.
# Piu' qualita' ma piu' calore: LOCALE_CTX=32768 e, per Ollama, LOCALE_OLLAMA=qwen3.5:9b; per llama.cpp, LOCALE_HF=unsloth/Qwen3.5-9B-GGUF:Q4_K_M
CTX="${LOCALE_CTX:-16384}"; THREAD="${LOCALE_THREAD:-4}"
OLLAMA_BASE="${LOCALE_OLLAMA:-qwen3:4b-instruct}"; HF="${LOCALE_HF:-unsloth/Qwen3-4B-Instruct-2507-GGUF:Q4_K_M}"
# llama-server: da brew (nel PATH), da LLAMA_SERVER, o compilato in ~/Documents/llama.cpp (Mac vecchi: vedi README).
LLAMA_BIN="${LLAMA_SERVER:-$(command -v llama-server 2>/dev/null)}"
[ -n "$LLAMA_BIN" ] || LLAMA_BIN="$HOME/Documents/llama.cpp/build/bin/llama-server"
MOTORE="${LOCALE_MOTORE:-}"
[ -n "$MOTORE" ] || { command -v ollama >/dev/null 2>&1 && MOTORE=ollama || MOTORE=llamacpp; }
PRONTO="$P/.locale_pronto_$MOTORE"
if [ "$MOTORE" = ollama ]; then
  URL="http://127.0.0.1:11434"; MODELLO="ollama/$NOME"
  server_vivo() { curl -fsS "$URL/api/tags" >/dev/null 2>&1; }
  installato() { command -v ollama >/dev/null 2>&1; }
  lancia() { OLLAMA_NUM_PARALLEL=1 nohup ollama serve > "$LOG" 2>&1 & echo "$!" > "$PID"; }
else
  PORTA="${LOCALE_PORTA:-8080}"; URL="http://127.0.0.1:$PORTA"; MODELLO="locale/$NOME"
  server_vivo() { curl -fsS "$URL/health" >/dev/null 2>&1; }
  installato() { [ -x "$LLAMA_BIN" ]; }
  # --jinja serve per le chiamate ai tools; -np 1 = una richiesta alla volta con tutto il contesto; --no-mmproj = niente parte immagini.
  lancia() { nohup "$LLAMA_BIN" -hf "$HF" --alias "$NOME" --jinja --no-mmproj -c "$CTX" -t "$THREAD" -np 1 \
    --host 127.0.0.1 --port "$PORTA" > "$LOG" 2>&1 & echo "$!" > "$PID"; }
fi
aspetta_server() {  # $1 = secondi massimi
  i=0; while [ "$i" -lt "$1" ]; do server_vivo && return 0
    [ -f "$PID" ] && ! kill -0 "$(cat "$PID")" 2>/dev/null && return 1
    sleep 5; i=$((i+5)); done; return 1
}
avvia() {
  server_vivo && return 0
  installato || { echo "ERRORE: ne' ollama ne' llama-server trovati. Vedi README (Riserva locale)."; return 1; }
  lancia
}
ferma() {
  # Spegne solo un server acceso da questo script (un "ollama serve" aperto a mano resta acceso).
  [ -f "$PID" ] || return 0
  p="$(cat "$PID")"
  case "$(ps -p "$p" -o command= 2>/dev/null)" in *llama-server*|*ollama*) kill "$p" 2>/dev/null ;; esac
  rm -f "$PID"
}
case "${1:-stato}" in
  prepara)
    avvia || exit 1
    echo "Motore $MOTORE: scarico e carico il modello locale (la prima volta puo' richiedere vari minuti; dettagli in sglavinated_src/locale.log)..."
    aspetta_server 3600 || { echo "ERRORE: il server non e' partito. Ultime righe di sglavinated_src/locale.log:"; tail -15 "$LOG"; ferma; exit 1; }
    if [ "$MOTORE" = ollama ]; then
      ollama pull "$OLLAMA_BASE" || { ferma; exit 1; }
      # Ollama di default usa 4096 token di contesto: troppo pochi per gli agenti. Il Modelfile lo alza.
      MF="$(mktemp)"; printf 'FROM %s\nPARAMETER num_ctx %s\nPARAMETER num_thread %s\n' "$OLLAMA_BASE" "$CTX" "$THREAD" > "$MF"
      ollama create "$NOME" -f "$MF"; rc=$?; rm -f "$MF"; [ "$rc" -eq 0 ] || { ferma; exit 1; }
    fi
    touch "$PRONTO"
    [ -f "$STATO" ] || ferma
    echo "Pronto: $MODELLO (motore $MOTORE, contesto $CTX). Il server si accende da solo quando serve." ;;
  disponibile) installato && [ -f "$PRONTO" ] ;;
  modello) echo "$MODELLO" ;;
  avvia) avvia || exit 1; aspetta_server 300 && echo "server locale acceso su $URL" || { echo "ERRORE: server non pronto, vedi sglavinated_src/locale.log"; exit 1; } ;;
  ferma) ferma; echo "server locale spento" ;;
  entra)
    [ -f "$STATO" ] && { echo "modo locale gia' attivo"; exit 0; }
    avvia || exit 1
    aspetta_server 300 || { echo "ERRORE: server locale non pronto, resto sui modelli free (vedi sglavinated_src/locale.log)"; ferma; exit 1; }
    { echo "inizio $(date +%s)"; [ "$2" = fisso ] && echo "fisso"; for f in "$P"/.opencode/agents/*.md; do
        echo "$(basename "$f" .md) $(grep -m1 '^model:' "$f" | sed 's/^model: *//')"; done; } > "$STATO.tmp" && mv "$STATO.tmp" "$STATO" || exit 1
    for f in "$P"/.opencode/agents/*.md; do V3_BASE="$BASE" sh "$HERE/set_model.sh" "$(basename "$f" .md)" "$MODELLO" >/dev/null; done
    echo "modo locale ATTIVO: tutti gli agenti usano $MODELLO" ;;
  esci)
    [ -f "$STATO" ] || { ferma; echo "modo locale gia' spento"; exit 0; }
    grep -v '^inizio \|^fisso$' "$STATO" | while read -r ag m; do
      [ -f "$P/.opencode/agents/$ag.md" ] || continue
      grep -qx "$m" "$HERE/panchina.txt" || m="$(head -1 "$HERE/panchina.txt")"
      V3_BASE="$BASE" sh "$HERE/set_model.sh" "$ag" "$m" >/dev/null
    done
    rm -f "$STATO"; ferma; echo "modo locale spento: agenti di nuovo sui modelli free" ;;
  attivo) [ -f "$STATO" ] ;;
  scaduto)
    [ -f "$STATO" ] || exit 1
    grep -qx fisso "$STATO" && exit 1
    t=$(sed -n 's/^inizio //p' "$STATO"); [ -n "$t" ] || exit 0
    [ $(( $(date +%s) - t )) -ge $(( ${RISERVA_ORE:-6} * 3600 )) ] ;;
  stato)
    if [ -f "$STATO" ]; then grep -qx fisso "$STATO" && echo "(fisso: resta finche' non lanci esci)"; echo "modo locale: ATTIVO da $(( ($(date +%s) - $(sed -n 's/^inizio //p' "$STATO")) / 60 )) min ($MODELLO)"; else echo "modo locale: spento"; fi
    if server_vivo; then echo "server $MOTORE: acceso su $URL"; else echo "server $MOTORE: spento (si accende da solo quando serve)"; fi
    if sh "$0" disponibile; then echo "riserva locale: pronta"; else echo "riserva locale: non pronta (lancia: sh strumenti/modo_locale.sh prepara)"; fi ;;
  *) echo "Uso: $0 prepara | disponibile | avvia | ferma | entra [fisso] | esci | attivo | scaduto | modello | stato"; exit 1 ;;
esac
