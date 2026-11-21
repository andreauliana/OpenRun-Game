# Stato corrente — OpenRun 4.7

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-21 (Turno 1, ora 0 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞), SCAMBIO_RECENTE, checkpoint e handoff. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE checkpoint automatici (fino CHK2 20/11 13:54) [APERTE]: controlli verdi (**non peer review in dossier**). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** (review) + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | **V9 ∞ [~]** dev 20/11 14:13: lv_02_atomico 25° giro ok (`cmake --build --target prova_gate -j4` exit 0; `prova_gate ... lv_02_atomico smoke` exit 0 medio 0.793 max 3.168 ms, replay IDENTICO; `prova_gate ... lv_02_atomico percorso` exit 0 2476f/41.3s/0% fuori; `gate_t1.sh` exit 0 6/6) → **prossimo lv_03_foglie 25°**. **VJ3 [~] 4/6** (prossimi lv_05/lv_06 50/60 Hz). **VJ4 ∞ [x] 6/6** chiuso 20/11 11:31 (picco soak lv_06 95.641 ms segnalato per V9). **VJ5 ∞ [~]** junior 20/11 14:04: review diff `renderer.cpp` hunk V9 lv_01 26° (`giroMenoFascia`), nessun errore; `run_tests.sh` exit 0 27/27 (226.16s); `prova_gate ... lv_01_neon percorso` exit 0 → prossimo VJ5 su prossimo diff. **VG3 ∞ [~]** grafico giro 116 20/11 10:18: `lato_bus_rosso` convertito (exit 0), 43 allineati, 0 da convertire; blocco `traffico_incrocio/lato_berlina_verde.sglraw` senza PNG (Andrea, **DEFAULT invariato**). VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e in `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.7). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff: dev V9 lv_02 25° + `gate_t1.sh` exit 0 6/6 20/11 14:13 (`UFFICIO/out/developer/ULTIMO_TURNO.md`); junior VJ5 20/11 14:04 `run_tests.sh` exit 0 27/27 + percorso lv_01 exit 0 (`UFFICIO/out/developer-junior/ULTIMO_TURNO.md`). `STATUS_OFFICINA.md` arretrato, **non peer review**. Gate non rieseguito questo turno dal capo. Log in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG3 giro 116, 20/11 10:18; blocco lato_berlina_verde senza PNG).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md) (arretrato), [ULTIMO_TURNO developer](../out/developer/ULTIMO_TURNO.md) (V9 lv_02_atomico 25° 20/11 14:13, prossimo lv_03_foglie 25°).
- Developer-junior: [ULTIMO_TURNO developer-junior](../out/developer-junior/ULTIMO_TURNO.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 6/6 chiuso, VJ5 20/11 14:04).

## Prossime azioni
1. **@developer**: V9 ∞ lv_03_foglie (25° giro) — `percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. Picco soak lv_06 (95.641 ms) da considerare quando toccherà lv_06. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor.
2. **@developer-junior**: VJ5 ∞ su prossimo diff `src/` (file/riga, ricompila prima se serve); VJ3 resta 4/6 (lv_05/lv_06 50/60 Hz) se utile; consolidare review V6/MIG1/V47 autore≠revisore solo se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`.
3. **@grafico**: VG3 ∞ confronto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**. Blocco noto: `lato_berlina_verde.sglraw` senza PNG (attendere Andrea, **DEFAULT: invariato**). Zero esterni/Sega.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
