# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, DIREZIONE_ARTISTICA.md, AGENTS.md, RICHIESTA_APERTA.md (BLOCCO MIG1), SCAMBIO_RECENTE.md, UFFICIO/Checkpoint Revision/ (STATO_CORRENTE, REGISTRO_REVISIONI, CP-006 CHIUSO), handoff Grafico/Officina.

**v4.6 CHIUSA 08/11; base legacy 08/11 approvata e NON si tocca (ANDREA_REVIEW sez. 2-3). Tappa attiva: BLOCCO MIG1 [~]** (un passo per turno, si procede, niente domande ad Andrea). Finiti i propri passi MIG1 → BLOCCO V47. T1-RUN [x], CP-006 CHIUSO (non riaprire).

## Verifica checkpoint (Turno 1)
| CP | Gate/review registrati? | Azione capo |
|---|---|---|
| CP-002/003/004 | review indipendente mancante | APERTI, assegnare revisore ≠ autore |
| CP-005/006/007 | già CHIUSI | non riaprire |
| DECO1 / F60 | review incrociata mancante | nessuna chiusura CP |
| MIG1 (D1,G1) | prove/handoff presenti, review indipendente non registrata (J2/J1/peer) | nessun CP; tracciare in handoff/log finché review positive |

## Avanzamento MIG1 (da handoff; prove/review da verificare)
| Passo | Stato proposto | Responsabile | Revisore indipendente | Evidenza | Blocco residuo |
|---|---|---|---|---|---|
| G1 cartelloni | [x] handoff | @grafico | @developer-junior (da assegnare) | `STATUS_GRAFICO_PER_CAPO.md` 09/11: 21 sponsor, raw/catalogo, anteprime `Test area/grafico/mig1_g1/`, voci `pronto` in attesa ok Andrea. `check_catalogo_grafico.py` exit 1 preesistente non-G1. | conferma visiva Officina; review indipendente |
| D1 paletti | [x] handoff | @developer | @developer-junior **J2** | RIASSUNTO dev 09/11 08:51: coord. mondo/erba, mai sotto riga ruote/fuori area; ctest 27/27 exit 0, gate_t1.sh exit 0, replay IDENTICI, ms/frame medio 1.07-1.66 max 10.47 ≤ 20. | J2 prova + screenshot |
| J1 review stato | [ ] | @developer-junior | @developer | `REVIEW_MIG1.md` assente. | produrre `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| D2 sportellata | [ ] | @developer | @developer-junior J3 | — | solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`; recupero ~5 s; danni/KO invariati |
| J2 prova D1 | [ ] | @developer-junior | @developer | — | paletto sempre dentro schermo e sopra riga ruote, screenshot in `Test area/` |
| D3/D4/D5, J3/J4, G2 | [ ] | come RICHIESTA_APERTA | incrociata dove codice | — | dopo passi precedenti |

## Deleghe (ruoli 4.6, percorsi UFFICIO/out/<ruolo>/)
| Ruolo | Prossimo passo | Criterio / vincolo |
|---|---|---|
| @developer-junior | **J1** → `UFFICIO/out/developer-junior/REVIEW_MIG1.md`; poi **J2** prova D1 con screenshot | passi piccoli; dopo passi D aggiorna `REVIEW_MIG1.md` (riscritto) |
| @developer | **D2** sportellata efficace | dopo ogni passo codice: ctest 27 verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame |
| @grafico | **G2** dopo D1-D3 (3 osservazioni) | mai ritoccare PNG Andrea; raw con `png_a_raw.py`, catalogo aggiornato |
| @capo | una riga registro append-only; STATI_PROPOSTI/STATO_CORRENTE allineati | niente codice/asset; niente domande ad Andrea; nessun CP chiuso senza gate+review registrati |

## Stati storici / sospesi
T1-RUN [x] (CP-006 CHIUSO) · P1 [x] · H [~] Andrea Pi 400 · FORK1 [x] · DECO1 [~] · F60 gate [x]/review [~] · CP-002/003/004 APERTI · MD0,T5,N,M [-].

## Vietati
Base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea). Costanti aperte solo MIG1. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto; niente file .md in `motore_nuovo/test/`. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

**FINITO:** non creato (blocchi non tutti [x]). **PER_CLAUDE.md:** non creato (non bloccato).
