# Esito Gate T1 — 17 ott 2026 14:45 — @developer/@developer-junior

Comandi eseguiti (regola 26, comando singolo, nessun pipe): `sh tools/gate_t1.sh` e `sh tools/gate_t1.sh soak`. Build cmake: OK (target sglavinated_core + prova_gate, cartella build_gate).

Risultati misurati (letti da out/debugger/gate_t1.log, non dedotti):

- lv_01_neon smoke 500 frame: exit 0; speed@60=33.76; X[-164.8,751.8]; zFinale=942.7; offroad raggiunto frame 476; ms/frame medio 0,081 max 0,295 (limite 20); replay 2x IDENTICO → GATE OK
- lv_02_atomico smoke 500 frame: exit 0; speed@60=33.76; offroad frame 476; ms/frame medio 0,079 max 0,201; replay 2x IDENTICO → GATE OK
- lv_03_foglie smoke 500 frame: exit 0; speed@60=33.76; offroad frame 475; ms/frame medio 0,097 max 1,181; replay 2x IDENTICO → GATE OK
- soak lv_01_neon 30000 frame: exit 0; speed@60=33.76; X[-164.8,900.0]; zFinale=2958.7; offroad frame 476; ms/frame medio 0,081 max 5,668 (limite 20, obiettivo 12); replay 2x IDENTICO → GATE OK

Gate T1: TUTTO OK. Screenshot partenza/curva/fuoristrada scritti in out/debugger/<livello>/ e out/debugger/soak/.

Note:
- runner compilato senza SDL2_image (linea 1 del log): cartelloni sponsor disabilitati, prova_gate disegna rettangoli. Sprite veri = T1-RUN punto 4, competenza @developer; non blocca il gate.
- prova_gate.cpp:89 (rilievo reviewer 14:09, voce SMOKE_FRAMES-1 → TOTAL_FRAMES-1): NON applicata. Su disco riga 52 `frame = frameAssoluto % SMOKE_FRAMES` e commenti righe 89-90 documentano che in soak `frame` avvolge a 500, quindi TOTAL_FRAMES-1 (29999) non si raggiunge mai: rilievo superato sul codice attuale. Nessun file di prodotto toccato (regola debugger).

Aperti per chiusura H: v91 di Andrea (guida su Mac, T1-RUN punto 8) + fps/temperatura su Pi 400 (`vcgencmd measure_temp`, `get_throttled`); build/gate su Pi 400/Mac non eseguibili qui (regole 9/21).
