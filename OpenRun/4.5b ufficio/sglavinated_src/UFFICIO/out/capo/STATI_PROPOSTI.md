# STATI PROPOSTI — Turno 2 (ora 2 di 12), 17/11/2026, @capo

**Tappa attiva:** v4.7 — **BLOCCO MIG1 [~]** (review) + **BLOCCO V47 [~]** continuo (passi ∞). Gate T1 storico verde (CP-006 **CHIUSO, non riaprire**). Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW, precedenza assoluta); G1 ANNULLATO (sponsor solo Andrea); decisioni aperte: nessuna; nessuna nuova `RISPOSTA DI ANDREA`. **Gate non rieseguito dal capo**; esiti letti dagli handoff correnti. SCAMBIO_RECENTE CHECKPOINT 1 17/11 10:16 [APERTA]: controlli automatici verdi, gate T1 OK (non peer review né chiusura dossier). Nessun checkpoint con gate+review indipendenti registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | registro | — |
| CP-002, CP-003, CP-004, DECO1, F60 | **APERTI [~]** | dossier esistenti | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 D1-D5,G2,J1-J4 | **[x] handoff, [~] review** | `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, handoff dev/junior | D2 `sportellata` exit 28 storico; J3 recupero `sbandataS` non misurato; D4 da confermare. **Costanti di guida/legacy NON si toccano.** Review incrociate autore≠revisore non registrate in dossier. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~] da consolidare** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6 [~] review junior da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 17/11 | lv_02_atomico 16° giro: `prova_gate ... percorso` exit 0 (2464f, 41.1s, 0% fuori, GATE PERCORSO OK), `smoke` exit 0 medio 1.306 max 6.863 ms (<8), replay IDENTICO; log `UFFICIO/Officina/Test area/v9_lv02_turno16*`. Nessun alleggerimento. **Prossimo:** lv_03_foglie 16° giro. |
| V47 VJ3 ∞ | **[~] 4/6** | `REVIEW_V47.md`, handoff junior | lv_01-lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% (causa nel gioco a 50 Hz, segnalato). **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[x] 6/6 completato** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 17/11 09:49 | Soak lv_06_desktop: build prova_gate exit 0, `prova_gate ... lv_06_desktop soak` exit 0, 30000f, replay 2x IDENTICO, ms/frame medio 1.761 max 72.711 (limite 20), nessun crash; log `UFFICIO/Officina/Test area/vj4_soak_lv06_1711/`. Può ripartire da lv_01 se utile. |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | Rilievi aperti cumulativi (es. `mixer.cpp:127-130`, `fasi.cpp:576`, `rng.h:19`, `game.h:49`/runner, `game.cpp:1809-1814`, `audio/mixer.cpp:91-99`/`mixer.h:88`, salvataggi/classifica/segnale). **Prossimo:** altra review `src/` o consolidare review V6/MIG1/V47 autore≠revisore in `REVIEW_*`. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 17/11 09:46 | Giro 71: `vg3_date_png_raw.py` exit 0, `converti_oggetti_raw.py` exit 0, 0 da convertire. Catalogo/licenze invariati, zero esterni/Sega. **Prossimo:** VG3 ∞ confronto date/contenuto PNG↔raw. |

## Checkpoint
- Nessun checkpoint con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessun nuovo dossier aperto/chiuso.
- CP-002/003/004, DECO1, F60, MIG1/V47: APERTI/[~] fino a prove e review registrate in dossier.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: lv_03_foglie 16° giro — gate `percorso`/smoke, exit code, ms/frame medio/max (<8 ms Mac obiettivo), replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. Valutare rilievi junior solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio standard. No `gate_t1.sh` completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`, `REVIEW_MIG1.md` | developer-junior | developer | VJ3 lv_05_spray o lv_06_desktop 50/60 Hz (ricompila prima; non usare `prova_gate` vecchio), exit code, replay IDENTICO, delta tempi; oppure VJ5 ∞ altra review `src/`/consolidare review V6/MIG1/V47 autore≠revisore con gate+exit code/misure in `REVIEW_*`. VJ4 6/6 completato (può ripartire da lv_01 se utile). Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/` prodotto. | Righe in REVIEW_* con file/riga; eventuale exit code, replay IDENTICO, ms/frame o delta 50/60. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli ribaditi
- Scrittura Capo solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*` (diario, RIASSUNTO). Percorsi relativi. Nessun file in radice `sglavinated_src/`. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio standard, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato: passi ∞ e blocchi non tutti [x]. `PER_CLAUDE.md` non necessario. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.
