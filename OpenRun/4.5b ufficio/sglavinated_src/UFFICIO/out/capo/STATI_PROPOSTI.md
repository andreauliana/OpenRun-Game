# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, DIREZIONE_ARTISTICA.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, UFFICIO/Checkpoint Revision/ (STATO_CORRENTE, registro, CP-006), `UFFICIO/Officina/STATUS_OFFICINA.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, RIASSUNTI developer/developer-junior.

**v4.6 CHIUSA 08/11; base legacy 08/11 approvata e NON si tocca (ANDREA_REVIEW sez. 2-3). Tappa attiva: BLOCCO MIG1** (un passo per turno, si procede, niente domande ad Andrea). Finiti i propri passi MIG1 → BLOCCO V47. T1-RUN [x], CP-006 CHIUSO (non riaprire).

## Avanzamento MIG1 (da handoff; prove/review da verificare nel flusso)

| Passo | Stato proposto | Responsabile | Revisore indipendente | Evidenza (handoff) | Blocco residuo |
|---|---|---|---|---|---|
| G1 cartelloni | [x] handoff grafico | @grafico | @developer-junior (review da assegnare) | `STATUS_GRAFICO_PER_CAPO.md` 09/11: 21 sponsor ridisegnati, raw/catalogo aggiornati, 21 voci `pronto` "in attesa ok Andrea", prima/dopo in `Test area/grafico/mig1_g1/`. `check_catalogo_grafico.py` exit 1 **preesistente** (5 `segnaposto` non G1 + 6 PNG non catalogati). Gate/ctest non eseguiti da @grafico (permesso negato). | conferma visiva in gioco da Officina; review indipendente |
| D1 paletti | [x] handoff developer | @developer | @developer-junior **J2** | RIASSUNTO dev 09/11 08:51: paletti in coord. mondo, atterrano sull'erba, mai sotto riga ruote/fuori area; ctest 27/27 exit 0, gate_t1.sh exit 0 (log riferito), replay IDENTICI, ms/frame medio 1.07-1.66 max 10.47 ≤ 20. | J2 prova + screenshot (`Test area/`) |
| J1 review stato | [ ] | @developer-junior | @developer | `REVIEW_MIG1.md` non presente in `UFFICIO/out/developer-junior/`; RIASSUNTO junior fermo a PERC1 (08/11). | J1 da produrre in `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| D2 sportellata | [ ] prossimo dev | @developer | @developer-junior J3 (dopo D2) | — | solo costanti aperte: `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, recupero ~5 s, danni/KO invariati |
| J2 prova D1 | [ ] prossimo junior | @developer-junior | @developer | — | paletto sempre dentro schermo e sopra riga ruote, con screenshot |
| G2 | [ ] dopo D1-D3 | @grafico | — (osservazioni) | — | 3 osservazioni in `ULTIMO_TURNO.md` |

## Stati e deleghe (ruoli 4.6)

| Blocco | Stato | Chi adesso | Revisore indipendente | Percorso | Criterio |
|---|---|---|---|---|---|
| MIG1 | [~] | @developer-junior **J1** (poi J2 su D1) · @developer **D2** · @grafico **G2** dopo D1-D3 | peer dev↔junior; grafico: review non peer ma handoff + verifica | `UFFICIO/out/<ruolo>/` | Ogni passo codice: ctest 27 verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame |
| V47 | [ ] | dopo passi MIG1: dev V1, junior VJ1, grafico VG1 | incrociata | `UFFICIO/out/<ruolo>/` | stesse prove |
| T1-RUN | [x] | — | incrociate POSITIVE | — | CP-006 CHIUSO 08/11 |
| P1 | [x] | — | — | — | screenshot P1 frame 446; non toccare `gate_t1.sh`/`salvaScreenshotP1` |
| H | [~] | Andrea | — | — | prova Pi 400 (D12/D14 = A) |
| DECO1 | [~] | — | review incrociata, poi CP-008 | — | — |
| F60 | [x] gate / [~] review | — | developer→junior pendente | — | poi CP F60 |
| CP-002/003/004 | APERTI | — | review indipendente ≠ autore (cand. junior) | — | review positiva → chiusura |
| MD0, T5, N, M | [-] | sospesi | — | — | — |

## Deleghe immediate
- **@developer-junior J1**: review breve stato attuale in `UFFICIO/out/developer-junior/REVIEW_MIG1.md` (cosa va, cosa no, una miglioria piccola). Poi **J2**: prova D1 paletti (sempre dentro schermo e sopra riga ruote) con screenshot in `UFFICIO/Officina/Test area/`.
- **@developer D2**: sportellata efficace (aperti solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`; rivale sbanda/perde, recupero ~5 s; danni -3%/KO 4a invariati). Dopo ogni passo: ctest + gate, replay identico.
- **@grafico G2** dopo D1-D3: 3 osservazioni in `UFFICIO/out/grafico/ULTIMO_TURNO.md` (se scrittura negata: in `STATUS_GRAFICO_PER_CAPO.md`). Nota: `check_catalogo_grafico.py` exit 1 preesistente non-G1 da segnalare al responsabile competente.
- **@capo**: una riga di registro per turno; STATI_PROPOSTI allineato; niente richieste ad Andrea.

## Vietati (ANDREA_REVIEW sez. 2-3, precedenza assoluta)
Auto/guida legacy (`REGOLE_AUTO_GIOCATORE.md`), HUD, audio esistente (solo aggiunte D5), tracciati/tempi, ponte/galleria/incroci, partenza, menu+logo, grafica di Andrea. Costanti aperte solo quelle indicate. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando il blocco lo chiede; niente file .md in `motore_nuovo/test/`.

## Checkpoint
Nessun CP chiudibile: CP-002/003/004 senza review indipendente; DECO1/F60 senza review incrociata. MIG1 [~] non crea CP finché non ci sono review indipendenti positive sui passi di codice. FINITO non creato. Nessuna domanda ad Andrea; nessun PER_CLAUDE.md.