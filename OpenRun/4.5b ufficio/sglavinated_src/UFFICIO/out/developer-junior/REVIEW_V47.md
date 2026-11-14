# REVIEW V47 (@developer-junior)
Riscritto 14/11 10:09.

## VJ4 Soak (6/6 chiuso)
| Data | Livello | Comando | Exit | Frame | Replay | ms/frame medio | ms/frame max | Crash | Note |
|---|---|---|---|---|---|---|---|---|---|
| precedenti | lv_01-lv_04 | soak | 0 | 30000 | IDENTICO | — | — | no | esiti registrati nei turni precedenti |
| 14/11 | lv_05_spray | `prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv05" lv_05_spray soak` | 0 | 30000 | IDENTICO | 2.013 | 42.081 | no | out `UFFICIO/Officina/Test area/vj4_soak_lv05/` |
| 14/11 | lv_06_desktop | `prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv06" lv_06_desktop soak` | 0 | 30000 | IDENTICO | 1.536 | 8.926 | no | out `UFFICIO/Officina/Test area/vj4_soak_lv06/` |

## VJ3 50/60 Hz (6/6 chiuso)
Delta tempi giro: lv_01 0.03%, lv_02 0.19%, lv_04 0.03%, lv_05 0.24%, lv_06 0.09%, **lv_03 4.44%** (anomalia gioco, segnalata a @developer). Replay IDENTICO su tutti.

## VJ5 ∞ review codice
- 14/11 09:43: `git log -5 --oneline -- motore_nuovo/src` exit 0, ultimo `da6c52b`. Rilievo `motore_nuovo/src/game/fasi.cpp:576` puntini riga `nostro` fissi `0xFFFFFFFFu` vs `col` lampeggiante (567), commento incoerente; possibile `e.nostro ? col : 0xFF9898B0u` o grigio fisso esplicito.
- 14/11 09:50: `git log -5 --oneline -- motore_nuovo/src` exit 0, nessun cambio nuovo. Rilievo `motore_nuovo/src/gameplay/show.cpp:141-145`: off-road sopra soglia reimposta ogni frame `offRoadPending_`, `tempoOffRoadAttesaS_=tempoS_` e chiama `esc_.notificaUscitaStrada(in.frame)` finche' resta off-road; possibile attesa 5 s mai maturata e notifiche/escalation ripetute; valutare trigger one-shot finche' non rientra.
- 14/11 09:55: `git log -5 --oneline -- motore_nuovo/src` exit 0, nessun cambio nuovo. Rilievo `motore_nuovo/src/game/game.cpp:2377-2383`: rilevazione "affiancata" con finestra dz ampia (450..1850) e `|dx| < AFFIANCATA_DX` azzera `caloPunta` per ogni frame in cui c'e' match; `guizzo` (`SPORTELLATA_GUIZZO`) si applica solo se `gara_` (2380). Effetto: sterzo pieno/reattivo anche prima della finestra di contatto laterale (fianco ~750-1100), utile ma potenzialmente "magnetico" se la finestra e' troppo larga; asimmetria `caloPunta` (senza guardia gara) vs `guizzo`. Possibile semplificazione: restringere finestra o allineare le guardie gara.
- 14/11 10:02: `git log -5 --oneline -- motore_nuovo/src` exit 0, `git status --short -- motore_nuovo/src` exit 0 (nessun cambio nuovo), review commit `da6c52b`. Rilievo `motore_nuovo/src/game/fasi.cpp:572-576`: i puntini fra nome e punti usano `e.nostro ? 0xFFFFFFFFu : 0xFF9898B0u`, ma il commento dice "grigi come i separatori" e il testo usa `col`; per `nostro` restano bianco fisso e possono non seguire `col`. Semplificazione: usare `col` o `0xFF9898B0u` per tutti e precalcolare `fineNome`.
- 14/11 10:09: `git log -1 --oneline -- motore_nuovo/src` exit 0, `git status --short -- motore_nuovo/src` exit 0, nessun cambio nuovo. Rilievo `motore_nuovo/src/game/fasi.cpp:512-520` (`RisultatiFase::salva`): dopo sort per punti desc e `resize(RIGHE_TABELLA)`, se `posizione_ > RIGHE_TABELLA` forza `tabella_.back() = {nomeVis_, punteggio_, true}`; la decima riga può non rispettare l'ordine per punteggio. Possibile semplificazione: mantenere ordine coerente e/o inserire la riga del giocatore nella posizione corretta visibile.

## Rilievi aperti
- `salvataggi/giocatore.json` da ripristinare a mano (`git checkout -- salvataggi/giocatore.json`, fuori perimetro).
- @developer: lv_03 anomalia 50/60 Hz 4.44%; lv_05 piu' pesante. gate_t1.sh non eseguito.