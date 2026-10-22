# STATI PROPOSTI — Turno 4 (ora 6 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**. Regola: una tappa si chiude quando passa il suo gate; poi review, poi Andrea, poi la tappa dopo.
Ruoli 4.6: **developer, developer-junior, designer** (= @grafico per ANDREA_REVIEW D17). Consegne: `out/developer/`, `out/developer-junior/`, `out/designer/`.

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note DEFAULT Andrea |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata (CP-005) | out/developer/, out/developer-junior/ | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest verde | D13 = B: resta [~] fino a ok Andrea runner Mac (v91 SDL2). D12 = A, D14 = A: prima Mac, poi Pi 400/RetroPie. Nessun lavoro dedicato Pi prima di ok Mac. |
| R1 (tracciati 6 piste) | [ ] | @developer (punti 1-3) + @developer-junior (punti 4-5) | developer rivede junior; junior rivede developer | out/developer/, out/developer-junior/ | gate percorso verde su tutte le 6 piste, ctest 17/17, review incrociata registrata in CP-005 | Priorita' Andrea 19 ott. Dopo R1, prima di R2: 60 Hz. |
| R2 (ciclo di gioco) | [ ] | @developer + @developer-junior + @designer | incrociata (CP-006) | out/developer/, out/developer-junior/, out/designer/ | ctest verde incluso gate ciclo; screenshot 5 fasi in UFFICIO/Officina/Test area/ | Dopo R1. |
| G1 (catalogo grafico) | [ ] | @designer (= @grafico) | review indipendente | out/designer/ | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo pronto e approvato | D15 = A, T1-Q4 = attivare. |
| H (prova Mac / Pi 400) | [~] | Andrea | — | — | v91 runner SDL2 su Mac registrato; poi fps/temperatura Pi 400 | D16: in attesa prova Andrea Mac. |

## Deleghe 4.6 (questa tappa)

- **@developer**: R1 punti 1-3 (`road.cpp` raccordi entrata/uscita, tracciato modificabile tramite `dati/tracciati/tratti.json`, `livello.json` compatibile), manutenzione punti aperti T1-RUN. Scrittura: `motore_nuovo/`, `UFFICIO/Officina/TOOL/`. Consegna: `out/developer/`.
- **@developer-junior**: R1 punti 4-5 (`dati/tracciati/regole.json`, rapporto tratta/segmento violato, test espansione tratti, riscrittura 6 `livello.json` con `tracciato`). Revisore di @developer per CP-005. Consegna: `out/developer-junior/`.
- **@designer** (= @grafico): G1 catalogo grafico (100 PNG fuori elenco, stati pronto/approvato), risorse R2 (splash, menu, risultati, HUD variante B). Consegna: `out/designer/`.

## Decisioni Andrea (ANDREA_REVIEW) — DEFAULT applicato, si prosegue senza aspettare

- D12 = A, D14 = A: Andrea ri-verifica runner su Mac prima di Pi 400/RetroPie.
- D13 = B: T1-RUN non chiuso a [x] in attesa di prova Andrea.
- D15 = A, T1-Q4 = attivare: G1 attivo su @designer.
- D17 = A: stati e deleghe confermati; si procede senza aspettare. T1-RUN resta [~] fino a CP-005 chiuso + ctest verde + ok Andrea Mac.
- D18 DEFAULT = A: dopo CP-005 + ctest verde, T1-RUN resta [~] fino a ok Andrea Mac (D16); poi avvio R2. Nessun lavoro Pi 400/RetroPie dedicato prima dell'ok.

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (T1-RUN [~], R1 [ ], R2 [ ], G1 [ ], H [~]). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente su R1 (CP-005) e G1 per DEFAULT.
