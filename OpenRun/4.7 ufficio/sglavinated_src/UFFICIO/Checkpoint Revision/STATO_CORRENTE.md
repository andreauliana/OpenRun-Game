# Stato corrente — OpenRun 4.7

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-21 (Turno 1, ora 0 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA (MIG1+V47 ∞), SCAMBIO_RECENTE, checkpoint/handoff. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHK auto [APERTE] (controlli verdi, non peer review). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. `STATI_PROPOSTI.md` riscritto in questo turno. Fonti: CP-006 CHIUSO, REGISTRO_REVISIONI, STATI_PROPOSTI.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (non riaprire). CP-005/007 CHIUSI. CP-002/003/004, DECO1, F60 APERTI. |
| Stato attività | V9 ∞ @developer: prossimo **lv_03_foglie 25°**. VJ3 [~] 4/6, VJ4 [x] 6/6, VJ5 [~] prossimo diff src. VG3 [~] confronto PNG/raw; blocco `lato_berlina_verde.sglraw` senza PNG → **DEFAULT invariato**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non tutte consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev/jr correnti in `UFFICIO/out/*/ULTIMO_TURNO.md`; `STATUS_OFFICINA.md` arretrato, non prova approvazione. Gate non rieseguito dal capo; Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md`
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`

## Prossime azioni
1. @developer: V9 ∞ lv_03_foglie 25° (percorso/smoke, replay IDENTICO, exit0, ms; se >8ms alleggerire senza cambiare resa).
2. @developer-junior: VJ5 ∞ su prossimo diff src (ricompila prima); VJ3 4/6 se utile; prove solo test/tool assegnati, no src.
3. @grafico: VG3 ∞ PNG vs .sglraw, converti solo se PNG più nuovo via script catalogo, nessun ritocco; blocco berlina verde DEFAULT invariato.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.
5. Vietati: legacy/vocabolario/costanti guida, sponsor/asset Andrea, RICHIESTA/SCAMBIO. Pi 400 no.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.
