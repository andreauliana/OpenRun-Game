# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, UFFICIO/Officina/STATUS_OFFICINA.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md.

Tappa attiva: **1 — Gate T1-RUN** ("Il gioco parte, si visualizza il player che si muove, si pilota": strada che scorre, auto che accelera (A), frena (B), sterza sinistra/destra).
Lavoro attivo: **BLOCCO PERC1** — percorsi più vari e migliorie di guida (Andrea 08/11), un passo per turno, senza aspettare nessuno.

Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.

Data: 2026-11-08.

## Verifica chiusure checkpoint

- **CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): APERTO** — Gate T1-RUN verde e verificato su disco il 08/11: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 piste `GATE PERCORSO OK`, accelerazione/ribaltamento/sterzo/curve continue/gara OK, frequenza 60 Hz e 50 Hz OK (replay IDENTICO diff=0.00%), ms/frame medio 1.079–1.758 max 17.003 (limite 20), log `UFFICIO/Officina/Test area/gate_t1.log`. Screenshot P1: `personaggio+fumetto frame 446` > 0 (era -1, causa dell'exit 12 del 07/11). Correzioni registrate: `motore_nuovo/test/prova_gate.cpp:969` da @developer-junior (cross-review @developer POSITIVA) e `UFFICIO/Officina/TOOL/gate_t1.sh` righe 50-84 `."`→`./` da @developer (review @developer-junior POSITIVA con rilievo su retry loop, in `out/developer-junior/REVIEW_GATE_T1_P1_2026-11-08.md`). Review indipendente registrata (autore ≠ revisore) su entrambe le modifiche. MA `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B` → T1-RUN resta `[~]`, CP-006 resta APERTO: chiusura richiede decisione di Andrea (regola 8), DEFAULT non applicabile (risposta Andrea presente). Attendere nuova prova Andrea runner Mac (prima Mac, poi Pi 400/RetroPie; D12/D14 = A). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale.
- **CP-007 (FORK1 bivio e transizione di tema): CHIUSO** 2026-10-27 (review incrociate indipendenti positive, ctest 20/20 verde). Non si riapre.
- **CP-005 (R1 tracciati 6 piste): CHIUSO** 2026-10-22. Non si riapre.
- **CP-002 (G1-catalogo inventario grafico): APERTO** — G1-catalogo `[x]` per handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (220 elementi, PNG non catalogati: 0, licenza CC0-1.0, `check_catalogo_grafico.py` exit 0), ma review indipendente ancora da assegnare (regola AGENTS 9): revisore diverso da @grafico (cand. @developer-junior). CP-002 si chiude solo dopo review indipendente positiva.
- **CP-003 e CP-004 restano APERTO** (review indipendente da assegnare, contesto storico).
- Nessun altro checkpoint ha gate + review indipendente registrati da chiudere in questo turno. `FINITO` non creabile: blocchi non tutti `[x]` (T1-RUN [~], PERC1 [~], H [~], DECO1 [~], R2 [~], RIV [~], PIST1 [ ], FIN [ ]).

## Stati proposti (tappa 1 / Gate T1 + BLOCCO PERC1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [~] | @developer + @developer-junior | incrociata (gate_t1.sh: junior→developer POSITIVA; prova_gate.cpp:969: developer→junior POSITIVA) | `out/developer/`, `out/developer-junior/` | soak exit 0 6/6 piste, replay 2× IDENTICO, ms/frame ≤ 20, ctest verde, screenshot P1 completo | CP-006 APERTO (D19 = B). DEFAULT non applicabile. Nessun tocco `src/runner/` prima di ok Andrea Mac (D16). Nessun lavoro Pi 400/RetroPie prima di ok Mac (D12/D14 = A). Gate verde verificato 08/11 (exit 0, log Test area/gate_t1.log). |
| PERC1 | [~] | @developer (D1-D5) · @developer-junior (J1-J4) · @grafico (G1-G3) | J4 review breve in `out/developer-junior/REVIEW_PERC1.md` dopo ogni passo D | `out/developer/`, `out/developer-junior/`, `out/grafico/` | D1-D5 dati percorsi (tratti.json, traffico, frecce, galleria); J1-J4 gate `tratti`/`traffico` + screenshot PPM; G1-G3 asset frecce/galleria + osservazioni; ctest 27/27 verde; `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 dopo ogni passo | Primo passo questo turno: @developer D1, @developer-junior J1, @grafico G1. HUD non si tocca (ANDREA_REVIEW 08/11). Parapetti ponte annullati per sempre. |
| P1 | [x] | @developer | — | `out/developer/` | build CMake, screenshot gameplay (prova differita) | T1-Q2 sì (Andrea). |
| G1-catalogo | [x] | @grafico | da assegnare (cand. @developer-junior, diverso da @grafico) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", solo `pronto`/`approvato` | D15 = A, T1-Q4 attivare. Handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`: 220 elementi, 0 non catalogati, CC0-1.0. CP-002 APERTO in attesa review indipendente. |
| H | [~] | Andrea | — | — | v91 runner Mac su build corretta, poi fps/temp Pi 400 | Prima Mac poi Pi (D12/D14 = A). |
| FORK1 | [x] | @developer + @developer-junior | incrociata positiva (CP-007) | `out/developer/`, `out/developer-junior/` | ctest verde, gate `bivio_*` / `tema_transizione` verdi, replay 2× IDENTICO | CP-007 CHIUSO. Dopo FORK1: DECO1. |
| DECO1 | [~] | @developer (passi piccoli 2a-2f; 2a-2d FATTO, 2e parallasse prossimo) + @grafico (asset; 2f FATTO; skyline e oggetti 6 livelli nel catalogo) | da assegnare per gate `decorazioni` (cand. @developer-junior) | `out/developer/`, `out/grafico/` | ctest verde con `decorazioni` (21 test OK, developer-junior 27/10 06:37); screenshot PPM 6 livelli; ms/frame ≤ 20; check exit 0 | DECO1 [x] prima di ogni prova Pi 400/RetroPie. @grafico fornisce asset e rivede screenshot in `Test area/`. Resta: review incrociata + CP DECO1 (@capo, CP-008 da aprire). |
| F60 | [x] gate / [~] review | @developer | @developer-junior (incrociata pendente) | `out/developer/` | ctest verde 60/50 Hz, replay 2× IDENTICO | Gate verde registrato 22/10 20:06; manca review incrociata developer→developer-junior e apertura CP F60. |
| R2 | [~] | @developer | @developer-junior (incrociata) | `out/developer/` | ctest verde con gate `ciclo`; tutte le 5 fasi | In corso (passi 1/1b/1c fatti, ctest 22/22 verde gate `ciclo`). NON subordinato a D18/D19 (Andrea 27/10). Prossimo: 1d nome a 3 lettere, poi RIV punti a-c, poi PIST1 punto 1. |
| RIV | [~] | @developer + @grafico | @developer-junior (gate `urto`, `gara`) | `out/developer/`, `out/grafico/` | ctest verde con `gara` e `urto`; sprite rivali catalogo; nessun ribaltamento su urto | Dopo R2 1b-1c e DECO1 2f. Punti a,b,c (bump, sprite rivali, comportamento realistico) lato codice fatti; grafica rivali in attesa. Punto 1 (percorso_gara.json per ogni rivale) dopo PIST1. |
| PIST1 | [ ] | @developer + @grafico | @developer-junior (gate `tratti`) | `out/developer/`, `out/grafico/` | ctest verde con `tratti`; screenshot ogni tipo tratto | G-P1 [x] per handoff grafico 2026-11-05 20:46 (parapetto.png 32x48 + guardrail.png 48x32 su disco, raw, 2 voci `pronto`, catalogo exit 0). Ora: @developer P3 (parapetti oggetti ai due lati ogni ~400 unità sui segmenti con `acqua`), poi `sh UFFICIO/Officina/TOOL/gate_t1.sh`; poi @developer-junior J2 e J1 (gate `tratti`). |
| FIN | [ ] | @developer + @grafico | @developer-junior (gate `gara` finale) | `out/developer/`, `out/grafico/` | ctest verde; screenshot 3 finali | Dopo R2 e RIV. 3 scene arrivo "pronto" per @grafico, varianti per posizione da definire con Andrea. |

## Deleghe 4.6 (BLOCCO PERC1, un passo per turno, senza aspettare nessuno)

- **@developer**: **D1** (solo dati) in `dati/tracciati/tratti.json`: `curva_cieca_dx` e `curva_cieca_sx` (curva +/-0.6, `dune` 0.12, `onde` 1, lunghezza 220: la cresta nasconde la curva) e `saliscendi` (`dune` 0.09, `onde` 3, lunghezza 300). Mettili nel `tracciato` di lv_02_atomico, lv_03_foglie e lv_05_spray: curve cieche sempre in coppia dx+sx, prima degli ultimi 1200 segmenti (bivio), somma curva*lunghezza del livello ~0, nessun tipo ripetuto due volte di fila. Poi D2 traffico per tratto, D3 frecce, D4 galleria, D5 una miglioria piccola scelta dalla review J4. Ogni passo: ctest verde (27 test) e `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0. Consegna `out/developer/`.
- **@developer-junior**: **J1** nel gate `tratti` (`test/prova_gate.cpp`): per ogni livello controlla almeno 7 tipi di tratto diversi e nessun tipo ripetuto due volte di fila; stampa una riga per livello. Poi J2 (gate `traffico`: più auto/km sull'autostrada che sul ponte), J3 (gate galleria: segmenti presenti in lv_02 e lv_05, zero oggetti/cartelloni dentro, screenshot PPM in `UFFICIO/Officina/Test area/`), J4 (review breve in `out/developer-junior/REVIEW_PERC1.md` riscritto dopo ogni passo D: cosa va, cosa no, una miglioria piccola proposta per D5). Consegna `out/developer-junior/`.
- **@grafico**: **G1** `freccia_cresta_dx.png` e `freccia_cresta_sx.png` (stessa misura e stile di `freccia_curva_dx/sx.png`, con il segno del dosso), in `assets_originali/scenari/comuni/oggetti/`, script in `UFFICIO/Dipartimento Grafico/TOOLS/`, raw con `png_a_raw.py`, 2 voci `pronto` nel catalogo, `check_catalogo_grafico.py` exit 0. Poi G2 (`parete_galleria.png` 32x32 ripetibile + `luce_galleria.png` 16x8, palette piccola + Bayer come gli skyline) e G3 (3 osservazioni sugli screenshot in `Test area/` in `out/grafico/ULTIMO_TURNO.md` dopo D4). Unico handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. Consegna `out/grafico/`.
- **@capo**: una riga di registro per turno in `bacheche/capo/diario.md`; `out/capo/STATI_PROPOSTI.md` allineato a PERC1. Niente richieste nuove ad Andrea.

## Decisioni Andrea (`ANDREA_REVIEW.md` — DEFAULT solo se mancante; si prosegue senza aspettare)

- T1-Q1 sì, T1-Q2 sì → P1 [x]; T1-Q4 attivare → G1-catalogo; T1-Q5 → R1 (CP-005 chiuso).
- D12 = A, D14 = A, D15 = A, D17 = A, D18 = B (chiusa 27/10).
- **D19: RISPOSTA DI ANDREA: B** → T1-RUN `[~]`, CP-006 APERTO. DEFAULT non si applica. Nuova prova/decisione Andrea richiesta prima di chiudere T1-RUN (runner Mac, prima Mac poi Pi 400; R2 resta slegato da D18/D19 per Andrea 27/10).
- Nessuna domanda di ANDREA_REVIEW.md priva di risposta: tutti i DEFAULT già superati da RISPOSTA DI ANDREA.

## Prossime azioni

1. CP-006 resta APERTO: attendere nuova prova/decisione Andrea su runner Mac (D19 = B; D12/D14 = A: prima Mac, poi Pi 400/RetroPie). DEFAULT non applicabile. Il gate è ora verde e le review indipendenti sono registrate: si aspetta solo la decisione di Andrea.
2. **@developer: PERC1 passo D1** (dati tratti: `curva_cieca_dx/sx`, `saliscendi` nei tracciati lv_02_atomico, lv_03_foglie, lv_05_spray), poi ctest 27/27 verde e `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0.
3. **@developer-junior: PERC1 passo J1** (gate `tratti`: almeno 7 tipi di tratto diversi per livello, nessun tipo ripetuto due volte di fila; una riga per livello).
4. **@grafico: PERC1 passo G1** (`freccia_cresta_dx/sx.png`, raw, 2 voci `pronto`, catalogo exit 0).
5. Dopo ogni passo D: **@developer-junior J4** review breve in `out/developer-junior/REVIEW_PERC1.md`.
6. Quando PERC1 avanza: @capo apre CP per PIST1 dopo P3 + gate, CP-008 per DECO1 dopo review incrociata, CP-002 review indipendente G1-catalogo.
