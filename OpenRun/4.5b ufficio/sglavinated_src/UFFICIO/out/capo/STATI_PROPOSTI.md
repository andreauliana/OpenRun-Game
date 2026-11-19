# STATI PROPOSTI — Turno 3 (ora 5 di 12), coordinamento 19/11/2026, @capo

**Tappa attiva:** v4.7 — **MIG1 [~] (review)** + **V47 [~] continuo** (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 e vocabolario **NON si toccano** (`ANDREA_REVIEW.md` precedenza assoluta). G1 ANNULLATO (sponsor/cartelloni solo Andrea). Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo. `SCAMBIO_RECENTE.md` CHECKPOINT 1-3 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | **APERTI [~]** | dossier in `UFFICIO/Checkpoint Revision/` | Review/prove indipendenti mancanti; nessuna chiusura. |
| MIG1 (review) | **[~]** | handoff dev/junior, dossier CP | Costanti guida/vocabolario/legacy intoccabili; review incrociate non consolidate in dossier. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6/review da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 19/11 12:52 | lv_04_cromo 23°: build/percorso/smoke pre+post exit 0; `percorso` OK (2125f, 35.4s, 0% fuori); smoke post medio 1.101 max 3.257 ms, replay IDENTICO, frameHash invariati. `gate_t1.sh` exit 0 "TUTTO OK" 6/6 (19/11 12:52). **Prossimo:** lv_05_spray 23°. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | lv_01-lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% segnalato. **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[~] 1/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 19/11 12:58 | lv_01_neon soak exit 0: 30000f, replay 2x IDENTICO, medio 1.552 max 7.964 ms, nessun crash. **Prossimo:** lv_02_atomico 2/6. (Precedenti soak 6/6 storici in REVIEW_V47; turno corrente riavvio ciclo come da handoff.) |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` 19/11 12:58 | VJ1/review diff src V9 (`palette.h`/`renderer.cpp`) nessun errore; `run_tests.sh` exit 0 ctest 27/27 (248.79 s). Prossimo: VJ5 su prossimo diff `src/`. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 18/11 16:14 | Giro 94: 3 script exit 0, 0 da convertire, 126 pari, pose 2/2. 1 noto bloccato Andrea, zero esterni/Sega. |

## Checkpoint
- Nessun CP con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura.
- SCAMBIO_RECENTE CHECKPOINT 1-3 [APERTE]: esiti automatici verdi, **non equivalgono a peer review in dossier**.
- `UFFICIO/Officina/STATUS_OFFICINA.md`: handoff tecnico sintetico, **non prova di approvazione/review** (non aggiornato con ultimi handoff).

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_05_spray 23°** — `percorso`/`smoke`, exit code, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta. | Exit 0, replay IDENTICO, misure ms/frame, log in `UFFICIO/Officina/Test area/`. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ4 ∞: **soak lv_02_atomico 2/6** (ricompila prima: `run_tests.sh`), replay/medio/max/nessun crash. VJ5 ∞: review prossimo diff `src/` del @developer (file/riga). VJ3 resta 4/6 (**lv_05/lv_06** 50/60 Hz). Consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`. | Righe in `REVIEW_*`; exit/replay/ms per soak/VJ3. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. Nessun file in radice. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non creato. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.