# Proposte del capo (sostituisci questo file a ogni turno)
Andrea e Claude leggono qui e aggiornano RICHIESTA.md e SCAMBIO.md.

## Stati proposti
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [~] | in esecuzione: @sviluppatore punti 1-4 (guida OutRun, colline quota assoluta, zRel sprite, sprite veri nel gate), @collaudatore punti 5-6 (casi prova_gate, 3 screenshot out/debugger/, indice_assets, quota_open), @reviewer punto 7 dopo le correzioni coder, Andrea punto 8 (v91 + fps/temp Pi 400, regole 9/21 non eseguibile qui). Prova: `sh tools/gate_t1.sh soak` exit 0, out/collaudatore/gate_t1.log (speed>0 frame 60, replay 2x IDENTICO, ms/frame <= 20, offroad raggiungibile), 3 screenshot out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione.
P1 | [~] | identico a T1-RUN punti 1-4 (@sviluppatore). Prova: build cmake ok + 3 screenshot out/coder/ (pista curva, traffico, cartelloni, personaggio con fumetto).
H | [~] | manca v91 di Andrea (FEEDBACK_ANDREA punto 8) + fps/temperatura Pi 400 (regole 9/21, non eseguibile qui). Chiude quando Andrea scrive v91.
J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti sullo stesso problema o fine blocco importante; non ancora.
M | [~] | tipografo HUD/font — Tappa 3, non indispensabile al Gate T1; resta in attesa, nessuna nuova funzione avviata.

## Deleghe del turno
@sviluppatore: T1-RUN punti 1-4 in motore_nuovo/src/ (+ CMakeLists.txt, strumenti_py): (1) player.cpp accelerazione asintotica verso MAX_SPEED, freno B/Down, rallentamento gas rilasciato, sterzo x*= (1+speed/MAX_SPEED), spinta esterno curva, fuoristrada rallentamento graduale (non crollo a ~6), tuning un parametro alla volta; (2) road.cpp altezza quota assoluta per segmento e camY = quota sotto auto + CAM_HEIGHT, ascissa x relativa al punto sotto l'auto; (3) drawSprites zRel = modulo totalLen (stop sprite che spariscono al wrap); (4) sprite veri nel gate headless: caricatore PNG minimo nel core OPPURE conversione PNG->raw in strumenti_py (no nuove dipendenze, MIT/CC0), setSponsorSprites attivo anche in prova_gate. Regole: SPDX MIT, zero alloc nel ciclo di gioco, determinismo, core senza SDL. Poi `sh tools/gate_t1.sh` smoke; replay 2x deve restare IDENTICO.
@collaudatore: T1-RUN punti 5-6: prova_gate.cpp aggiungi casi wrap playerZ oltre totalLen_ e inattivita 10s -> show/fumetto visibile; 3 screenshot out/debugger/ (partenza, curva, fuori strada) + log velocita e posizione; rilancia `python3 motore_nuovo/strumenti_py/indice_assets.py` e `quota_open.py` (>= 50%). Dopo ogni modifica coder: `sh tools/gate_t1.sh soak`. Report con exit code e numeri da out/collaudatore/gate_t1.log.
@reviewer: punto 7 — dopo le correzioni @sviluppatore, rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh, regola 26 AGENTS.md (max 3 rilievi azionabili, sola lettura, out/REVIEW/).
Andrea (punto 8 T1-RUN): provare sglavinated_runner sul Mac, scrivere v91 in FEEDBACK_ANDREA.md, misurare fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled) — regole 9/21, non eseguibile qui.

Prossimo: esecuzione deleghe; questo file si sostituisce al turno successivo.
