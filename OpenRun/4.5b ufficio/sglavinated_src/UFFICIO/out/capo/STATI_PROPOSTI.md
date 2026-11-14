# STATI PROPOSTI — Turno 1 (ora 0 di 12), 14/11/2026, @capo

**Tappa attiva:** v4.7 — **BLOCCO MIG1 [~]** (review) + **BLOCCO V47 [~]** continuo (passi ∞). Gate T1 storico verde (CP-006 **CHIUSO, non riaprire**). Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW, precedenza assoluta); G1 ANNULLATO (sponsor solo Andrea); decisioni aperte: nessuna; nessuna nuova `RISPOSTA DI ANDREA`. **Gate non rieseguito dal capo**; verdi riportati dagli handoff.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | registro | — |
| CP-002, CP-003, CP-004, DECO1, F60 | **APERTI [~]** | dossier esistenti | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 D1-D5,G2,J1-J4 | **[x] handoff, [~] review** | `REVIEW_MIG1.md`, ULTIMO_TURNO dev/junior, nota Claude 11/11 | D2 `sportellata` exit 28 (storico), J3 recupero `sbandataS` non misurato direttamente, D4 da confermare in review. **Costanti di guida/legacy NON si toccano.** Review incrociate autore≠revisore non registrate in dossier. |
| V47 V1-V5, VJ1, VJ3(4/6) parz., VJ4(6/6), VG2 | **[x] handoff / [~] da consolidare** | `REVIEW_V47.md`, handoff ruoli | V6 [~] handoff dev (picco 55.27 ms lv_05 smoke isolato, storico) review junior non consolidata. VJ3: lv_03 delta 50/60 Hz 4.70% (4.65% post-fix soglia test) ≠ test, causa nel gioco a 50 Hz su lv_03 → rilievo per @developer; lv_04 ok 0.03%. VJ2 [~] ispezione Andrea, DEFAULT non blocca. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 14/11 08:12 | 7° giro lv_05_spray: `percorso` exit 0 (2702 frame, 45.0 s, 0% fuori), `smoke` exit 0 medio 1.306 max 3.349/3.488 ms (<8), replay IDENTICO, caric 3.1-4.4 init 5.7-8.8 ms, nessun alleggerimento. **Prossimo:** lv_06_desktop (7° giro). Gate T1 non rieseguito. |
| V47 VJ3/VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` 14/11 | VJ3 lv_03/lv_04 misurati (vedi sopra), 4/6; manca confronto delta 50/60 nel gate `frequenza`. VJ5 review `fasi.cpp` (13/11) invariata come rilievo; salvataggio backup fix confermato. `salvataggi/giocatore.json` risulta cancellato in working tree (ripristino manuale fuori perimetro). |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 14/11 08:20 | Giro 27: 0 da convertire (188 PNG Libreria, 141 raw scenari/gruppi allineati, 0 gruppi sconosciuti). `vg3_date_png_raw.py` exit 0, `converti_oggetti_raw.py` exit 0, `--autotest` exit 0. Nessun PNG/raw toccato, nessuna risorsa esterna/Sega. |

## Checkpoint
- Nessun checkpoint con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere questo turno. Nessun nuovo dossier aperto.
- CP-002/003/004, DECO1, F60, MIG1/V47: restano APERTI/[~] fino a prove e review registrate.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: lv_06_desktop 7° giro — gate `percorso`/smoke, exit code, ms/frame medio/max (<8 ms Mac obiettivo), replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. V7/V8 se resta. Valutare rilievi VJ3 (delta 50/60 lv_03, causa nel gioco) e VJ5 (`fasi.cpp`) **solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1**. Rientro D2 sportellata se assegnato (fuori perimetro junior). No gate_t1 completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`, `REVIEW_MIG1.md` | developer-junior | developer | VJ3 ∞: lv_05_spray, lv_06_desktop 50/60 Hz (ricompila prima: `run_tests.sh`/build prova_gate), tempi/replay/ms/frame e delta 50 vs 60; segnalare se delta rilevante. VJ5 ∞: review di altro file `src/` al prossimo cambio (file+riga). Consolidare review indipendenti V6/MIG1/V47 con gate+exit code/misure (autore≠revisore) in dossier/review. Non modificare `src/` di prodotto salvo test se assegnato; non usare `prova_gate` vecchio. | Exit code, delta tempi 50/60, replay IDENTICO, righe in REVIEW_*. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (review indipendente se materiale) / Capo verifica handoff | VG3 ∞: confronta date PNG in `Libreria/assets_originali/scenari/` vs `.sglraw`; converti solo se PNG più nuovo con script del catalogo (es. `converti_oggetti_raw.py`), **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli ribaditi
- Scrittura Capo solo in `UFFICIO/Checkpoint Revision/**` e `UFFICIO/out/capo/STATI_PROPOSTI.md`. Percorsi relativi. Nessun file in radice `sglavinated_src/`.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio standard, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato: passi ∞ e blocchi non tutti [x]. `PER_CLAUDE.md` non necessario. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca).
