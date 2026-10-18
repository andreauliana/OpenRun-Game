# VOCE_SCAMBIO_1159 — @collaudatore | 18/10/2026 13:40

## Esito misurato (gate T1)
`sh tools/gate_t1.sh` dalla radice: exit 0, `GATE T1: TUTTO OK`.
`sh tools/gate_t1.sh soak`: exit 0, `GATE T1: TUTTO OK`.
- 3 livelli (lv_01_neon, lv_02_atomico, lv_03_foglie): smoke 500 frame + replay 2x IDENTICO su ogni livello.
- speed@60=161.79 > 0, speed@1=9.90; offroad raggiunto frame 278/167/285.
- ms/frame medio 0.193-0.205, max 0.562 (fumo) / 10.931 (soak) vs limite 20 (obiettivo 12).
- Avvolgimento playerOltre totalLen_: wrapCount 630/617/641, deterministico IDENTICO.
- Inattività 10 s -> show/fumetto visibile (maxBollaDopoIdle=4655 >= 100), deterministico.
- Accelerazione 10 s: 246.66 (entro 240-250). Ribaltamento + ripartenza: OK. Sterzo sx/dx: OK. Curve: salto pendenza max 0.50 <= 0.51.
- Log con velocità e posizione per frame: out/collaudatore/gate_t1.log.
- Screenshot partenza/curva/fuoristrada: out/collaudatore/lv_01_neon/prova_gate_partenza/curva/fuoristrada.ppm.
- quota_open.py: 100.0% (soglia 50%). Nessuna ROM Sega.

## Requisito FEEDBACK_ANDREA.md (programma di prova headless 10 s)
Già soddisfatto: target CMake `cmake --build build --target prova_gate`, core-only (niente SDL), guida da sola per 10 s (smoke 500 frame @ 50 Hz), salva 3 screenshot (partenza frame 1, curva frame 250, fuori strada dopo il primo offroad), velocità e posizione nel log. Default outDir `out/debugger/`; il gate scrive in `out/collaudatore/<livello>/`.

## APERTO / PASSAGGIO richiesto
- Frenata (Pad::B) e costa (gas rilasciato, COAST=0.995) esistono in `prova_gate.cpp` (modi `frenata`, `costa`, già implementati) ma `gate_t1.sh` non le invoca. `tools/gate_t1.sh` NON è nei percorsi di scrittura del ruolo collaudatore (AGENTS.md): non posso modificarlo. @sviluppatore o @capo aggiunga le due righe (`./build_gate/prova_gate ... frenata`, `... costa`) al gate, oppure confermi che i numeri sono già coperti.
- Stati proposti (promozione a [x] solo Andrea/Claude): T1-RUN resta [~] (frenata/costa mancanti nel gate); H resta [~] in attesa Andrea v91 + fps/temperatura Pi 400 (non misurabili su macOS).

## File toccati
Nessuna modifica a codice di prodotto. Solo lettura e scrittura di output/test propri del ruolo.
