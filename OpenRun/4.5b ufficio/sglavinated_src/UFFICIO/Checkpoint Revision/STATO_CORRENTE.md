# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-17 (Turno 2, ora 2 di 12, @capo) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata; decisioni aperte nessuna)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞)/SCAMBIO_RECENTE + STATO_CORRENTE/REGISTRO/handoff correnti. Tappa attiva: MIG1+V47. SCAMBIO_RECENTE CHECKPOINT 1 17/11 10:16 [APERTA]: controlli automatici verdi, gate T1 OK (non peer review in dossier). Gate non rieseguito dal capo; verdi riportati dagli handoff correnti (dev V9 lv_02_atomico 16° giro 17/11 exit 0 percorso 2464f 41.1s 0% fuori, smoke medio 1.306 max 6.863 ms replay IDENTICO, prossimo lv_03 16°; junior VJ4 soak lv_06_desktop 17/11 09:49 exit 0 30000f replay IDENTICO medio 1.761 max 72.711, nessun crash, VJ4 6/6 completato, VJ3 resta 4/6; grafico VG3 giro 71 17/11 09:46 `vg3_date_png_raw.py` exit 0 + `converti_oggetti_raw.py` exit 0, 0 da convertire). Nessun CP chiudibile (review indipendenti autore≠revisore non registrate in dossier). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] review** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1-D5,G2,J1-J4 **[x] handoff** — **costanti di guida NON si toccano**; review incrociate non consolidate (D2 `sportellata` exit 28 storico, J3 `sbandataS` non misurato, D4 da confermare). **V47:** V1-V5,VJ1,VG2 [x] handoff; V6 [~] review da consolidare. **V9 ∞ [~]** dev: lv_02_atomico 16° giro ok 17/11 → **prossimo lv_03_foglie 16° giro**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz; lv_03 delta 4.70% segnalato). **VJ4 ∞ [x] 6/6 completato** junior: soak lv_06 17/11 exit 0 30000f IDENTICO medio 1.761 max 72.711, no crash → prossimo **VJ3 lv_05/lv_06 o VJ5 ∞**. **VJ5 ∞ [~]** rilievi aperti cumulativi in `REVIEW_V47.md`. **VG3 ∞ [~]** grafico giro 71 17/11 09:46, 0 da convertire. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; gate non rieseguito questo turno. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 71, 17/11 09:46, `vg3_date_png_raw.py` exit 0 + `converti_oggetti_raw.py` exit 0, 0 da convertire).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_02_atomico 16° giro 17/11, exit 0, medio 1.306 max 6.863 ms, replay IDENTICO; prossimo lv_03 16°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak lv_06 17/11 09:49 exit 0 30000f IDENTICO medio 1.761 max 72.711, no crash, VJ4 6/6 completato; VJ3 4/6).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ lv_03_foglie 16° giro (gate `percorso`/smoke, exit code, ms/frame, replay IDENTICO); se >8 ms alleggerire senza cambiare resa/replay. Valutare rilievi junior solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ3 lv_05_spray o lv_06_desktop 50/60 Hz (ricompila prima, non usare `prova_gate` vecchio) oppure VJ5 ∞ review/consolidare in `REVIEW_*` su file/riga diversi (autore≠revisore, gate/exit/misure). VJ4 6/6 completato. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): ∞ VG3 (confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga). No sponsor/asset Andrea, no `frasi.json`.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
