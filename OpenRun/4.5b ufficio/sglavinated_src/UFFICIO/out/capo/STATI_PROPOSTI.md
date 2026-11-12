# STATI PROPOSTI — @capo (Turno 1, ora 1 di 12, 12/11/2026)

**Fonti lette (solo):** `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**: nessuna RISPOSTA DI ANDREA nuova; base legacy 08/11 approvata e intoccata; G1 ANNULLATO/sponsor solo Andrea), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (regole 4.6), `RICHIESTA_APERTA.md` (MIG1+V47 continuo, passi ∞), `SCAMBIO_RECENTE.md` + `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Grafico/Officina/Junior.

**Tappa attiva / Gate:** MIG1 [~] (review) + V47 [~] (solo aggiunte; no refactor legacy/vocabolario). Gate green riportato (Officina 11/11: ctest 27/27 exit 0, `gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 PERCORSO OK, replay IDENTICO; picco 55.27 ms lv_05 smoke isolato da valutare, log `UFFICIO/Officina/Test area/gate_t1.log`). Nessuna prova Pi 400.

**Checkpoint:** nessun CP con gate+review indipendenti (autore≠revisore) registrati in dossier → **nessuna chiusura**. CP-005/006/007 **CHIUSI** (CP-006 **non riaprire**). CP-002/003/004, DECO1, F60 **APERTI** (review mancanti). MIG1/V47: passi con prove/handoff ma review incrociate non consolidate → nessun nuovo CP chiudibile.

## Stati proposti per blocco

| Blocco | Stato | Evidenza sintetica | Blocco residuo / prossimo |
|---|---|---|---|
| MIG1 G1 cartelloni | **ANNULLATO** | Sponsor solo Andrea (ANDREA_REVIEW). | Vietato ritoccare sponsor/asset Andrea. |
| MIG1 D1-D5, G2, J1-J4 | **[x] handoff** | STATO_CORRENTE/nota Claude 11/11: D2/D4/J3/J4 chiusi (`sportellata` exit 0 sul main, exit 28 vecchio). **Costanti di guida NON si toccano.** | Review incrociate non registrate in dossier → nessun CP MIG1 chiudibile. |
| V47 V1-V5, VJ1, VJ3 | **[x] handoff** | Handoff Officina/junior (ctest/gate verdi riportati). | Review incrociate non consolidate in dossier. |
| V47 V6 core libretro | **[~]** | Handoff dev: ctest 27/27, gate_t1 exit 0, 6/6 PERCORSO OK; picco 55.27 ms lv_05 smoke isolato. | Consolidare review junior in `REVIEW_V47.md` con misure/exit code (autore≠revisore). |
| V47 V7 caricamento / V8 | **[~]/da fare** | Da proseguire per RICHIESTA_APERTA. | Misure prima/dopo, replay/resa identici. |
| V47 V9 ∞ fluidità | **[~]** | @developer 11/11 12:54: lv_01 `percorso` exit 0, `smoke` exit 0 ms/frame medio 0.951 max 3.420 < 8. | **Prossimo: lv_02_atomico** (un livello/turno). |
| V47 VJ4 soak 50/60 | **[~]** | Junior: 4/6 completati (lv_01-04 exit 0, 30000f IDENTICO, no crash). | **Prossimo: lv_05_spray, poi lv_06_desktop**. |
| V47 VJ5 ∞ review codice | **[~]** | Junior 11/11 12:55: review `src/game/salvataggi.cpp/.h`, caricamento exit 0. | Continuare su altro file `src/` (una riga, prima ricompila). |
| V47 VG2 frasi | **[x] chiuso** | RICHIESTA_APERTA: VG2 chiuso (Claude 11/11), `verifica_frasi_vg2.py` exit 0 156/156. | Non riscrivere `frasi.json`. |
| V47 VG3 ∞ PNG→raw | **disponibile** | RICHIESTA_APERTA: "Chi tocca adesso" @grafico VG3 ∞. | Confronto date PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; no asset Andrea/sponsor. |
| V47 VJ2 riva | **[~]** | `vj2_riva` exit 0; densità soggetta a ispezione Andrea. | **DEFAULT: non blocca** (si prosegue). |
| Legacy CP-002/003/004, DECO1, F60 | **APERTI** | Senza review indipendenti registrate. | Non chiudere senza prove+review in dossier. |

## Deleghe (ruoli AGENTS.md 4.6, consegne `UFFICIO/out/<ruolo>/`)

| Ruolo | Responsabile | Task / criteri misurabili | Revisore indipendente | Percorsi scrivibili (task) | Consegna |
|---|---|---|---|---|---|
| @developer | developer | **V9 ∞** fluidità: lv_02_atomico (poi ciclico), gate `percorso`/smoke, riportare ms/frame medio/max e exit code; se > 8 ms sul Mac alleggerire senza cambiare resa/replay. **V7/V8** solo se resta qualcosa (misure prima/dopo, replay identico). MIG1 chiuso: **non toccare** `AFFIANCATA_DX`/`URTO_SPINTA_RIVALE`/`RIVALE_SORPASSO_DX` né altre costanti legacy. | @developer-junior | `motore_nuovo/src/**`, `UFFICIO/Officina/TOOL/**` se necessario, bacheca/out dev | `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| @developer-junior | developer-junior | **VJ4** soak lv_05_spray poi lv_06_desktop (exit 0, 30000f IDENTICO, ms/frame max). Consolidare **review V6** in `REVIEW_V47.md` (autore≠revisore, exit code/misure; rilievi parziali noti da chiudere/verificare). **VJ5 ∞** su altro file `src/` (una riga). Prima di ogni prova: ricompila (`run_tests.sh`/target), mai `prova_gate` vecchio. | @developer | `motore_nuovo/test/*.cpp`, `dati/tracciati/regole.json` (se serve), bacheca/out junior | `UFFICIO/out/developer-junior/REVIEW_V47.md`, `REVIEW_MIG1.md` (solo se integrazione necessaria), bacheca |
| @grafico | grafico | **∞ VG3**: confrontare date PNG in `Libreria/assets_originali/scenari/` vs `.sglraw`; se un PNG è più nuovo convertire con script del catalogo (es. `converti_oggetti_raw.py` per `comuni/oggetti`) senza ritoccare il disegno e riportare quali file; se tutto allineato, scriverlo in una riga. VG2 chiuso (non toccare `frasi.json`). Nessun PNG Andrea/sponsor/logo/sfondi/oggetti ridisegnati; nessun codice/livelli/tracciati. | Review incaricata da @capo se richiesta; handoff unico Grafico | `UFFICIO/Dipartimento Grafico/**`, `motore_nuovo/dati/sprite_raw/**` solo con script catalogo, anteprime `UFFICIO/Officina/Test area/grafico/`, bacheca/out grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| @capo | capo | Coordinamento/checkpoint: append registro, sync `STATO_CORRENTE.md`/`STATI_PROPOSTI.md`. Creare/chiudere CP solo a gate+review indipendenti registrate in dossier. Nessuna approvazione tecnica, nessun codice/asset. | — | `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md` | presente documento + checkpoint |

## Domande ad Andrea
- Nessuna nuova. **DEFAULT:** si prosegue senza blocchi (VJ2 ispezione densità riva: non blocca). Vale solo eventuale **RISPOSTA DI ANDREA** esplicita (nessuna registrata ora).

## Blocco / Fine
- **FINITO:** no (V47 passi ∞ in corso; review/CP non chiudibili).
- **PER_CLAUDE.md:** non creato (nessun blocco operativo del capo che richieda escalation).
