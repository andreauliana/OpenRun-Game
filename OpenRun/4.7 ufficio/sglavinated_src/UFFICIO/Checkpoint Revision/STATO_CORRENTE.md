# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-26 (T1 ora 1/12, @capo) — fonti rilette. ANDREA_REVIEW prec. ass., §7 nessuna decisione aperta; D20 blocco procedurale drawPlayerCar (REGOLA BLOCCATA §2, non registrata in §7, capo non può scrivere ANDREA_REVIEW). Legacy 08/11+vocabolario intoccati. Tappa V47 [~] ∞. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. 0 CP chiudibili.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | **D20 aperto**: jr VJ5 segnala hunk drawPlayerCar :2628-2642/:2715-2727 ancora bloccati (REGOLA BLOCCATA). Dev 26/11 giro39 lv_05_spray drawGalleria/fronte collina exit 0, replay IDENTICO 4250821778, medio 0.679 max 9.828ms, percorso 2920f/48.7s/0% fuori, regressione lv_02 hash invariato. Jr 26/11 08:29 VJ3 6/6: lv_05 50/60Hz **FAIL 2.40%** deterministico (48.67/47.50s), altri 5 OK; VJ5 drawImboccoGalleria ok; run_tests 27/27 exit 0. Gf 26/11 08:22 g207: 0 conv, script exit 0, autotest 26 OK. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; VJ5 segnala D20 drawPlayerCar + VJ3 lv_05 FAIL. Nessun CP chiudibile. |
| Build/test | Handoff dev 26/11, jr 26/11 08:29 (run_tests 27/27 exit 0), gf 26/11 08:22. Capo non riesegue. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (26/11 08:22, g207)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (26/11, V9 lv_05_spray giro39; D20 drawPlayerCar ancora aperto)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (26/11 08:29, VJ3 lv_05 FAIL 2.40%, VJ5 D20 aperto), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (arretrato; usare handoff dev/jr)

## Prossime azioni
1. @developer: **DEFAULT D20** — revert solo hunk drawPlayerCar bloccati (:2628-2642/:2715-2727 segnalati da jr), tenere drawDetrito; run_tests exit 0, smoke/percorso lv_02 replay IDENTICO. Poi V9 ∞ lv_06_desktop; indagare VJ3 lv_05 50/60Hz diff 2.40% (gioco vs autopilota sogliaX). Reviser: @developer-junior.
2. @developer-junior: verificare revert D20 quando presente; indagare/ripetere VJ3 lv_05 con log; VJ5 ∞ prossimo diff. `run_tests.sh` prima prova; file/riga in `REVIEW_V47.md`; no src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT: `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA. Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; nessuna chiusura CP senza gate+review indipendenti in dossier. Monitorare D20 e VJ3 lv_05.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.