# STATI PROPOSTI — @capo (Turno 2, ora 2 di 12, 2026-11-13)

**Fonti:** `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; base legacy 08/11 **non si tocca**; G1 annullato/sponsor solo Andrea; decisioni aperte nessuna), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (4.6), `RICHIESTA_APERTA.md` (MIG1+V47, passi ∞), `SCAMBIO_RECENTE.md`, `STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Officina/Grafico/junior.

**Tappa attiva/Gate:** MIG1 [~] review + V47 [~] continuo (solo aggiunte; vocabolario/costanti/legacy intoccati). Gate non rieseguito dal capo; verdi riportati: Officina 13/11 V9 lv_04 `GATE PERCORSO OK`+`GATE OK` exit 0, ctest 27/27, ms/frame medio 1.728 max 4.176, replay 2x IDENTICO; junior VJ4 soak 6/6 exit 0, 30000f IDENTICO, medi < 20 ms (max 124.129 lv_06); grafico VG3 giro 18 0 da convertire. Pi 400 non verificato.

**Checkpoint:** nessuna chiusura. CP-005/006/007 **CHIUSI** (CP-006 **non riaprire**). CP-002/003/004, DECO1, F60 APERTI. MIG1/V47: prove/handoff riportati ma review indipendenti (autore≠revisore) non registrate in dossier.

## Stati per blocco
| Blocco | Stato | Evidenza | Nota |
|---|---|---|---|
| MIG1 G1 | ANNULLATO | Sponsor solo Andrea | Vietato toccare sponsor/asset Andrea |
| MIG1 D1-D5,G2,J1-J4 | [x] handoff | Nota Claude 11/11; costanti guida intoccate | Review non in dossier |
| V47 V1-V5,VJ1,VJ3,VG2 | [x] handoff/chiuso | Gate verdi riportati; VG2 chiuso 11/11 | Review non consolidate |
| V47 V6 libretro | [~] | ctest 27/27, gate_t1 exit 0; picco 55.27 ms lv_05 isolato | Serve review junior consolidata |
| V47 V7/V8 | [~]/[ ] | Allocazioni/caricamento | Misure prima/dopo, replay/resa identici |
| V47 V9 ∞ | [~] | Officina 13/11: lv_04_cromo 5° giro exit 0, medio 1.728 max 4.176 ms | **Prossimo: lv_05_spray** |
| V47 VJ4 soak | [x]/completato 6/6 | Junior 13/11: lv_01-06 exit 0, 30000f IDENTICO, medi < 20 ms (max 124.129 lv_06) | Dati in `REVIEW_V47.md`; valutare picchi |
| V47 VJ5 ∞ review | [~] | Junior 13/11 09:45/10:05: review `fasi.cpp` (da6c52b), rilievo puntini/blink | Altro file `src/` al prossimo cambio, prima ricompila |
| V47 VG3 ∞ | [~] | Grafico 13/11 09:16 giro 18: 0 da convertire, copertura estesa | Continuare ∞ |
| V47 VJ2 riva | [~] | `vj2_riva` exit 0 | DEFAULT: non blocca (ispezione Andrea) |
| CP-002/003/004,DECO1,F60 | APERTI | Review indipendenti mancanti | No chiusura senza dossier |

## Deleghe (ruoli AGENTS.md 4.6, consegne `UFFICIO/out/<ruolo>/`)
| Responsabile | Task / criterio | Revisore | Consegna |
|---|---|---|---|
| @developer | **V9 ∞** lv_05_spray (`percorso`/smoke, exit code, ms/frame medio/max); se >8 ms alleggerire senza cambiare resa/replay. V7/V8 se resta. Valutare rilievo VJ5 su `fasi.cpp` solo se compatibile e senza toccare legacy/vocabolario. **Non toccare** costanti guida/MIG1/legacy. | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| @developer-junior | Consolidare **review V6** e prove MIG1/V47 autore≠revisore in `REVIEW_V47.md` (gate+exit code/misure). **VJ5 ∞** altro file `src/` al prossimo cambio (1 riga, prima ricompila). | @developer | `UFFICIO/out/developer-junior/REVIEW_V47.md`, `RIASSUNTO.md` |
| @grafico | **VG3 ∞** confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. | @capo se richiesto | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| @capo | Registro append-only, sync STATO/STATI. Aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. | — | `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md` |

**Domande ad Andrea:** nessuna. DEFAULT: si prosegue (VJ2 non blocca). Vale solo eventuale `RISPOSTA DI ANDREA`.
**Blocchi:** `FINITO` no (passi ∞). `PER_CLAUDE.md` no. `RICHIESTA.md`/`SCAMBIO.md` sola lettura. Nessuna modifica fuori percorsi consentiti.