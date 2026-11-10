# STATI PROPOSTI — Turno 4 (ora 6 di 12) — @capo

Data: 2026-11-10. Fonti lette: `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**; base legacy 08/11 NON si tocca; G1 **ANNULLATO**, sponsor solo Andrea; nessuna nuova RISPOSTA DI ANDREA — vale risposta Andrea, mai DEFAULT), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` 4.6, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo), `SCAMBIO_RECENTE.md`, `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`.

**Tappa attiva / Gate:** MIG1 [~] + V47 [~] (gate T1 green riportato, junior 10/11: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0). **Nessun CP chiudibile**: review incrociate MIG1/V47 non registrate in dossier. VJ2 densità `vj2_riva` (PPM exit 0): ispezione Andrea, **DEFAULT non blocca**, proseguire. **FINITO**: no. **PER_CLAUDE**: no. Domande ad Andrea: nessuna.

## Checkpoint
| ID | Stato proposto | Motivo |
|---|---|---|
| CP-002, CP-003, CP-004 | APERTO | review indipendente da assegnare/completare |
| CP-005, CP-006, CP-007 | CHIUSO | **CP-006 non riaprire** |
| DECO1, F60 | APERTO | review incrociata mancante |
| MIG1 | APERTO | D2/J3/J4/D4 aperti; review incrociate non registrate |
| V47 | APERTO | V5 frasi [~]; VJ2 ispezione Andrea (non bloccante); review incrociate non registrate |

## Stati proposti e deleghe 4.6 (consegne `UFFICIO/out/<ruolo>/`)
| Task | Stato | Responsabile | Revisore indipendente | Criterio / consegna |
|---|---|---|---|---|
| D1,D3,D5,G2,J1,J2 | [x] handoff | developer/grafico/junior | incrociato | review incrociate registrate in dossier (cmd, exit, misure, path relativi) |
| D2 sportellata | [~] | @developer | @developer-junior | fix mirato `motore_nuovo/src/game/game.cpp` (solo costanti MIG1 aperte: `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`; trigger coerente sterzo 60 Hz). Prove: `sportellata` lv_01 exit 0, ctest verde, gate_t1 exit 0, replay 2x IDENTICO → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| D4 nomi rivali | [~] | @developer | @developer-junior | 7 nomi (max 6 MAIUSC.) con origine/tema allineati a `NOMI_RIVALI` in `game.cpp` → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| J3,J4 | [~] | @developer-junior | @developer | post-fix D2: `sportellata` exit 0 + recupero ~5 s misurabile; aggiorna `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| V1-V4,VG1,VJ1 | [x] handoff | ruoli assegnati | incrociato | review incrociate registrate |
| V5 frasi | [~] | @grafico | @developer-junior | `motore_nuovo/dati/frasi.json` EN semplice; struttura/CC0-1.0 preservate; no Sega/marchi veri; no sponsor/asset Andrea/HUD/UI/auto/logo → `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| VJ2 riva | [~] | @developer-junior | Andrea (ispezione) | `vj2_riva` exit 0 registrato; densità spiagge/gabbiani/barche **DEFAULT non blocca** |
| Review incrociate MIG1/V47 | [ ] | @developer-junior, @developer | autore≠revisore | evidenze in `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md` prima di aprire/chiudere CP |

## Prossime azioni
1. **@developer:** D2 fix mirato + chiarire D4 → `UFFICIO/out/developer/ULTIMO_TURNO.md`.
2. **@developer-junior:** J3/J4 post-fix D2 + review incrociate MIG1/V47 → `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md`.
3. **@grafico:** V5 `motore_nuovo/dati/frasi.json` EN semplice → `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.
4. **@capo:** registro append-only; dossier/STATO/registro solo con gate+review indipendenti registrati. Nessuna chiusura CP ora.
