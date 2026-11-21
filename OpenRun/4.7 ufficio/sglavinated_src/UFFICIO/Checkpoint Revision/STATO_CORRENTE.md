# Stato corrente — OpenRun 4.7

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-21 (Turno 1, ora 0 di 12, @capo) — lette TAPPE, ANDREA_REVIEW (**precedenza assoluta**, nessuna `RISPOSTA DI ANDREA` nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 e vocabolario intoccati; decisioni aperte: nessuna), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA (MIG1+V47 ∞), SCAMBIO_RECENTE, checkpoint/handoff. Tappa attiva: MIG1+V47 [~]. SCAMBIO_RECENTE CHK auto [APERTE] (controlli verdi, non peer review). Gate non rieseguito dal capo. Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. STATI_PROPOSTI/STATO_CORRENTE/REGISTRO/RIASSUNTO/diario aggiornati.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (passi ∞). Base legacy 08/11 e vocabolario **NON si toccano**. |
| Ultimo checkpoint | CP-006 CHIUSO (non riaprire). CP-005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier non in cartella). |
| Stato attività | V9 ∞ @developer: lv_04_cromo 27° fatto (21/11 11:08, exit 0, replay IDENTICO, gate_t1 6/6) → prossimo **lv_05_spray 27°**. VJ3 [~] 4/6, VJ4 [x] 6/6, VJ5 [~] prossimo diff src (21/11 11:13 shiftSchiuma ok). VG3 [~] giro 133, 0 da convertire; blocco `lato_furgone_bianco.png.png` → **DEFAULT invariato**. |
| Sorgenti attivi | `motore_nuovo/`, `GRAFICA/`, `UFFICIO/Officina/TOOL/`. Scrittura solo `UFFICIO/` e `motore_nuovo/` se richiesto. |
| Ruoli | @capo,@developer,@developer-junior,@grafico (AGENTS.md). |
| Review | Peer review dev↔jr; non tutte consolidate in dossier → nessun CP chiudibile ora. |
| Build/test | Handoff dev/jr 21/11 in `UFFICIO/out/*/ULTIMO_TURNO.md`; STATUS_OFFICINA riga aggiornata; gate non rieseguito dal capo; Pi 400 non verificato. |
| Limite prova | macOS; Pi 400 non verificato. |

## Handoff
- Grafico: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (21/11 11:17, giro 133)
- Developer: `UFFICIO/out/developer/ULTIMO_TURNO.md` (21/11 11:08, V9 lv_04 27°→lv_05)
- Developer-junior: `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (21/11 11:13, VJ5 shiftSchiuma), `REVIEW_V47.md`
- Officina: `UFFICIO/Officina/STATUS_OFFICINA.md` (riga dev/jr 21/11; storico parziale)

## Prossime azioni
1. @developer: V9 ∞ lv_05_spray 27° (percorso/smoke, replay IDENTICO, exit0, ms; se >8ms alleggerire senza cambiare resa).
2. @developer-junior: VJ5 ∞ su prossimo diff src (ricompila prima); VJ3 4/6 se utile; prove solo test/tool assegnati, no src.
3. @grafico: VG3 ∞ PNG vs .sglraw, converti solo se PNG più nuovo via script catalogo, nessun ritocco; blocco furgone_bianco.png.png DEFAULT invariato.
4. @capo: append-only REGISTRO; chiudi CP solo con gate+review indipendenti in dossier. Nessuna chiusura ora.

Cronologia: `REGISTRO_REVISIONI.md`. Template: `TEMPLATE_CHECKPOINT.md`.