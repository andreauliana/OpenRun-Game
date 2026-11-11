# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-11. Fonti lette (solo): `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova, vale risposta Andrea mai DEFAULT; base legacy 08/11 **NON si tocca**; G1 **ANNULLATO**, sponsor solo Andrea), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` 4.6, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo, passi ∞), `SCAMBIO_RECENTE.md` (+ `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Grafico/Dev/Junior/Officina).

**Tappa attiva / Gate:** MIG1 [~] + V47 [~] (solo aggiunte). Gate T1 green riportato (SCAMBIO_RECENTE 10/11 09:46; Officina 11/11: `gate_t1.sh` exit 0 `GATE T1: TUTTO OK`, ctest 27/27 exit 0 — nota picco ms/frame max 55.27 isolato lv_05 smoke, media ~0.97-1.43, da valutare in review). **Nessun CP chiudibile:** review incrociate MIG1/V47/V6 non registrate in dossier (autore≠revisore). VJ2 `vj2_riva` exit 0: densità da ispezione Andrea, **DEFAULT non blocca**. **FINITO:** no (blocchi non tutti [x]). **PER_CLAUDE:** no. Domande ad Andrea: nessuna.

## Checkpoint
| ID | Stato proposto | Motivo |
|---|---|---|
| CP-002, CP-003, CP-004 | APERTO | review indipendente da assegnare/completare |
| CP-005, CP-006, CP-007 | CHIUSO | **CP-006 non riaprire** |
| DECO1, F60 | APERTO | review incrociata mancante |
| MIG1 | APERTO | D2 `sportellata` exit 28, J3/J4 post-fix, D4 da chiarire; review non in dossier |
| V47 | APERTO | V6 handoff senza review registrata; VG2 [~]; VJ2 ispezione Andrea (DEFAULT non blocca); passi ∞ in corso |

## Stati proposti e deleghe 4.6
| Task | Stato | Responsabile | Revisore indipendente | Criterio misurabile / consegna |
|---|---|---|---|---|
| G1 sponsor | ANNULLATO | Andrea | — | Cartelloni solo Andrea (nessun tocco agenti) |
| D1,D3,D5,J1,J2,VG1,G2,V1-V5,VJ1 | [x] handoff | @developer/@developer-junior/@grafico | incrociato (autore≠revisore) | Evidenze in out/RIASSUNTI/REVIEW; registrare review in dossier (cmd/exit/misure) prima di chiusure CP |
| V6 libretro | [~] handoff | @developer | @developer-junior | Verificare officina 11/11: ctest 27/27 exit 0, gate_t1 exit 0, 6/6 GATE PERCORSO OK, replay IDENTICO; valutare picco ms/frame max 55.27 lv_05 smoke (log `UFFICIO/Officina/Test area/gate_t1.log`); review in `UFFICIO/out/developer-junior/REVIEW_V47.md` |
| D2 sportellata | [~] | @developer | @developer-junior | Fix mirato `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, trigger/clamp sterzo 60 Hz). Prove: `sportellata` exit 0, ctest verde, `gate_t1.sh` exit 0, replay 2x IDENTICO → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| D4 nomi rivali | [~] | @developer | @developer-junior | 7 nomi (max 6 MAIUSC.) con origine/tema allineati a `NOMI_RIVALI`; chiarire discrepanza → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| J3,J4 MIG1 | [~] | @developer-junior | @developer | Post-fix D2: `sportellata` exit 0 + recupero ~5 s misurabile; aggiornare `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| VG2 frasi EN | [~] | @grafico | @developer-junior | Giro 3 (20 frasi) punteggiatura/spezzature, max 16 char MAIUSC., `it1/it2`/id/personaggio intatti; verifica `UFFICIO/Dipartimento Grafico/TOOLS/verifica_frasi_vg2.py` exit 0 → `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| VJ2 riva | [~] | @developer-junior | Andrea (ispezione) | `vj2_riva` exit 0; densità lv_01/04/06 da ispezione Andrea, **DEFAULT non blocca** |
| V7/V8/VJ3/VJ4, passi ∞ | [ ]/[∞] | ruoli 4.6 | incrociato | Un passo per turno (RICHIESTA_APERTA); ctest/gate exit 0, replay IDENTICO, Pi 400 ≤ 12 ms (non verificato) dove applicabile; prove in `UFFICIO/Officina/Test area/` |

## Prossime azioni (un passo per ruolo)
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): D2 fix mirato in `src/game/game.cpp` (sole costanti/trigger aperti) per `sportellata` exit 0; chiarire/allineare D4; riportare cmd/exit/misure + ctest/gate/replay. Poi V7 o passo ∞ V9 (senza toccare legacy/costanti fuori MIG1).
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): post-fix rieseguire J3 (`sportellata` exit 0 + recupero ~5 s) e aggiornare J4 `REVIEW_MIG1.md`; review indipendente V6 in `REVIEW_V47.md` (cmd/exit/misure); proseguire VJ3/VJ4/∞ VJ5.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): VG2 giro 3 (20 frasi) o passo ∞ VG3; no sponsor/asset Andrea.
4. **@capo** (`UFFICIO/Checkpoint Revision/`, `UFFICIO/out/capo/STATI_PROPOSTI.md`): registro append-only + `STATO_CORRENTE.md`; aprire/chiudere CP solo con gate+review indipendenti registrati. Nessuna chiusura CP ora.

Vincoli: base legacy 08/11 e vocabolario non si toccano (solo aggiunte); no sponsor; no `RICHIESTA.md`/`SCAMBIO.md`; log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato da gate.