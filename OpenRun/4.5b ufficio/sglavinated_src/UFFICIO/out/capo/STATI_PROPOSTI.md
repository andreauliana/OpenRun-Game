# STATI PROPOSTI — Turno 1 (ora 1 di 12), coordinamento 20/11/2026, @capo

**Tappa attiva:** v4.7 — **MIG1 [~] (review)** + **V47 [~] continuo** (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 e vocabolario **NON si toccano** (`ANDREA_REVIEW.md` precedenza assoluta). G1 ANNULLATO (sponsor/cartelloni solo Andrea). Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo. SCAMBIO_RECENTE CHECKPOINT 6-8 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | **APERTI [~]** | dossier in `UFFICIO/Checkpoint Revision/` | Review/prove indipendenti mancanti; nessuna chiusura. |
| MIG1 (review) | **[~]** | handoff dev/junior, dossier CP | Costanti guida/vocabolario/legacy intoccabili; review incrociate non consolidate in dossier. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 20/11 11:04: lv_04_cromo 24° giro, `percorso` exit 0 (2125f, 35.4s), `smoke` exit 0 medio 0.836 max 3.227 ms, replay IDENTICO, frameHash identici; `gate_t1.sh` exit 0 6/6 11:04. | **Prossimo:** lv_05_spray (24° giro), V9 ∞. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | lv_01-lv_04 50/60 Hz. **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[~] 5/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 20/11 11:10: soak lv_05_spray exit 0, 30000f, replay 2x IDENTICO, medio 1.486 max 14.066 ms (frame 25351), no crash; `run_tests.sh` exit 0 ctest 27/27. | **Prossimo:** soak lv_06_desktop 6/6 (ricompila prima). Max 14.066 ms > soglia V9 8 ms segnalato. |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 20/11 11:10 | Nessun errore nei nuovi hunk src (drawSegmentTrapezoid centroNear/centroFar; semplificazione `fineRiga=rigaCercaBase+pCol->tx1`). **Prossimo:** VJ5 su prossimo diff `src/` (file/riga in `REVIEW_V47.md`). |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 20/11 10:18 (giro 116) | `lato_bus_rosso` convertito (exit 0), 43 allineati, 0 da convertire. Blocco: `traffico_incrocio/lato_berlina_verde.sglraw` senza PNG (decisione Andrea, **DEFAULT: lasciare invariato e proseguire**). Pose 5 PNG `_b` mancanti (area Andrea). Zero esterni/Sega. |

## Checkpoint
- Nessun CP con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura.
- SCAMBIO_RECENTE CHECKPOINT 6-8 [APERTE]: esiti automatici verdi, **non equivalgono a peer review in dossier**.
- `UFFICIO/Officina/STATUS_OFFICINA.md`: handoff tecnico sintetico, **non prova di approvazione/review**.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_05_spray (24° giro)** — `percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta. | Exit 0, replay IDENTICO, misure ms/frame, log in `UFFICIO/Officina/Test area/`. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ4 ∞: **soak lv_06_desktop 6/6** (ricompila prima `run_tests.sh`), replay/medio/max/nessun crash; segnalare picchi > soglia V9 8 ms. VJ5 ∞: review prossimo diff `src/` (file/riga). VJ3 resta 4/6 (**lv_05/lv_06** 50/60 Hz). Consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`. | Righe in `REVIEW_V47.md`; exit/replay/ms per soak/VJ3. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**. Blocco noto: `lato_berlina_verde.sglraw` senza PNG (attendere Andrea, **DEFAULT: invariato**). No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non creato. Domande ad Andrea: nessuna nuova (DEFAULT prosegue; VJ2/grafico blocco non bloccano). Pi 400 non verificato.
