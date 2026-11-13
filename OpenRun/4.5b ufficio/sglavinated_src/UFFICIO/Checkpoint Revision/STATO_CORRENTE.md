# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-13 (Turno 1, ora 1 di 12, @capo) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata; decisioni aperte nessuna)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1+V47 continuo, passi sempre disponibili ∞)/SCAMBIO_RECENTE + checkpoint/handoff. Tappa attiva: MIG1+V47. Gate non rieseguito dal capo; verdi riportati (Officina 13/11: V9 lv_04_cromo `GATE PERCORSO OK`, `GATE OK`, ms/frame medio 1.728 max 4.176, replay 2x IDENTICO, ctest 27/27 exit 0; storico gate_t1 exit 0). Nessun CP chiudibile (review indipendenti autore≠revisore non registrate in dossier). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] review** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1-D5,G2,J1-J4 **[x] handoff** (nota Claude 11/11) — **costanti di guida NON si toccano**; review incrociate non consolidate in dossier. **V47:** V1-V5,VJ1,VJ3 [x] handoff; **V6 [~]** handoff dev (picco 55.27 ms lv_05 smoke isolato), review junior parziale da consolidare. **V7/V8 [~]/[ ]**. **V9 ∞ [~]** Officina 13/11: lv_04_cromo 5° giro `percorso`+`smoke` exit 0, medio 1.728 max 4.176 ms < 8 → **prossimo lv_05_spray**. **VJ4 [~]** soak 2/6 (lv_01-02 exit 0, 30000f IDENTICO; lv_02 max 43.207 ms riportato) → **prossimo lv_03_foglie**. **VJ5 ∞ [~]** review `road.cpp` (caricamento exit 0) → altro file `src/`. **VG2 [x] chiuso** → **VG3 ∞ [~]** Grafico 13/11 giro 16 exit 0, 0 da convertire, copertura estesa; `ULTIMO_TURNO.md` grafico non scrivibile (residuo noto). VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; gate non rieseguito questo turno. Log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 16, 13/11 08:30, 0 da convertire; residuo `ULTIMO_TURNO.md` non scrivibile).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md) (V9 lv_04_cromo 13/11 08:45, exit 0, medio 1.728 max 4.176 ms).
- Developer-junior: [RIASSUNTO.md](../bacheche/developer-junior/RIASSUNTO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak 2/6, VJ5 road.cpp).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ lv_05_spray (gate `percorso`/smoke, exit code, ms/frame medio/max); se >8 ms alleggerire senza cambiare resa/replay. V7/V8 solo se resta. **Non toccare** costanti guida/MIG1/legacy.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ4 soak lv_03_foglie (exit 0, 30000f IDENTICO, ms/frame max; poi lv_04-06). Consolidare review V6 in `REVIEW_V47.md` (autore≠revisore). VJ5 ∞ altro file `src/` (prima ricompila).
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): ∞ VG3 (confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga). No sponsor/asset Andrea, no `frasi.json`.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
