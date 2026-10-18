# Proposte del capo (sostituisci questo file a ogni turno)
Turno 1 — ora 0 di 12. Tappa attiva: 1 — Gate T1-RUN (`sh tools/gate_t1.sh soak`).
Fonti lette: TAPPE.md, ANDREA_REVIEW.md, CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.

## Stati proposti (Tappa 1)
Una riga per blocco: `blocco | stato proposto | prova: exit code o file | responsabile -> revisore | percorsi scrivibili`

T1-RUN | [~] (DEFAULT T1-Q1: sì, chiudi [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20) | `sh tools/gate_t1.sh soak` exit 0 (17 ott 14:51); out/collaudatore/gate_t1.log: replay 2x IDENTICO 3 livelli, ms/frame medio 0.05 max 3.5 <= 20, soak 30000 OK | @collaudatore -> @sviluppatore | out/collaudatore/
P1 | [~] (DEFAULT T1-Q2: sì, promuovi [x] solo Andrea/Claude) | mancano 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto) — shell negata, prova differita | @sviluppatore -> @collaudatore | out/sviluppatore/
H | [~] (DEFAULT T1-Q3: sì) | attende Andrea punto 8: `sglavinated_runner` su Mac SDL2 + fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`), v91 in ANDREA_REVIEW.md; non eseguibile qui (regole 9/21) | Andrea -> @capo | out/capo/ (solo registrazione)
J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | —
M | [~] | HUD/font (Tappa 3) — non avviato; Andrea 17 ott: niente tappe successive ora | — | —
G1 | [ ] (DEFAULT T1-Q4: non attivare, fuori scope T1) | catalogo grafico @grafico; gate: check_catalogo_grafico.py exit 0 e "PNG non catalogati: 0" | @grafico -> @capo | out/grafico/
MD0 | [-] | sospeso (scelta Andrea 17 ott: prima il C++) | — | —
T5 | [-] | tappa 5, non avviata | — | —

## Deleghe del turno — 5 ruoli v4.5, percorsi out/<ruolo>/

@capo (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md e riepilogo; verifica review indipendenti e prove. Non scrive codice/asset; non è revisore tecnico del proprio lavoro.

@sviluppatore (out/sviluppatore/): presidia motore_nuovo/src/ (player/, road/, render/, game/), motore_nuovo/CMakeLists.txt, motore_nuovo/dati/livelli/. Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Shell solo `sh tools/gate_t1.sh` [soak] e `cmake --build build --target prova_gate` (D9). Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1). I 3 screenshot P1 restano bloccati finché la shell non è sbloccata.

@collaudatore (out/collaudatore/): gate T1 verde (`sh tools/gate_t1.sh soak` exit 0, log out/collaudatore/gate_t1.log, screenshot out/collaudatore/lv_0*/). Consegnare esito con numeri (speed, replay 2x, ms/frame, offroad). Se richiesto da @sviluppatore: rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh (max 3 rilievi azionabili). Lanciare indice_assets.py e quota_open.py (>= 50%) se non fatti dopo l'ultima generazione asset. Non avviare nuove tappe.

@grafico (out/grafico/): non attivato in questo turno (G1 DEFAULT no, nessuna richiesta Andrea, fuori scope Tappa 1).

@ricercatore (out/ricercatore/): non attivato in questo turno (nessuna richiesta Andrea, fuori scope Tappa 1).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

T1-Q1 chiusura gate T1: DEFAULT sì (chiudi T1-RUN a [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20).
T1-Q2 promozione P1: DEFAULT sì (screenshot bloccati da shell negata, prova differita).
T1-Q3 prova Mac/Pi 400: DEFAULT sì (Andrea esegua appena possibile; v91 + misure chiudono H).
T1-Q4 attivazione G1: DEFAULT non attivare (già registrato, fuori scope T1).

## Esito turno

Non tutti i blocchi sono [x] (P1 [~], H [~], T1-RUN [~] proposto): nessun FINITO. Nessun blocco hard (CONTROLLO.md: agenti aperti collaudatore/sviluppatore, decisioni aperte per Andrea 0, nessun PER_CLAUDE.md). Si prosegue con DEFAULT; prossimo: risposte Andrea T1-Q1..T1-Q3 -> 3 screenshot P1 in out/sviluppatore/ (se shell sbloccata) -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e Tappa 2.
