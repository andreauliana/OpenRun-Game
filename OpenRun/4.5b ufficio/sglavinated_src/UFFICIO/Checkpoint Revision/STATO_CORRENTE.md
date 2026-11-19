# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-19 (Turno 3, ora 4 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.6, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE, checkpoint, handoff officina/grafico/dev/junior. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHECKPOINT 1-3 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** (review/consolidamento) + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **V9 ∞ [~]** dev 19/11: lv_01_neon 23° — build/percorso/smoke pre+post exit 0, `percorso` OK (1989f, 33.2s, 0% fuori), smoke post medio 0.919 max 3.604 ms, replay IDENTICO, frameHash invariati → **prossimo lv_02_atomico 23°**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz). **VJ4 ∞ [x] 6/6**. **VJ5 ∞ [~]** junior 19/11 11:55: review diff invariato `renderer.cpp:1804`/`renderer.h` (`colCerca_`) nessun errore; `run_tests.sh` exit 0 ctest 27/27 (274.81 s), smoke lv_06_desktop exit 0 medio 0.964 max 3.675 ms replay IDENTICO; prossimo VJ5 su prossimo diff dev. **VG3 ∞ [~]** grafico giro 94 18/11 16:14: 3 script exit 0, 0 da convertire, 126 pari, pose 2/2, 1 noto bloccato Andrea. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; `STATUS_OFFICINA.md` handoff sintetico, non peer review. Gate non rieseguito questo turno dal capo. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 94, 18/11 16:14, 3 script exit 0, 0 da convertire, 126 pari, pose 2/2; zero esterni/Sega).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_01_neon 23° 19/11, percorso/smoke pre+post exit 0, medio 0.919 max 3.604 ms post, replay IDENTICO; prossimo lv_02_atomico 23°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ5 19/11 11:55, review `renderer.cpp`/`renderer.h` colCerca_, `run_tests.sh` exit 0 ctest 27/27, smoke lv_06 exit 0 replay IDENTICO; VJ3 4/6, VJ4 6/6).

## Prossime azioni
1. **@developer**: V9 ∞ lv_02_atomico 23° (`percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO); se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta.
2. **@developer-junior**: VJ5 ∞ su prossimo diff `src/` del @developer (ricompila prima, prova mirata, file/riga in `REVIEW_V47.md`) oppure VJ3 lv_05_spray/lv_06_desktop 50/60 Hz; consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. VJ4 6/6. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`.
3. **@grafico**: VG3 ∞ confronto date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, senza ritoccare il disegno; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
