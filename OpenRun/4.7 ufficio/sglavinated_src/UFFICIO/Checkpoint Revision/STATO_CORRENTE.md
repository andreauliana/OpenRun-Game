# Stato corrente — OpenRun 4.7

> Scheda di avvio. Aggiornata: 2026-11-25 (T3 ora 5/12, @capo) — fonti richieste rilette. ANDREA_REVIEW prec. ass., nessuna RISPOSTA DI ANDREA; G1 ANNULLATO/sponsor solo Andrea; legacy 08/11+vocabolario intoccati; decisioni aperte 0. Tappa MIG1+V47 [~] ∞. CHK auto SCAMBIO_RECENTE [APERTE]=non peer review. Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti registrati → nessuna chiusura.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). |
| Stato attività | Dev V9 lv_05_spray giro38b fatto (25/11 12:36, pre/post percorso+smoke exit 0, replay IDENTICO 4250821778, 0.590/2.960ms <8ms, lv_02 invariante 2961894142; gate_t1 ref 24/11 10:03 exit 0) → **lv_06_desktop giro38b**. Jr VJ4 soak lv_04_cromo fatto (25/11 12:45, run_tests 27/27 exit 0, soak exit 0 30000f replay IDENTICO, 0.660/2.757ms <20) → VJ4 soak lv_05_spray poi lv_06; VJ5 ∞ review hunk renderer.cpp/renderer.h 12:41:39 (nit non bloccante skylineLut_). Gf VG3 giro 183 (24/11 13:20): 0 conversioni; vg3_date exit 1 (solo flag Andrea), vg3_raw_senza_catalogo exit 0, vg3_png_non_catalogati exit 0, autotest 18 OK. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev 25/11 12:36, jr 25/11 12:45, gf 24/11 13:20 (capo non riesegue). STATUS_OFFICINA arretrato (18/11); usare handoff dev/jr. Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (24/11 13:20, giro 183)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (25/11 12:36, V9 lv_05 giro38b→lv_06 giro38b)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (25/11 12:45, VJ4 soak lv_04→lv_05/lv_06 + VJ5 ∞), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (arretrato; usare handoff dev/jr per ultimi exit)

## Prossime azioni
1. @developer: V9 ∞ lv_06_desktop giro38b (percorso/smoke, replay IDENTICO, exit 0, ms; se >8ms Mac alleggerire senza cambiare resa/replay). Reviser: @developer-junior.
2. @developer-junior: VJ4 soak lv_05_spray poi lv_06_desktop (run_tests prima, soak exit 0 replay IDENTICO ms/frame) e VJ5 ∞ review prossimo diff src (file/riga in `REVIEW_V47.md`); no src. Reviser: @developer se fix / @capo verifica.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco area Andrea. DEFAULT Andrea: `traffico_incrocio/lato_furgone_bianco.png.png` (no rinomina/converti senza RISPOSTA). Reviser: @capo verifica catalogo/licenza/provenienza.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.
