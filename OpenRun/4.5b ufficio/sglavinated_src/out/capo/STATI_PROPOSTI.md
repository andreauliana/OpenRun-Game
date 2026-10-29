# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

> Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, UFFICIO/Officina/STATUS_OFFICINA.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md.
> Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**.
> Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.
> Data: 2026-10-29.

## Verifica chiusure checkpoint

- **CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): APERTO** — gate verde registrato (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20), ma `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B` → T1-RUN resta `[~]`. Chiudere CP-006 richiede decisione di Andrea (regola 8); DEFAULT non applicabile (risposta Andrea presente). Si procede senza aspettare. Avvio R2 subordinato a conferma formale Andrea (D18 = B); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A).
- **CP-007 (FORK1 bivio e transizione di tema): CHIUSO** 2026-10-27. Non si riapre.
- **CP-005 (R1 tracciati 6 piste): CHIUSO** 2026-10-22. Non si riapre.
- **CP-002 (G1 inventario grafico): CHIUSO** — G1 `[x]` (`check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", licenza CC0-1.0, handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` consultato), review indipendente completata.
- CP-003 e CP-004 restano APERTO (review indipendente da assegnare, contesto storico).
- Nessun altro checkpoint ha gate + review indipendente registrati da chiudere in questo turno.

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata CP-005 già positiva | `out/developer/`, `out/developer-junior/` | soak exit 0 6/6 piste, replay 2x IDENTICAL, ms/frame <= 20, ctest verde | CP-006 APERTO (D19 = B). DEFAULT non applicabile. Nessun tocco `src/runner/` prima di ok Andrea Mac (D16). Nessun lavoro Pi 400/RetroPie prima di ok Mac (D12/D14 = A). |
| P1 | [x] | @developer | — | `out/developer/` | build CMake, screenshot gameplay (prova differita) | T1-Q2 sì (Andrea). |
| G1 | [x] | @grafico | da assegnare (cand. @developer-junior, diverso da @grafico) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", solo `pronto`/`approvato` | D15 = A, T1-Q4 attivare. G1 chiuso per STATUS_GRAFICO_PER_CAPO.md 29/10 07:18 (181 elementi, PNG non catalogati: 0, CC0-1.0). CP-002 chiuso. |
| H | [~] | Andrea | — | — | v91 runner Mac su build corretta, poi fps/temp Pi 400 | Prova Mac 23/10 ok (ctest 17/17, runner ok); prima Mac poi Pi (D12/D14 = A). |
| FORK1 | [x] | @developer + @developer-junior | incrociata positiva (CP-007) | `out/developer/`, `out/developer-junior/` | ctest verde, gate `bivio_*` / `tema_transizione` verdi, replay 2x IDENTICAL | CP-007 CHIUSO. Dopo FORK1: DECO1. |
| DECO1 | [~] | @developer (passi piccoli 2a-2f; 2a-2d FATTO, 2e parallasse prossimo; 2f FATTO 27/10 20:02 da @grafico) + @grafico (asset, 2f FATTO) | da assegnare per gate `decorazioni` (cand. @developer-junior) | `out/developer/`, `out/grafico/` | ctest verde con `decorazioni` (21 test OK, developer-junior 27/10 06:37); screenshot PPM 6 livelli; ms/frame <= 20; check exit 0 | DECO1 [x] prima di ogni prova Pi 400/RetroPie. @grafico fornisce asset e rivede screenshot in `Test area/`. Resta: review incrociata + CP DECO1 (@capo, CP-008 da aprire). |
| F60 | [x] gate / [~] review | @developer | @developer-junior (incrociata pendente) | `out/developer/` | ctest verde 60/50 Hz, replay 2x IDENTICAL | Gate verde registrato 22/10 20:06; manca review incrociata developer→developer-junior e apertura CP F60. |
| R2 | [~] | @developer | @developer-junior (incrociata) | `out/developer/` | ctest verde con gate `ciclo`; tutte le 5 fasi | In corso (passi 1/1b/1c fatti, ctest 22/22 verde con gate `ciclo`). NON subordinato a D18/D19 (Andrea 27/10). Prossimo: 1d nome a 3 lettere, poi RIV punti a-c, poi PIST1 punto 1. |
| RIV | [~] | @developer + @grafico | @developer-junior (gate `urto`, `gara`) | `out/developer/`, `out/grafico/` | ctest verde con `gara` e `urto`; sprite rivali catalogo (7 sprite rivali "pronto"); nessun ribaltamento su urto | Dopo R2 1b-1c e DECO1 2f. Punti a,b,c (bump, sprite rivali, comportamento realistico). Punto 1 (percorso_gara.json per ogni rivale) dopo PIST1. |
| PIST1 | [ ] | @developer + @grafico | @developer-junior (gate `tratti`) | `out/developer/`, `out/grafico/` | ctest verde con `tratti`; screenshot ogni tipo tratto | Dopo RIV punti a-c. |
| FIN | [ ] | @developer + @grafico | @developer-junior (gate `gara` finale) | `out/developer/`, `out/grafico/` | ctest verde; screenshot 3 finali | Dopo R2 e RIV. 3 scene arrivo "pronto" per @grafico, varianti per posizione da definire con Andrea. |

## Deleghe 4.6 (tappa 1)

- **@developer**: codice C++, integrazione, CMake, build multipiattaforma; DECO1 passi piccoli (ogni passo compila e lascia ctest verde); R2 passo 1d poi RIV punti a-c; nessun tocco `src/runner/` prima di ok Andrea Mac (D16). Consegna `out/developer/`.
- **@developer-junior**: test, strumenti, diagnosi; revisore incrociato di @developer (FORK1 fatto); revisore indipendente CP-002 e gate `decorazioni` di DECO1; gate `urto`, `tratti`, `gara`. Consegna `out/developer-junior/`.
- **@grafico**: asset e catalogo unico; G1 chiuso (181 elementi, CC0-1.0); DECO1 fornisce skyline e oggetti per i 6 livelli (nessun prestito, stile proprio lv_04/lv_05/lv_06), converte PNG->raw, passo 2f eseguito (6 oggetti alti >=32x96); RIV 7 sprite rivali "pronto"; FIN 3 scene arrivo "pronto". Unico handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. Consegna `out/grafico/`.

## Decisioni Andrea (`ANDREA_REVIEW.md` — DEFAULT solo se mancante; si prosegue senza aspettare)

- T1-Q1 sì, T1-Q2 sì → P1 [x]; T1-Q4 attivare → G1; T1-Q5 → R1 (CP-005 chiuso).
- D12 = A, D13 = B, D14 = A, D15 = A, D17 = A, D18 = B (chiusa 27/10).
- D19: **RISPOSTA DI ANDREA: B** → T1-RUN `[~]`, CP-006 APERTO. DEFAULT non si applica. Nuova prova/decisione Andrea richiesta prima di chiudere T1-RUN (R2 resta slegato da D19 per Andrea 27/10).
- D16: Andrea sta testando la review del runner Mac (v91, SDL2); proseguire autonomamente in attesa.

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (T1-RUN [~], H [~], DECO1 [~], R2 [~], RIV [~], PIST1 [ ], FIN [ ]). **FINITO non creabile.** Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente.

## Prossime azioni

1. CP-006 resta APERTO: attendere nuova prova/decisione Andrea su runner Mac (D19 = B; D12/D14 = A: prima Mac, poi Pi 400/RetroPie). DEFAULT non applicabile (risposta Andrea presente). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale. CP-006 si chiude solo con decisione Andrea (regola 8).
2. DECO1: review incrociata (developer → developer-junior e viceversa), @grafico rivede screenshot in `Test area/`, poi @capo apre CP-008 e chiude quando review positive.
3. R2: @developer passo 1d (nome a 3 lettere nei Risultati, gate `ciclo` verde), poi RIV punti a-c, poi PIST1 punto 1.
4. F60: review incrociata developer→developer-junior, poi apertura CP F60.
5. G1: CP-002 review indipendente completata (già chiuso).