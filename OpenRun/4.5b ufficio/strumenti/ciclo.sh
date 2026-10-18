#!/bin/sh
# Ciclo a turni: OpenCode lavora; ogni CHK_MIN minuti (default 120) c'e' un CHECKPOINT
# (controlli automatici + @collaudatore + Claude CLI se abilitato); poi @capo applica i consigli.
# Dopo MAX_ORE (default 12) si ferma e prepara il pacchetto per Claude app (revisione umana).
# Fermare: file STOP in sglavinated_src/ oppure Ctrl+C.
BASE="${V3_BASE:-$(cd "$(dirname "$0")/.." && pwd)}"
export V3_BASE="$BASE"
P="$BASE/sglavinated_src"
MAX_CICLI="${MAX_CICLI:-1000}"; MAX_ORE="${MAX_ORE:-12}"; ORE_PACCHETTO="${ORE_PACCHETTO:-8}"; fatto8=0; CHK_MIN="${CHK_MIN:-120}"
[ "$CHK_MIN" -ge 1 ] 2>/dev/null || CHK_MIN=120
cd "$P" || exit 1
if [ -f .ciclo.pid ]; then
  pid_esistente="$(cat .ciclo.pid)"
  case "$pid_esistente" in
    ''|*[!0-9]*) ;;
    *)
      cmd_esistente="$(ps -p "$pid_esistente" -o command= 2>/dev/null)"
      case "$cmd_esistente" in *"$BASE/strumenti/ciclo.sh"*) echo "[ciclo] $(date '+%d/%m %H:%M') un altro ciclo e gia attivo (pid $pid_esistente): esco"; exit 1 ;; esac
      ;;
  esac
fi
echo $$ > .ciclo.pid
OC_PID=""; WATCHDOG_PID=""
pulizia_ciclo() {
  [ -n "$OC_PID" ] && kill "$OC_PID" 2>/dev/null
  [ -n "$WATCHDOG_PID" ] && kill "$WATCHDOG_PID" 2>/dev/null
  rm -f .ciclo.pid .agente_corrente
}
trap pulizia_ciclo EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
# OpenCode v2: run parte dalla cartella corrente (nessun --dir); --standalone = server privato nella cartella
# oc: opencode run con --auto (nessuna richiesta di conferma: i divieti espliciti restano) e watchdog (OC_TIMEOUT secondi, default 2400)
oc() {
  agente=""; precedente=""
  for arg in "$@"; do
    if [ "$precedente" = "--agent" ]; then agente="$arg"; break; fi
    precedente="$arg"
  done
  if [ -n "$agente" ]; then
    printf '%s %s\n' "$agente" "$(date +%s)" > .agente_corrente.tmp && mv .agente_corrente.tmp .agente_corrente || return 1
  fi
  FL=""; [ "${OC_STANDALONE:-0}" = 1 ] && FL="--standalone"
  (cd "$P" && exec opencode run $FL --auto "$@") &
  pid=$!; OC_PID="$pid"
  ( sleep "${OC_TIMEOUT:-2400}"; kill "$pid" 2>/dev/null ) >/dev/null 2>&1 &
  w=$!; WATCHDOG_PID="$w"
  wait "$pid"; rc=$?
  kill "$w" 2>/dev/null
  OC_PID=""; WATCHDOG_PID=""
  rm -f .agente_corrente .agente_corrente.tmp
  [ "$rc" -ge 128 ] && echo "[ciclo] $(date '+%d/%m %H:%M') opencode interrotto dal watchdog (timeout)"
  return $rc
}
mkdir -p archivio_per_claude; [ -f .marker_review ] || touch .marker_review
[ -f .marker_checkpoint ] || touch .marker_checkpoint
# Ruoli 4.6 (file in .opencode/agents/). I tag v4.5 di RICHIESTA.md si traducono con ruolo_46().
AGENTI_ATTIVI="${AGENTI_ATTIVI:-builder-developer developer-builder-junior}"
REVISORE="${REVISORE:-developer-builder-junior}"
echo "$AGENTI_ATTIVI" > .agenti_attivi   # letto dal pannello
t0=$(date +%s); last=$t0; n=0; fail=0; chk=0

checkpoint() {
  chk=$((chk+1)); echo "[ciclo] $(date '+%d/%m %H:%M') CHECKPOINT $chk"
  estratti
  # Ogni controllo che fallisce aggiunge una riga a .auto_rosso: se resta vuoto e' tutto verde.
  rm -f .auto_rosso
  AUTO="$( { sh "$BASE/strumenti/verifica.sh" > .auto_verifica.tmp 2>&1 || echo verifica.sh >> .auto_rosso; tail -12 .auto_verifica.tmp; rm -f .auto_verifica.tmp
             for s in check_catalogo quota_open quota_bacheche; do python3 "motore_nuovo/strumenti_py/$s.py" || echo "$s" >> .auto_rosso; done
             grep -c '\[x\]' RICHIESTA.md | sed 's/^/blocchi fatti [x]: /'; grep -c '\[ \]' RICHIESTA.md | sed 's/^/blocchi da fare [ ]: /'; } 2>&1 )"
  ROSSI="$(cat .auto_rosso 2>/dev/null | tr '\n' ' ')"; rm -f .auto_rosso
  REVIEW_FILES="$(find motore_nuovo assets_originali -type f -newer .marker_checkpoint \( -name '*.cpp' -o -name '*.h' -o -name '*.json' -o -name '*.md' -o -name '*.py' -o -name 'CMakeLists.txt' -o -name '*.cmake' \) ! -path '*/.git/*' ! -path '*/__pycache__/*' ! -name '.DS_Store' 2>/dev/null | sort)"
  if [ -n "$REVIEW_FILES" ] && [ -z "$ROSSI" ] && [ "${CHK_LLM_SEMPRE:-0}" != 1 ]; then
    # Tutti i controlli automatici sono verdi: il collaudatore risponderebbe solo "OK".
    # Si risparmia la chiamata al modello (CHK_LLM_SEMPRE=1 per chiamarlo comunque).
    R="OK (controlli automatici verdi: review del modello saltata, zero crediti)"
    touch .marker_checkpoint
  elif [ -n "$REVIEW_FILES" ]; then
    REVIEW_OUTPUT="$(oc --agent "$REVISORE" "CHECKPOINT $chk. Officina: leggi SOLO gli esiti degli script qui sotto e le ultime righe di UFFICIO/Officina/Test area/gate_t1.log; non aprire il codice. File cambiati dall'ultimo checkpoint: $REVIEW_FILES. Controlli rossi: $ROSSI. Esiti: $AUTO. Rispondi con UNA riga: OK, oppure CORREGGI: <script rosso o numero fuori soglia> -> @builder-developer." 2>&1)"
    REVIEW_RC=$?
    if [ "$REVIEW_RC" -eq 0 ]; then
      R="$REVIEW_OUTPUT"
      touch .marker_checkpoint
    else
      R="REVIEW FALLITA (exit $REVIEW_RC): $REVIEW_OUTPUT"
      echo "[ciclo] $(date '+%d/%m %H:%M') review fallita; i file restano da rivedere al prossimo checkpoint"
    fi
  else
    R="REVIEW SALTATA: nessun file di codice, dati o asset testuale modificato dall'ultimo checkpoint."
  fi
  { echo; echo "### CHECKPOINT $chk | REVIEWER | $(date '+%F %H:%M') | [APERTA]"; echo "$AUTO"; echo; echo "$R"; } >> SCAMBIO.md
  C=""
  if [ "${CHK_CLAUDE_OGNI:-0}" -gt 0 ] && command -v claude >/dev/null 2>&1 && [ $(( chk % CHK_CLAUDE_OGNI )) -eq 1 ]; then
    C="$(claude --model "${CLAUDE_CHK_MODEL:-haiku}" -p "Sei il revisore superiore del progetto V3 (open). Sola lettura. Leggi SCAMBIO.md (ultimo checkpoint), RICHIESTA.md, ANDREA_REVIEW.md, AGENTS.md. Rispondi in italiano, breve: VERDETTO (OK/CORREGGI/FERMA); max 5 punti con file:riga; istruzioni per capo. Se un agente rende poco: 'CAMBIA_MODELLO: <agente>'." --allowedTools "Read,Grep,Glob" 2>&1)"
    { echo; echo "### CHECKPOINT $chk | CLAUDE CLI -> CAPO | $(date '+%F %H:%M') | [APERTA]"; echo "$C"; } >> SCAMBIO.md
  else
    echo "[ciclo] $(date '+%d/%m %H:%M') review Claude spenta (CHK_CLAUDE_OGNI=0, salva i tuoi crediti): solo review interna"
  fi
  if [ -s PER_CLAUDE.md ]; then mv PER_CLAUDE.md "archivio_per_claude/$(date +%Y%m%d-%H%M).md"; fi
  RC="$(sed -n 's/^RICHIESTA_CAMBIO_MODELLO: *//p' SCAMBIO.md | tr -d ' ' | sort -u)"
  [ -n "$RC" ] && sed 's/^RICHIESTA_CAMBIO_MODELLO:/FATTO_CAMBIO_MODELLO:/' SCAMBIO.md > SCAMBIO.md.tmp && mv SCAMBIO.md.tmp SCAMBIO.md
  # Solo il primo nome dopo "CAMBIA_MODELLO:" (accetta @, backtick, maiuscole); il resto della riga e' commento.
  for AG in $( { printf '%s\n%s\n' "$R" "$C" | sed -n 's/.*CAMBIA_MODELLO:[^A-Za-z]*\([A-Za-z]*\).*/\1/p'; printf '%s\n' "$RC" | sed -n 's/^[^A-Za-z]*\([A-Za-z]*\).*/\1/p'; } | tr 'A-Z' 'a-z' | grep -v '^$' | sort -u ); do
    [ -f ".opencode/agents/$AG.md" ] || { echo "[ciclo] $(date '+%d/%m %H:%M') CAMBIA_MODELLO ignorato: agente '$AG' inesistente"; continue; }
    V3_BASE="$BASE" sh "$BASE/strumenti/cambia_modello_auto.sh" "$AG"
  done
  last=$(date +%s)
}

# chiusura: checkpoint + riepilogo + pacchetto per la revisione umana con Claude app
chiusura() {
checkpoint
command -v opencode >/dev/null 2>&1 && oc --agent capo "Scrivi RIEPILOGO_MATTINA.md (max 40 righe): cosa e' stato fatto per blocco/agente, cosa non funziona, decisioni aperte in ANDREA_REVIEW.md (le 3 piu' importanti), prossimo passo consigliato." || true
{
  echo "# Pacchetto per Claude app (capo supremo) — $(date '+%F %H:%M')"
  echo "Incolla in Claude app (con accesso alla cartella): \"Leggi questo pacchetto, RIEPILOGO_MATTINA.md, ANDREA_REVIEW.md e SCAMBIO.md. Dai verdetto, correggi la rotta, aiutami a rispondere alle decisioni a blocchi.\""
  echo; echo "## Stato blocchi"; grep -n '^## BLOCCO\|^| Stato' RICHIESTA.md
  echo; echo "## Decisioni aperte"; grep -n '^## D[0-9]' ANDREA_REVIEW.md
  echo; echo "## Ultime righe di SCAMBIO.md"; tail -60 SCAMBIO.md
} > PACCHETTO_PER_CLAUDE_APP.md
echo "[ciclo] $(date '+%d/%m %H:%M') fine: leggi RIEPILOGO_MATTINA.md, ANDREA_REVIEW.md, PACCHETTO_PER_CLAUDE_APP.md"
}

# Controllo automatico (solo shell, nessuna richiesta ai modelli: non consuma il limite). Scrive CONTROLLO.md per il capo.
# 1) modelli: ogni agente deve avere un modello presente in panchina.txt (altrimenti torna al primo); 2) errori ricorrenti nei log;
# 3) stato workflow (blocchi, agenti con lavoro aperto, pause per limite). Taglia i log troppo grandi.
manutenzione() {
  # Registri comuni (solo script, nessun modello: non consuma crediti).
  python3 motore_nuovo/strumenti_py/indice_strumenti.py > /dev/null 2>&1 || true
  # Bacheche: restano solo diario, RIASSUNTO e i file del ciclo. Il resto (script, cartelle,
  # copie, file di prova) va in archivio_v4/fuori_posto/<data>/<ruolo>/ e se ne parla in CONTROLLO.md.
  for d in bacheche/*/; do
    [ -d "$d" ] || continue; ag=$(basename "$d")
    for x in "$d".* "$d"*; do
      [ -e "$x" ] || continue; n=$(basename "$x")
      case "$n" in .|..|diario.md|RIASSUNTO.md|DIARIO_RECENTE.md|diario_archivio.md) continue ;; esac
      q="archivio_v4/fuori_posto/$(date +%Y%m%d)/$ag"; mkdir -p "$q"
      mv "$x" "$q/" && echo "[ciclo] $(date '+%d/%m %H:%M') bacheca $ag: spostato $n in $q/ (fuori posto)" | tee -a .fuori_posto.log
    done
  done
  # Log: un file al giorno per agente; i giorni precedenti vanno in log_agenti/vecchi/ e restano 3 giorni (minimo 24 ore).
  mkdir -p log_agenti/vecchi; oggi=$(date +%Y%m%d); ieri=$(cat log_agenti/.giorno 2>/dev/null || echo "$oggi")
  if [ "$oggi" != "$ieri" ]; then
    for l in log_agenti/*.log; do [ -f "$l" ] && mv "$l" "log_agenti/vecchi/$(basename "$l" .log)_$ieri.log"; done
    find log_agenti/vecchi -name '*.log' -mtime +2 -exec rm -f {} + 2>/dev/null
  fi
  echo "$oggi" > log_agenti/.giorno
  ESC=$(printf '\033'); PAN="$BASE/strumenti/panchina.txt"; C=CONTROLLO.md; mkdir -p log_agenti
  { echo "# Controllo automatico — $(date '+%F %H:%M') (generato dallo script, non modificare)"
    echo; echo "## Modelli assegnati"
    for f in .opencode/agents/*.md; do
      ag=$(basename "$f" .md); m=$(grep -m1 '^model:' "$f" | sed 's/^model: *//')
      if ! grep -qx "$m" "$PAN" 2>/dev/null; then n=$(head -1 "$PAN"); V3_BASE="$BASE" sh "$BASE/strumenti/set_model.sh" "$ag" "$n" >/dev/null 2>&1; echo "- $ag: modello '$m' non in panchina -> riportato a $n"; m=$n; fi
      echo "- $ag: $m"
    done
    echo; echo "Distribuzione:"; for f in .opencode/agents/*.md; do grep -m1 '^model:' "$f" | sed 's/^model: *//'; done | sort | uniq -c | sort -rn | sed 's/^/  /'
    echo; echo "## Errori ricorrenti (ultime 300 righe di ogni log agente)"
    tot=0
    for l in log_agenti/*.log; do
      [ -f "$l" ] || continue; ag=$(basename "$l" .log); t=$(tail -n 300 "$l" | sed "s/${ESC}\[[0-9;]*[A-Za-z]//g")
      rl=$(printf '%s\n' "$t" | grep -c 'Rate limit'); pd=$(printf '%s\n' "$t" | grep -c 'Permission denied'); nf=$(printf '%s\n' "$t" | grep -c 'File not found\|does not exist'); tot=$((tot+rl+pd+nf))
      [ $((rl+pd+nf)) -gt 0 ] && echo "- $ag: rate limit $rl, permessi negati $pd, file/cartella inesistente $nf"
      [ "$pd" -ge 3 ] && echo "  -> $ag: usa solo percorsi relativi e solo le tue cartelle (AGENTS.md); non provare shell/cd."
      [ "$nf" -ge 3 ] && echo "  -> $ag: controlla il percorso con 'ls' prima di leggere; niente percorsi assoluti."
      sz=$(wc -c < "$l" | tr -d ' '); [ "$sz" -gt 20000000 ] && { tail -n 20000 "$l" > "$l.tmp" && mv "$l.tmp" "$l"; echo "  (log tagliato: era $sz byte)"; }
    done
    [ "$tot" -eq 0 ] && echo "- nessuno"
    echo; echo "## Memoria agenti (compattazione automatica)"
    for d in bacheche/*/; do
      [ -d "$d" ] || continue; ag=$(basename "$d"); f="$d/diario.md"
      if [ -f "$f" ] && [ "$(wc -l < "$f")" -gt "${DIARIO_MAX:-40}" ]; then
        k="${DIARIO_TIENI:-20}"; tot_l=$(wc -l < "$f"); { echo; echo "## archiviato il $(date '+%d/%m/%Y %H:%M')"; head -n $((tot_l - k)) "$f"; } >> "$d/diario_archivio.md"
        tail -n "$k" "$f" > "$f.tmp" && mv "$f.tmp" "$f"; echo "- $ag: diario compattato ($tot_l -> $k righe, il resto in diario_archivio.md)"
      fi
      r="$d/RIASSUNTO.md"
      if [ -f "$r" ] && [ "$(wc -l < "$r")" -gt 12 ]; then head -n 12 "$r" > "$r.tmp" && mv "$r.tmp" "$r"; echo "- $ag: RIASSUNTO.md tagliato a 12 righe"; fi
      if [ -f "$f" ]; then tail -n 6 "$f" > "$d/DIARIO_RECENTE.md.tmp" && mv "$d/DIARIO_RECENTE.md.tmp" "$d/DIARIO_RECENTE.md"; fi
      [ -f "$r" ] || echo "- $ag: manca RIASSUNTO.md (lo scrive al prossimo turno)"
    done
    echo; echo "## Workflow"
    echo "- blocchi chiusi [x]: $(grep -c '^| Stato.*\[x\]' RICHIESTA.md); in corso [~]: $(grep -c '^| Stato.*\[~\]' RICHIESTA.md); da fare [ ]: $(grep -c '^| Stato.*\[ \]' RICHIESTA.md)"
    echo "- agenti con lavoro aperto: $(agenti_liberi | tr '\n' ' ')"
    echo "- decisioni aperte per Andrea: $(grep -c '^## D[0-9].*\[APERTA\]' ANDREA_REVIEW.md)"
    [ -s .fuori_posto.log ] && { echo; echo "## File spostati dalle bacheche (fuori posto, ultimi 10)"; tail -10 .fuori_posto.log | sed 's/^\[ciclo\] /- /'; }
    echo "- istruzione: in pausa non servono nuove richieste; riprendi dai blocchi [~] e chiudi i file lasciati a meta."
  } > "$C.tmp" && mv "$C.tmp" "$C"
}

# Pausa adattiva per limite di richieste (vale per tutti i modelli free): 15, 30, 60, 120 min (max RL_MAX, default 7200 s).
# Si azzera appena una richiesta riesce. Nessun registro su file: solo a video.
rl_n=0
pausa_limite() {
  d=$(( ${RL_BASE:-900} * (1 << rl_n) )); [ "$d" -gt "${RL_MAX:-7200}" ] && d="${RL_MAX:-7200}"
  rl_n=$((rl_n+1)); [ "$rl_n" -gt 4 ] && rl_n=4
  echo "[ciclo] $(date '+%d/%m %H:%M') LIMITE DI RICHIESTE (tutti i modelli): pausa $((d/60)) min, riprovo alle $(date -v+${d}S '+%H:%M' 2>/dev/null || date -d "+$d sec" '+%H:%M' 2>/dev/null)"
  fine=$(( $(date +%s) + d )); manutenzione
  while [ "$(date +%s)" -lt "$fine" ]; do [ -f STOP ] && break; r=$(( fine - $(date +%s) )); [ "$r" -gt "${CHK_PAUSA:-600}" ] && r="${CHK_PAUSA:-600}"; sleep "$r"; manutenzione; done
}
ok_limite() { rl_n=0; }

agenti_liberi() {
  # agenti con almeno un blocco aperto ([ ] o [~]) in RICHIESTA.md, in ordine
  # RICHIESTA.md usa ancora i nomi v4.5: si traducono nei ruoli 4.6 (sviluppatore -> builder-developer,
  # collaudatore -> developer-builder-junior, grafico -> designer; ricercatore non ha piu' un agente).
  awk '/^## BLOCCO/ { ag=""; if (match($0, /@[a-z-]+/)) ag=substr($0, RSTART+1, RLENGTH-1)
                      if (ag=="sviluppatore") ag="builder-developer"; else if (ag=="collaudatore") ag="developer-builder-junior"
                      else if (ag=="grafico") ag="designer" }
       /^\| Stato/ { if (ag!="" && ($0 ~ /\[ \]/ || $0 ~ /\[~\]/) && ag!="capo" && ag!="agente") print ag }' RICHIESTA.md | awk '!s[$0]++' \
    | awk -v att=" $AGENTI_ATTIVI " 'index(att, " " $0 " ")' \
    | while read -r ag; do [ -f ".opencode/agents/$ag.md" ] && echo "$ag"; done
  # Default 4.6: builder-developer e developer-builder-junior; designer solo su richiesta
  # (AGENTI_ATTIVI="builder-developer developer-builder-junior designer").
}

# Estratti brevi (solo shell, zero crediti): gli agenti leggono questi invece dei file lunghi.
estratti() {
  [ -f "$BASE/TAPPE.md" ] && cp "$BASE/TAPPE.md" TAPPE.md
  # Il "MODELLO" (blocchi di esempio dentro un recinto ```) non va passato agli agenti: ~3 KB inutili a ogni turno.
  awk '/^## MODELLO/{skip=1; f=0; next} skip{ if($0=="```"){f++; if(f==2) skip=0} next } {print}' RICHIESTA.md | awk '/^## BLOCCO/{if(b!="" && b!~/\| Stato \| \[x\]/) printf "%s", b; b=""} {b=b $0 "\n"} END{if(b!~/\| Stato \| \[x\]/) printf "%s", b}' > RICHIESTA_APERTA.md.tmp && mv RICHIESTA_APERTA.md.tmp RICHIESTA_APERTA.md
  awk -v k="${SCAMBIO_VOCI:-3}" -v m="${SCAMBIO_RIGHE:-30}" '/^### /{n++} {l[NR]=$0; v[NR]=n} END{for(i=1;i<=NR;i++) if(v[i]>n-k){ if(v[i]!=pv){c=0; pv=v[i]} c++; if(c<=m) print l[i]; else if(c==m+1) print "[... voce troncata dopo " m " righe: intera in SCAMBIO.md]" }}' SCAMBIO.md > SCAMBIO_RECENTE.md.tmp && mv SCAMBIO_RECENTE.md.tmp SCAMBIO_RECENTE.md
  tail -n "${BACHECA_RIGHE:-24}" BACHECA_PUBBLICA.md > BACHECA_RECENTE.md.tmp && mv BACHECA_RECENTE.md.tmp BACHECA_RECENTE.md
  awk '
    /^## A[0-9]+/ {
      if (aperta && blocco != "") printf "%s", blocco
      aperta = ($0 ~ /\[APERTA\]/)
      blocco = aperta ? $0 "\n" : ""
      next
    }
    aperta { blocco = blocco $0 "\n" }
    END { if (aperta && blocco != "") printf "%s", blocco }
  ' ASSEMBLEA.md > ASSEMBLEE_APERTE.md.tmp && mv ASSEMBLEE_APERTE.md.tmp ASSEMBLEE_APERTE.md
}

review_versione() {
  v=$(cat .versione 2>/dev/null || echo 90); mkdir -p out/review; f="out/review/v$v.md"
  { echo "# Versione v$v pronta per la review di Andrea — $(date '+%d/%m/%Y %H:%M')"
    echo; echo "Tappa: $(grep -m1 -i 'tappa attiva' ANDREA_REVIEW.md 2>/dev/null || echo 'vedi TAPPE.md')"
    echo; echo "## Blocchi chiusi"; grep -B6 '^| Stato | \[x\]' RICHIESTA.md | grep '^## BLOCCO' | sed 's/^## /- /'
    echo; echo "## Memoria degli agenti"; for r in bacheche/*/RIASSUNTO.md; do [ -f "$r" ] && { echo "### $(basename "$(dirname "$r")")"; cat "$r"; echo; }; done
    echo "## File cambiati in questa versione"; find . -newer .marker_review -type f ! -path './log_agenti/*' ! -path './.opencode/*' ! -name '*.log' ! -name '.*' 2>/dev/null | sort | head -80
    echo; echo "## Cosa fare (Andrea)"; echo "1. Prova la build e guarda gli screenshot in out/."; echo "2. Registra feedback e decisioni in sglavinated_src/ANDREA_REVIEW.md sotto 'Feedback' o nella voce pertinente."; echo "3. Riavvia: sh comandi/v3.sh start"
  } > "$f"
  echo $((v+1)) > .versione; touch .marker_review
  echo "[ciclo] $(date '+%d/%m %H:%M') nessun lavoro aperto: versione v$v pronta per la review -> sglavinated_src/$f. Ciclo fermo in attesa del tuo feedback."
}

giro_diretto() {
  estratti
  # Rotazione: al massimo GIRO_MAX (3) agenti con lavoro aperto per ciclo, a turno, uno alla volta (niente richieste in parallelo).
  lista="$(agenti_liberi)"; tot=$(echo "$lista" | grep -c .)
  [ "$tot" -eq 0 ] && { echo "[ciclo] $(date '+%d/%m %H:%M') giro diretto: nessun agente con blocchi aperti"; return 0; }
  max="${GIRO_MAX:-3}"; off=$(( ${rot:-0} % tot )); c=0
  ordine="$(echo "$lista" | awk -v o="$off" '{a[NR]=$0} END{for(i=0;i<NR;i++) print a[(i+o)%NR+1]}')"
  mkdir -p log_agenti
  for ag in $ordine; do
    [ "$c" -ge "$max" ] && break; c=$((c+1))
    [ -f STOP ] && break
    touch .marker_giro
    echo "[ciclo] $(date '+%d/%m %H:%M') giro diretto: avvio $ag ($(grep -m1 '^model:' ".opencode/agents/$ag.md" | sed 's/^model: *//'))"
    OC_TIMEOUT="${GIRO_TIMEOUT:-1200}"
    oc --agent "$ag" "Turno $turno, ora $(date '+%d/%m/%Y %H:%M'). Sei @$ag. Leggi in questo ordine, e nient'altro se il blocco non lo chiede: bacheche/$ag/RIASSUNTO.md, bacheche/$ag/DIARIO_RECENTE.md, ANDREA_REVIEW.md (precedenza assoluta), AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Lavora SOLO sui tuoi blocchi [ ] o [~] (max 15 minuti) e scrivi solo nei percorsi del tuo ruolo; percorsi relativi, mai cd. RICHIESTA.md e SCAMBIO.md sono di Andrea e Claude: una voce da pubblicare va in VOCE_SCAMBIO_<n>.md nella tua cartella. Fine turno: UNA riga in bacheche/$ag/diario.md (regola d'oro 2), RISCRIVI bacheche/$ag/RIASSUNTO.md in max 12 righe, risultati condivisi in out/$ag/. Se aspetti altri o hai finito, fermati. Non usare altri modelli e non delegare." >> "log_agenti/$ag.log" 2>&1
    rc=$?
    OC_TIMEOUT=2400
    nf=$(find . -newer .marker_giro -type f ! -path './.opencode/*' ! -path './log_agenti/*' ! -name 'notte.log' ! -name '.marker_giro' ! -path './.registro_bar/*' 2>/dev/null | wc -l | tr -d ' ')
    if [ "$rc" -eq 0 ]; then ok_limite; echo "[ciclo] $(date '+%d/%m %H:%M') giro diretto: $ag ok, $nf file toccati"
    elif [ "$rc" -ge 128 ]; then echo "[ciclo] $(date '+%d/%m %H:%M') giro diretto: $ag fermato dal timeout, $nf file toccati (modello invariato)"
    elif tail -n 40 "log_agenti/$ag.log" | grep -q "Rate limit"; then pausa_limite; break
    else echo "[ciclo] $(date '+%d/%m %H:%M') giro diretto: $ag in errore ($rc), $nf file toccati: passo al modello successivo della panchina"; V3_BASE="$BASE" sh "$BASE/strumenti/cambia_modello_auto.sh" "$ag" >/dev/null 2>&1; fi
    sleep "${GIRO_SLEEP:-10}"
  done
  rot=$(( ${rot:-0} + c ))
}

pausa=0
while [ "$n" -lt "$MAX_CICLI" ]; do
  [ -f STOP ] && { echo "[ciclo] $(date '+%d/%m %H:%M') STOP"; break; }
  [ -f FINITO ] && { echo "[ciclo] $(date '+%d/%m %H:%M') tutti i blocchi fatti"; break; }
  ore=$(( ($(date +%s) - t0) / 3600 ))
  [ "$ore" -ge "$MAX_ORE" ] && { echo "[ciclo] $(date '+%d/%m %H:%M') limite ore ($MAX_ORE)"; break; }
  if [ "$ore" -ge "$ORE_PACCHETTO" ] && [ "$fatto8" -eq 0 ]; then fatto8=1; echo "[ciclo] $(date '+%d/%m %H:%M') $ORE_PACCHETTO ore: pacchetto pronto, si continua fino a $MAX_ORE ore (crea STOP per fermare)"; chiusura; fi
  turno=$(( ($(date +%s) - t0) / (CHK_MIN*60) + 1 ))
  n=$((n+1)); echo "[ciclo] $(date '+%d/%m %H:%M') ciclo $n, turno $turno, ora $ore"
  manutenzione
  estratti
  if [ "$n" -gt 1 ] && [ $((n % 3)) -ne 0 ]; then echo "[ciclo] $(date '+%d/%m %H:%M') capo: salto (v4.5: il capo lavora un giro su tre), lavorano gli agenti"; true
  else turno_capo=$turno; OC_TIMEOUT="${CAPO_TIMEOUT:-1500}"
  if oc --agent capo "Turno $turno (ora $ore di $MAX_ORE). Leggi solo: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Lavora solo sulla tappa attiva e sul suo Gate. Riscrivi out/capo/STATI_PROPOSTI.md con stati proposti e deleghe (ruoli 4.6 di AGENTS.md: builder-developer, developer-builder-junior, designer; percorsi out/<ruolo>/). Domande ad Andrea in ANDREA_REVIEW.md sempre con un DEFAULT, poi prosegui senza aspettare. Se sei bloccato scrivi PER_CLAUDE.md e fermati. Se tutti i blocchi della tappa sono [x] crea FINITO. RICHIESTA.md e SCAMBIO.md sono di sola lettura. Percorsi relativi, mai cd. Risposta breve."; then fail=0; ok_limite
  else fail=$((fail+1)); echo "[ciclo] $(date '+%d/%m %H:%M') errore opencode ($fail)"; [ "$fail" -ge 2 ] && { pausa=$((pausa+1)); [ "$pausa" -gt 24 ] && { echo "[ciclo] $(date '+%d/%m %H:%M') troppi errori: mi fermo"; break; }; pausa_limite; fail=0; }; fi
  OC_TIMEOUT=2400; fi
  giro_diretto
  if [ -z "$(agenti_liberi)" ]; then review_versione; break; fi
  el=$(( ($(date +%s) - last) / 60 ))
  if [ "$el" -ge "$CHK_MIN" ] || [ -s PER_CLAUDE.md ]; then checkpoint; fi
done


chiusura
