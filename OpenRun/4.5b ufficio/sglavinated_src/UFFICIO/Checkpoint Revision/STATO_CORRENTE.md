# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-14 (Turno 1, ora 0 di 12, @capo) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata; decisioni aperte nessuna)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞)/SCAMBIO_RECENTE + checkpoint/handoff. Tappa attiva: MIG1+V47. Gate non rieseguito dal capo; verdi riportati (dev V9 lv_05_spray 7° giro 14/11 exit 0 medio 1.306 max 3.349/3.488 ms < 8, replay IDENTICO; junior VJ3 lv_03/lv_04 14/11 exit 0, 4/6, lv_03 delta 50/60 Hz 4.70%>tolleranza implicita segnalato, lv_04 0.03%; VJ4 soak 6/6 storico; grafico VG3 giro 27 14/11 exit 0, 0 da convertire). Nessun CP chiudibile (review indipendenti autore≠revisore non registrate in dossier). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] review** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1-D5,G2,J1-J4 **[x] handoff** (nota Claude 11/11) — **costanti di guida NON si toccano**; review incrociate non consolidate in dossier (residui: D2 `sportellata` exit 28 storico, J3 recupero `sbandataS` non misurato, D4 da confermare). **V47:** V1-V5,VJ1 [x] handoff; **V6 [~]** handoff dev (picco 55.27 ms lv_05 smoke isolato), review junior parziale da consolidare. **V7/V8 [~]/[ ]**. **V9 ∞ [~]** dev 14/11 08:12: lv_05_spray 7° giro `percorso`+`smoke` exit 0, medio 1.306 max 3.349/3.488 ms < 8, replay IDENTICO → **prossimo lv_06_desktop** (7° giro). **VJ3 [~] 4/6** junior 14/11: lv_03/lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% (causa gioco a 50 Hz, non guida), lv_04 0.03%; prossimi lv_05/lv_06. **VJ4 [x] 6/6** soak storico (medi < 20 ms, max 124.129 lv_06). **VJ5 ∞ [~]** review `fasi.cpp` rilievo invariato; `salvataggi/giocatore.json` segnalato cancellato in working tree (ripristino manuale fuori perimetro). **VG2 [x] chiuso** → **VG3 ∞ [~]** Grafico 14/11 08:20 giro 27, 0 da convertire, tool `vg3_date_png_raw.py`/autotest exit 0. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; gate non rieseguito questo turno. Log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 27, 14/11 08:20, 0 da convertire).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_05 7° giro 14/11 08:12, exit 0, medio 1.306 max 3.349/3.488 ms).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ3 4/6 14/11, VJ4 6/6 storico).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ lv_06_desktop 7° giro (gate `percorso`/smoke, exit code, ms/frame medio/max); se >8 ms alleggerire senza cambiare resa/replay. V7/V8 se resta. Valutare rilievo VJ3 (delta 50/60 lv_03 nel gioco) e VJ5 (`fasi.cpp`) solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1. **Non toccare** costanti guida/MIG1/legacy.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ3 ∞ lv_05/lv_06 50/60 Hz (ricompila prima), riportare delta/replay/ms; consolidare review V6/MIG1/V47 autore≠revisore in `REVIEW_V47.md`/`REVIEW_MIG1.md` (gate+exit code/misure). VJ5 ∞ altro file `src/` al prossimo cambio.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): ∞ VG3 (confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga). No sponsor/asset Andrea, no `frasi.json`.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).