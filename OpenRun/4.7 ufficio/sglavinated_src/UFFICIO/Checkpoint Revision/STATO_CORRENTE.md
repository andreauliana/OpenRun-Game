# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-24 (Turno 2, ora 3 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (precedenza assoluta, nessuna RISPOSTA DI ANDREA; G1 ANNULLATO/sponsor solo Andrea; legacy 08/11+vocabolario intoccati; decisioni aperte 0), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA, SCAMBIO_RECENTE, checkpoint/handoff. Tappa MIG1+V47 [~]. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti registrati → nessuna chiusura.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | Dev V9 lv_06_desktop 35° fatto (24/11 12:52, smoke/percorso pre/post exit 0, replay IDENTICO frameHash 3627137594, medio 0.675 max 2.961 ms; gate_t1 ref 10:03 exit 0) → **lv_01_neon 36°**. Jr VJ5 ∞ review roadside-markers (24/11 12:55, run_tests 27/27 exit 0, prova_gate lv_01 percorso exit 0, no src) → VJ5 ∞ prossimo diff src. Gf VG3 giro 183 (24/11 13:20): 0 conversioni; vg3_date exit 1 (solo flag Andrea), vg3_raw_senza_catalogo exit 0, vg3_png_non_catalogati exit 0, autotest 18 OK. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev 24/11 12:52, jr 24/11 12:55, gf 24/11 13:20 (capo non riesegue). STATUS_OFFICINA arretrato (18/11); usare handoff dev/jr. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (24/11 13:20, giro 183)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (24/11 12:52, V9 lv_06 35°→lv_01 36°)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (24/11 12:55, VJ5 ∞ roadside-markers), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (arretrato; usare handoff dev/jr per ultimi exit)

## Prossime azioni
1. @developer: V9 ∞ lv_01_neon 36° (percorso/smoke, replay IDENTICO, exit 0, ms; se >8ms Mac alleggerire senza cambiare resa/replay). Reviser: @developer-junior.
2. @developer-junior: VJ5 ∞ review prossimo diff src (≠ roadside-markers; ricompila prima `run_tests.sh`); no modifica src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT Andrea: `traffico_incrocio/lato_furgone_bianco.png.png` (no rinomina/converti senza RISPOSTA). Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.