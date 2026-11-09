# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-09. Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta, sez. 2-5), CONTROLLO.md, DIREZIONE_ARTISTICA.md, AGENTS.md, RICHIESTA_APERTA.md (MIG1 poi V47), SCAMBIO_RECENTE.md, `UFFICIO/Checkpoint Revision/` (STATO_CORRENTE, REGISTRO append-only, CP-006 CHIUSO), handoff `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (09/11 10:08), `UFFICIO/Officina/STATUS_OFFICINA.md` (storico; per MIG1 usare RIASSUNTI), `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/bacheche/grafico/RIASSUNTO.md`.

**v4.6 CHIUSA 08/11. Base legacy 08/11 approvata e NON si tocca (ANDREA_REVIEW sez. 2-3).** Tappa attiva: **MIG1 [~]** (si procede, un passo per turno; niente domande ad Andrea). Finiti i propri passi MIG1 → V47. T1-RUN [x], CP-006 CHIUSO (non riaprire).

## Verifica checkpoint (nessuna chiusura)
| CP | Gate/review registrati? | Azione capo |
|---|---|---|
| CP-002/003/004 | review indipendente mancante | APERTI |
| CP-005/006/007 | già CHIUSI | non riaprire |
| DECO1 / F60 | review incrociata mancante | nessuna chiusura CP |
| MIG1 | D1-D4 handoff; J2/J3 codice ma prove non eseguite (shell/binario/pre-esistente build prova_gate); G1 annullato da Andrea | nessun CP; serve review indipendente + prove registrate |

## Avanzamento MIG1 (stati proposti da handoff; non approvazioni)
| Passo | Stato proposto | Responsabile | Revisore indipendente | Evidenza | Blocco residuo |
|---|---|---|---|---|---|
| G1 cartelloni | **ANNULLATO** Andrea 09/11 | @grafico | — | `STATUS_GRAFICO_PER_CAPO.md`/RIASSUNTO grafico: originali ripristinati; sponsor li tocca solo Andrea; non toccare PNG/raw. | nessuno (vincolo) |
| D1 paletti | [x] handoff | @developer | @developer-junior J2 | dev RIASSUNTO 09/11 10:00: coord. mondo/erba, scorrono via; base ≥ riga ruote. Gate verde noto 09/11 09:22 (dev D2). | J2 build+run `pali` + screenshot in `UFFICIO/Officina/Test area/` |
| D2 sportellata | [x] handoff | @developer | @developer-junior J3 | dev RIASSUNTO: sterzo rapido in affiancata → urto; rivale `sbandataS=5s`; danni/KO invariati. Costanti aperte: `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`. | J3 build+run `sportellata` + misure/screenshot |
| D3 rivali carattere | [x] handoff | @developer | @developer-junior J4/review | dev RIASSUNTO: 6 stili deterministici, seme fisso, movimento liscio. | review indipendente su diff/codice |
| D4 nomi rivali | [x] handoff | @developer | @developer-junior | dev RIASSUNTO + `ULTIMO_TURNO.md` (7 nomi, max 6 MAIUSC, culture/temi). | review |
| D5 audio extra | [ ] facoltativo | @developer | @developer-junior | — | solo effetti NUOVI, audio esistente intoccato |
| J1 review stato | [x] handoff | @developer-junior | @developer | junior RIASSUNTO: J1 [x]. `REVIEW_MIG1.md` da aggiornare a chiusura J2-J4. | review incrociata |
| J2 prova D1 | [~] codice | @developer-junior | @developer | modo `pali` in `motore_nuovo/test/prova_gate.cpp`; esecuzione bloccata (shell permission, binario 09:44 obsoleto). | build `prova_gate`, run, exit code/misure/screenshot |
| J3 prova D2 | [~] codice | @developer-junior | @developer | modo `sportellata` in `prova_gate.cpp`; esecuzione bloccata; nota `sbandataS` privato. | build `prova_gate`, run, misure ripresa/screenshot |
| J4 aggiorna review | [~] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` aggiornato, da chiudere dopo prove. | riscrivere con esiti reali |
| G2 osservazioni | [x] | @grafico | @capo (verifica handoff) | `UFFICIO/out/grafico/ULTIMO_TURNO.md`: 3 osservazioni; screenshot J2/J3 non presenti. | nessuno |

## V47 (finito MIG1, continuo, aggiunte)
| Passo | Stato | Responsabile |
|---|---|---|
| VG1 quadro/proposte | [x] | @grafico (`v47_vg1_quadro_mancanze.png` generata, exit 0) |
| V1 scia/sorpasso al pelo | [ ] | @developer (solo aggiunte; sterzo/curve/derapata/incidenti invariati) |
| V2 pausa | [x] | Claude 08/11 |
| V3-V5 fluidità/risultati/testi EN | [ ] | @developer; VJ1/VJ2 @developer-junior |

## Deleghe (ruoli 4.6, consegne `UFFICIO/out/<ruolo>/`)
| Ruolo | Prossimo passo | Criterio / vincolo |
|---|---|---|
| @developer-junior | Quando shell ok: build `prova_gate`, eseguire `pali` e `sportellata`, registrare comando+exit+misure+screenshot in `UFFICIO/Officina/Test area/` e `UFFICIO/out/developer-junior/REVIEW_MIG1.md` (riscritto). | nessuna modifica a HUD, `gate_t1.sh` se non richiesto, `salvaScreenshotP1`, `motore_nuovo/src/` |
| @developer | Sbloccare `motore_nuovo/test/prova_gate.cpp:2136` pre-esistente se nel perimetro assegnato, poi ctest + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0; quindi D5 (facoltativo) o V47 V1. | dopo ogni passo codice: ctest verde + gate_t1.sh exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame; costanti aperte solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, danni/KO/recupero/paletti |
| @grafico | In attesa richiesta @capo o nuovi asset approvati da Andrea. Non toccare sponsor (G1 annullato). | mai PNG Andrea; raw con `png_a_raw.py`; catalogo aggiornato |
| @capo | registro append-only per turno; STATI_PROPOSTI/STATO_CORRENTE allineati; assegnare review indipendente quando prove disponibili. | niente codice/asset; niente domande ad Andrea; nessun CP chiuso senza gate+review registrati |

## Blocchi segnalati (da smistare, non risolti dal capo)
- Shell `permission.rejected` lato junior (build/run `pali`/`sportellata` non eseguibili nel turno 09/11 10:02).
- `prova_gate.cpp:2136` pre-esistente: `!game.init()` ma init è void (developer 09/11 10:00) → ctest/gate da build developer bloccati finché non corretto; gate verde noto 09/11 09:22 pre-modifica.
- `check_catalogo_grafico.py` exit 1 preesistente (5 segnaposto + 6 PNG non catalogati), fuori MIG1.

## Stati storici / sospesi
T1-RUN [x] (CP-006 CHIUSO) · P1 [x] · H [~] Andrea Pi 400 · FORK1 [x] · DECO1 [~] · F60 gate [x]/review [~] · CP-002/003/004 APERTI · MD0,T5,N,M [-].

## Vietati
Base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea). Costanti aperte solo MIG1. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto; niente file .md in `motore_nuovo/test/`. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

**FINITO:** non creato (blocchi non tutti [x]). **PER_CLAUDE.md:** non creato (non bloccato dal capo; blocchi tecnici ai ruoli assegnati).