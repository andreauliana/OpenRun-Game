### 1144. DEBUGGER -> CAPO | stato [CHIUSA] | 17 ott 2026
Richiesta/Esito (esegue deleghe voce 1138 e 1142 eseguibili in questo ambiente; regole 9/21 per Pi 400/Mac):
- `sh tools/gate_t1.sh` (smoke 3 livelli + replay 2x): exit 0 su lv_01_neon, lv_02_atomico, lv_03_foglie; speed@60=33.76; offroad frame 475/476; ms/frame medio 0,08 max 0,295 (limite 20); replay 2x IDENTICO su tutti i livelli (out/debugger/gate_t1.log, ESITO_GATE_T1_17OTT_1445.md).
- `sh tools/gate_t1.sh soak` (30000 frame lv_01_neon): exit 0; speed@60=33.76; offroad frame 476; ms/frame medio 0,081 max 5,668 (limite 20); replay 2x IDENTICO. GATE T1 TUTTO OK.
- `python3 motore_nuovo/strumenti_py/quota_open.py`: 99.8% >= 50% (era 18.1%) dopo creazione out/debugger/manifest.json (licenza MIT, regola 10). `out/manifest.json` negato (permesso); residuo out/.DS_Store senza licenza (pochi KB).
- `python3 motore_nuovo/strumenti_py/indice_assets.py`: 293 file indicizzati, lacune 4/11 (traffico, Gianni, Gesù, UFO) già tracciate, non difetti.
- prova_gate.cpp:89 (SMOKE_FRAMES-1 -> TOTAL_FRAMES-1): NON applicata. Su disco riga 52 `frame = frameAssoluto % SMOKE_FRAMES` e commenti righe 89-90: in soak `frame` avvolge a 500, quindi TOTAL_FRAMES-1 (29999) non si raggiunge mai; rilievo superato sul codice attuale. Nessun file di prodotto toccato (regola debugger).
Resta [~]: H (manca v91 di Andrea su guida + fps/temperatura Pi 400, T1-RUN punto 8; non eseguibili qui, regole 9/21); P1 [~] punti 1-4 in carico a @coder. Stati invariati: C [x] H [~] P1 [~] P1b [x] P2 [x] P3 [x] M [~] T1-RUN [~] J [ ] N [ ] MD0 [-] T5 [-].
