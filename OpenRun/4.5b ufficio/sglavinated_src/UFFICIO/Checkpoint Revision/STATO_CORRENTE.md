# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-10 (Turno 1, ora 0 di 12, @capo) — verificate TAPPE/ANDREA_REVIEW (**precedenza assoluta**; nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 intoccata)/DIREZIONE_ARTISTICA/CONTROLLO/AGENTS 4.6/RICHIESTA_APERTA (MIG1 attivo, V47 continuo)/SCAMBIO_RECENTE + checkpoint/handoff. Gate T1 green riportato (SCAMBIO_RECENTE/hand off: `gate_t1.sh` exit 0). Nessun CP chiudibile: review incrociate MIG1/V47 non registrate in dossier; D2 `sportellata` exit 28. Fonti: [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO, non riaprire**, [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md) (append-only), [STATI_PROPOSTI.md](../out/capo/STATI_PROPOSTI.md).

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (solo aggiunte). Base legacy 08/11 approvata e **NON si tocca**. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI. |
| Stato attività | **MIG1:** G1 **ANNULLATO** (sponsor solo Andrea). D1,D3,D5,G2,J1,J2 [x] handoff; **D2 [~]** `sportellata` exit 28 (10/11, junior): trigger `game.cpp:2386` `|dSterzo|>0.22` irraggiungibile al profilo 60 Hz + clamp `game.cpp:2390` vs `URTO_LARGHEZZA` — fix mirato in `motore_nuovo/src/game/game.cpp`, solo costanti MIG1 aperte; **D4 [~]** chiarire allineamento nomi/origine/tema in `UFFICIO/out/developer/ULTIMO_TURNO.md` vs `NOMI_RIVALI`; **J3 [~]** rieseguire `sportellata` post-fix; J4 aggiorna `REVIEW_MIG1.md`. **V47:** VG1,V1-V4,VJ1 [x] handoff; VJ2 [~] (PPM `vj2_riva` exit 0, densità non auto-valutabile — ispezione Andrea, DEFAULT: non blocca, proseguire); V5 codice/UI [x] parziale; **V5 frasi [~]** `motore_nuovo/dati/frasi.json` ancora IT → @grafico. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). MIG1/V47: review incrociate non registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Gate T1 green riportato 10/11 (junior: `gate_t1.sh` exit 0). Log/screenshot in `UFFICIO/Officina/Test area/`. Riportare comando, exit code, misure. Pi 400 non verificato da gate. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (G2 [x]; **prossimo V5 frasi**).
- Developer: `UFFICIO/bacheche/developer/RIASSUNTO.md`, [ULTIMO_TURNO.md](../out/developer/ULTIMO_TURNO.md) (**D2 fix mirato + D4 chiarire**).
- Developer-junior: `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, [REVIEW_MIG1.md](../out/developer-junior/REVIEW_MIG1.md), [REVIEW_V47.md](../out/developer-junior/REVIEW_V47.md) (**J3 [~]** exit 28).

## Prossime azioni
1. **@developer**: fix mirato D2 in `motore_nuovo/src/game/game.cpp` (solo `AFFIANCATA_DX`/`URTO_SPINTA_RIVALE`/`RIVALE_SORPASSO_DX` e trigger sportellata coerente col profilo sterzo 60 Hz; nessuna costante fuori MIG1). Rieseguire build + `sportellata`; poi ctest verde + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO. Chiarire D4 in `UFFICIO/out/developer/ULTIMO_TURNO.md`.
2. **@developer-junior**: dopo fix D2 rieseguire `sportellata` (exit 0 + recupero 5-8 s misurabile); review incrociate MIG1/V47; aggiornare `REVIEW_MIG1.md`, `REVIEW_V47.md` con cmd/exit/misure, autore≠revisore.
3. **@grafico**: V5 frasi — `motore_nuovo/dati/frasi.json` in inglese semplice, struttura/CC0-1.0 preservate, no Sega/marchi veri; no sponsor/asset Andrea/HUD/UI/auto/logo/menu extra.
4. **@capo**: registro append-only; aprire/chiudere CP solo con gate+review indipendenti registrati.
5. Vietati: base legacy, costanti fuori MIG1, sponsor/asset Andrea, `RICHIESTA.md`/`SCAMBIO.md`.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
