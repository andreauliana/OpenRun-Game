# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Data: 2026-11-10. Fonti lette (solo): `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova — vale risposta Andrea, mai DEFAULT; base legacy 08/11 NON si tocca; G1 **ANNULLATO**, sponsor solo Andrea), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` 4.6, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo), `SCAMBIO_RECENTE.md` (+ `STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md` per avvio capo).

**Tappa attiva / Gate:** MIG1 [~] + V47 [~] (gate T1 green riportato, SCAMBIO_RECENTE 10/11: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, screenshot P1 ok; replay/determinismo come da handoff). **Nessun CP chiudibile**: review incrociate MIG1/V47 non registrate in dossier CP. VJ2 `vj2_riva` exit 0: densità da ispezione Andrea, **DEFAULT: non blocca**. **FINITO**: no (blocchi non tutti [x]). **PER_CLAUDE**: no. Domande ad Andrea: nessuna.

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
| D1,D3,D5,G2,J1,J2,VG1,V1-V4,VJ1 | [x] handoff | developer/grafico/junior | incrociato (autore≠revisore) | review incrociate con cmd/exit/misure registrate in dossier prima di chiudere CP |
| D2 sportellata | [~] | @developer | @developer-junior | fix mirato `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`; trigger coerente sterzo 60 Hz). Prove: `sportellata` exit 0, ctest verde, `gate_t1.sh` exit 0, replay 2x IDENTICO → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| D4 nomi rivali | [~] | @developer | @developer-junior | 7 nomi (max 6 MAIUSC.) con origine/tema allineati a `NOMI_RIVALI` → `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| J3,J4 | [~] | @developer-junior | @developer | post-fix D2: `sportellata` exit 0 + recupero ~5 s misurabile; aggiorna `UFFICIO/out/developer-junior/REVIEW_MIG1.md` |
| V5 frasi | [~] | @grafico | @developer-junior | `motore_nuovo/dati/frasi.json` EN semplice; struttura/CC0-1.0 preservate; no Sega/marchi veri; no sponsor/asset Andrea/HUD/UI/auto/logo → `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| VJ2 riva | [~] | @developer-junior | Andrea (ispezione) | `vj2_riva` exit 0 registrato; densità **DEFAULT non blocca** |
| Review incrociate MIG1/V47 | [ ] | @developer-junior, @developer | autore≠revisore | evidenze in `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md` |

## Prossime azioni (un passo per ruolo, senza aspettare)
1. **@developer:** D2 fix mirato + chiarire D4 → `UFFICIO/out/developer/ULTIMO_TURNO.md` (poi V4/V5 se il proprio MIG1 è completo e nel perimetro).
2. **@developer-junior:** J3/J4 post-fix D2 + review incrociate MIG1/V47 → `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md`.
3. **@grafico:** V5 `motore_nuovo/dati/frasi.json` EN semplice → `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.
4. **@capo:** registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrati. Nessuna chiusura CP ora.
