# Proposte del capo (sostituisci questo file a ogni turno)
Andrea e Claude leggono qui e aggiornano RICHIESTA.md e SCAMBIO.md.

## Stati proposti
Una riga per blocco: `blocco | stato proposto | prova: exit code o file`

T1-RUN | [~] | @sviluppatore punto 4 (genera .sglraw / dati raw per sprite-cartelloni nel gate headless, DEFAULT B conversione PNG->raw), punti 1-3 gia' fatti. @collaudatore 4b (setSponsorSprites in prova_gate.cpp prima di init()), 5 (casi wrap playerZ oltre totalLen_ e inattivita 10s -> show/fumetto, 3 screenshot out/debugger/ partenza/curva/fuoristrada + log velocita e posizione), 6 (indice_assets.py + quota_open.py >= 50%), 7 (review renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh dopo modifiche coder). Prova: `sh tools/gate_t1.sh soak` exit 0, replay 2x IDENTICO, out/collaudatore/gate_t1.log (speed>0 frame 60, ms/frame <= 20, offroad raggiungibile), 3 screenshot out/debugger/ con cartelloni veri + log velocita/posizione.
P1 | [~] | @sviluppatore punto 4 (sprite veri nel gate, stesso di T1-RUN punto 4). Punti 1-3 e 5 gia' fatti. Prova: build cmake ok + gate smoke exit 0.
H | [~] | attende Andrea punto 8 (v91 su FEEDBACK_ANDREA.md, runner SDL2 su Mac, fps/temperatura Pi 400 vcgencmd). Chiude quando Andrea scrive v91.
J | [ ] | escalazione a Claude solo dopo 2 tentativi falliti sullo stesso problema o fine blocco importante; non ancora.
M | [~] | tipografo HUD/font — Tappa 3, non indispensabile al Gate T1; resta in attesa, nessuna nuova funzione avviata.

## Deleghe del turno
@sviluppatore: T1-RUN punto 4 + P1 punto 4 in motore_nuovo/src/ e motore_nuovo/strumenti_py/: generare i 21 .sglraw in motore_nuovo/dati/sprite_raw/sponsor/ (eseguire python3 motore_nuovo/strumenti_py/png_a_raw.py o fornire dati raw equivalenti) oppure, se DEFAULT B non e' confermato da Andrea, preparare caricatore PNG minimo nel core. Licenza SPDX MIT, zero alloc nel ciclo di gioco, determinismo, core senza SDL. Poi `sh tools/gate_t1.sh` smoke; replay 2x deve restare IDENTICO.
@collaudatore: T1-RUN punti 4b, 5, 6, 7: (4b) chiamare setSponsorSprites in prova_gate.cpp prima di init(); (5) aggiungere casi wrap playerZ oltre totalLen_ e inattivita 10s -> show/fumetto visibile; (5) 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita e posizione; (6) lanciare python3 motore_nuovo/strumenti_py/indice_assets.py e quota_open.py (>= 50%); (7) dopo le correzioni @sviluppatore, rivedere SOLO renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh, regola 26 AGENTS.md (max 3 rilievi azionabili, sola lettura, out/REVIEW/). Dopo ogni modifica coder: `sh tools/gate_t1.sh soak`. Report con exit code e numeri da out/collaudatore/gate_t1.log.
Andrea (punto 8 T1-RUN): provare sglavinated_runner sul Mac (SDL2), scrivere v91 in FEEDBACK_ANDREA.md, misurare fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled) — regole 9/21, non eseguibile qui.

Prossimo: esecuzione deleghe; questo file si sostituisce al turno successivo.
