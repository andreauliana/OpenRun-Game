# STATI PROPOSTI — @capo (Turno 1, ora 1 di 12, 2026-11-12)

**Fonti lette:** `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**: nessuna nuova `RISPOSTA DI ANDREA`; base legacy 08/11 approvata e **non si tocca**; G1 ANNULLATO/sponsor solo Andrea; decisioni aperte nessuna), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (4.6), `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo con passi ∞), `SCAMBIO_RECENTE.md` + `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Grafico/Officina/Junior.

**Tappa attiva / Gate:** MIG1 [~] (review) + V47 [~] (solo aggiunte; no refactor legacy/vocabolario/costanti guida). Gate green riportato (Officina 11/11: ctest 27/27 exit 0, `gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 PERCORSO OK, replay IDENTICO; picco 55.27 ms lv_05 smoke isolato da valutare, log `UFFICIO/Officina/Test area/gate_t1.log`). Pi 400 non verificato.

**Checkpoint:** nessuna chiusura possibile. CP-005/006/007 **CHIUSI** (CP-006 **non riaprire**). CP-002/003/004, DECO1, F60 **APERTI** (review mancanti). MIG1/V47: handoff/prove riportate ma review incrociate autore≠revisore non registrate in dossier → nessun CP chiudibile.

## Stati proposti per blocco

| Blocco | Stato | Evidenza sintetica | Prossimo / blocco |
|---|---|---|---|
| MIG1 G1 cartelloni | **ANNULLATO** | Sponsor solo Andrea. | Vietato ritoccare sponsor/asset Andrea. |
| MIG1 D1-D5, G2, J1-J4 | **[x] handoff** | STATO_CORRENTE/nota 11/11: D2/D4/J3/J4 chiusi; costanti guida NON si toccano. | Review incrociate non in dossier. |
| V47 V1-V5, VJ1, VJ3 | **[x] handoff** | Handoff Officina/junior, gate/ctest verdi riportati. | Review non consolidate in dossier. |
| V47 V6 core libretro | **[~]** | Handoff dev: ctest 27/27, gate_t1 exit 0, 6/6 PERCORSO OK; picco 55.27 ms lv_05 isolato. | Review junior in `REVIEW_V47.md` con misure/exit code (autore≠revisore). |
| V47 V7/V8 | **[~]/da fare** | Allocazioni/caricamento. | Misure prima/dopo, replay/resa identici. |
| V47 V9 ∞ fluidità | **[~]** | Dev 11/11 12:54: lv_01 `percorso` exit 0, `smoke` exit 0 medio 0.951 max 3.420 ms < 8. | **Prossimo: lv_02_atomico**. |
| V47 VJ4 soak | **[~]** | Junior: 4/6 (lv_01-04 exit 0, 30000f IDENTICO). | **Prossimo: lv_05_spray, poi lv_06_desktop**. |
| V47 VJ5 ∞ review | **[~]** | Junior 11/11 12:55: review `salvataggi.cpp/.h`, caricamento exit 0. | Altro file `src/`, prima ricompila. |
| V47 VG2 frasi | **[x] chiuso** | VG2 chiuso (11/11), verifica exit 0 156/156. | Non riscrivere `frasi.json`. |
| V47 VG3 ∞ PNG→raw | **disponibile** | Passo ∞ grafico. | Confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi. |
| V47 VJ2 riva | **[~]** | `vj2_riva` exit 0. | **DEFAULT: non blocca** (ispezione Andrea). |
| CP-002/003/004, DECO1, F60 | **APERTI** | Review indipendenti mancanti. | Non chiudere senza prove+review in dossier. |

## Deleghe (AGENTS.md 4.6, consegne `UFFICIO/out/<ruolo>/`)

| Ruolo | Responsabile | Task / criteri misurabili | Revisore indipendente | Consegna |
|---|---|---|---|---|
| @developer | developer | **V9 ∞**: lv_02_atomico (ciclico), gate `percorso`/smoke, riportare ms/frame medio/max ed exit code; se > 8 ms alleggerire senza cambiare resa/replay. **V7/V8** se resta. MIG1 chiuso: **non toccare** `AFFIANCATA_DX`/`URTO_SPINTA_RIVALE`/`RIVALE_SORPASSO_DX` né costanti legacy. | @developer-junior | `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| @developer-junior | developer-junior | **VJ4**: soak lv_05_spray poi lv_06_desktop (exit 0, 30000f IDENTICO, ms/frame max). **Review V6** consolidata in `REVIEW_V47.md` (autore≠revisore, exit code/misure). **VJ5 ∞**: altro file `src/`, una riga. Prima di ogni prova ricompila (mai `prova_gate` vecchio). | @developer | `UFFICIO/out/developer-junior/REVIEW_V47.md` |
| @grafico | grafico | **∞ VG3**: confrontare date PNG `Libreria/assets_originali/scenari/` vs `.sglraw`; se PNG più nuovo convertire con script del catalogo (es. `converti_oggetti_raw.py`) senza ritocchi e riportare file; se allineato, una riga. VG2 chiuso (no `frasi.json`), no sponsor/asset Andrea, no codice/livelli/tracciati. | @capo assegna se richiesta | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| @capo | capo | Registro append-only, sync `STATO_CORRENTE.md`/`STATI_PROPOSTI.md`. Aprire/chiudere CP solo con gate+review indipendenti in dossier. Nessuna approvazione tecnica/codice/asset. | — | `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md` |

## Domande ad Andrea
- Nessuna nuova. **DEFAULT:** si prosegue (VJ2 ispezione densità riva non blocca). Vale solo eventuale `RISPOSTA DI ANDREA` esplicita.

## Blocco
- **FINITO:** no (passi ∞ V47 in corso; review/CP non chiudibili).
- **PER_CLAUDE.md:** no.