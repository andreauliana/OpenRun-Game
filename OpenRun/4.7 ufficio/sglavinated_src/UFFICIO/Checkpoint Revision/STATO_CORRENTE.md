# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-26 (T1 ora 1/12, @capo) — fonti rilette. ANDREA_REVIEW prec. ass. §7 nessuna RISPOSTA; REGOLA BLOCCATA drawPlayerCar §2. Legacy 08/11+vocabolario intoccati. Tappa V47 [~] ∞. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. 0 CP chiudibili.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI (review/prove mancanti). DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | **D20 bloccante (REGOLA BLOCCATA §2):** 5 hunk `drawPlayerCar` (`renderer.cpp:2798-2816,2875-2889,2890-2905,2910-2922,2932-2952`); jr 26/11 17:46 conferma D20 invariato. Serve autorizzazione Andrea o revert. Dev 26/11 17:47 lv_02 g40: no mod codice; percorso exit 0 (2476f,41.3s,0% fuori), smoke exit 0 medio 0.811 max 61.088ms spike frame 248 (gate OK), replay IDENTICO 2961894142. Jr 26/11 17:46: run_tests 27/27 exit 0; VJ1 lv01 smoke exit 0 hash 3594390208; gate pali lv06 exit 27 (soglia tarata lv01, non regressione). **VJ3 lv_05 50/60Hz FAIL 2.40%** deterministico aperto; altri 5 OK. VJ4 6/6 chiuso. Gf 26/11 09:27 g212: 0 conv, exit 0. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; D20 richiede verifica jr dopo revert/autorizzazione. Nessun CP chiudibile. |
| Build/test | Handoff dev 26/11 17:47, jr 26/11 17:46, gf 26/11 09:27. Capo non riesegue. Gate T1 riferimento 25/11 13:04 exit 0. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (26/11 09:27, g212)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (26/11 17:47, V9 lv_02 g40, no mod codice)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (26/11 17:46, VJ5/VJ1, pali lv06 exit27; D20 invariato; VJ3 lv05 FAIL), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (aggiornato jr 18:29/dev 17:20)

## Prossime azioni
1. @developer: **DEFAULT D20** — revert o autorizzazione Andrea registrata per 5 hunk drawPlayerCar; `run_tests.sh` exit 0, smoke+percorso lv_02 replay IDENTICO. Poi V9 ∞ lv_03_foglie; indagare VJ3 lv_05 50/60Hz diff 2.40%. Reviser: @developer-junior.
2. @developer-junior: verificare risoluzione D20 quando presente; indagare/ripetere VJ3 lv_05 con log; VJ5 ∞ prossimo diff; valutare portabilità gate pali. `run_tests.sh` prima prova; file/riga in `REVIEW_V47.md`; no src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT: `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA. Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; nessuna chiusura CP senza gate+review indipendenti in dossier. Monitorare D20 e VJ3 lv_05.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.