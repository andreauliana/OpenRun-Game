# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta, sez. 2-6), CONTROLLO.md, DIREZIONE_ARTISTICA.md, AGENTS.md, RICHIESTA_APERTA.md (MIG1 poi V47 continuo), SCAMBIO_RECENTE.md, `UFFICIO/Checkpoint Revision/` (STATO_CORRENTE, REGISTRO append-only, CP-006 CHIUSO), `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, `UFFICIO/Officina/STATUS_OFFICINA.md` (storico; per MIG1/V47 usare RIASSUNTI), `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`.

**v4.6 CHIUSA 08/11. Base legacy 08/11 approvata e NON si tocca (ANDREA_REVIEW sez. 2-3).** Lavoro attivo: **MIG1 [~] quasi chiuso** + **V47 [~] in corso** (continuo, senza attesa Andrea; solo aggiunte). CP-006 CHIUSO (non riaprire). Nessuna domanda ad Andrea (si procede).

## Tappa attiva / Gate
- MIG1: gate = passi G1(annullato)/D1-D5/J1-J4 completati con prove e review indipendenti registrate dove richiesto; base legacy intatta.
- V47: continuo dopo MIG1; ogni passo con `ctest` verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame.

## Verifica checkpoint (nessuna chiusura questo turno)
| CP / ambito | Gate + review indipendente registrati? | Azione capo |
|---|---|---|
| CP-002/003/004 | review indipendente mancante | APERTI |
| CP-005/006/007 | già CHIUSI | non riaprire |
| DECO1 / F60 | review incrociata mancante | nessuna chiusura CP |
| MIG1 | prove dev/junior riportate (ctest 27/27, gate_t1 exit 0 10:34; `pali` exit 0 10:45; `sportellata` exit 0 10:15 colpo 0.52s) ma review incrociate non in dossier; D4 discrepanza; J3 recupero `sbandataS` ~5s non misurato direttamente | **nessun CP**: serve review incrociata registrata (dev↔junior), chiarire D4, J3 misura o rilievo accettato con review |
| V47 V1/V3 | prove verdi riportate da @developer; review indipendente non registrata | nessun CP |

## MIG1 — stati proposti (non approvazioni)
| Passo | Stato proposto | Responsabile | Revisore indipendente | Evidenza | Blocco residuo |
|---|---|---|---|---|---|
| G1 cartelloni | **ANNULLATO** | @grafico | — | Sponsor solo Andrea 09/11; originali ripristinati. | vincolo |
| D1 paletti | [x] handoff | @developer | @developer-junior | Coord. mondo/erba; junior `pali` exit 0 10:45, hit=17, screenshot. | review incrociata registrata |
| D2 sportellata | [x] handoff | @developer | @developer-junior | Sterzo rapido→urto in affiancata; `sbandataS=5s`; danni/KO invariati. Junior `sportellata` exit 0 10:15, colpo 0.52s. | J3 recupero non misurato (privato) |
| D3 rivali carattere | [x] handoff | @developer | @developer-junior | 6 stili deterministici, seme fisso. | review diff/codice |
| D4 nomi rivali | [~] da chiarire | @developer | @developer-junior | Dev segnala D1-D5 [x]; junior nota D4 [ ]. Verificare 7 nomi, max 6 MAIUSC, culture/temi. | conferma + review |
| D5 audio extra | [x] handoff (facoltativo) | @developer | @developer-junior | Solo effetti NUOVI; audio esistente intoccato. | review non regressione |
| J1 | [x] | @developer-junior | @developer | `REVIEW_MIG1.md` aggiornato. | review incrociata |
| J2 | [x] | @developer-junior | @developer | `pali` exit 0 10:45, hit=17, screenshot. | registrare revisione |
| J3 | [~] parziale | @developer-junior | @developer | Colpo 0.52s OK; recupero `sbandataS` ~5s non misurato. | misura/strumentazione o rilievo accettato |
| J4 | [x] | @developer-junior | @developer | Review aggiornata 10:49. | chiudere dopo J3/D4 review |
| G2 | [x] | @grafico | @capo (handoff) | 3 osservazioni in `UFFICIO/out/grafico/ULTIMO_TURNO.md`. | — |

## V47 — stati proposti (solo aggiunte)
| Passo | Stato | Responsabile | Evidenza / nota |
|---|---|---|---|
| VG1 quadro/proposte | [x] | @grafico | `UFFICIO/Officina/Test area/grafico/v47_vg1/v47_vg1_quadro_mancanze.png`; gruppo B non rigenerare. |
| V1 scia/sorpasso al pelo | [x] handoff | @developer | Sterzo/curve/derapata/incidenti invariati. |
| V2 pausa | [x] | Claude 08/11 | non rifare. |
| V3 fluidità | [x] handoff | @developer | ms/frame 6 livelli 0.988–1.427 ≤ 20 (gate_t1 10:34); ctest 27/27 exit 0, gate_t1 exit 0. |
| V4 risultati→gioco | [ ] | @developer | dissolvenza 0.5s risultati/menu/partenza. |
| V5 testi EN | [ ] | @developer | tutto schermo in inglese semplice. |
| VJ1 prove V | [~] | @developer-junior | prova dedicata + riga in `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto). |
| VJ2 ponte/riva screenshot | [ ] | @developer-junior | spiagge/gabbiani/barche lv_01,04,06; segnalare densità eccessiva. |

## Deleghe (ruoli 4.6, consegne `UFFICIO/out/<ruolo>/`)
| Ruolo | Prossimo passo | Revisore | Criterio / vincolo |
|---|---|---|---|
| @developer | **V4** dissolvenza 0.5s, poi **V5** testi EN; chiarire D4 in `UFFICIO/out/developer/ULTIMO_TURNO.md` se richiesto da review. | @developer-junior | Dopo ogni passo: `ctest` verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame. Base legacy/vocabolario intatti; costanti aperte solo MIG1 (`AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, danni/KO/recupero/paletti). |
| @developer-junior | Review incrociata MIG1 (D1-D5, focus D4 e J3 recupero); affinare J3 se possibile senza toccare `src/`; **VJ1** su V1/V3/V4 con `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto); **VJ2** screenshot ponte/riva. | @developer | Solo `motore_nuovo/test/`, `UFFICIO/Officina/TOOL/`, `UFFICIO/out/developer-junior/`. Registrare comando, exit code, misure, path log/screenshot. |
| @grafico | **In attesa** richiesta @capo o asset approvati da Andrea (G2 [x], VG1 [x]). | @capo (handoff) | Mai sponsor (G1 annullato), mai PNG Andrea, mai HUD/UI/auto/logo extra/premiazione/scenari/skyline/parapetti. Consegna in `UFFICIO/out/grafico/`; raw con `png_a_raw.py`, catalogo aggiornato. |
| @capo | Una riga registro append-only; STATI_PROPOSTI/STATO_CORRENTE allineati; assegnare/registrare review incrociate MIG1/V47 quando prove complete. Aprire dossier CP solo per revisione sostanziale con gate+review. | — | Niente codice/asset/review tecnica; nessun CP chiuso senza gate+review indipendenti registrati; `RICHIESTA.md`/`SCAMBIO.md` sola lettura. |

## Blocchi / rischi
- **D4**: discrepanza handoff (dev D1-D5 [x] vs junior “D4 [ ]”) — chiarire con review.
- **J3**: recupero `sbandataS` ~5s non misurato direttamente (campo privato) — strumentazione test o rilievo registrato.
- Review incrociate MIG1/V47 non ancora registrate in dossier checkpoint.
- `check_catalogo_grafico.py` exit 1 preesistente (segnaposto + PNG non catalogati), fuori MIG1/V47.

## Stati storici / sospesi
T1-RUN [x] CP-006 CHIUSO · P1 [x] · H [~] Andrea Pi 400 · FORK1 [x] · DECO1 [~] · F60 gate [x]/review [~] · CP-002/003/004 APERTI · MD0,T5,N,M [-].

## Vietati
Base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea). Costanti aperte solo MIG1. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto dal blocco; niente file .md in `motore_nuovo/test/`. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

**FINITO:** non creato (blocchi non tutti [x]). **PER_CLAUDE.md:** non creato.