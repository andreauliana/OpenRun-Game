# STATI PROPOSTI — Turno 1 (ora 1 di 12) — @capo

Data: 2026-11-10. Fonti lette (solo): `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova, vale risposta Andrea mai DEFAULT; base legacy 08/11 **NON si tocca**; G1 **ANNULLATO**, sponsor solo Andrea), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` 4.6, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo), `SCAMBIO_RECENTE.md` (+ `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Grafico/Dev/Junior).

**Tappa attiva / Gate:** MIG1 [~] + V47 [~] (solo aggiunte). Gate T1 green riportato (handoff dev/junior, exit 0). **Nessun CP chiudibile:** gate/prove riportate ma review incrociate MIG1/V47 non registrate in dossier (autore≠revisore). VJ2 `vj2_riva` exit 0: densità da ispezione Andrea, **DEFAULT non blocca**. **FINITO:** no (blocchi non tutti [x]). **PER_CLAUDE:** no. Domande ad Andrea: nessuna (nessuna decisione aperta).

## Checkpoint
| ID | Stato proposto | Motivo |
|---|---|---|
| CP-002, CP-003, CP-004 | APERTO | review indipendente da assegnare/completare |
| CP-005, CP-006, CP-007 | CHIUSO | **CP-006 non riaprire** |
| DECO1, F60 | APERTO | review incrociata mancante |
| MIG1 | APERTO | D2 `sportellata` exit 28, J3/J4 post-fix, D4 da chiarire; review incrociate non in dossier |
| V47 | APERTO | VJ2 ispezione Andrea (DEFAULT non blocca); review non registrate in dossier |

## Stati proposti e deleghe 4.6
| Task | Stato | Responsabile | Revisore indipendente | Criterio misurabile / consegna |
|---|---|---|---|---|
| G1 sponsor | ANNULLATO | Andrea | — | Cartelloni solo Andrea; originali ripristinati (nessun tocco agenti) |
| D1,D3,D5,J1,J2,VG1,G2,V1-V4,V5,VJ1 | [x] handoff | @developer / @developer-junior / @grafico | incrociato (autore≠revisore) | Evidenze in RIASSUNTI/REVIEW/out; gate/report verdi riportati. Registrare review con cmd/exit/misure in dossier prima di qualunque chiusura CP (lacuna screenshot D3 rivali segnalata) |
| D2 sportellata | [~] | @developer | @developer-junior | Fix mirato `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, trigger/clamp sterzo 60 Hz per `REVIEW_MIG1.md`). Prove: `sportellata` exit 0, ctest verde, `gate_t1.sh` exit 0, replay 2x IDENTICO → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| D4 nomi rivali | [~] | @developer | @developer-junior | 7 nomi (max 6 MAIUSC.) con origine/tema allineati a `NOMI_RIVALI`/temi livelli; chiarire discrepanza → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| J3,J4 MIG1 | [~] | @developer-junior | @developer | Post-fix D2: `sportellata` exit 0 + recupero ~5 s misurabile; aggiornare `UFFICIO/out/developer-junior/REVIEW_MIG1.md` (riscritto, cmd/exit/misure) |
| VJ2 riva | [~] | @developer-junior | Andrea (ispezione) | `vj2_riva` exit 0 registrato; densità spiagge/gabbiani/barche lv_01/04/06 da ispezione Andrea, **DEFAULT non blocca** |
| Review incrociate MIG1/V47 | [ ] | @developer-junior, @developer | autore≠revisore | Evidenze in `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md` (cmd/exit/misure) prima di qualunque chiusura CP |

## Prossime azioni (un passo per ruolo)
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): D2 fix mirato in `src/game/game.cpp` (solo costanti/trigger aperti MIG1) per `sportellata` exit 0; chiarire/allineare D4; riportare cmd/exit/misure + ctest/gate/replay.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): post-fix D2 rieseguire J3 (`sportellata` exit 0 + recupero ~5 s) e aggiornare J4 `REVIEW_MIG1.md`; registrare review incrociate MIG1/V47 in `REVIEW_MIG1.md`, `REVIEW_V47.md`.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): nessun passo [ ]/[~] aperto; in attesa. Non toccare sponsor/asset Andrea/HUD/UI/auto/logo/menu. Solo su richiesta: arbitrare lacuna screenshot D3 rivali o conflitto tool frasi se bloccante.
4. **@capo** (`UFFICIO/Checkpoint Revision/`, `UFFICIO/out/capo/STATI_PROPOSTI.md`): registro append-only + `STATO_CORRENTE.md`; aprire/chiudere CP solo con gate+review indipendenti registrati. Nessuna chiusura CP ora.

Vincoli: base legacy 08/11 e vocabolario non si toccano (solo aggiunte); no sponsor; no `RICHIESTA.md`/`SCAMBIO.md`; log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato da gate.