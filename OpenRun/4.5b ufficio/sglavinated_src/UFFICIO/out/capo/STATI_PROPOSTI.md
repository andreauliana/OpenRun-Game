# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta, sez. 2-6), CONTROLLO.md, DIREZIONE_ARTISTICA.md, AGENTS.md, RICHIESTA_APERTA.md (MIG1 poi V47 continuo), SCAMBIO_RECENTE.md, `UFFICIO/Checkpoint Revision/` (STATO_CORRENTE, REGISTRO append-only, CP-006 CHIUSO), `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`.

**v4.6 CHIUSA 08/11. Base legacy 08/11 approvata e NON si tocca.** Lavoro attivo: **MIG1 [~] quasi chiuso** + **V47 [~] continuo** (solo aggiunte, senza attesa Andrea). Nessun checkpoint chiuso questo turno. Nessuna domanda ad Andrea (si procede; G1 già annullato da Andrea).

## Tappa attiva / Gate
- MIG1: gate = passi completati con prove e review indipendenti registrate dove richiesto; base legacy intatta.
- V47: continuo dopo MIG1; ogni passo con `ctest` verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame.

## Verifica checkpoint
| CP / ambito | Gate + review indipendente registrati? | Azione |
|---|---:|---|
| CP-002/003/004 | no (review indipendente mancante) | APERTI |
| CP-005/006/007 | già CHIUSI | non riaprire |
| DECO1 / F60 | review incrociata mancante | nessuna chiusura |
| MIG1 | prove riportate, review incrociate non in dossier; D4 da chiarire; J3 recupero `sbandataS` non misurato direttamente | nessun CP chiudibile |
| V47 V1/V3 | prove verdi riportate, review non registrata | nessun CP |

## MIG1 — stati proposti (non approvazioni)
| Passo | Stato | Responsabile | Revisore | Consegna | Blocco residuo |
|---|---|---|---|---|---|
| G1 | ANNULLATO | @grafico | — | — | sponsor solo Andrea |
| D1 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | review incrociata registrata |
| D2 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | J3 recupero non misurato |
| D3 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | review codice |
| D4 | [~] da chiarire | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | dev D1-D5 [x] vs junior D4 [ ] |
| D5 | [x] handoff (facoltativo) | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | review non regressione audio |
| J1 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | registrare revisione |
| J2 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | `pali` exit 0 10:45, hit=17 |
| J3 | [~] parziale | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | colpo 0.52s ok; recupero ~5s non misurato |
| J4 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | chiudere dopo J3/D4 review |
| G2 | [x] | @grafico | @capo (handoff) | `UFFICIO/out/grafico/ULTIMO_TURNO.md` | — |

## V47 — stati proposti
| Passo | Stato | Responsabile | Consegna / nota |
|---|---|---|---|
| VG1 | [x] | @grafico | quadro in `UFFICIO/Officina/Test area/grafico/v47_vg1/` |
| V1 | [x] handoff | @developer | scia/sorpasso al pelo; sterzo/curve/derapata/incidenti invariati |
| V2 | [x] | Claude 08/11 | non rifare |
| V3 | [x] handoff | @developer | ms/frame 6 livelli 0.988–1.427 ≤ 20; ctest 27/27, gate_t1 exit 0 (10:34) |
| V4 | [ ] | @developer | dissolvenza 0.5s risultati/menu/partenza |
| V5 | [ ] | @developer | testi a schermo in inglese semplice |
| VJ1 | [~] | @developer-junior | `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto) |
| VJ2 | [ ] | @developer-junior | screenshot spiagge/gabbiani/barche lv_01,04,06 |

## Deleghe (ruoli 4.6)
| Ruolo | Prossimo passo | Revisore | Criterio misurabile / vincolo |
|---|---|---|---|
| @developer | V4 dissolvenza 0.5s, poi V5 testi EN; chiarire D4 in `UFFICIO/out/developer/ULTIMO_TURNO.md`. | @developer-junior | Dopo ogni passo: `ctest` verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms. Solo aggiunte; costanti aperte solo MIG1 (`AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, danni/KO/recupero/paletti). |
| @developer-junior | Review incrociata MIG1 (focus D4, J3); VJ1 su V1/V3/V4; VJ2 screenshot ponte/riva lv_01,04,06. | @developer | Scrive solo `motore_nuovo/test/*.cpp`, `dati/tracciati/regole.json`, `UFFICIO/Officina/TOOL/**`, `UFFICIO/out/developer-junior/`. Registra comando, exit code, misure, path. |
| @grafico | In attesa richiesta @capo o asset approvati da Andrea. | @capo (handoff) | Mai sponsor, mai PNG Andrea, mai HUD/UI/auto/logo extra/premiazione/scenari/skyline/parapetti. Consegna `UFFICIO/out/grafico/`. |
| @capo | Registro append-only; assegnare/registrare review incrociate quando prove complete; aprire CP solo se gate+review indipendenti registrati. | — | Niente codice/asset/review tecnica. |

## Blocchi
- D4 discrepanza handoff (dev vs junior).
- J3 recupero `sbandataS` ~5s non misurato direttamente.
- Review incrociate MIG1/V47 non registrate in dossier.
- `check_catalogo_grafico.py` exit 1 preesistente (fuori MIG1/V47).

## Vietati
Base legacy (auto/HUD/audio/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea/sponsor). Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto; niente file .md in `motore_nuovo/test/`. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

**FINITO:** non creato (blocchi non tutti [x]). **PER_CLAUDE.md:** non creato.