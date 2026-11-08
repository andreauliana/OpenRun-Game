# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, UFFICIO/Officina/STATUS_OFFICINA.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md.

Tappa attiva: **1 — Gate T1-RUN** ("Il gioco parte, si visualizza il player che si muove, si pilota": strada che scorre, auto che accelera (A), frena (B), sterza sinistra/destra).

Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.

Data: 2026-11-08.

## Verifica chiusure checkpoint

- **CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): APERTO** — Gate T1-RUN RAPPORTATO A VERDE e verificato su disco il 08/11: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 piste `GATE PERCORSO OK`, accelerazione/ribaltamento/sterzo/curve continue/gara OK, frequenza 60 Hz e 50 Hz OK (replay IDENTICO diff=0.00%), ms/frame medio 1.079–1.758 max 17.003 (limite 20), log `UFFICIO/Officina/Test area/gate_t1.log`. Screenshot P1: `personaggio+fumetto frame 446` > 0 (era -1, causa dell'exit 12 del 07/11). Correzioni registrate: `motore_nuovo/test/prova_gate.cpp:969` da @developer-junior (cross-review @developer POSITIVA) e `UFFICIO/Officina/TOOL/gate_t1.sh` righe 50-84 `."`→`./` da @developer (review @developer-junior POSITIVA con rilievo su retry loop, in `out/developer-junior/REVIEW_GATE_T1_P1_2026-11-08.md`). Review indipendente registrata (autore ≠ revisore) su entrambe le modifiche. MA `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B` → T1-RUN resta `[~]`, CP-006 resta APERTO: chiusura richiede decisione di Andrea (regola 8), DEFAULT non applicabile (risposta Andrea presente). Attendere nuova prova Andrea runner Mac (prima Mac, poi Pi 400/RetroPie; D12/D14 = A). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale.
- **CP-007 (FORK1 bivio e transizione di tema): CHIUSO** 2026-10-27 (review incrociate indipendenti positive, ctest 20/20 verde). Non si riapre.
- **CP-005 (R1 tracciati 6 piste): CHIUSO** 2026-10-22. Non si riapre.
- **CP-002 (G1 inventario grafico): APERTO** — G1 `[x]` per handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (220 elementi, PNG non catalogati: 0, licenza CC0-1.0, `check_catalogo_grafico.py` exit 0), ma review indipendente ancora da assegnare (regola AGENTS 9): revisore diverso da @grafico (cand. @developer-junior). CP-002 si chiude solo dopo review indipendente positiva.
- **CP-003 e CP-004 restano APERTO** (review indipendente da assegnare, contesto storico).
- Nessun altro checkpoint ha gate + review indipendente registrati da chiudere in questo turno. `FINITO` non creabile: blocchi non tutti `[x]` (T1-RUN [~], H [~], DECO1 [~], R2 [~], RIV [~], PIST1 [ ], FIN [ ]).

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata (gate_t1.sh: junior→developer POSITIVA; prova_gate.cpp:969: developer→junior POSITIVA) | `out/developer/`, `out/developer-junior/` | soak exit 0 6/6 piste, replay 2× IDENTICO, ms/frame ≤ 20, ctest verde, screenshot P1 completo | CP-006 APERTO (D19 = B). DEFAULT non applicabile. Nessun tocco `src/runner/` prima di ok Andrea Mac (D16). Nessun lavoro Pi 400/RetroPie prima di ok Mac (D12/D14 = A). Gate verde verificato 08/11 (exit 0, log Test area/gate_t1.log). |
| P1 | [x] | @developer | — | `out/developer/` | build CMake, screenshot gameplay (prova differita) | T1-Q2 sì (Andrea). |
| G1 | [x] | @grafico | da assegnare (cand. @developer-junior, diverso da @grafico) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", solo `pronto`/`approvato` | D15 = A, T1-Q4 attivare. Handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`: 220 elementi, 0 non catalogati, CC0-1.0. CP-002 APERTO in attesa review indipendente. |
| H | [~] | Andrea | — | — | v91 runner Mac su build corretta, poi fps/temp Pi 400 | Prima Mac poi Pi (D12/D14 = A). |
| FORK1 | [x] | @developer + @developer-junior | incrociata positiva (CP-007) | `out/developer/`, `out/developer-junior/` | ctest verde, gate `bivio_*` / `tema_transizione` verdi, replay 2× IDENTICO | CP-007 CHIUSO. Dopo FORK1: DECO1. |
| DECO1 | [~] | @developer (passi piccoli 2a-2f; 2a-2d FATTO, 2e parallasse prossimo) + @grafico (asset; 2f FATTO; skyline e oggetti 6 livelli nel catalogo) | da assegnare per gate `decorazioni` (cand. @developer-junior) | `out/developer/`, `out/grafico/` | ctest verde con `decorazioni` (21 test OK, developer-junior 27/10 06:37); screenshot PPM 6 livelli; ms/frame ≤ 20; check exit 0 | DECO1 [x] prima di ogni prova Pi 400/RetroPie. @grafico fornisce asset e rivede screenshot in `Test area/`. Resta: review incrociata + CP DECO1 (@capo, CP-008 da aprire). |
| F60 | [x] gate / [~] review | @developer | @developer-junior (incrociata pendente) | `out/developer/` | ctest verde 60/50 Hz, replay 2× IDENTICO | Gate verde registrato 22/10 20:06; manca review incrociata developer→developer-junior e apertura CP F60. |
| R2 | [~] | @developer | @developer-junior (incrociata) | `out/developer/` | ctest verde con gate `ciclo`; tutte le 5 fasi | In corso (passi 1/1b/1c fatti, ctest 22/22 verde gate `ciclo`). NON subordinato a D18/D19 (Andrea 27/10). Prossimo: 1d nome a 3 lettere, poi RIV punti a-c, poi PIST1 punto 1. |
| RIV | [~] | @developer + @grafico | @developer-junior (gate `urto`, `gara`) | `out/developer/`, `out/grafico/` | ctest verde con `gara` e `urto`; sprite rivali catalogo; nessun ribaltamento su urto | Dopo R2 1b-1c e DECO1 2f. Punti a,b,c (bump, sprite rivali, comportamento realistico) lato codice fatti; grafica rivali in attesa. Punto 1 (percorso_gara.json per ogni rivale) dopo PIST1. |
| PIST1 | [ ] | @developer + @grafico | @developer-junior (gate `tratti`) | `out/developer/`, `out/grafico/` | ctest verde con `tratti`; screenshot ogni tipo tratto | G-P1 [x] per handoff grafico 2026-11-05 20:46 (parapetto.png 32x48 + guardrail.png 48x32 su disco, raw, 2 voci `pronto`, catalogo exit 0). Ora: @developer P3 (parapetti oggetti ai due lati ogni ~400 unità sui segmenti con `acqua`), poi `sh UFFICIO/Officina/TOOL/gate_t1.sh`; poi @developer-junior J2 e J1 (gate `tratti`). |
| FIN | [ ] | @developer + @grafico | @developer-junior (gate `gara` finale) | `out/developer/`, `out/grafico/` | ctest verde; screenshot 3 finali | Dopo R2 e RIV. 3 scene arrivo "pronto" per @grafico, varianti per posizione da definire con Andrea. |

## Deleghe 4.6 (tappa 1)

- **@developer**: codice C++, integrazione, CMake, build multipiattaforma; `gate_t1.sh` corretto (08/11); DECO1 passi piccoli (ogni passo compila e lascia ctest verde); R2 passo 1d poi RIV punti a-c; PIST1 passo P3 (parapetti oggetti su segmenti `acqua`, dopo G-P1 [x]); nessun tocco `src/runner/` prima di ok Andrea Mac (D16). Consegna `out/developer/`.
- **@developer-junior**: test, strumenti, diagnosi; `prova_gate.cpp:969` fixato (08/11); review indipendente `gate_t1.sh` fatta (`out/developer-junior/REVIEW_GATE_T1_P1_2026-11-08.md`); gate `urto`, `tratti`, `gara`; PIST1 J2 poi J1; cross-review di modifiche @developer. Consegna `out/developer-junior/`.
- **@grafico**: asset e catalogo unico; G1 [x] (220 elementi, CC0-1.0, 0 non catalogati); DECO1 fornisce skyline e oggetti per i 6 livelli (nessun prestito, stile proprio lv_04/lv_05/lv_06), converte PNG->raw, passo 2f FATTO e PIST1 G-P1 FATTO (parapetto/guardrail); RIV 7 sprite rivali "pronto"; FIN 3 scene arrivo "pronto". Unico handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. Consegna `out/grafico/`.

## Decisioni Andrea (`ANDREA_REVIEW.md` — DEFAULT solo se mancante; si prosegue senza aspettare)

- T1-Q1 sì, T1-Q2 sì → P1 [x]; T1-Q4 attivare → G1; T1-Q5 → R1 (CP-005 chiuso).
- D12 = A, D14 = A, D15 = A, D17 = A, D18 = B (chiusa 27/10).
- **D19: RISPOSTA DI ANDREA: B** → T1-RUN `[~]`, CP-006 APERTO. DEFAULT non si applica. Nuova prova/decisione Andrea richiesta prima di chiudere T1-RUN (runner Mac, prima Mac poi Pi 400; R2 resta slegato da D18/D19 per Andrea 27/10).
- Nessuna domanda di ANDREA_REVIEW.md priva di risposta: tutti i DEFAULT già superati da RISPOSTA DI ANDREA.

## Prossime azioni

1. CP-006 resta APERTO: attendere nuova prova/decisione Andrea su runner Mac (D19 = B; D12/D14 = A: prima Mac, poi Pi 400/RetroPie). DEFAULT non applicabile. Il gate è ora verde e le review indipendenti sono registrate: si aspetta solo la decisione di Andrea.
2. **@developer: PIST1 passo P3** (parapetti come oggetti ai due lati ogni ~400 unità sui segmenti con `acqua`), poi `sh UFFICIO/Officina/TOOL/gate_t1.sh`.
3. Dopo P3: **@developer-junior J2 e J1** (gate `tratti`), poi @capo apre/chiude il CP PIST1.
4. DECO1: review incrociata (developer → developer-junior e viceversa), @grafico rivede screenshot in `Test area/`, poi @capo apre CP-008 e chiude quando review positive.
5. R2: @developer passo 1d (nome a 3 lettere nei Risultati, gate `ciclo` verde), poi RIV punti a-c, poi PIST1 punto 1.
6. F60: review incrociata developer→developer-junior, poi apertura CP F60.
7. G1: CP-002 review indipendente da assegnare (revisore diverso da @grafico, cand. @developer-junior).
