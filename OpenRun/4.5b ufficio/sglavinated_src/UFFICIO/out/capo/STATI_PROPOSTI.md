# STATI PROPOSTI — Turno 3 (ora 4 di 12), coordinamento 20/11/2026, @capo

**Tappa attiva:** v4.7 — **MIG1 [~] (review)** + **V47 [~] continuo** (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 e vocabolario **NON si toccano** (`ANDREA_REVIEW.md` precedenza assoluta). G1 ANNULLATO (sponsor/cartelloni solo Andrea). Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo. SCAMBIO_RECENTE: checkpoint automatici fino a **CHK2 20/11 13:54 [APERTA]**, controlli verdi (**non peer review in dossier**). Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | **APERTI [~]** | dossier in `UFFICIO/Checkpoint Revision/` | Review/prove indipendenti mancanti; nessuna chiusura. |
| MIG1 (review) | **[~]** | handoff dev/junior, dossier CP | Costanti guida/vocabolario/legacy intoccabili; review incrociate non consolidate in dossier. |
| V47 V1-V5, VJ1, VG2 | **[x]/[~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 20/11 13:00: lv_04_cromo 25° — `cmake` exit 0; `percorso` pre/post exit 0 (2125f, 35.4s, 0% fuori); `smoke` pre medio 0.852 max 3.590, post medio 0.830 max 3.422 ms, replay IDENTICO; `gate_t1.sh` 13:00 exit 0 6/6. | **Prossimo:** lv_05_spray (25°). No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | Prossimi: lv_05_spray, lv_06_desktop 50/60 Hz (ricompila prima). |
| V47 VJ4 ∞ | **[x] 6/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 20/11 11:31: soak lv_06 exit 0, 30000f, replay 2x IDENTICO, medio 1.381 max 95.641 ms, no crash; ctest 27/27. | Serie chiusa; picco >8/>20 ms segnalato per V9 lv_06. |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 20/11 13:06: review diff `renderer.cpp` `hwDelta`/`fattoreDelta` (lv_04 giro 25), nessun errore; `run_tests.sh` exit 0 27/27; `percorso` lv_04 exit 0. | **Prossimo:** VJ5 ∞ su prossimo diff `src/` (file/riga, ricompila prima se prova). |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 20/11 10:18 (giro 116): `lato_bus_rosso` convertito (exit 0), 43 allineati, 0 da convertire. | Blocco: `traffico_incrocio/lato_berlina_verde.sglraw` senza PNG (**DEFAULT: attendere Andrea, invariato**). Zero esterni/Sega. |

## Checkpoint
- Nessun CP con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura.
- SCAMBIO_RECENTE checkpoint automatici [APERTE]: esiti verdi, **non peer review in dossier**.
- `UFFICIO/Officina/STATUS_OFFICINA.md`: handoff tecnico sintetico, **non prova di approvazione/review**.

## Deleghe (ruoli 4.6, AGENTS.md)

| Ruolo | Handoff | Responsabile | Revisore indipendente | Consegna/criteri | Prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_05_spray (25° giro)** — `percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. Considerare picco soak lv_06 (95.641 ms). No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. | Exit 0, replay IDENTICO, misure, log in `UFFICIO/Officina/Test area/`. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md` | developer-junior | developer | VJ5 ∞ su prossimo diff `src/` (file/riga; `run_tests.sh`/ricompila prima se prova). VJ3 resta 4/6 (lv_05/lv_06 50/60 Hz) se utile; consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. Prove nuove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`. | Righe in `REVIEW_V47.md`; exit/replay/ms per eventuali prove. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**. Blocco noto: `lato_berlina_verde.sglraw` senza PNG (**DEFAULT: attendere Andrea, invariato**). No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non creato. Domande ad Andrea: nessuna nuova (DEFAULT prosegue). Pi 400 non verificato.