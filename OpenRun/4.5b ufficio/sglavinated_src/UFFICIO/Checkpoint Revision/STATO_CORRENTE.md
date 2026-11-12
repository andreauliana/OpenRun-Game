# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-12 (Turno 1, ora 1 di 12, @capo, verifica) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**, nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1+V47 continuo, passi ∞)/SCAMBIO_RECENTE + checkpoint/handoff. Tappa attiva: MIG1+V47. Gate green riportato (Officina 11/11: ctest 27/27 exit 0, `gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 PERCORSO OK, replay IDENTICO; picco 55.27 ms lv_05 smoke isolato da valutare; log `UFFICIO/Officina/Test area/gate_t1.log`). Nessun CP chiudibile (review indipendenti autore≠revisore non registrate in dossier). STATI_PROPOSTI riscritto con deleghe 4.6 (developer V9 lv_02, developer-junior VJ4 lv_05/06+review V6+VJ5, grafico VG3 ∞). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] review** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO**. D1-D5,G2,J1-J4 **[x] handoff** (nota Claude 11/11: D2/D4/J3/J4 chiusi, `sportellata` exit 0 sul main, exit 28 vecchio) — **costanti di guida NON si toccano**; review incrociate non consolidate in dossier. **V47:** V1-V5,VJ1,VJ3 [x] handoff; **V6 [~]** handoff dev con gate/ctest verdi ma review junior parziale non consolidata (picco 55.27 ms lv_05 smoke isolato da valutare); **V7/V8 [~]/da fare**; **V9 ∞ [~]** dev 11/11 12:54 lv_01 exit 0 max 3.420 ms < 8, **prossimo lv_02**; **VJ4 [~]** soak 4/6 (lv_01-04 exit 0, 30000f IDENTICO) — restano lv_05-06; **VJ5 ∞ [~]** junior 11/11 12:55 review `salvataggi.cpp/.h`, caricamento exit 0; **VG2 [x] chiuso** (Claude 11/11, verifica exit 0 156/156) → **VG3 ∞ disponibile**; VJ2 [~] (`vj2_riva` exit 0, densità ispezione Andrea, **DEFAULT non blocca**). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate non tutte registrate → nessun CP chiudibile ora. |
| Build/test | Gate green riportato (Officina 11/11: exit 0; picco isolato max 55.27 ms lv_05 da valutare). Log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG2 chiuso, prossimo ∞ VG3).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO.md](../out/developer/ULTIMO_TURNO.md) (V9 lv_01 exit 0, prossimo lv_02; V6 handoff/review parziale).
- Developer-junior: [RIASSUNTO.md](../bacheche/developer-junior/RIASSUNTO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (VJ4 soak 4/6, VJ5 salvataggi 12:55).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): V9 ∞ fluidità lv_02_atomico (gate `percorso`/smoke, ms/frame), V7/V8 solo se resta qualcosa. MIG1 chiuso: **non toccare** `AFFIANCATA_DX`/`URTO_SPINTA_RIVALE`/`RIVALE_SORPASSO_DX` né costanti legacy.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): VJ4 soak lv_05_spray poi lv_06_desktop (exit 0, 30000f IDENTICO, ms/frame max); consolidare review V6 in `REVIEW_V47.md` (autore≠revisore, misure); VJ5 ∞ su altro file `src/` (prima ricompila).
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): ∞ VG3 (PNG scenari più nuovi dei `.sglraw` → convertire senza ritocchi, script catalogo); VG2 chiuso, no sponsor/asset Andrea.
4. **@capo** (`UFFICIO/Checkpoint Revision/`): registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrate in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy 08/11/vocabolario, costanti di guida, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`. Pi 400 non verificato.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).