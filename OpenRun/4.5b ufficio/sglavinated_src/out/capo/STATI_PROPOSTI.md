# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, Checkpoint Revision/ (STATO_CORRENTE.md, REGISTRO_REVISIONI.md, dossier CP-006), UFFICIO/Officina/STATUS_OFFICINA.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md.

Data: 2026-11-08. **Novità del turno: CP-006 CHIUSO su mandato di Andrea (D19 aggiornata e CHIUSA in ANDREA_REVIEW.md 08/11: "T1-RUN [x]. Il @capo chiude CP-006. Non riaprirla"). Gate T1-RUN → [x]. Tappa attiva: BLOCCO PERC1 (rifinitura del percorso).**

Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.

## Verifica chiusure checkpoint

- **CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): CHIUSO** 2026-11-08 su mandato di Andrea — D19 aggiornata e CHIUSA ("il gioco gira sul Mac di Andrea, prove 06-07/11; gate T1 TUTTO OK 08/11; T1-RUN [x]; Il @capo chiude CP-006. Non riaprirla"). Prove: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 (`GATE T1: TUTTO OK`, 6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio 1.079-1.758 max 17.003 <= 20, screenshot P1 frame 446 > 0; log `UFFICIO/Officina/Test area/gate_t1.log`); review indipendenti POSITIVE (developer-junior → developer su `gate_t1.sh` in `out/developer-junior/REVIEW_GATE_T1_P1_2026-11-08.md`; developer → developer-junior su `prova_gate.cpp:969`). Non riaprire.
- **CP-005 (R1) e CP-007 (FORK1): CHIUSI** (22/10 e 27/10). Non si riaprono.
- **CP-002 (G1-catalogo): APERTO** — lavoro [x] per handoff grafico (220 elementi, 0 non catalogati, CC0-1.0), resta review indipendente da assegnare a un ruolo diverso da @grafico (cand. @developer-junior).
- **CP-003 e CP-004: APERTI** (review indipendente da assegnare; contesto storico).
- Nessun altro checkpoint con gate + review registrati da chiudere in questo turno. `FINITO` non creabile: blocchi non tutti `[x]` (PERC1, DECO1, F60 review, R2, RIV, PIST1, FIN, H).

## Stati proposti (dopo chiusura CP-006; lavoro attivo = BLOCCO PERC1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [x] | @developer + @developer-junior | incrociata POSITIVA (gate_t1.sh: junior→developer; prova_gate.cpp:969: developer→junior) | `out/developer/`, `out/developer-junior/` | gate exit 0 6/6 piste, replay 2× IDENTICO, ms/frame <= 20 | CP-006 CHIUSO 08/11 (D19 chiusa, mandato Andrea). Chiudere la Tappa 1. |
| PERC1 | [~] | @developer (D1-D5) · @developer-junior (J1-J4) · @grafico (G1-G3) | J4 review breve in `out/developer-junior/REVIEW_PERC1.md` dopo ogni passo D | `out/developer/`, `out/developer-junior/`, `out/grafico/` | D1-D5 dati percorsi (tratti, traffico, frecce, galleria); J1-J4 gate `tratti`/`traffico`/galleria + screenshot PPM in `UFFICIO/Officina/Test area/`; G1-G3 asset + osservazioni; ctest 27/27 verde e `gate_t1.sh` exit 0 dopo ogni passo | Tappa attiva (TAPPE.md 08/11). Un passo per turno, senza aspettare nessuno. Vietati: HUD, auto giocatore, premiazione, musica/mixer.*, `src/runner/`, scenari/skyline, parapetti ponte. |
| P1 | [x] | @developer | — | `out/developer/` | build CMake, screenshot gameplay | T1-Q2 sì (Andrea). |
| G1-catalogo | [x] | @grafico | da assegnare (cand. @developer-junior; diverso da @grafico) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", solo `pronto`/`approvato` | Handoff 07/11 12:04: 220 elementi, 0 non catalogati, CC0-1.0. CP-002 APERTO in attesa review. |
| H | [~] | Andrea | — | — | prova Pi 400/RetroPie quando Andrea vorra' | Ordine test T1-Q5/D12/D14 = A: prima Mac poi Pi 400; nessun lavoro dedicato Pi prima della prova di Andrea. |
| FORK1 | [x] | @developer + @developer-junior | incrociata positiva (CP-007) | `out/developer/`, `out/developer-junior/` | ctest 20/20, gate `bivio_*`/`tema_transizione` verdi | CP-007 CHIUSO. |
| DECO1 | [~] | @developer (2e parallasse prossimo) + @grafico (2f FATTO; asset catalogo) | da assegnare (cand. @developer-junior) | `out/developer/`, `out/grafico/` | review incrociata gate `decorazioni` positiva + CP-008 del @capo | DECO1 [x] prima di ogni prova Pi 400/RetroPie. |
| F60 | [x] gate / [~] review | @developer | @developer-junior (incrociata pendente) | `out/developer/` | review incrociata registrata + CP F60 | Gate verde 22/10 20:06 (60/50 Hz, replay IDENTICO). |
| R2 | [~] | @developer | @developer-junior (incrociata) | `out/developer/` | ctest verde con gate `ciclo`; tutte le 5 fasi | NON subordinato a D18/D19 (Andrea 27/10). Prossimo: 1d, poi RIV a-c, poi PIST1 punto 1. |
| RIV | [~] | @developer + @grafico | @developer-junior (gate `urto`, `gara`) | `out/developer/`, `out/grafico/` | ctest verde con `gara`/`urto`; sprite rivali nel catalogo | Punti a-c codice fatti; grafica rivali a @grafico. Punto 1 dopo PIST1. |
| PIST1 | [ ] | @developer + @grafico | @developer-junior | `out/developer/`, `out/grafico/` | ctest verde con `tratti`; screenshot ogni tipo tratto | **P3 annullato per sempre** (Andrea 06/11, confermato 08/11): ponte aperto, filo basso grigio/bianco, niente parapetti. Restano gate `tratti` J1/J2 dopo le basi D1-D2. |
| FIN | [ ] | @developer + @grafico | @developer-junior | `out/developer/`, `out/grafico/` | ctest verde; screenshot 3 finali | Dopo R2 e RIV. |
| MD0, T5, N, M | [-] | Andrea/grafico | — | — | — | Sospesi (musica seguire playlist.json; M attesa ok Andrea PNG B/N). |

## Deleghe 4.6 (BLOCCO PERC1, un passo per turno, senza aspettare nessuno)

- **@developer — D1** (solo dati) in `dati/tracciati/tratti.json`: `curva_cieca_dx` e `curva_cieca_sx` (curva ±0.6, `dune` 0.12, `onde` 1, lunghezza 220), `saliscendi` (`dune` 0.09, `onde` 3, lunghezza 300); nel `tracciato` di lv_02_atomico, lv_03_foglie, lv_05_spray: curve cieche sempre in coppia dx+sx, prima degli ultimi 1200 segmenti (bivio), somma curva*lunghezza ~0, nessun tipo ripetuto due volte di fila. Poi D2 (traffico per tratto), D3 (frecce cresta se @grafico le ha fatte), D4 (galleria in lv_02 e lv_05), D5 (una miglioria dalla review J4). Ogni passo: ctest 27/27 verde e `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0. Consegna `out/developer/`.
- **@developer-junior — J1** nel gate `tratti` (`test/prova_gate.cpp`): per ogni livello almeno 7 tipi di tratto diversi e nessun tipo ripetuto due volte di fila; una riga per livello. Poi J2 (gate `traffico` dopo D2), J3 (gate galleria + screenshot PPM dopo D4), J4 (review breve in `out/developer-junior/REVIEW_PERC1.md` riscritto dopo ogni passo D, con una miglioria piccola per D5). Consegna `out/developer-junior/`.
- **@grafico — G1** `freccia_cresta_dx.png`/`freccia_cresta_sx.png` (stessa misura e stile di `freccia_curva_dx/sx.png`, con il segno del dosso) in `assets_originali/scenari/comuni/oggetti/`, script in `UFFICIO/Dipartimento Grafico/TOOLS/`, raw con `png_a_raw.py`, 2 voci `pronto`, `check_catalogo_grafico.py` exit 0. Poi G2 (`parete_galleria.png` 32x32 + `luce_galleria.png` 16x8), G3 (3 osservazioni sugli screenshot in `Test area/` in `out/grafico/ULTIMO_TURNO.md` dopo D4). Unico handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md`. Consegna `out/grafico/`.
- **@capo**: una riga di registro per turno in `bacheche/capo/diario.md`; `out/capo/STATI_PROPOSTI.md` allineato a PERC1; chiusura CP-006 eseguita questo turno. Niente richieste nuove ad Andrea.

## Decisioni Andrea (`ANDREA_REVIEW.md` — attenzione all'ordine con l'ultima)

- **D19 CHIUSA (08/11, mandato sblocco agenti) → T1-RUN [x], CP-006 chiuso.** D12 = A, D14 = A resta l'ordine: nessun lavoro dedicato Pi 400/RetroPie prima della prova Mac di Andrea.
- T1-Q1/Q2 sì (P1 [x]), T1-Q4 attivare (G1), T1-Q5 R1 (CP-005 chiuso), D15 = A, D17 = A, D18 = B ma Andrea 27/10: R2 e blocchi dopo non aspettano D18/D19.
- HUD non si tocca (08/11); parapetti ponte annullati per sempre (06/11, confermato 08/11); auto del giocatore in `REGOLE_AUTO_GIOCATORE.md` — non si tocca; noto ANNULLATO 08/11 11:40 (Claude, copia vecchia di RICHIESTA): curve/rampa del ponte e regole rivali invariate.
- Tutte le domande aperte di `ANDREA_REVIEW.md` sono chiuse o con risposta di Andrea: nessun DEFAULT in uso, nessuna domanda nuova per PERC1 ("si procede").

## Prossime azioni

1. **@developer: PERC1 D1** (tratti curva cieca dx+sx e saliscendi in lv_02/lv_03/lv_05), poi ctest 27/27 verde e `gate_t1.sh` exit 0.
2. **@developer-junior: PERC1 J1** (gate `tratti`), poi J4 review dopo ogni passo D.
3. **@grafico: PERC1 G1** (freccia_cresta_dx/sx.png), poi G2/G3.
4. Quando PERC1 avanza: CP-002 review indipendente; DECO1 review incrociata + CP-008; F60 review incrociata + CP F60; R2 1d; PIST1 (P3 annullato: solo gate `tratti` J1/J2); FIN dopo R2/RIV.
5. FINITO non creabile finché PERC1, DECO1, H, R2, RIV, PIST1, FIN non sono [x].
