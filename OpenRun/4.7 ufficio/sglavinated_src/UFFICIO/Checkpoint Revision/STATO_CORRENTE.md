# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-26 (T5 ora 9/12, @capo) — fonti rilette. ANDREA_REVIEW prec. ass., §7 nessuna RISPOSTA/decisione aperta; REGOLA BLOCCATA drawPlayerCar §2. Legacy 08/11+vocabolario intoccati. Tappa V47 [~] ∞. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. 0 CP chiudibili.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | **D20 bloccante (REGOLA BLOCCATA §2):** 5 hunk in `drawPlayerCar` ancora presenti (`renderer.cpp:2798-2816,:2875-2889,:2890-2905,:2910-2922,:2932-2952`); jr VJ5 26/11 16:52 conferma diff `src/` pulito (HEAD c146cb5), nessun cambio nuovo, D20 invariato. Serve autorizzazione Andrea o revert. Dev V9 26/11 16:36 lv_04_cromo giro40: nessuna mod codice; percorso exit 0 (2125f,35.4s,0% fuori), smoke exit 0 medio 0.621 max 2.762ms, replay IDENTICO 1415607011. Jr 16:52: run_tests 27/27 exit 0; prova_gate lv_05_spray smoke exit 0 (offroad frame156, IDENTICO, 0.584/2.758ms); VJ1 lv_04 smoke exit 0 (hash identico dev). **VJ3 lv_05 50/60Hz FAIL 2.40%** deterministico resta aperto; altri 5 OK. VJ4 6/6 chiuso. Gf 26/11 09:27 g212: 0 conv, exit 0. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; D20 richiede verifica jr dopo revert/autorizzazione. Nessun CP chiudibile. |
| Build/test | Handoff dev 26/11 16:36, jr 26/11 16:52, gf 26/11 09:27. Capo non riesegue. Gate T1 riferimento 25/11 13:04 exit 0. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (26/11 09:27, g212)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (26/11 16:36, V9 lv_04 giro40, no mod codice)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (26/11 16:52, VJ5/VJ1, D20 invariato; VJ3 lv05 FAIL), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (aggiornare a cura dev/jr se serve; usare handoff dev/jr)

## Prossime azioni
1. @developer: **DEFAULT D20** — revert o autorizzazione Andrea registrata per 5 hunk drawPlayerCar bloccati; `run_tests.sh` exit 0, smoke+percorso lv_02 replay IDENTICO. Poi V9 ∞ lv_05_spray; indagare VJ3 lv_05 50/60Hz diff 2.40%. Reviser: @developer-junior.
2. @developer-junior: verificare risoluzione D20 quando presente; indagare/ripetere VJ3 lv_05 con log; VJ5 ∞ prossimo diff. `run_tests.sh` prima prova; file/riga in `REVIEW_V47.md`; no src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT: `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA. Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; nessuna chiusura CP senza gate+review indipendenti in dossier. Monitorare D20 e VJ3 lv_05.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.