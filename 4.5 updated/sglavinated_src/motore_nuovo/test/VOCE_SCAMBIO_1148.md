# VOCE_SCAMBIO_1148 — @collaudatore | 18/10/2026 07:25

## Esito gate T1 (verifica turno)
Comando: `sh tools/gate_t1.sh` (smoke, 3 livelli) — exit 0.
Comando: `sh tools/gate_t1.sh soak` (30000 frame lv_01_neon) — exit 0.
- speed@60=161.79 > 0 (partenza OK), speed@1=9.90
- offroad raggiunto (frame 278)
- ms/frame medio 0.367, max 6.202 (limite 20, obiettivo <=12)
- replay 2x: IDENTICO ogni 60 frame (determinismo OK)
- Avvolgimento playerZ oltre totalLen_: OK su lv_01/02/03 (replay 2x IDENTICO)
- Inattivita 10s -> show/fumetto: OK su lv_01/02/03 (replay 2x IDENTICO)
- accelerazione / ribaltamento / sterzo / continuita curve: OK
- Log: out/collaudatore/gate_t1.log
- Screenshot: out/collaudatore/<livello>/

## Risolto (era aperto in VOCE_SCAMBIO_1147 / RIASSUNTO)
- game.cpp:363 hardcoded `% 3000`: gia' corretto nel codice attuale (`road_.totalLength()`, commento "prima 3000 fisso"). Nessun intervento richiesto a @sviluppatore. Verificato con grep su motore_nuovo/src/game/game.cpp.

## Bloccati / in attesa
- H [~]: chiude solo Andrea (v91 FEEDBACK_ANDREA.md + fps/temperatura Pi 400; Pi 400 non misurabile da Mac).
- T1-RUN [~]: gate soak soddisfatto; chiusura completa attende v91 Andrea (punto 8, FEEDBACK_ANDREA.md).
- T1-RUN 4 [~]: di @sviluppatore (abilitare setSponsorSprites nel core headless).
- quota_open 100%, check_catalogo OK, nessuna ROM Sega.
