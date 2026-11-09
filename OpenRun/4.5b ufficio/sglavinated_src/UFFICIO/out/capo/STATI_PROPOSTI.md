# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

Data: 2026-11-09. Fonti lette: `TAPPE.md`, `ANDREA_REVIEW.md` (precedenza assoluta; decisioni aperte: nessuna), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (4.6), `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo), `SCAMBIO_RECENTE.md`, `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md`, handoff `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`.

**Base legacy 08/11 approvata e NON si tocca** (ANDREA_REVIEW sez. 2-3). Lavoro solo su MIG1/V47 (aggiunte). Nessuna domanda ad Andrea.

## Checkpoint — stato proposto
| ID | Stato proposto | Motivo |
|---|---|---|
| CP-002, CP-003, CP-004 | APERTO | review indipendente da assegnare/completare |
| CP-005, CP-006, CP-007 | CHIUSO | già chiusi; **CP-006 non riaprire** |
| DECO1 / F60 | nessun nuovo CP | review incrociata mancante |
| MIG1 / V47 | nessun nuovo CP | prove riportate ma review incrociate non registrate in dossier; residui D4/J3/V5 instradati sotto |

Nessun checkpoint con gate + review indipendente registrati da chiudere in questo turno.

## MIG1 — stati e deleghe (ruoli 4.6)
| Passo | Stato | Responsabile | Revisore indipendente | Consegna | Criterio / blocco |
|---|---|---|---|---|---|
| G1 | ANNULLATO | @grafico | — | — | cartelloni solo Andrea (09/11) |
| D1 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | paletti su erba; review da registrare |
| D2 | [~] da correggere | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | fix mirato: sportellata rivale bloccata da `playerBump_` in `src/game/game.cpp` (evidenza junior 12:53, `sportellata` exit 28 su lv_01_neon; vecchia evidenza 0.52s era urto traffico). Solo costanti/rami MIG1 aperti; base legacy intoccata |
| D3 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | carattere rivali deterministico |
| D4 | [~] | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | discrepanza dev D1-D5 [x] vs junior D4 [ ]: chiarire con elenco nomi/origine/tema |
| D5 | [x] facolt. handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | solo audio extra; review |
| J1 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | — |
| J2 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | `pali` exit 0 10:45, hit=17 |
| J3 | [~] bloccato | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | dopo fix D2: rieseguire `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj3_rivale" lv_01_neon sportellata` (atteso exit 0, ripresa 5-8s); misurare/registrare recupero (strumentare se `sbandataS` resta privato) |
| J4 | [x]/aggiornare | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | riscritto dopo D2 fix + J3 + D4 |
| G2 | [x] prove | @grafico | @capo (handoff) | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | screenshot J2/J3 visti 12:31; 3 osservazioni (nota cosmetica targhette rivali = render dev) |

## V47 — stati e deleghe (ruoli 4.6)
| Passo | Stato | Responsabile | Revisore indipendente | Consegna | Criterio |
|---|---|---|---|---|---|
| VG1 | [x] | @grafico | @capo (handoff) | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | quadro `UFFICIO/Officina/Test area/grafico/v47_vg1/` |
| V1 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | scia/sorpasso al pelo; sterzo/curve/derapata/incidenti invariati |
| V2 | [x] | Claude 08/11 | — | — | non rifare |
| V3 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | ms/frame 6 livelli ≤ 20 (gate_t1 10:34) |
| V4 | [x] handoff | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | dissolvenza 0.5s (da review junior) |
| V5 codice/UI | [x] parziale | @developer | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` | `src/` UI EN verificata (dev 12:59) |
| V5 frasi | [~] | @grafico | @developer-junior | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, `motore_nuovo/dati/frasi.json` | tradurre/adattare 156 frasi Gianni/Gesù in inglese semplice; preservare struttura/attribuzione CC0-1.0 progetto, nessun materiale Sega/marchi veri; nessun PNG Andrea/sponsor/HUD/UI extra; dopo modifica: ctest + `gate_t1.sh` exit 0 a cura @developer o come assegnato |
| VJ1 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_V47.md` | riscritto con prove |
| VJ2 | [x] | @developer-junior | @developer | `UFFICIO/out/developer-junior/REVIEW_V47.md` | screenshot riva lv_01,04,06 |

Prove riportate (handoff dev 10:34): `ctest` 27/27 exit 0, `gate_t1.sh` exit 0, 6/6 replay IDENTICI, ms/frame ≤ 20 (macOS). Junior 12:53: `sportellata` exit 28 (blocco D2), VJ2 [x].

## Deleghe sintetiche
| Ruolo | Prossimo passo | Criterio di completamento |
|---|---|---|
| @developer | 1) Fix mirato D2 sportellata in `src/game/game.cpp` (solo MIG1), rieseguire build/test; 2) chiarire D4 in `UFFICIO/out/developer/ULTIMO_TURNO.md`; 3) supportare V5 frasi se serve gate dopo modifica dati | dopo ogni passo codice/dati: `ctest` verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO; solo aggiunte; costanti aperte solo MIG1; base legacy intoccata |
| @developer-junior | Dopo fix D2: rieseguire `sportellata` (evidenza exit code + recupero); review incrociata MIG1 (focus D2/D4/J3) e V5 frasi | `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md` con comando, exit code, misure, path relativi |
| @grafico | V5 frasi: `motore_nuovo/dati/frasi.json` in inglese semplice (struttura preservata) | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`; mai sponsor/PNG Andrea/HUD/UI/auto/logo extra/premiazione/scenari/skyline/parapetti; nessun codice |
| @capo | Registro append-only; registra review incrociate in dossier/CP solo a gate+review indipendenti completi | no codice/asset/review tecnica; `RICHIESTA.md`/`SCAMBIO.md` sola lettura |

## Blocchi / decisioni
- Review incrociate MIG1/V47 non registrate in dossier/checkpoint → nessuna chiusura CP.
- D2: `sportellata` exit 28 (junior 12:53), fix `src/game/game.cpp` fuori perimetro junior → @developer.
- D4 discrepanza dev/junior → chiarire in review/ULTIMO_TURNO.
- V5: `frasi.json` dato @grafico, fuori perimetro dev → @grafico.
- CP-002/003/004, DECO1, F60: review indipendente/incrociata mancante (fuori MIG1).
- `check_catalogo_grafico.py` exit 1 preesistente, fuori MIG1/V47.
- Decisioni Andrea: nessuna aperta. **FINITO:** no. **PER_CLAUDE.md:** no.
