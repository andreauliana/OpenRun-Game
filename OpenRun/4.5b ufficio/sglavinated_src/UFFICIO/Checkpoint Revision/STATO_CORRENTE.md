# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-09 (Turno 1, ora 1 di 12 @capo). Fonti: `TAPPE.md`, `ANDREA_REVIEW.md` (precedenza assoluta), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md`, `RICHIESTA_APERTA.md` (MIG1 attivo, V47 continuo), `SCAMBIO_RECENTE.md`, [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO, non riaprire], `REGISTRO_REVISIONI.md` (append-only), `UFFICIO/out/capo/STATI_PROPOSTI.md`, handoff Grafico + RIASSUNTI developer/developer-junior.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~]** + **V47 [~] continuo** (solo aggiunte). Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW sez. 2-3). |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI (review indipendente). |
| Stato attività | **MIG1:** G1 **ANNULLATO** (sponsor solo Andrea). D1,D3,D5,G2,J1,J2 [x]; **D2 [~] da correggere** (`sportellata` exit 28 junior 12:53, `playerBump_`/src/game fuori perimetro junior → @developer); **D4 [~]** discrepanza; **J3 [~]** bloccata da D2; J4 aggiornare dopo. **V47:** VG1,V1-V4,VJ1,VJ2 [x]; V5 codice/UI [x] parziale; **V5 frasi [~]** `motore_nuovo/dati/frasi.json` fuori perimetro dev → @grafico. Prove dev 10:34: ctest 27/27, gate_t1 exit 0, 6/6 IDENTICI, ms/frame ≤ 20 (macOS). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/` (symlink Libreria), `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto. |
| Ruoli | @capo, @developer, @developer-junior, @grafico (AGENTS.md 4.6). |
| Review | Peer review tra sviluppatori (autore ≠ revisore). MIG1/V47: review incrociate non registrate in dossier → nessun CP chiudibile ora. |
| Build/test | Log/screenshot in `UFFICIO/Officina/Test area/`. Riportare comando, exit code, misure. Pi 400 non verificato da gate. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (G1 annullato, G2 [x] prove viste 12:31, VG1 [x]; **prossimo V5 frasi**).
- Developer: `UFFICIO/bacheche/developer/RIASSUNTO.md` (V5 codice/UI [x] parziale, **blocco residuo `frasi.json` @grafico**; nessun prossimo dev finché V5 frasi non instradato).
- Developer-junior: `UFFICIO/bacheche/developer-junior/RIASSUNTO.md` (**J3 [~]** `sportellata` exit 28, attende fix D2 @developer; VJ1/VJ2 [x]).

## Prossime azioni
1. **@developer**: fix mirato D2 in `src/game/game.cpp` (solo MIG1), poi rieseguire build/test; chiarire D4 in `UFFICIO/out/developer/ULTIMO_TURNO.md`. Dopo ogni passo: ctest verde + `gate_t1.sh` exit 0, replay 2x IDENTICO.
2. **@developer-junior**: dopo fix D2 rieseguire `sportellata` (exit 0 + recupero misurato/registrato); review incrociata MIG1/V47; aggiornare `REVIEW_MIG1.md`, `REVIEW_V47.md`.
3. **@grafico**: V5 frasi — `motore_nuovo/dati/frasi.json` in inglese semplice, struttura preservata, CC0-1.0 progetto, no Sega/marchi veri; no sponsor/PNG Andrea/HUD/UI extra.
4. **@capo**: registro append-only per turno; registra review incrociate in dossier/CP solo con gate+review indipendenti completi.
5. Vietati: base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea/sponsor). Costanti aperte solo MIG1. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
