# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, UFFICIO/Checkpoint Revision/ (STATO_CORRENTE.md, REGISTRO_REVISIONI.md, dossier CP-006), UFFICIO/Officina/STATUS_OFFICINA.md, UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md.

Data: 2026-11-08. **Chiusura CP-006 (T1-RUN, Tappa 1) gia' registrata e verificata su disco: dossier CP-006 (esito CHIUSO), REGISTRO_REVISIONI (riga 2026-11-08) e STATO_CORRENTE allineati. Tappa attiva: BLOCCO PERC1 (rifinitura del percorso, TAPPE.md 08/11).**

Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `UFFICIO/out/developer/`, `UFFICIO/out/developer-junior/`, `UFFICIO/out/grafico/`.

## Chiusura checkpoint verificata (Turno 1, ora 0 di 12)

- CP-006: CHIUSO su mandato Andrea (D19 in ANDREA_REVIEW.md 08/11: T1-RUN [x]; non riaprire). Gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 08/11, `GATE T1: TUTTO OK`, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20 (log `UFFICIO/Officina/Test area/gate_t1.log`). Review indipendenti positive (junior→developer su gate_t1.sh; developer→junior su `prova_gate.cpp:969`).
- CP-005 (R1) e CP-007 (FORK1): CHIUSI, non si riaprono.
- CP-002/CP-003/CP-004: APERTI, in attesa review indipendente da ruolo diverso dall'autore. DECO1 e F60 attendono review incrociata. Nessun altro CP chiudibile ora.
- FINITO non creabile: non tutti i blocchi sono [x] (PERC1, DECO1, F60 review, R2, RIV, PIST1, FIN, H).

## Stati proposti (lavoro attivo = BLOCCO PERC1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Criterio di chiusura | Note |
|---|---|---|---|---|---|---|
| T1-RUN | [x] | @developer + @developer-junior | incrociata POSITIVA | `UFFICIO/out/developer/`, `UFFICIO/out/developer-junior/` | gate exit 0 6/6 piste, replay 2× IDENTICO, ms/frame <= 20 | CP-006 CHIUSO 08/11. Tappa 1 chiusa. |
| PERC1 | [~] | @developer (D1-D5) · @developer-junior (J1-J4) · @grafico (G1-G3) | J4 in `UFFICIO/out/developer-junior/REVIEW_PERC1.md` dopo ogni passo D | `UFFICIO/out/developer/`, `UFFICIO/out/developer-junior/`, `UFFICIO/out/grafico/` | ctest 27/27 verde e `gate_t1.sh` exit 0 dopo ogni passo | Tappa attiva (TAPPE.md 08/11). Un passo per turno, senza aspettare nessuno. Vietati: HUD/UI, auto giocatore (`REGOLE_AUTO_GIOCATORE.md`), premiazione, musica/mixer.*, `src/runner/`, scenari/skyline, parapetti ponte (annullati; si aggiungono solo tratti D1/D4). |
| P1 | [x] | @developer | — | `UFFICIO/out/developer/` | build CMake, screenshot gameplay | T1-Q2 sì. Screenshot P1 passa (frame 446): non ritoccare `gate_t1.sh`/`salvaScreenshotP1`. |
| G1-catalogo | [x] | @grafico | da assegnare (cand. @developer-junior, ≠ @grafico) | `UFFICIO/out/grafico/` | review positiva su CP-002 | Handoff 07/11 12:04: 220 elementi, 0 non catalogati, CC0-1.0. CP-002 APERTO. |
| H | [~] | Andrea | — | — | prova Pi 400/RetroPie quando Andrea vorra' | Ordine test T1-Q5/D12/D14 = A: prima Mac poi Pi 400. |
| FORK1 | [x] | @developer + @developer-junior | incrociata (CP-007) | `UFFICIO/out/developer/`, `UFFICIO/out/developer-junior/` | ctest 20/20, gate `bivio_*`/`tema_transizione` | CP-007 CHIUSO. |
| DECO1 | [~] | @developer (@grafico 2f FATTO) | da assegnare (cand. @developer-junior) | `UFFICIO/out/developer/`, `UFFICIO/out/grafico/` | review incrociata gate `decorazioni` + CP-008 | — |
| F60 | [x] gate / [~] review | @developer | @developer-junior (incrociata pendente) | `UFFICIO/out/developer/` | review incrociata + CP F60 | Gate verde (60/50 Hz, replay IDENTICO). |
| R2 | [~] | @developer | @developer-junior | `UFFICIO/out/developer/` | ctest verde con tutti i gate | NON subordinato a D18/D19 (Andrea 27/10). Ma su PERC1 ora si lavora a passi D, non R2. |
| RIV | [~] | @developer + @grafico | @developer-junior | `UFFICIO/out/developer/`, `UFFICIO/out/grafico/` | ctest verde; sprite rivali in catalogo | Codice a-c fatto; bug sovrapposizioni rivali risolto 08/11 (`progressoTappa`); grafica rivali a @grafico. |
| PIST1 | [~]/[ ] | @developer + @grafico | @developer-junior | `UFFICIO/out/developer/`, `UFFICIO/out/grafico/` | ctest verde con `tratti` | G-P1 [x]. P3 parapetti ANNULLATO per sempre (Andrea 06/11, confermato 08/11). |
| FIN | [ ] | @developer + @grafico | @developer-junior | `UFFICIO/out/developer/`, `UFFICIO/out/grafico/` | ctest verde; screenshot 3 finali | Dopo R2 e RIV. |
| MD0, T5, N, M | [-] | Andrea/grafico | — | — | — | Sospesi. |

## Deleghe 4.6 (BLOCCO PERC1, un passo per turno)

- **@developer — D1** (solo dati) in `dati/tracciati/tratti.json`: `curva_cieca_dx`/`curva_cieca_sx` (curva ±0.6, `dune` 0.12, `onde` 1, lunghezza 220) e `saliscendi` (`dune` 0.09, `onde` 3, lunghezza 300); nel `tracciato` di lv_02_atomico, lv_03_foglie, lv_05_spray: curve cieche sempre in coppia dx+sx, prima degli ultimi 1200 segmenti, somma curva*lunghezza ~0, nessun tipo ripetuto di fila. Poi D2 (traffico per tratto, `Road::trafficoAt`), D3 (frecce cresta), D4 (galleria lv_02/lv_05 + Renderer), D5 (miglioria da review J4). Ogni passo: ctest 27/27 e `gate_t1.sh` exit 0. Consegna `UFFICIO/out/developer/`.
- **@developer-junior — J1** gate `tratti` in `test/prova_gate.cpp`: >= 7 tipi di tratto per livello, nessun tipo ripetuto due volte di fila, una riga per livello. Poi J2 (gate `traffico` dopo D2), J3 (gate galleria + screenshot PPM in `UFFICIO/Officina/Test area/` dopo D4), J4 (review breve in `UFFICIO/out/developer-junior/REVIEW_PERC1.md` riscritto, con una miglioria per D5). Consegna `UFFICIO/out/developer-junior/`.
- **@grafico — G1** `freccia_cresta_dx.png`/`freccia_cresta_sx.png` (stile/misura di `freccia_curva_dx/sx.png`, segno del dosso) in `assets_originali/scenari/comuni/oggetti/`, script in `UFFICIO/Dipartimento Grafico/TOOLS/`, raw con `png_a_raw.py`, 2 voci `pronto`, `check_catalogo_grafico.py` exit 0. Poi G2 (`parete_galleria.png` 32x32 + `luce_galleria.png` 16x8), G3 (3 osservazioni in `UFFICIO/out/grafico/ULTIMO_TURNO.md` dopo D4). Handoff unico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`. Consegna `UFFICIO/out/grafico/`.
- **@capo**: una riga di registro per turno; STATI_PROPOSTI allineato a PERC1. Chiusura CP-006 eseguita e registrata. Niente richieste nuove ad Andrea ("Niente domande ad Andrea per PERC1: si procede").

## Decisioni Andrea (ANDREA_REVIEW.md)

- **D19 CHIUSA (08/11, mandato sblocco agenti) → T1-RUN [x], CP-006 chiuso.** D12 = A, D14 = A: nessun lavoro dedicato Pi 400/RetroPie prima della prova Mac di Andrea.
- HUD NON si tocca (08/11); parapetti ponte annullati per sempre; auto giocatore bloccata (`REGOLE_AUTO_GIOCATORE.md`); 11:40 ANNULLATO 08/11 (curve/rampa ponte e regole rivali TORNATE ai valori bloccati; i 6 gate percorso passano gia').
- Zero domande aperte a Andrea: nessun DEFAULT in uso.

## Prossime azioni

1. @developer: **D1**; @developer-junior: **J1**; @grafico: **G1**. Un passo per turno, gate + ctest verde dopo ogni passo.
2. Quando PERC1 avanza: CP-002 review indipendente; DECO1 incrociata + CP-008; F60 incrociata + CP F60; PIST1 (solo gate `tratti` J1/J2); FIN dopo R2/RIV.
3. FINITO solo quando tutti i blocchi della tappa sono [x].
