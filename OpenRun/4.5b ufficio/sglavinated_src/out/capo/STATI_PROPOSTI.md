# STATI PROPOSTI — Turno 2 (ora 2 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**. Regola: una tappa si chiude quando passa il suo gate; poi review, poi Andrea, poi la tappa dopo.
Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note DEFAULT Andrea |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata (CP-005 chiuso) | out/developer/, out/developer-junior/ | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20 | D13 = B: resta [~] fino a ok Andrea runner Mac v91 (D12/D14 = A: prima Mac, poi Pi 400/RetroPie). Nessun lavoro dedicato Pi prima di ok Mac. |
| R1 (tracciati 6 piste) | [x] | @developer (punti 1-3) + @developer-junior (punti 4-5) | incrociata (CP-005) | out/developer/, out/developer-junior/ | gate percorso verde su tutte le 6 piste, ctest 17/17, review incrociata registrata in CP-005, soglie invariate | Chiuso CP-005 il 2026-10-22. Dopo R1, prima di R2: 60 Hz (F60). |
| F60 (60 Hz, tempi in secondi) | [ ] | @developer + @developer-junior | incrociata (checkpoint dedicato) | out/developer/, out/developer-junior/ | ctest verde a 60 Hz e 50 Hz; giro guidato stessi secondi +/- 2%; replay 2x IDENTICO a ciascuna | Dopo R1 (CP-005 chiuso), prima di R2 (Andrea 19 ott). Non toccare `src/runner/` prima di ok Andrea Mac (D16). |
| R2 (ciclo di gioco) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP-006 da aprire) | out/developer/, out/developer-junior/, out/grafico/ | ctest verde incluso gate ciclo; screenshot 5 fasi in `UFFICIO/Officina/Test area/`; @grafico rivede schermate | Dopo F60. @capo apre CP-006. |
| G1 (catalogo grafico) | [~] attivo | @grafico | review indipendente | out/grafico/ | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo pronto e approvato | D15 = A, T1-Q4 = attivare. Residuo: sanare riferimento scartato a file inesistente `obj_insegna_fumosa` (lv_03_foglie variante B) per exit 0. Handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. |
| H (prova Mac / Pi 400) | [~] | Andrea | — | — | v91 runner SDL2 su Mac registrato; poi fps/temperatura Pi 400 | D16: Andrea in prova sul runner Mac; blocco H resta [~]. Nessun lavoro dedicato Pi 400/RetroPie prima dell'ok Mac. |

## Deleghe 4.6 (questa tappa)

- **@developer**: manutenzione punti aperti T1-RUN (colline, wrap sprite, sprite veri bloccati su G1), ctest per chiusura CP-005 (eseguito 2026-10-22, 17/17). Scrittura: `motore_nuovo/` (solo percorsi assegnati), `UFFICIO/Officina/TOOL/`, `out/developer/`. Non toccare `src/runner/` finché Andrea non chiude la prova runner su Mac (D16).
- **@developer-junior**: review indipendente CP-005 punti 1-3 (eseguita 2026-10-22, POSITIVA), punti 4-5 già revisionati, test espansione tratti. Revisore di @developer per CP-005. Consegna: `out/developer-junior/`.
- **@grafico**: G1 catalogo grafico (100 PNG fuori elenco con stati pronto/approvato), risorse grafiche R2 (splash, menu, risultati, HUD variante B). Consegna: `out/grafico/`. Unico handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

## Decisioni Andrea (ANDREA_REVIEW) — DEFAULT applicato, si prosegue senza aspettare

- T1-Q1: risposta Andrea: sì — gate soak verde (exit 0, replay 2x IDENTICO, ms/frame <= 20) soddisfa il DEFAULT; T1-RUN resta [~] per D13.
- D12 = A, D14 = A: Andrea ri-verifica runner su Mac prima di Pi 400/RetroPie.
- D13 = B: T1-RUN non chiuso a [x] in attesa di prova Andrea (v91 runner SDL2 su Mac).
- D15 = A, T1-Q4 = attivare: G1 attivo su @grafico.
- D16 = in corso (Andrea testa il runner Mac v91; non segnata come superata).
- D17 = A: stati e deleghe confermati; si procede senza aspettare.
- D18 = risposta mancante; DEFAULT A applicato: dopo CP-005 chiuso + ctest verde, T1-RUN resta [~] fino a ok Andrea Mac; poi avvio R2. Nessun lavoro dedicato a Pi 400/RetroPie prima dell'ok.

## Checkpoint

- CP-005 (R1 tracciati): **CHIUSO** 2026-10-22 — review incrociata indipendente completa e ctest 17/17 verde, gate soak verde 6/6 piste. Registrato in REGISTRO_REVISIONI e STATO_CORRENTE. Nessun nuovo checkpoint da aprire in questo turno (tappa 1 ancora aperta in attesa di ok Andrea Mac).
- CP-002/CP-003/CP-004: restano APERTI in attesa di review indipendente (nessuna review registrata; non chiudibili).

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (T1-RUN [~], F60 [ ], R2 [ ], G1 [~], H [~]). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente su CP-005 chiuso, G1 e attesa ok Andrea Mac.
