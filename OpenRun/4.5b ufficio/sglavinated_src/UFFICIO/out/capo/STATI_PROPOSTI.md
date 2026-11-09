# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, UFFICIO/Checkpoint Revision/ (STATO_CORRENTE, registro, CP-006), status Officina e Grafico.

**v4.6 CHIUSA 08/11 sera (Andrea: "circa 80-90%"); base legacy 08/11 approvata e NON si tocca (ANDREA_REVIEW sez. 2-3). Tappa attiva: BLOCCO MIG1** (Andrea 08/11 sera: "un passo per turno, niente domande ad Andrea, si procede"). Finiti i propri passi MIG1, ciascuno passa al BLOCCO V47 (continuo, nessuna attesa). T1-RUN [x] e CP-006 CHIUSO (D19 con RISPOSTA DI ANDREA, non riaprire).

## Stati e deleghe (ruoli 4.6: developer, developer-junior, grafico)

| Blocco | Stato | Chi adesso | Revisore indipendente | Percorso | Criterio |
|---|---|---|---|---|---|
| MIG1 | [ ] | @grafico **G1** · @developer **D1** · @developer-junior **J1** | J per G/D (cand. junior); J1 in `REVIEW_MIG1.md` per D | `UFFICIO/out/<ruolo>/` | Ogni passo: ctest verde (27) + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 <= 12 ms/frame |
| V47 | [ ] | dopo MIG1: developer **V1**, junior **VJ1**, grafico **VG1** | incrociata a ogni passo V | `UFFICIO/out/<ruolo>/` | stesse prove di MIG1 |
| T1-RUN | [x] | — | review incrociate POSITIVE | — | CP-006 CHIUSO 08/11 su mandato Andrea. Gate exit 0, replay identico, ms/frame <= 20 |
| P1 | [x] | — | — | — | screenshot P1 passa (frame 446); non toccare `gate_t1.sh`/`salvaScreenshotP1` |
| H | [~] | Andrea | — | — | prova Pi 400 quando vorra' (D12/D14 = A: prima Mac) |
| DECO1 | [~] | — | review incrociata da completare, poi CP-008 | — | — |
| F60 | [x] gate / [~] review | — | developer→junior pendente | — | review registrata, poi CP F60 |
| CP-002/003/004 | APERTI | — | review indipendente da assegnare (≠ autore; cand. junior) | — | review positiva → chiusura |
| MD0, T5, N, M | [-] | sospesi | — | — | — |

## Deleghe MIG1 (un passo per turno)

- **@grafico G1** cartelloni sponsor leggibili (stesso nome e misura, parola corta grande, 2-3 colori, contorno scuro, dithering Bayer solo su sfondo): `Libreria/assets_originali/sponsor/` + `genera_sponsor.py`, raw con `png_a_raw.py`, catalogo aggiornato, prima/dopo in `Test area/grafico/`. Poi G2 (3 osservazioni dopo D1-D3).
- **@developer D1** paletti abbattuti che volano di lato e atterrano sull'erba, posizione nel mondo, mai sotto la riga ruote (`Game::abbattiPali` + render). Poi D2 sportellata efficace, D3 rivali con carattere, D4 nomi 7 rivali, D5 audio extra (facoltativo). **NOTA BLOCCO:** auto del giocatore = `REGOLE_AUTO_GIOCATORE.md` intoccata; costanti aperte solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, danni/KO/recupero rivali, volo dei paletti.
- **@developer-junior J1** review breve stato attuale in `UFFICIO/out/developer-junior/REVIEW_MIG1.md` (cosa va, cosa no, una miglioria piccola). Poi J2 dopo D1, J3 dopo D2, J4 review aggiornata a ogni passo D.
- **@capo**: una riga di registro per turno; STATI_PROPOSTI allineato; niente richieste ad Andrea.

## Vietati MIG1 (base legacy)

Auto e guida, HUD, audio esistente (solo aggiunte), tracciati/tempi, ponte/galleria/incroci, partenza, menu+logo, grafica di Andrea — elenco completo in ANDREA_REVIEW sez. 2-3. Si migliora SOLO aggiungendo. Niente file .md in `motore_nuovo/test/`; solo dentro `UFFICIO/` e `motore_nuovo/` su richiesta del blocco.

## Checkpoint

CP-002/003/004: APERTI in attesa di review indipendente — nessuna review su disco da chiudere; DECO1 e F60 attendono review incrociata. Nessun CP chiudibile questo turno. FINITO non creato (MIG1/V47 [ ], blocchi non tutti [x]). Nessuna domanda a Andrea (MIG1: "si procede"); nessun PER_CLAUDE.md.
