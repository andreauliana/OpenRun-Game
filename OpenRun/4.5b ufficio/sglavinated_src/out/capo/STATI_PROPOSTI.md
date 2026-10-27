# STATI PROPOSTI — Turno 2 (ora 2 di 12) — @capo

> Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md, UFFICIO/Officina/STATUS_OFFICINA.md.
> Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**.
> Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.
> Data: 2026-10-27.

## Verifica chiusure checkpoint (Turno 2)

- **CP-005 (R1 tracciati 6 piste): CHIUSO** 2026-10-22 — gate soak verde 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest 17/17, review incrociata indipendente positiva. Non si riapre.
- **CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): APERTO** — gate e review indipendente registrati e positivi (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10; prova runner Mac Andrea 23/10 ok, ctest 17/17, runner ok), ma `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B` → T1-RUN resta `[~]`. Chiudere CP-006 richiede decisione di Andrea (regola 8); DEFAULT non applicabile (risposta Andrea presente). Si procede senza aspettare.
- **CP-007 (FORK1 bivio e transizione di tema): CHIUSO** 2026-10-27 — review incrociate indipendenti positive (developer-junior → developer punti 1-3/6 POSITIVA 07:14; developer → developer-junior gate 4/5/7 POSITIVA 07:20), ctest 20/20 verde 26/10 07:00 (exit 0), gate `bivio_uscita`/`bivio_bordi`/`tema_transizione` exit 0, replay 2x IDENTICO (dossier CP-007, REGISTRO_REVISIONI.md). Chiusura operativa del @capo, decisione di Andrea non richiesta. Non si riapre.
- **CP-002 (G1 inventario grafico): APERTO** — check verde (`check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", catalogo 181 elementi, licenza CC0-1.0) ma **review indipendente mancante** — da assegnare (candidato @developer-junior, diverso da @grafico).
- Nessun altro checkpoint ha gate + review già registrati da chiudere in questo turno.

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata CP-005 (developer-junior → developer POSITIVA; developer → developer-junior POSITIVA) | `out/developer/`, `out/developer-junior/` | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest verde | Gate e review registrati; CP-006 APERTO in attesa decisione Andrea su D19 (RISPOSTA DI ANDREA: B -> T1-RUN resta [~]). DEFAULT non applicabile. Avvio R2 subordinato a conferma Andrea (D18 = B). Nessun tocco a `src/runner/` (D16). Nessun lavoro Pi 400/RetroPie prima di ok Mac (D12/D14 = A). |
| P1 | [x] | @developer | — | `out/developer/` | build CMake verificata, screenshot gameplay (prova differita) | T1-Q2: sì (Andrea). |
| G1 | [x] (@grafico) / CP-002 [~] | @grafico | review indipendente diversa da @grafico (CP-002, da assegnare; candidato @developer-junior) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato` | Check verde 25/10 10:24: exit 0, PNG non catalogati: 0, catalogo 181 elementi (handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`). CP-002 aperto in attesa review indipendente. D15 = A, T1-Q4 = attivare. |
| H | [~] | Andrea | — | — | v91 runner SDL2 su Mac da confermare su build corretta; poi fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`) | Prova Mac 23/10: runner ok, ctest 17/17 (D19). D12 = A, D14 = A: prima Mac, poi Pi 400/RetroPie; nessun lavoro dedicato Pi prima di ok Mac. Misure Pi 400 mancanti. |
| FORK1 | [x] | @developer (codice) + @developer-junior (gate) | incrociata: developer-junior → developer POSITIVA (07:14); developer → developer-junior POSITIVA (07:20) | `out/developer/`, `out/developer-junior/` | ctest verde; gate `bivio_uscita`, `bivio_bordi`, `tema_transizione` verdi; replay 2x IDENTICO; soglie `regole.json` non abbassate | CP-007 CHIUSO 2026-10-27. Elemento di passaggio al bivio @grafico facoltativo, non blocca. Dopo FORK1: DECO1. |
| DECO1 | [ ] | @developer (passi 2a-2d fatti, prossimo 2e parallasse) + @grafico (asset) | da assegnare per gate `decorazioni` (candidato @developer-junior, diverso da @grafico) | `out/developer/`, `out/grafico/` | ctest verde con `decorazioni`; screenshot PPM dei 6 livelli in `Test area/`; ms/frame <= 20; `check_catalogo_grafico.py` exit 0 | Passi 2a-2d fatti (skyline lv_01, skyline 6 livelli, oggetti bordo strada lv_01..03, densità cartelloni ~26-27%); prossimo 2e parallasse leggero in curva. Gate `decorazioni` già verde (ctest 21/21, 27/10). DECO1 [x] prima di ogni prova Pi 400/RetroPie. |

## Deleghe 4.6 (questa tappa)

- **@developer**: codice C++, integrazione, CMake, build multipiattaforma; FORK1 concluso (CP-007 chiuso, nessun tocco a `src/runner/` prima di ok Andrea Mac per D16); DECO1 passo 2e (parallasse leggero in curva), poi completamento; scrittura: percorsi assegnati, `UFFICIO/Officina/TOOL/`, `out/developer/`.
- **@developer-junior**: test, strumenti, diagnosi; review incrociata di @developer (FORK1 gate 4/5/7 già revisionata POSITIVA); candidabile come revisore indipendente di CP-002 (diverso da @grafico) e del gate `decorazioni` di DECO1. Consegna `out/developer-junior/`.
- **@grafico**: asset e catalogo; G1 chiuso (check verde, catalogo 181 elementi, licenza CC0-1.0); DECO1 fornisce skyline e oggetti per i 6 livelli (nessun prestito, lv_04/lv_05/lv_06 con stile proprio) e converte PNG->raw; unico handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (aggiornato in place). Consegna `out/grafico/`.

## Decisioni Andrea (`ANDREA_REVIEW.md`) — DEFAULT applicato solo se mancante; si prosegue senza aspettare

- T1-Q1: sì (Andrea). T1-Q2: sì -> P1 [x]. T1-Q4: attivare (Andrea) -> G1 attiva. T1-Q5: risposta Andrea 2026-10-19 (ottimizzazione percorsi, logica tracciati consecutivi, blocco R1 fatto; CP-005 chiuso).
- D12 = A, D13 = B, D14 = A, D15 = A, D17 = A, D18 = B.
- D19 [APERTA]: **RISPOSTA DI ANDREA: B** -> T1-RUN resta [~], CP-006 APERTO. DEFAULT NON si applica (risposta Andrea presente: B). Nuova prova/decisione Andrea richiesta prima di chiudere T1-RUN e avviare R2.
- Nessuna nuova domanda aperta in questo turno.

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (T1-RUN [~], H [~], DECO1 [ ], CP-002 review mancante). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente.

## Prossime azioni

1. CP-006 resta APERTO: attendere nuova prova/decisione Andrea su runner Mac (D19 = B; DEFAULT non applicabile).
2. DECO1: @developer esegue passo 2e (parallasse leggero in curva); @grafico fornisce asset e rivede screenshot lv_01_neon in `Test area/`; CP DECO1 dopo completamento passi.
3. Dopo ok Andrea Mac: F60 review incrociata developer → developer-junior, poi R2 (D18 = B).
