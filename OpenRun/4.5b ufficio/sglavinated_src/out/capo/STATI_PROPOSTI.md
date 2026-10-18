# Proposte del capo (sostituisci questo file a ogni turno)
Turno 1 — ora 0 di 12. Tappa attiva: 1 — Gate T1-RUN.

## Stati proposti (Tappa 1)
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [~] -> [x] proposto (DEFAULT T1-Q1: sì) | `sh tools/gate_t1.sh soak` exit 0; out/collaudatore/gate_t1.log "GATE T1: TUTTO OK": speed@60=161.79, replay 2x IDENTICO 3 livelli, ms/frame medio 0.36 max 0.651 <= 20, offroad raggiunto, soak 30000 OK; screenshot in out/collaudatore/lv_0*/.
P1 | [~] | @sviluppatore punti 1-3 e 5 fatti (Pad::A accelera, B/Down freno, coast 0.995, sterzo vs velocita', segmenti da livello.json, build cmake OK, prova_gate compilato). Mancante: 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto) — shell negata `Permission denied`. Promozione [x] solo Andrea/Claude (DEFAULT T1-Q2: sì, bloccato shell).
H | [~] | attende Andrea punto 8 T1-RUN: `sglavinated_runner` su Mac SDL2, v91 in ANDREA_REVIEW.md, fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`) — non eseguibile qui (regole 9/21). Chiude con v91 + misure (DEFAULT T1-Q3: sì).
J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md.
M | [~] | HUD/font (Tappa 3) — non avviato; Andrea 17 ott: niente tappe successive ora.
G1 | [ ] | catalogo grafico @grafico — DEFAULT T1-Q4: non attivare, fuori scope Tappa 1. Gate: check_catalogo_grafico.py exit 0 e "PNG non catalogati: 0".
MD0 | [-] | sospeso (scelta Andrea 17 ott: prima il C++).
T5 | [-] | tappa 5, non avviata.

## Deleghe del turno — 5 ruoli v4.5, percorsi out/<ruolo>/

@capo (out/capo/): assegna task, responsabile, revisore, percorsi e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md, riepilogo e registro; verifica review indipendenti e prove; nessuna scrittura di codice/asset. Non si autovaluta come revisore tecnico.

@sviluppatore (out/sviluppatore/): presidia `motore_nuovo/src/` (player/, road/, render/, game/), `motore_nuovo/CMakeLists.txt`, `motore_nuovo/dati/livelli/`. Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1). Shell negata: usare solo `sh tools/gate_t1.sh` [soak] e `cmake --build build --target prova_gate` (D9); scrittura file via tool. I 3 screenshot P1 restano bloccati finché la shell non e' sbloccata.

@collaudatore (out/collaudatore/): gate T1 già verde (`sh tools/gate_t1.sh soak` exit 0, log out/collaudatore/gate_t1.log, screenshot out/collaudatore/lv_0*/). Consegnare VOCE_SCAMBIO con esito e numeri (speed, replay 2x, ms/frame, offroad). Se richiesto da @sviluppatore: rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh (max 3 rilievi azionabili). Lanciare indice_assets.py e quota_open.py (>= 50%) se non fatti dopo l'ultima generazione asset. Non avviare nuove tappe.

@grafico (out/grafico/): non attivato in questo turno (G1 DEFAULT no, nessuna richiesta Andrea, fuori scope Tappa 1).

@ricercatore (out/ricercatore/): non attivato in questo turno (nessuna richiesta Andrea, fuori scope Tappa 1).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

T1-Q1 chiusura gate T1: DEFAULT sì (chiudi T1-RUN a [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20).
T1-Q2 promozione P1: DEFAULT sì (screenshot bloccati da shell negata, prova differita).
T1-Q3 prova Mac/Pi 400: DEFAULT sì (Andrea esegua appena possibile; v91 + misure chiudono H).
T1-Q4 attivazione G1: DEFAULT non attivare (già registrato, fuori scope T1).

## Esito turno

Non tutti i blocchi sono [x] (P1 [~], H [~]): nessun FINITO. Nessun blocco hard: si prosegue con DEFAULT, nessun PER_CLAUDE.md. Prossimo: v91 Andrea + risposte T1-Q1..T1-Q3 -> 3 screenshot P1 in out/sviluppatore/ (se shell sbloccata) -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e Tappa 2.
