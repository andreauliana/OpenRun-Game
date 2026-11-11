# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-11 (Turno 3, ora 5 di 12, @capo) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**, nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1 attivo, V47 continuo, passi ∞)/SCAMBIO_RECENTE + checkpoint/handoff. Tappa attiva: MIG1+V47. Gate green riportato (Officina 11/11: ctest 27/27 exit 0, `gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 PERCORSO OK, replay IDENTICO; picco 55.27 ms lv_05 smoke isolato da valutare; log `UFFICIO/Officina/Test area/gate_t1.log`). Nessun CP chiudibile (review indipendenti autore≠revisore non tutte registrate). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1,D3,D5,G2,J1,J2 [x] handoff; **D2 [x] D4 [x] J3 [x] J4 [x]** (Claude 11/11: `sportellata` exit 0 sul main, l'exit 28 era vecchio; MIG1 chiuso, costanti di guida NON si toccano). **V47:** V1-V5,VJ1,VJ3 [x] handoff; **V6 [~]** handoff dev con gate/ctest verdi ma review junior parziale non consolidata (picco 55.27 ms lv_05 smoke isolato da valutare); **V7/V8 [~]/da fare**; **V9 ∞ [~]** dev 11/11 12:54 lv_01 exit 0 max 3.420 ms < 8, prossimo lv_02; **VJ4 [~]** soak 4/6 (lv_01-04 exit 0, 30000f IDENTICO) — restano lv_05-06; **VJ5 ∞ [~]** junior 11/11 12:55 review `salvataggi.cpp/.h`, caricamento exit 0; **VG2 [~]** giro 17 completato 11/11 11:47 (`verifica_frasi_vg2.py` exit 0, 156/156) — prossimo giro 18 o ∞ VG3; VJ2 [~] (`vj2_riva` exit 0, densità ispezione Andrea, **DEFAULT non blocca**). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate green riportato (Officina 11/11: exit 0; picco isolato max 55.27 ms lv_05 da valutare). Log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG2 giro 17 [~], verifica exit 0 11/11 11:47, 156/156).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO.md](../out/developer/ULTIMO_TURNO.md) (V9 lv_01 exit 0; V6 handoff, D2/D4 residui).
- Developer-junior: [RIASSUNTO.md](../bacheche/developer-junior/RIASSUNTO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak 4/6, VJ5 salvataggi 12:55, review V6 parziale).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ fluidita' sul livello successivo (gate `percorso`), V7/V8 solo se resta qualcosa. D2 e' chiuso: non toccare `AFFIANCATA_DX`/`URTO_SPINTA_RIVALE`/`RIVALE_SORPASSO_DX` (Claude 11/11).
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ4 soak lv_05_spray poi lv_06_desktop (exit 0, 30000f IDENTICO, ms/frame max); consolidare review V6 in `REVIEW_V47.md` (autore≠revisore, misure); VJ5 ∞ su altro file `src/`.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): VG2 CHIUSO (Claude 11/11): solo ∞ VG3 (PNG scenari più nuovi dei `.sglraw` → convertire senza ritocchi); `verifica_frasi_vg2.py` exit 0; no sponsor/asset Andrea.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti fuori MIG1, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).