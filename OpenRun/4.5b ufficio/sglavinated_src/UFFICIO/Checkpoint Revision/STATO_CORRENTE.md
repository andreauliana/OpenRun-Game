# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-18 (Turno 1, ora 1 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata; decisioni aperte nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.6, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE + STATO_CORRENTE/REGISTRO/handoff. Tappa attiva: MIG1+V47. SCAMBIO_RECENTE CHECKPOINT 1 17/11 10:16 [APERTA]: controlli automatici verdi, gate T1 OK (non peer review in dossier). Gate non rieseguito dal capo; verdi riportati dagli handoff correnti (dev V9 lv_05_spray 17° giro 18/11 12:13 build exit 0, percorso exit 0 2739f/45.7s/0% fuori, smoke exit 0 medio 1.314 max 3.740 ms replay IDENTICO, prossimo lv_06 17°; junior VJ5 18/11 12:05 review `3ea407e` `renderer.cpp:29` reserve(384) hardcoded, `run_tests.sh` exit 0 ctest 27/27, VJ4 6/6, VJ3 4/6; grafico VG3 giro 80 18/11 12:08 `vg3_date_png_raw.py`/`vg3_raw_senza_catalogo.py`/`vg3_png_non_catalogati.py` exit 0, 0 da convertire). Nessun CP chiudibile (review indipendenti autore≠revisore non registrate in dossier). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] review** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1-D5,G2,J1-J4 **[x] handoff** — **costanti di guida NON si toccano**; review incrociate non consolidate (D2 `sportellata` exit 28 storico, J3 `sbandataS` non misurato, D4 da confermare). **V47:** V1-V5,VJ1,VG2 [x] handoff; V6 review da consolidare. **V9 ∞ [~]** dev: lv_05_spray 17° giro 18/11 exit 0 medio 1.314 max 3.740 ms replay IDENTICO → **prossimo lv_06_desktop 17°**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz; lv_03 delta 4.70% segnalato). **VJ4 ∞ [x] 6/6 completato** (soak 6/6 exit 0 IDENTICO). **VJ5 ∞ [~]** rilievi cumulativi in `REVIEW_V47.md` (ultimo 18/11 `renderer.cpp:29` reserve(384), run_tests exit 0 27/27). **VG3 ∞ [~]** grafico giro 80 18/11 12:08, 0 da convertire. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; gate non rieseguito questo turno. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 80, 18/11 12:08, `vg3_date_png_raw.py`/`vg3_raw_senza_catalogo.py`/`vg3_png_non_catalogati.py` exit 0, 0 da convertire).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_05_spray 17° giro 18/11 12:13, percorso exit 0, smoke exit 0 medio 1.314 max 3.740 ms, replay IDENTICO; prossimo lv_06 17°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ5 18/11 12:05 review `renderer.cpp:29` reserve hardcoded, `run_tests.sh` exit 0 ctest 27/27; VJ3 4/6, VJ4 6/6).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ lv_06_desktop 17° giro (gate `percorso`/`smoke`, exit code, ms/frame, replay IDENTICO); se >8 ms alleggerire senza cambiare resa/replay. Valutare rilievi junior solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ3 lv_05_spray o lv_06_desktop 50/60 Hz (ricompila prima) oppure VJ5 ∞ review/consolidare in `REVIEW_*` autore≠revisore con file/riga/prove. VJ4 6/6 completato. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): ∞ VG3 (confronto PNG scenari vs `.sglraw`; convertire solo se PNG più nuovo con script catalogo, senza ritocchi; altrimenti 1 riga). No sponsor/asset Andrea, no `frasi.json`.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
