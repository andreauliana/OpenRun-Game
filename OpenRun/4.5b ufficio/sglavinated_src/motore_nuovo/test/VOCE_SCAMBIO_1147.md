# VOCE_SCAMBIO_1147 — @collaudatore | 17/10/2026 18:38

## Esito gate T1
Comando: `sh tools/gate_t1.sh` (smoke, 3 livelli) — exit 0.
- speed@60=33.76 > 0 (partenza OK)
- offroad raggiunto (frame 476)
- ms/frame medio 0.072, max 0.436 (limite 20, obiettivo <=12)
- replay 2x: IDENTICO ogni 60 frame (determinismo OK)
- Avvolgimento playerZ oltre totalLen_: OK su lv_01/02/03
- Inattivita 10s -> show/fumetto: OK su lv_01/02/03
- Log: out/collaudatore/gate_t1.log
- Screenshot: out/collaudatore/<livello>/

## Review punto 7 (renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh)
- renderer.cpp drawSprites: fix `fmod` wrap con `totalLen` già presente (OK)
- road.cpp project(): `camY = camWorldY + CAM_HEIGHT`, colline integrate in `build()` (OK)
- prova_gate.cpp: casi avvolgimento + inattività già presenti (OK)
- game.h: `setSponsorSprites` dichiarato (riga 53)
- tools/gate_t1.sh: rispetta regola 26 (comando singolo, nessun pipe)

## Trovato: game.cpp:363 — hardcoded `% 3000`
`motore_nuovo/src/game/game.cpp:363`: `s.worldZ = (f32)(i * 500) + (f32)((int)road_.playerZ % 3000);`
Il `3000` e' hardcoded invece di usare `road_.totalLength()`. Il totale del livello lv_01_neon e' la somma delle lunghezze segmenti (~1880), non 3000.
Impatto attuale: nullo (sprite e' `nullptr`, non disegna nulla), ma bug latente per quando i sprite veri saranno caricati (punto T1-RUN 4).
Correzione proposta (diff): sostituire `3000` con `road_.totalLength()`.

## Bloccati / in attesa
- H [~]: chiude solo Andrea (v91 su FEEDBACK_ANDREA.md + fps/temperatura Pi 400).
- T1-RUN 4 [~]: di @sviluppatore (abilitare setSponsorSprites nel core headless). Punto 4b (collaudatore: chiamarlo in prova_gate.cpp) bloccato.
- quota_open 100%, check_catalogo OK, nessuna ROM Sega.
