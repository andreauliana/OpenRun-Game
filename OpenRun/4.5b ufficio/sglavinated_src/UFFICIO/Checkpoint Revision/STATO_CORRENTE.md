# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-19 (Turno 1, ora 0 di 12, @capo, coord.) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.6, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE, handoff officina/grafico/dev/junior. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHECKPOINT 1-2-3 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** (review/consolidamento) + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **V9 ∞ [~]** dev 19/11 08:31: lv_02_atomico 21° — build exit 0 pre/post, `percorso` exit 0 post (2476f, 41.3s, 0% fuori), smoke post exit 0 medio 0.981 max 3.605 ms (<8), replay IDENTICO → **prossimo lv_03_foglie 21°**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz; lv_03 delta 4.70%). **VJ4 ∞ [x] 6/6**. **VJ5 ∞ [~]** junior 19/11 08:45: review drawSprite precalc `renderer.cpp:1784-1793`/`renderer.h:497-498` (nessun errore, nota replay stesso binario), semplificazione `drawLuceGalleria` segnalata; `run_tests.sh` exit 0 ctest 27/27, percorso/smoke lv_02 exit 0 replay IDENTICO; backlog aperto. **VG3 ∞ [~]** grafico giro 94 18/11 16:14: 3 script exit 0, 0 da convertire, 126 pari, pose 2/2, 1 noto bloccato Andrea. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate/prove verdi riportati dagli handoff; `STATUS_OFFICINA.md` letto (handoff sintetico, non peer review). Gate non rieseguito questo turno. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 94, 18/11 16:14, 3 script exit 0, 0 da convertire, 126 pari contenuto, pose 2/2; catalogo/licenze invariati, zero esterni/Sega).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md) (handoff sintetico), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_02_atomico 21° 19/11 08:31, percorso exit 0 post 2476f/41.3s/0% fuori, smoke post exit 0 medio 0.981 max 3.605 ms, replay IDENTICO; prossimo lv_03_foglie 21°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ5 19/11 08:45 review drawSprite precalc + `run_tests.sh` exit 0 ctest 27/27, percorso/smoke lv_02 exit 0; VJ3 4/6, VJ4 6/6).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ lv_03_foglie 21° giro (`percorso`/`smoke`, exit code, ms/frame medio/max, replay IDENTICO); se >8 ms alleggerire senza cambiare resa/replay. Valutare rilievi junior VJ5 solo se misurati/compatibili; non toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta.
2. **@developer-junior** (`UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`): VJ3 lv_05_spray o lv_06_desktop 50/60 Hz (ricompila prima) oppure VJ5 ∞ su prossimo cambio `src/`/consolidare review V6/MIG1/V47 autore≠revisore con file/riga/prove. VJ4 6/6 completato. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/` prodotto.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): VG3 ∞ confronto date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
