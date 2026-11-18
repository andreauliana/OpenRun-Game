# STATI PROPOSTI — Turno 1 (ora 1 di 12), 18/11/2026, @capo

**Tappa attiva:** v4.7 — **BLOCCO MIG1 [~]** (review) + **BLOCCO V47 [~]** continuo (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 approvata e **NON si tocca** (`ANDREA_REVIEW.md`, precedenza assoluta); G1 ANNULLATO (sponsor/cartelloni solo Andrea); nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo. `SCAMBIO_RECENTE.md` CHECKPOINT 1 17/11 10:16 [APERTA]: controlli automatici verdi, **non peer review in dossier**. Nessun checkpoint con gate+review indipendenti registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `REGISTRO_REVISIONI.md` | — |
| CP-002, CP-003, CP-004, DECO1, F60 | **APERTI [~]** | dossier esistenti | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 D1-D5,G2,J1-J4 | **[x] handoff, [~] review** | `UFFICIO/out/developer-junior/REVIEW_MIG1.md` | D2 `sportellata` exit 28 storico; J3 `sbandataS` non misurato; D4 da confermare. **Costanti di guida/vocabolario/legacy NON si toccano.** Review autore≠revisore non registrate in dossier. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6 review da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 18/11 12:13 | lv_05_spray 17° giro: build exit 0; `prova_gate ... percorso` exit 0 (2739f, 45.7s, 0% fuori); `prova_gate ... smoke` exit 0, medio 1.314 max 3.740 ms (<8), replay IDENTICO (`UFFICIO/Officina/Test area/v9_lv05_turno20*`). Nessun alleggerimento. **Prossimo:** lv_06_desktop 17°. |
| V47 VJ3 ∞ | **[~] 4/6** | `REVIEW_V47.md` | lv_01-lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% segnalato. **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[x] 6/6 completato** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 17/11 | Soak 6/6 exit 0, replay IDENTICO, nessun crash (max 126.451 ms lv_05; medi <20). Può ripartire da lv_01 se utile. |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 18/11 12:05, `REVIEW_V47.md` | Review commit `3ea407e` `renderer.cpp:29`: `terraOrdine_.reserve(384)` hardcoded, possibile riallocazione heap in `drawRoad` oltre 384 visibili; `run_tests.sh` exit 0, ctest 27/27 (275.42 s). Rilievi aperti cumulativi: `mixer.cpp:127-130`, `fasi.cpp:576`, `rng.h:19`, audio 50 Hz, `game.cpp:1809-1814`, salvataggi/classifica/segnale, ecc. **Prossimo:** altro cambio `src/` o consolidare review V6/MIG1/V47 autore≠revisore in `REVIEW_*`. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 18/11 12:08 | Giro 80: `vg3_date_png_raw.py` exit 0, 0 da convertire (145 PNG, 86 raw scenari, 47 altri gruppi). `vg3_raw_senza_catalogo.py`/`vg3_png_non_catalogati.py` exit 0. Catalogo/licenze invariati, zero esterni/Sega. **Prossimo:** confronto date/contenuto PNG↔raw. |

## Checkpoint
- Nessun checkpoint con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessun nuovo dossier aperto/chiuso.
- CP-002/003/004, DECO1, F60, MIG1/V47: restano APERTI/[~] fino a prove e review registrate in dossier.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: lv_06_desktop 17° giro — gate `percorso`/`smoke`, exit code, ms/frame medio/max (<8 ms Mac obiettivo), replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. Valutare rilievi junior (es. `renderer.cpp:29` reserve) solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio. No `gate_t1.sh` completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`, `REVIEW_MIG1.md` | developer-junior | developer | VJ3 lv_05_spray o lv_06_desktop 50/60 Hz (ricompila prima; non usare `prova_gate` vecchio), exit code, replay IDENTICO, delta tempi; oppure VJ5 ∞ altra review `src/`/consolidare review V6/MIG1/V47 autore≠revisore con gate+exit/misure in `REVIEW_*`. VJ4 6/6 completato (può ripartire da lv_01 se utile). Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/` prodotto. | Righe in `REVIEW_*` (file/riga); eventuale exit code, replay IDENTICO, ms/frame o delta 50/60. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. Nessun file in radice. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non necessario. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.
