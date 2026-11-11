# STATI PROPOSTI — Turno 3 (ora 4 di 12) — @capo

Data: 2026-11-11. Fonti lette (solo): `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova, base legacy 08/11 **NON si tocca**; G1 **ANNULLATO**, sponsor solo Andrea), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` 4.6, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo, passi ∞), `SCAMBIO_RECENTE.md`, `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, `UFFICIO/Officina/STATUS_OFFICINA.md` (11/11: `gate_t1.sh` exit 0 `GATE T1: TUTTO OK`, ctest 27/27 exit 0, picco max 55.27 ms lv_05 smoke isolato, log `UFFICIO/Officina/Test area/gate_t1.log`), `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (11/11 11:47 VG2 giro 17, `verifica_frasi_vg2.py` exit 0 156/156).

**Tappa attiva / Gate:** MIG1 [~] + V47 [~]. Gate T1 green riportato; **nessun CP chiudibile** (nessun dossier con gate+review indipendenti autore≠revisore tutte registrate). **FINITO:** no. **PER_CLAUDE:** no. Domande ad Andrea: nessuna nuova (domande aperte in `ANDREA_REVIEW.md` restano con DEFAULT finché non c'è RISPOSTA DI ANDREA).

## Checkpoint
| ID | Stato proposto | Motivo |
|---|---|---|
| CP-002, CP-003, CP-004 | APERTO | review indipendente da assegnare/completare |
| CP-005, CP-006, CP-007 | CHIUSO | **CP-006 non riaprire** |
| DECO1, F60 | APERTO | review incrociata mancante |
| MIG1 | APERTO | D2 `sportellata` exit 28 aperto; D4 da chiarire; review J3/J4/D2 non consolidate in dossier |
| V47 | APERTO | V6 review parziale; VJ4 soak lv_05-06 mancanti; VG2 giro 17 fatto (giro 18/resto ∞); passi ∞ in corso |

## Stati proposti e deleghe 4.6
| Task | Stato | Responsabile | Revisore indipendente | Criterio misurabile / consegna |
|---|---|---|---|---|
| G1 sponsor | ANNULLATO | Andrea | — | Cartelloni solo Andrea |
| D1,D3,D5,J1,J2,VG1,G2,V1-V5,VJ1,VJ3 | [x]/handoff | @developer/@developer-junior/@grafico | incrociato (autore≠revisore) | Evidenze in `UFFICIO/out/<ruolo>/`; registrare review in dossier (cmd/exit/misure) |
| V6 libretro | [~] | @developer | @developer-junior | Consolidare review: ctest 27/27 exit 0, gate_t1 exit 0, 6/6 GATE PERCORSO OK, replay IDENTICO; valutare picco 55.27 ms lv_05 → `UFFICIO/out/developer-junior/REVIEW_V47.md` |
| D2 sportellata | [~] | @developer | @developer-junior | Fix mirato `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, trigger/clamp sterzo 60 Hz). Prove: `sportellata` exit 0, ctest verde, `gate_t1.sh` exit 0, replay 2x IDENTICO |
| D4 nomi rivali | [~] | @developer | @developer-junior | 7 nomi (max 6 MAIUSC.) allineati a `NOMI_RIVALI` con origine/tema → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| J3,J4 MIG1 | [~] | @developer-junior | @developer | Post-fix D2: `sportellata` exit 0 + recupero ~5 s misurabile; aggiornare `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| VJ4 soak | [~] | @developer-junior | @developer | lv_01-04 completati (exit 0, 30000f IDENTICO). Prossimo: lv_05_spray poi lv_06_desktop; log in `UFFICIO/Officina/Test area/` |
| VG2 frasi EN | [~] | @grafico | @developer-junior | Giro 18 (20 frasi) o ∞ VG3; max 16 char MAIUSC., `it1/it2`/id/personaggio intatti; `verifica_frasi_vg2.py` exit 0 |
| VJ2 riva | [~] | @developer-junior | Andrea (ispezione) | `vj2_riva` exit 0; densità lv_01/04/06 da ispezione Andrea (**DEFAULT non blocca**) |
| V7/V8, passi ∞ (V9/VJ5/VG3) | [ ]/[∞] | @developer/@developer-junior/@grafico | incrociato | Un passo per turno; ctest/gate exit 0, replay IDENTICO, Pi 400 ≤ 12 ms (non verificato) dove applicabile |

## Prossime azioni (un passo per ruolo)
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): D2 fix mirato `src/game/game.cpp` per `sportellata` exit 0; chiarire/allineare D4; cmd/exit/misure + ctest/gate/replay. Poi V7 o ∞ V9 (no legacy/costanti fuori MIG1).
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ4 soak lv_05_spray (poi lv_06); post-fix D2 rieseguire J3/J4 e aggiornare `REVIEW_MIG1.md`; consolidare review V6/VJ5 in `REVIEW_V47.md`.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): VG2 giro 18 o ∞ VG3; no sponsor/asset Andrea.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only + `STATO_CORRENTE.md`; aprire/chiudere CP solo con gate+review indipendenti registrati. Nessuna chiusura CP ora.

Vincoli: base legacy 08/11 e vocabolario intoccati (solo aggiunte); no sponsor; no `RICHIESTA.md`/`SCAMBIO.md`; log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato da gate.
