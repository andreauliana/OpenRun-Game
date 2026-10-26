# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

> Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md, UFFICIO/Officina/STATUS_OFFICINA.md.
> Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**.
> Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.

## Verifica chiusure checkpoint (Turno 1)

- CP-005 (R1 tracciati 6 piste): **CHIUSO** 2026-10-22 — gate soak verde 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest 17/17, review incrociata indipendente positiva registrata (dossier CP-005, REGISTRO_REVISIONI.md). Non si riapre.
- CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): **APERTO** — gate e review indipendente già registrati e positivi (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10; prova runner Mac Andrea 23/10 ok, ctest 17/17, runner ok), **ma** `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B`: T1-RUN resta [~]. Chiudere CP-006 richiede decisione di Andrea (regola 8); il DEFAULT non si applica perché c'è già una risposta di Andrea (B). Si procede senza aspettare: T1-RUN [~], nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A), avvio R2 subordinato a conferma formale Andrea (D18 = B), non toccare `src/runner/` (D16).
- CP-002 (G1 inventario grafico): check verde (exit 0, PNG non catalogati: 0) ma **review indipendente mancante** — non chiudibile finché non assegnata e positiva.
- Nessun altro checkpoint ha gate + review già registrati da chiudere in questo turno.

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata CP-005 (developer-junior → developer POSITIVA; developer → developer-junior POSITIVA) | `out/developer/`, `out/developer-junior/` | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest verde | Gate e review registrati; CP-006 APERTO in attesa decisione Andrea su D19 (RISPOSTA DI ANDREA: B -> T1-RUN resta [~]). DEFAULT non applicabile (risposta Andrea presente). Avvio R2 subordinato a conferma Andrea (D18 = B). |
| P1 | [x] | @developer | — | `out/developer/` | build CMake verificata, screenshot gameplay (prova differita) | T1-Q2: sì (Andrea). |
| G1 | [~] | @grafico | review indipendente diversa da @grafico (CP-002, da assegnare) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato` | Check verde 25/10 10:24: exit 0, PNG non catalogati: 0, catalogo 181 elementi (handoff STATUS_GRAFICO_PER_CAPO.md). CP-002 aperto in attesa review indipendente. D15 = A, T1-Q4 = attivare. |
| H | [~] | Andrea | — | — | v91 runner SDL2 su Mac da confermare su build corretta; poi fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`) | Prova Mac 23/10: runner ok, ctest 17/17 (D19). D12 = A, D14 = A: prima Mac, poi Pi 400/RetroPie; nessun lavoro dedicato Pi prima di ok Mac. Misure Pi 400 mancanti. |

## Deleghe 4.6 (questa tappa)

- **@developer**: R1 concluso (CP-005 chiuso, tracciati 6 piste guidabili); nessun tocco a `src/runner/` prima di ok Andrea Mac (D16). Scrittura: percorsi assegnati, `UFFICIO/Officina/TOOL/`, `out/developer/`.
- **@developer-junior**: revisore indipendente di @developer (già registrato CP-005); candidabile come revisore indipendente di CP-002 (diverso da @grafico). Consegna `out/developer-junior/`.
- **@grafico**: G1 catalogo grafico (check verde, "PNG non catalogati: 0"); CP-002 in attesa review indipendente. Consegna `out/grafico/`. Unico handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (aggiornato in place).

## Decisioni Andrea (ANDREA_REVIEW.md) — DEFAULT applicato solo se mancante; si prosegue senza aspettare

- T1-Q1: sì (Andrea). T1-Q2: sì -> P1 [x]. T1-Q4: attivare (Andrea) -> G1 attiva. T1-Q5: risposta Andrea 2026-10-19 (ottimizzazione percorsi, logica tracciati consecutivi, blocco R1 fatto; CP-005 chiuso).
- D12 = A, D13 = B, D14 = A, D15 = A, D17 = A, D18 = B.
- D19 [APERTA]: **RISPOSTA DI ANDREA: B** -> T1-RUN resta [~], CP-006 APERTO. DEFAULT NON si applica (risposta Andrea presente: B). Nuova prova/decisione Andrea richiesta prima di chiudere T1-RUN e avviare R2.
- Nessuna nuova domanda aperta in questo turno.

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (T1-RUN [~], G1 [~], H [~]). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente.

## Prossime azioni (ripresa da CONTROLLO.md)

1. Attendere decisione Andrea su D19 (nuova prova runner Mac / conferma formale) per chiudere CP-006 e T1-RUN [x].
2. Assegnare review indipendente per CP-002 (G1, check già verde).
3. Dopo ok Andrea Mac: F60 review incrociata developer → developer-junior, poi R2 (D18 = B).
