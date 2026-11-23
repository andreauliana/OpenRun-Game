# Stato corrente — OpenRun 4.7

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-23 (Turno 3, ora 4 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (precedenza assoluta, nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; legacy/vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA (MIG1+V47 ∞), SCAMBIO_RECENTE, checkpoint/handoff. Tappa attiva MIG1+V47 [~]. SCAMBIO_RECENTE CHK auto [APERTE] (non peer review). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti registrati → nessuna chiusura.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (non riaprire). CP-005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier non in cartella). |
| Stato attività | Handoff aggiornati: Dev V9 lv_05_spray 33° fatto (23/11 10:06, gate_t1 exit 0 6/6, replay IDENTICO, hash 4250821778) → prossimo **lv_06_desktop 33°**. Jr VJ5 ∞ review imbocco galleria (23/11 10:38, run_tests exit 0 27/27, prova_gate lv_02 percorso exit 0) → VJ5 ∞ prossimo diff src. Gf VG3 giro 167 (23/11 10:01), 0 da convertire; blocco Andrea DEFAULT: `lato_furgone_bianco.png.png`. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev 23/11 10:06 (gate_t1 exit 0 6/6), jr 23/11 10:38 (run_tests exit 0 27/27, prova_gate lv_02 exit 0); capo non riesegue; Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (23/11 10:01, giro 167)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (23/11 10:06, V9 lv_05 33°→lv_06 33°)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (23/11 10:38, VJ5 ∞ imbocco galleria), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (usare handoff dev/jr per ultimi exit)

## Prossime azioni
1. @developer: V9 ∞ lv_06_desktop 33° (percorso/smoke, replay IDENTICO, exit 0, ms; se >8ms Mac alleggerire senza cambiare resa/replay).
2. @developer-junior: VJ5 ∞ review prossimo diff src (ricompila prima, `run_tests.sh`); no modifica src.
3. @grafico: VG3 ∞ PNG vs `.sglraw`, converti solo se PNG più nuovo via script catalogo, nessun ritocco; blocchi Andrea DEFAULT invariati.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.
