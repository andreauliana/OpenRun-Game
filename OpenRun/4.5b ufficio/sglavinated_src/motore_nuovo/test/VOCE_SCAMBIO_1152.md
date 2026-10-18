# VOCE_SCAMBIO_1152 — @collaudatore, 18/10/2026 09:09

## Gate T1 (sh tools/gate_t1.sh)
- Comando singolo dalla radice sglavinated_src, exit 0.
- 3 livelli smoke (500 frame, replay 2x): speed@60=161.79>0 lv_01/02/03; offroad f278/f167/f285; ms/frame medio 0.361/0.349/0.377, max 0.602/0.542/0.553 (obiettivo <=12, limite 20); replay 2x IDENTICO ogni 60 frame.
- Avvolgimento playerZ oltre totalLen_ e inattivita 10s -> show/fumetto inclusi nello smoke: OK su tutti i livelli.
- Estesi: accelerazione 9.90/160.83/246.66 OK; ribaltamento e ripartenza OK; sterzo sinistra=-218.3 destra=218.3 OK; curve 1199 campioni, salto pendenza max 0.50 OK.
- Sentenza: GATE T1: TUTTO OK.

## Misure indice asset (motore_nuovo/strumenti_py/indice_assets.py, exit 0)
- 294 file, 147 con avvisi, lacune 4/11: mancano traffico/altri veicoli (0/1), personaggio Gianni (sprite) (0/1), Gesu' (0/1), UFO (0/1).

## Aperto (non bloccante, gate esce 0)
- Casi frenata (Pad::B) e costa (COAST) compilati in motore_nuovo/test/prova_gate.cpp (modi frenata/costa) ma NON invocati da tools/gate_t1.sh: numeri runtime mancanti. tools/gate_t1.sh non e' nei percorsi scrivibili del collaudatore ed esecuzione diretta del binario e' negata dai permessi shell; estendere lo script spetta a chi lo mantiene (@sviluppatore o @capo).

## Rispetto vincoli
- Determinismo: stesso seed/input, stesso hash ogni 60 frame su due run (IDENTICO su 3 livelli).
- Nessun materiale Sega (nessuna ROM Sega). quota_open 100%, check_catalogo OK, prova_gate.cpp SPDX MIT.
- FPS/temperatura Pi 400 (vcgencmd) e v91 di Andrea (guida) non misurabili su macOS -> H resta [~].
