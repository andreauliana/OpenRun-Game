# STATI PROPOSTI — Turno 2 (ora 2 di 12), 15/11/2026, @capo

**Tappa attiva:** v4.7 — **BLOCCO MIG1 [~]** (review) + **BLOCCO V47 [~]** continuo (passi ∞). Gate T1 storico verde (CP-006 **CHIUSO, non riaprire**). Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW, precedenza assoluta); G1 ANNULLATO (sponsor solo Andrea); decisioni aperte: nessuna; nessuna nuova `RISPOSTA DI ANDREA`. **Gate non rieseguito dal capo**; esiti letti dagli handoff correnti (15/11). Nessun checkpoint con gate+review indipendenti registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | registro | — |
| CP-002, CP-003, CP-004, DECO1, F60 | **APERTI [~]** | dossier esistenti | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 D1-D5,G2,J1-J4 | **[x] handoff, [~] review** | `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, handoff dev/junior | D2 `sportellata` exit 28 storico; J3 recupero `sbandataS` non misurato direttamente; D4 da confermare. **Costanti di guida/legacy NON si toccano.** Review incrociate autore≠revisore non registrate in dossier. |
| V47 V1-V5, VJ1, VJ4(6/6), VG2 | **[x] handoff / [~] da consolidare** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6 [~] review junior da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 15/11 10:16 | lv_04_cromo 11° giro: `prova_gate ... percorso` exit 0 (2114f, 35.2s, 0% fuori, GATE PERCORSO OK), `smoke` exit 0 medio 1.548 max 4.440 ms (<8), replay IDENTICO; log `UFFICIO/Officina/Test area/v9_lv04_turno11*`. **Prossimo:** lv_05_spray 11° giro. |
| V47 VJ3 ∞ | **[~] 4/6** | `REVIEW_V47.md`, handoff junior | lv_03/lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% (causa nel gioco a 50 Hz, segnalato). **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 15/11 10:17 | Review `src/game/game.h:49` + uso `game.cpp:511`, `runner/main.cpp:344,346` (AUDIO_FRAME_SAMPLES fisso 44100/60: possibile desync/sottoriempimento a 50 Hz). Nessuna build/test questo turno. Rilievi aperti: `mixer.cpp:127-130`, `rng.h:19`, `core/replay.cpp:9,20`, `game.cpp:912/2234/2812/2794`, altri in `REVIEW_V47.md`. **Prossimo:** altra review `src/` su file/riga diversi. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 15/11 09:27 | Giro 40: 0 PNG da convertire. `vg3_date_png_raw.py` exit 0, `converti_oggetti_raw.py` exit 0. Catalogo invariato (184 elem., exit 0 15/11 08:50), zero esterni/Sega. **Prossimo:** VG3 ∞ confronto date PNG↔raw. |

## Checkpoint
- Nessun checkpoint con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessun nuovo dossier aperto/chiuso. Nessuna chiusura eseguibile questo turno.
- CP-002/003/004, DECO1, F60, MIG1/V47: APERTI/[~] fino a prove e review registrate in dossier.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: lv_05_spray 11° giro — gate `percorso`/smoke, exit code, ms/frame medio/max (<8 ms Mac obiettivo), replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. Valutare rilievi junior (VJ3 delta 50/60 lv_03; audio 50 Hz `game.h:49`/runner; `mixer.cpp:127-130`/`rng.h:19`) **solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio standard**. No `gate_t1.sh` completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`, `REVIEW_MIG1.md` | developer-junior | developer | VJ5 ∞ (VJ4 6/6 fatto): altra review `src/` su file/riga diversi (o consolidare review V6/MIG1/V47 autore≠revisore con gate+exit code/misure in `REVIEW_*`). Se rieseguito: VJ3 lv_05/lv_06 50/60 Hz con ricompila prima, exit code, delta/replay/ms. Non modificare `src/` prodotto salvo test assegnati; non usare `prova_gate` vecchio. | Righe in REVIEW_* con file/riga; eventuale exit code/delta 50/60/replay IDENTICO. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli ribaditi
- Scrittura Capo solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*` (diario, RIASSUNTO). Percorsi relativi. Nessun file in radice `sglavinated_src/`. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio standard, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato: passi ∞ e blocchi non tutti [x]. `PER_CLAUDE.md` non necessario. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.
