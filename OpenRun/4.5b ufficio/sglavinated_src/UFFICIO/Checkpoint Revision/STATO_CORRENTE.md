# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-10 (Turno 1, ora 1 di 12, @capo) — verificate TAPPE/ANDREA_REVIEW (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1 attivo, V47 continuo)/SCAMBIO_RECENTE + checkpoint/handoff. Gate T1 green riportato (handoff). Nessun CP chiudibile: review incrociate MIG1/V47 non registrate in dossier. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md) (append-only), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (solo aggiunte). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO** (sponsor solo Andrea). D1,D3,D5,G2,J1,J2 [x] handoff; **D2 [~]** `sportellata` exit 28 (junior 10/11, `REVIEW_MIG1.md`: trigger/clamp in `motore_nuovo/src/game/game.cpp`, sole costanti aperte MIG1); **D4 [~]** chiarire allineamento nomi/origine/tema vs `NOMI_RIVALI`; **J3/J4 [~]** post-fix D2 rieseguire `sportellata` (exit 0 + recupero ~5 s misurabile) e aggiornare `REVIEW_MIG1.md`. **V47:** VG1,G2,V1-V4,V5 (frasi EN linea1/linea2 + testi `fasi.cpp` per handoff dev, VJ1 su V5 exit 0) [x] handoff; VJ2 [~] (`vj2_riva` exit 0, densità ispezione Andrea, **DEFAULT non blocca**). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). MIG1/V47: review incrociate non registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate T1 green riportato (handoff dev/junior). Log/screenshot in `UFFICIO/Officina/Test area/`. Riportare comando, exit code, misure. Pi 400 non verificato da gate. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (G2 [x] 10/11; nessun passo aperto; in attesa).
- Developer: [RIASSUNTO.md](../bacheche/developer/RIASSUNTO.md), [ULTIMO_TURNO.md](../out/developer/ULTIMO_TURNO.md) (D2 fix mirato + D4 chiarire, prove).
- Developer-junior: [RIASSUNTO.md](../bacheche/developer-junior/RIASSUNTO.md), [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (J3/J4 [~] post-fix D2, review incrociate).

## Prossime azioni
1. **@developer** (`UFFICIO/out/developer/ULTIMO_TURNO.md`): fix mirato D2 in `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`, trigger/clamp sterzo 60 Hz). Rieseguire `sportellata` exit 0, ctest verde, `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO. Chiarire/allineare D4 (nomi max 6 MAIUSC., origine/tema).
2. **@developer-junior** (`UFFICIO/out/developer-junior/`): post-fix D2 rieseguire J3 (`sportellata` exit 0 + recupero ~5 s misurabile), aggiornare J4 `REVIEW_MIG1.md`; registrare review incrociate MIG1/V47 in `REVIEW_MIG1.md`,`REVIEW_V47.md` (cmd/exit/misure, autore≠revisore).
3. **@grafico** (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`): in attesa, nessun passo [ ]/[~] aperto (no sponsor/asset Andrea/HUD/UI/auto/logo/menu). Lacuna screenshot D3 rivali segnalata da arbitrare se richiesta.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrati in dossier.
5. Vietati: base legacy, costanti fuori MIG1, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
