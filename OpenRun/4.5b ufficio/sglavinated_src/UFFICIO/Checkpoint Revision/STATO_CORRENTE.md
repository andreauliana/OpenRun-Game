# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-19 (Turno 3, ora 5 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.6, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE, checkpoint, handoff officina/grafico/dev/junior. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHECKPOINT 1-3 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** (review/consolidamento) + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **V9 ∞ [~]** dev 19/11 12:52: lv_04_cromo 23° — build/percorso/smoke pre+post exit 0, `percorso` OK (2125f, 35.4s, 0% fuori), smoke post medio 1.101 max 3.257 ms, replay IDENTICO, frameHash invariati; `gate_t1.sh` exit 0 "TUTTO OK" 6/6 → **prossimo lv_05_spray 23°**. **VJ3 [~] 4/6** (lv_05/lv_06 50/60 Hz). **VJ4 ∞ [~] 1/6** junior 19/11 12:58: soak lv_01_neon exit 0 (30000f, replay 2x IDENTICO, medio 1.552 max 7.964 ms, no crash) → **prossimo lv_02_atomico 2/6**. **VJ5 ∞ [~]** junior 19/11 12:58: review V9 diff `palette.h`/`renderer.cpp` nessun errore; `run_tests.sh` exit 0 ctest 27/27 (248.79 s); prossimo VJ5 su prossimo diff dev. **VG3 ∞ [~]** grafico giro 94 18/11 16:14: 3 script exit 0, 0 da convertire, 126 pari, pose 2/2, 1 noto bloccato Andrea. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff (dev: gate_t1.sh exit 0 19/11 12:52; junior: ctest 27/27 + soak lv_01 exit 0 19/11 12:58). `STATUS_OFFICINA.md` handoff sintetico, non peer review e non allineato agli ultimi turni. Gate non rieseguito questo turno dal capo. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 94, 18/11 16:14, 3 script exit 0, 0 da convertire, 126 pari, pose 2/2; zero esterni/Sega).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md) (sintetico, non allineato agli ultimi turni), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_04_cromo 23° 19/11 12:52, percorso/smoke pre+post exit 0, medio 1.101 max 3.257 ms post, replay IDENTICO, gate_t1.sh exit 0; prossimo lv_05_spray 23°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak lv_01_neon 1/6 exit 0 + VJ1/VJ5 review diff V9 19/11 12:58, `run_tests.sh` exit 0 ctest 27/27; VJ3 4/6).

## Prossime azioni
1. **@developer**: V9 ∞ lv_05_spray 23° (`percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO); se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta.
2. **@developer-junior**: VJ4 ∞ soak lv_02_atomico 2/6 (ricompila prima) e/o VJ5 ∞ su prossimo diff `src/` del @developer (file/riga in `REVIEW_V47.md`), oppure VJ3 lv_05_spray/lv_06_desktop 50/60 Hz; consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`.
3. **@grafico**: VG3 ∞ confronto date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, senza ritoccare il disegno; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
