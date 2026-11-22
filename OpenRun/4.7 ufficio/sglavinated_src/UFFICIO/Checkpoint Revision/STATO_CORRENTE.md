# Stato corrente — OpenRun 4.7

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-22 (Turno 2, ora 3 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (precedenza assoluta, nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; legacy/vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA (MIG1+V47 ∞), SCAMBIO_RECENTE, checkpoint/handoff. Tappa attiva MIG1+V47 [~]. SCAMBIO_RECENTE CHK auto [APERTE] (non peer review). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti registrati → nessuna chiusura. STATI_PROPOSTI.md riscritto.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (non riaprire). CP-005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier non in cartella). |
| Stato attività | V9 ∞ @developer: lv_06_desktop 30° fatto (22/11 11:47, exit 0, replay IDENTICO frameHash 3627137594, gate_t1 6/6) → prossimo **lv_01_neon 31°**. VJ3 [~] 4/6, VJ4 [x] 6/6, VJ5 [~] review luceEsterna/soffittoRiga ok (22/11 11:52; run_tests 26/27 fail preesistente gate_decorazioni skyline lv_04 mancante; prova_gate lv_02 percorso exit 0). VG3 [~] giro 144, 0 da convertire; blocchi Andrea DEFAULT invariati (skyline lv_04 mancante/raw ok, `lato_furgone_bianco.png.png`). |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non tutte consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev 22/11 11:47, jr 22/11 11:52, grafico 22/11 09:56; gate_t1 exit 0 6/6 dichiarato da dev; jr run_tests exit 8 (26/27, preesistente skyline lv_04). Capo non riesegue; Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (22/11 09:56, giro 144)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (22/11 11:47, V9 lv_06 30°→lv_01 31°)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (22/11 11:52, VJ5 luceEsterna), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (parziale/storico; usare handoff dev/jr per ultimi exit)

## Prossime azioni
1. @developer: V9 ∞ lv_01_neon 31° (percorso/smoke, replay IDENTICO, exit 0, ms; se >8ms alleggerire senza cambiare resa/replay).
2. @developer-junior: VJ5 ∞ su prossimo diff src (ricompila prima); VJ3 4/6 se utile; prove solo test/tool assegnati, no src.
3. @grafico: VG3 ∞ PNG vs .sglraw, converti solo se PNG più nuovo via script catalogo, nessun ritocco; blocchi Andrea DEFAULT invariati.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.