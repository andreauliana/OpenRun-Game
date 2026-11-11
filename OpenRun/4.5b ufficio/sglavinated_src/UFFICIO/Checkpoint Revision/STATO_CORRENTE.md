# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-11 (Turno 1, ora 1 di 12, @capo) — lette TAPPE/ANDREA_REVIEW (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1 attivo, V47 continuo, passi ∞)/SCAMBIO_RECENTE + checkpoint/handoff. Gate T1 green riportato (SCAMBIO_RECENTE 10/11 09:46; Officina 11/11 ctest 27/27 exit 0, gate_t1 exit 0). Nessun CP chiudibile: review incrociate MIG1/V47/V6 non registrate in dossier. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md) (append-only), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (solo aggiunte, passi ∞). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO** (sponsor solo Andrea). D1,D3,D5,G2,J1,J2 [x] handoff; **D2 [~]** `sportellata` exit 28 (junior 10/11, `REVIEW_MIG1.md`: trigger/clamp in `motore_nuovo/src/game/game.cpp`, sole costanti aperte MIG1); **D4 [~]** chiarire allineamento nomi/origine/tema vs `NOMI_RIVALI`; **J3/J4 [~]** post-fix D2 rieseguire `sportellata` (exit 0 + recupero ~5 s misurabile) e aggiornare `REVIEW_MIG1.md`. **V47:** V1-V5,VJ1 [x] handoff; **V6 [~] handoff @developer** (Officina 11/11: ctest 27/27 exit 0 ~08:05, `gate_t1.sh` exit 0 ~08:10, `GATE T1: TUTTO OK`, 6/6 GATE PERCORSO OK, replay IDENTICO; nota: ms/frame max 55.27 picco isolato lv_05 smoke, media ~0.97-1.43, log `UFFICIO/Officina/Test area/gate_t1.log`) — review indipendente @developer-junior da registrare; **VG2 [~]** giro 2 @grafico (20 frasi, verifica `verifica_frasi_vg2.py` exit 0 11/11; giro 3 resta); VJ2 [~] (`vj2_riva` exit 0, densità ispezione Andrea, **DEFAULT non blocca**); V7/V8/VJ3/VJ4 e passi ∞ da proseguire. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). Review incrociate MIG1/V47/V6 non registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate T1 green riportato (SCAMBIO_RECENTE 10/11 09:46; Officina 11/11 ~08:10: exit 0, picco isolato max 55.27 lv_05 smoke da valutare in review). Log/screenshot in `UFFICIO/Officina/Test area/`. Pi 400 non verificato da gate. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (VG2 giro 2 [~], verifica exit 0 11/11).
- Developer/Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md), [ULTIMO_TURNO.md](../out/developer/ULTIMO_TURNO.md) (V6 handoff, D2/D4 residui).
- Developer-junior: [RIASSUNTO.md](../bacheche/developer-junior/RIASSUNTO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): D2 fix mirato in `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, trigger/clamp sterzo 60 Hz) → `sportellata` exit 0, ctest verde, `gate_t1.sh` exit 0, replay 2x IDENTICO; chiarire/allineare D4. Poi proseguire V47 (V7 o passo ∞) senza toccare legacy.
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): post-fix D2 rieseguire J3 (`sportellata` exit 0 + recupero ~5 s misurabile) e aggiornare J4 `REVIEW_MIG1.md`; review indipendente V6 (cmd/exit/misure, autore≠revisore) in `REVIEW_V47.md`; proseguire VJ3/VJ4/∞.
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): VG2 giro 3 (20 frasi, rilettura punteggiatura/spezzature) o passo ∞ VG3; no sponsor/asset Andrea.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrati in dossier. Nessuna chiusura CP ora.
5. Vietati: base legacy, costanti fuori MIG1, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).