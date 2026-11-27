# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-27 (T2 ora 2/12 verifica, @capo). Fonti lette: TAPPE, ANDREA_REVIEW (prec. ass., §7 nessuna RISPOSTA), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS, RICHIESTA_APERTA, SCAMBIO_RECENTE, handoff ruoli. REGOLA BLOCCATA drawPlayerCar §2. Legacy 08/11+vocabolario intoccati. Tappa V47 [~] ∞. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. 0 CP chiudibili.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI (review/prove mancanti). DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | **D20 bloccante (REGOLA BLOCCATA §2):** 5 hunk `drawPlayerCar` (`renderer.cpp:2798-2816,2875-2889,2890-2905,2910-2922,2932-2952`); serve autorizzazione Andrea o revert. Dev 27/11 09:20 V9 lv_06_desktop g44: no mod codice; percorso exit 0 (2125f,35.4s,0% fuori); smoke exit 0 medio 0.671 max 4.006ms frame 0, replay IDENTICO hash 3627137594 → V9 lv_01_neon g45. Jr 27/11 09:21 VJ5: src invariato HEAD dad983b; rilievo `paesaggio.cpp:260-262` disponiTerraPonte hoist conteggi O(n) (no REGOLA BLOCCATA, resa/replay invariati), smoke lv_04 exit 0; VJ3 lv_05 50/60Hz FAIL 2.40% deterministico aperto; VJ4 6/6 chiuso. Gf 27/11 09:24 g257: 0 conv, exit 0 (3 tool+autotest 43/42/37). |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; D20 richiede verifica jr dopo revert/autorizzazione. Nessun CP chiudibile. |
| Build/test | Handoff dev 27/11 09:20, jr 27/11 09:21, gf 27/11 09:24. Capo non riesegue. Gate T1 riferimento 25/11 13:04 exit 0. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (27/11 09:24, g257)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (27/11 09:20, V9 lv_06 g44, no mod codice)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (27/11 09:21, VJ5 paesaggio)
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (riferimento storico 18/11; handoff ruolo sopra più recenti)

## Prossime azioni
1. @developer: **DEFAULT D20** — revert o autorizzazione Andrea registrata per 5 hunk drawPlayerCar; `run_tests.sh` exit 0, smoke+percorso replay IDENTICO. Poi V9 ∞ lv_01_neon g45; indagare VJ3 lv_05 50/60Hz diff 2.40%. Reviser: @developer-junior.
2. @developer-junior: verificare risoluzione D20 quando presente; indagare/ripetere VJ3 lv_05 con log; VJ5 ∞ prossimo diff (paesaggio già segnalato); valutare portabilità gate pali. `run_tests.sh` prima prova; file/riga in `REVIEW_V47.md`; no src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT: `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA. Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; nessuna chiusura CP senza gate+review indipendenti in dossier. Monitorare D20 e VJ3 lv_05.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.
