# VOCE_SCAMBIO_1150 — @collaudatore, 18/10/2026 08:12

## Gate T1 (sh tools/gate_t1.sh e sh tools/gate_t1.sh soak)
- Comando singolo dalla radice sglavinated_src, exit 0 sia smoke sia soak.
- 3 livelli smoke (500 frame, replay 2x): speed@60=161.79>0 su lv_01/02/03; offroad raggiunto f278 / f167 / f285; ms/frame medio 0.381 / 0.369 / 0.401, max 1.273 / 1.010 / 1.535 (obiettivo <=12, limite 20); replay 2x IDENTICO ogni 60 frame.
- Avvolgimento playerOltre totalLen_ e inattivita' 10s -> show/fumetto inclusi nello smoke: OK su tutti i livelli.
- Soak 30000 frame lv_01_neon: speed@60=161.79, ms/frame medio 0.407, max 55.601 (picco singolo frame, medio entro limite), replay 2x IDENTICO, GATE OK.
- Estesi: accelerazione 0-10s OK (frame 1=9.90, 60=160.83, 600=246.66; strada 12747 in 1.2s); ribaltamento e ripartenza OK; sterzo sinistra=-218.3 destra=218.3 OK; curve 1199 campioni, salto pendenza max 0.50 OK.
- Sentenza: GATE T1: TUTTO OK.

## Aperto (non bloccante, gate esce 0)
- Casi frenata (Pad::B) e costa (COAST) presenti e compilati in motore_nuovo/test/prova_gate.cpp (modi frenata/costa) ma NON invocati da tools/gate_t1.sh: numeri runtime mancanti. tools/gate_t1.sh non e' nei percorsi scrivibili del collaudatore; estendere lo script (aggiungere i due passaggi + riepilogo) spetta a chi lo mantiene (@sviluppatore o @capo).

## Rispetto vincoli
- Determinismo: stesso seed/input, stesso hash ogni 60 frame su due run (IDENTICO su 3 livelli + soak).
- Nessun materiale Sega (SCAMBIO checkpoint 18/10 07:55: nessuna ROM Sega).
- quota_open 100%, check_catalogo OK, licenza MIT su prova_gate.cpp.
- FPS/temperatura Pi 400 non misurabili su macOS; v91 di Andrea (guida) pendente -> H resta [~].
