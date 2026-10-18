# Proposte del capo (sostituisci questo file a ogni turno)
Turno 1 — ora 0 di 12. Tappa attiva: 1 — Gate T1-RUN (`sh tools/gate_t1.sh soak`).
Fonti lette: TAPPE.md, ANDREA_REVIEW.md, CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.

## Stati proposti (Tappa 1)
Una riga per blocco: `blocco | stato proposto | prova: exit code o file | responsabile -> revisore | percorsi scrivibili`

T1-RUN | [x] proposto DEFAULT T1-Q1 | `sh tools/gate_t1.sh soak` (riferimento, non rieseguito questo turno): `out/collaudatore/gate_t1.log` 18/10 11:52 — build `sglavinated_core`+`prova_gate` 100%, 3 livelli, replay 2x IDENTICO, ms/frame medio ~0.20 max 0.393 (limite 20), runner senza SDL2_image (PNG/cartelloni disabilitati) | builder-developer -> developer-builder-junior | out/builder-developer/, out/developer-builder-junior/
P1 | [~] DEFAULT T1-Q2 | mancano 3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto); shell negata, prova differita; CMake: SDL2_image non trovata -> runner senza PNG | builder-developer -> developer-builder-junior | out/builder-developer/
H | [~] DEFAULT T1-Q3 | attende Andrea punto 8: `sglavinated_runner` su Mac SDL2 + fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`), v91 in ANDREA_REVIEW.md; non eseguibile qui (regole 9/21) | Andrea -> capo (solo registrazione) | out/capo/
G1 | [ ] DEFAULT T1-Q4 | non attivato, fuori scope T1; gate: `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0" | designer -> capo | out/designer/
J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — | —
M | [~] | HUD/font (Tappa 3) non avviato; Andrea 17 ott: niente tappe successive ora | designer -> capo | out/designer/
MD0 | [-] | sospeso (scelta Andrea 17 ott: prima il C++) | — | — | —
T5 | [-] | Tappa 5 non avviata | — | — | —

Nota: C [x] già chiuso (riassunto storico 17 ott). Peer review 4.6: builder-developer <-> developer-builder-junior (autore != revisore); G1/M designer con revisione indipendente del capo (handoff STATUS_GRAFICO_PER_CAPO.md citato, non prova di approvazione).

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

capo (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review indipendenti e prove. Non scrive codice/asset; non è revisore tecnico del proprio lavoro.

builder-developer (out/builder-developer/): presidia motore_nuovo/src/ (player/, road/, render/, game/), motore_nuovo/CMakeLists.txt, motore_nuovo/dati/livelli/. Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Shell solo `sh tools/gate_t1.sh` [soak] e `cmake --build build --target prova_gate` (D9). Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1). I 3 screenshot P1 restano bloccati finché la shell non è sbloccata.

developer-builder-junior (out/developer-builder-junior/): test, gate T1, replay deterministico, diagnosi, screenshot/report. Legge diff, log ed esiti di builder-developer; revisione indipendente su T1-RUN e P1. Non avviare nuove tappe.

designer (out/designer/): catalogo grafico G1 (assets_originali/CATALOGO_GRAFICO.json, check_catalogo_grafico.py). Non attivato in questo turno (G1 DEFAULT no, fuori scope T1).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

T1-Q1 chiusura gate T1: DEFAULT sì (chiudi T1-RUN a [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20).
T1-Q2 promozione P1: DEFAULT sì (screenshot bloccati da shell negata, prova differita).
T1-Q3 prova Mac/Pi 400: DEFAULT sì (Andrea esegua appena possibile; v91 + misure chiudono H).
T1-Q4 attivazione G1: DEFAULT non attivare (già registrato, fuori scope T1).

## Esito turno

Non tutti i blocchi sono [x] (P1 [~], H [~], M [~], G1 [ ]): nessun FINITO. Nessun blocco hard (CONTROLLO.md: agenti aperti builder-developer/developer-builder-junior, decisioni aperte per Andrea 0, nessun PER_CLAUDE.md). Si prosegue con DEFAULT; prossimo: risposte Andrea T1-Q1..T1-Q3 -> 3 screenshot P1 (se shell sbloccata) -> promozioni [x] solo Andrea/Claude -> chiusura Tappa 1, poi review e Tappa 2.
