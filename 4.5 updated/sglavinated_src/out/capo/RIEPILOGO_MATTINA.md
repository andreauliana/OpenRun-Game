# Riepilogo mattina — Turno 1 (ora 0 di 12), 18 ott 2026
Tappa attiva: 1 (Gate: strada che scorre, accelera A, frena B, sterza sx/dx, 5 min senza crash; `sh tools/gate_t1.sh soak` exit 0 + v91 Andrea).

## Stati blocchi T1
| Blocco | Stato | Nota |
|---|---|---|
| T1-RUN | [~] | punti 1-4 codice su disco; manca gate soak exit 0 + v91 Andrea |
| P1 | [~] | sprite veri (.sglraw + prova_gate) su disco; manca gate exit 0 |
| H | [~] | attende Andrea v91 (runner Mac, fps/temp Pi 400) |
| P1b | [~] | codice su disco, gate da eseguire |
| P2 | [x] | segmenti nei 3 livello.json fatti |
| P3 | [~] | interfaccia show pronta, collegamento renderer non indispensabile al Gate |
| M | [~] | bloccato fino a screenshot out/coder/ |
| J | [ ] | non ancora |
| N | [ ] | attende C su Pi 400 (regola 9) |
| MD0 | [-] | sospeso (Andrea 17 ott) |
| T5 | [-] | non iniziare (tappa 5) |

## Cosa e' su disco (letto, non dedotto)
- motore_nuovo/src/player/player.cpp: dinamica OutRun completa (ACC 110, BRAKE 120, COAST 0.995, OFFROAD_LIMIT=700 < X_LIMIT=900, sterzo vs velocita')
- motore_nuovo/src/road/road.cpp: proiezione pseudo-3D depth/z corretta, camera segue auto, colline integrate una volta
- motore_nuovo/src/render/renderer.cpp: sprite veri con zRel modulo totalLen, disegno lontano->vicino, strisce di corsia tratteggiate
- motore_nuovo/dati/sprite_raw/sponsor/: 21 .sglraw (7 sponsor x 3 stili)
- motore_nuovo/test/prova_gate.cpp: smoke/soak/avvolgimento/inattivita/sterzo/accelerazione/curve/crash, caricaSponsor prima di init
- motore_nuovo/CMakeLists.txt: target prova_gate, SDL2_image CONFIG QUIET, json_mini.h, disegno_show/testi_frasi inclusi

## Manca per chiudere il Gate T1
1. `sh tools/gate_t1.sh soak` exit 0 (non eseguibile dal capo: shell negata) -> delegato a @collaudatore
2. 3 screenshot out/collaudatore/<livello>/ (partenza, curva, fuori strada) + log velocita e posizione
3. v91 di Andrea in FEEDBACK_ANDREA.md (runner Mac, guida OK) + fps/temperatura Pi 400
4. check_catalogo.py + quota_open.py >= 50% (da @collaudatore)

## Deleghe del turno
- @sviluppatore: presidio codice su disco; se gate rosso torna qui con il log (regola 5)
- @collaudatore: esegue `sh tools/gate_t1.sh soak`, report exit code + numeri da gate_t1.log, screenshot, indice_assets/quota_open, review 7 (max 3 rilievi)
- Andrea (punto 8): provare runner su Mac, scrivere v91, fps/temp Pi 400

## Decisioni aperte
D10=B e D11=A già risposte da Andrea in DECISIONI_ANDREA.md (non chiuse formalmente, ancora [APERTA]). Nessun nuovo PER_CLAUDE.md. FINITO non creabile (T1 non tutti [x]).

Prossimo: collaudatore esegue gate soak -> exit 0 -> v91 Andrea -> promozione T1-RUN/P1/H a [x] (solo Andrea/Claude).
