# STATI PROPOSTI — @capo (Turno 1, ora 1 di 12, 2026-11-13)

**Fonti:** `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; base legacy 08/11 approvata **non si tocca**; G1 annullato/sponsor solo Andrea; nessuna decisione aperta), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (4.6), `RICHIESTA_APERTA.md` (MIG1+V47, passi sempre disponibili ∞), `SCAMBIO_RECENTE.md`, `STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Officina/Grafico/junior.

**Tappa attiva/Gate:** MIG1 [~] review + V47 [~] continuo (solo aggiunte; vocabolario/costanti/legacy intoccati). Gate non rieseguito dal capo; verdi riportati: Officina 13/11 V9 lv_04_cromo `GATE PERCORSO OK` + `GATE OK` exit 0, ctest 27/27 exit 0, ms/frame medio 1.728 max 4.176, replay 2x IDENTICO (storico `gate_t1.sh` exit 0). Pi 400 non verificato.

**Checkpoint:** nessuna chiusura. CP-005/006/007 **CHIUSI** (CP-006 **non riaprire**). CP-002/003/004, DECO1, F60 APERTI (review mancanti). MIG1/V47: handoff/prove riportate ma review incrociate autore≠revisore non registrate in dossier.

## Stati per blocco
| Blocco | Stato | Evidenza | Nota |
|---|---|---|---|
| MIG1 G1 | ANNULLATO | Sponsor solo Andrea | Vietato toccare sponsor/asset Andrea |
| MIG1 D1-D5,G2,J1-J4 | [x] handoff | Nota Claude 11/11; costanti guida intoccate | Review non in dossier |
| V47 V1-V5,VJ1,VJ3,VG2 | [x] handoff/chiuso | Gate verdi riportati; VG2 chiuso 11/11 | Review non consolidate |
| V47 V6 libretro | [~] | ctest 27/27, gate_t1 exit 0; picco 55.27 ms lv_05 isolato | Serve review junior consolidata |
| V47 V7/V8 | [~]/[ ] | Allocazioni/caricamento | Misure prima/dopo, replay/resa identici |
| V47 V9 ∞ | [~] | Officina 13/11: lv_04_cromo 5° giro exit 0, medio 1.728 max 4.176 ms | **Prossimo: lv_05_spray** |
| V47 VJ4 soak | [~] | 2/6 (lv_01-02 exit 0, 30000f IDENTICO; lv_02 max 43.207 ms) | **Prossimo: lv_03_foglie** |
| V47 VJ5 ∞ review | [~] | Junior 12/11: review `road.cpp`, caricamento exit 0 | Altro file `src/`, prima ricompila |
| V47 VG3 ∞ | [~] | Grafico 13/11 giro 16: 0 da convertire, copertura estesa | Continuare ∞; residuo `ULTIMO_TURNO.md` noto |
| V47 VJ2 riva | [~] | `vj2_riva` exit 0 | DEFAULT: non blocca (ispezione Andrea) |
| CP-002/003/004,DECO1,F60 | APERTI | Review indipendenti mancanti | No chiusura senza dossier |

## Deleghe (ruoli AGENTS.md 4.6, consegne `UFFICIO/out/<ruolo>/`)
| Responsabile | Task / criterio | Revisore | Consegna |
|---|---|---|---|
| @developer | **V9 ∞** lv_05_spray (`percorso`/smoke, exit code, ms/frame medio/max); se >8 ms alleggerire senza cambiare resa/replay. V7/V8 se resta. **Non toccare** costanti guida/MIG1/legacy. | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| @developer-junior | **VJ4** soak lv_03_foglie (exit 0, 30000f IDENTICO, ms/frame max; poi lv_04-06). **V6** review consolidata autore≠revisore in `REVIEW_V47.md`. **VJ5 ∞** altro file `src/` (1 riga). Prima di ogni prova ricompila. | @developer | `UFFICIO/out/developer-junior/REVIEW_V47.md`, `RIASSUNTO.md` |
| @grafico | **VG3 ∞** confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. | @capo se richiesto | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| @capo | Registro append-only, sync STATO/STATI. Aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. | — | `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md` |

**Domande ad Andrea:** nessuna. DEFAULT: si prosegue (VJ2 non blocca). Vale solo eventuale `RISPOSTA DI ANDREA`.
**Blocchi:** `FINITO` no (passi ∞). `PER_CLAUDE.md` no. `RICHIESTA.md`/`SCAMBIO.md` sola lettura. Nessuna modifica fuori percorsi consentiti.
