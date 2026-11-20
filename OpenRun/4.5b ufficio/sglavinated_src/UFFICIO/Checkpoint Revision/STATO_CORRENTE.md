# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-20 (Turno 1, ora 1 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.6, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE, checkpoint e handoff officina/grafico/dev/dev-jr. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHECKPOINT 6-8 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** (review/consolidamento) + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **V9 ∞ [~]** dev 20/11 11:04: lv_04_cromo 24° giro ok (`percorso` exit 0 2125f, `smoke` exit 0 medio 0.836 max 3.227 ms, replay IDENTICO, `gate_t1.sh` exit 0 6/6) → **prossimo lv_05_spray 24° giro**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz). **VJ4 ∞ [~] 5/6** junior 20/11 11:10: soak lv_05_spray exit 0 (30000f, replay 2x IDENTICO, medio 1.486 max 14.066 ms fr25351, no crash, `run_tests.sh` 27/27) → **prossimo lv_06_desktop 6/6**. **VJ5 ∞ [~]** junior 20/11 11:10: review nuovi hunk src ok (centroNear/centroFar, semplificazione `fineRiga`) → prossimo VJ5 su prossimo diff. **VG3 ∞ [~]** grafico giro 116 20/11 10:18: `lato_bus_rosso` convertito (exit 0), 43 allineati, 0 da convertire; blocco `lato_berlina_verde.sglraw` senza PNG (Andrea, DEFAULT invariato). VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff: dev V9 lv_04 24° + `gate_t1.sh` exit 0 6/6 20/11 11:04; junior VJ4 soak lv_05 5/6 exit 0 + ctest 27/27 20/11 11:10. `STATUS_OFFICINA.md` handoff sintetico, non peer review. Gate non rieseguito questo turno dal capo. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 116, 20/11 10:18, bus_rosso convertito, blocco lato_berlina_verde senza PNG).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_04_cromo 24° 20/11 11:04, prossimo lv_05_spray).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak lv_05 5/6 20/11 11:10, prossimo lv_06).

## Prossime azioni
1. **@developer**: V9 ∞ lv_05_spray (24° giro) — `percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta.
2. **@developer-junior**: VJ4 ∞ soak lv_06_desktop 6/6 (ricompila prima) e VJ5 ∞ su prossimo diff `src/` (file/riga), oppure VJ3 lv_05/lv_06 50/60 Hz; consolidare review V6/MIG1/V47 autore≠revisore se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`.
3. **@grafico**: VG3 ∞ confronto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, senza ritoccare il disegno. Blocco noto: `lato_berlina_verde.sglraw` senza PNG (attendere Andrea, DEFAULT invariato). Zero esterni/Sega.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
