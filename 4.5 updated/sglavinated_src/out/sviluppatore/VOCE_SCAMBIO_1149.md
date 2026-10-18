# Voce scambio @sviluppatore — 18/10 08:22

Stato blocchi: P1 [~], T1-RUN [~].

## Eseguito questo turno
Lettura dello stato: bacheche/sviluppatore/{RIASSUNTO,DIARIO_RECENTE}.md, FEEDBACK_ANDREA.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, VISIONE.md. Sorgenti letti: src/player/player.{h,cpp}, src/game/game.{h,cpp}, src/road/road.{h,cpp}, src/render/renderer.{h,cpp}, motore_nuovo/dati/livelli/{lv_01_neon,lv_02_atomico,lv_03_foglie}/livello.json, out/collaudatore/gate_t1.log.

## Build: NON ESEGUIBILE
Il comando di build/gate `sh tools/gate_t1.sh` non puo' essere avviato: il tool shell e' negato con l'errore esatto `Permission denied: shell`. Nessun binario rieseguito, nessun comando cmake lanciato, nessun screenshot generato. Nessun file di codice modificato (niente di non verificabile senza build).

## Esito misurato disponibile (ultimo log, non rieseguito)
out/collaudatore/gate_t1.log riporta GATE OK su lv_01_neon, lv_02_atomico, lv_03_foglie + soak 30000 frame lv_01_neon:
- speed@60=161.79; accelerazione OK (0-10s progressiva fino a ~246); frenata OK; sterzo OK (sinistra=-218.3, destra=218.3); curve OK (salto pendenza max 0.50); ribaltamento OK; avvolgimento playerZ OK (wrapCount >= 1); inattivita show OK (bolla 4655 px >= 100).
- ms/frame medio 0.407-0.744, max <=6.322 (limite 20); replay 2x IDENTICO su tutti i run; offroad raggiunto.

## Guida (feedback Andrea 17 ott, punti 1-3)
Già coerente con i sorgenti: accelerazione che cala verso MAX_SPEED (ACCEL_BASE + ACCEL_BOOST*exp() * (1-speed/MAX_SPEED)), freno secondario B/Down (BRAKE 120), coast al rilascio (COAST 0.995), sterzo dipendente dalla velocita', spinta esterna in curva, rallentamento offroad (OFFROAD_SLOW 0.99), ribaltamento per uscita/collisione, pannello info drawHud (velocita, marcia, offroad).

## Mancante per chiudere P1
3 screenshot in out/sviluppatore/ con pista curva, cartelloni sponsor, personaggio con fumetto (gate P1: build cmake ok + 3 screenshot). Richiede shell per lanciare prova_gate con outDir out/sviluppatore/ (il binary accetta outDir come argv[1], nessun cambiamento di codice necessario).

## Pendente per chiudere T1-RUN / H
v91 di Andrea: test sglavinated_runner sul Mac (SDL2) e giudizio guida in FEEDBACK_ANDREA.md (punto 8); fps/temperatura su Pi 400 (Andrea sul Pi).

## Prossimo
Ottenere shell -> sh tools/gate_t1.sh (exit 0 atteso) -> ./build_gate/prova_gate out/sviluppatore lv_01_neon smoke (e lv_02/lv_03) per i 3 screenshot P1 -> aggiornare RIASSUNTO.md e diario.md con exit code ed esito misurato.
