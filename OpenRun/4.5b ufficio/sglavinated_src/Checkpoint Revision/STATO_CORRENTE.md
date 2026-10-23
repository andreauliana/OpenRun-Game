# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-10-23 (Turno 1, ora 0 di 12). Fonte dettagliata: [CP-005](./CP-005-tracciati-percorsi.md) [CHIUSO], `out/capo/STATI_PROPOSTI.md` (riscritto Turno 3), `ANDREA_REVIEW.md`, `UFFICIO/Officina/STATUS_OFFICINA.md`, `out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-005](./CP-005-tracciati-percorsi.md) — blocco R1 tracciati per le 6 piste, **CHIUSO** 2026-10-22 dal @capo: review incrociata indipendente completa positiva, ctest 17/17 verde (`sh UFFICIO/Officina/TOOL/run_tests.sh`, exit 0, log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`), gate soak verde registrato (6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame <= 20); [CP-006](./CP-006-t1-run-chiusura.md) — chiusura T1-RUN, **PRONTO PER ANDREA** 2026-10-23: gate soak verde registrato, prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok), decisione Andrea in attesa su D19 (DEFAULT A applicato, si procede senza aspettare). |
| Stato attività | Tappa 1, Gate T1-RUN [x] proposto (DEFAULT D19=A): gate verde (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10), CP-005 chiusa con review incrociata indipendente positiva (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA; log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`), prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok). CP-006 aperta come PRONTO PER ANDREA in attesa di decisione Andrea su D19 (risposta mancante, DEFAULT A applicato, si procede senza aspettare). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A). R1 [x] (CP-005 chiuso). G1 [~] check [x], CP-002 in attesa review indipendente (D15 = A, T1-Q4). F60 [x] gate, [~] review incrociata developer→developer-junior pendente (CP-006 è per T1-RUN, nessun CP F60 ancora aperto). Dopo R1, prima di R2 (Andrea 19 ott). H [~] in attesa misure Pi 400 (prova Mac 23/10 registrata; D12 = A). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Dal 2026-10-19 `assets_originali/` e' un symlink a `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` (un solo originale, nella Libreria) |
| Ruoli | @capo, @developer, @developer-junior, @grafico |
| Review | Peer review tra i due sviluppatori; autore distinto dal revisore. CP-005 chiuso: review incrociata registrata positiva su entrambi i lati. |
| Build/test | Ultimo gate 21/10 ~09:00 (e 20/10 14:53, 19:23) exit 0, `GATE T1: TUTTO OK` (log `UFFICIO/Officina/Test area/gate_t1.log`). ctest rieseguito 2026-10-22: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, 17/17 passati (log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`, report `out/developer/ESITO_CTEST_CP005_22OTT_0940.md`). Build CMake in `UFFICIO/Officina/build/`. Runner SDL2 compila; PNG disabilitato (SDL2_image non presente). |
| Limite della prova | Eseguito su macOS; prestazioni e compatibilità su Raspberry Pi 400 non verificate da questo gate |

## Handoff grafico

Leggere [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) come unico riepilogo aggiornato del @grafico. Inventario dettagliato: [CATALOGO_RISORSE.md](../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md). Catalogo: 52 voci (30 approvate, 16 pronte, 2 in costruzione, 4 mancanti); 100 PNG non catalogati da classificare. Mancano asset approvati per personaggi/concorrenti e i 3 finali animati.

## Handoff Officina

Leggere [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) per l'ultimo report di sviluppo, build, test e blocchi. Nel checkpoint riportare soltanto esito, misure e riferimenti ai log pertinenti; lo status non sostituisce la peer review.

## Prossime azioni

1. Prova runner Mac Andrea 23/10 eseguita: runner ok, ctest 17/17 (da registrare formalmente). Decisione Andrea su T1-RUN [x] e avvio R2 in attesa (D19 [APERTA], DEFAULT A). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A).
2. T1-RUN [x] proposto (gate + review CP-005 + prova Mac 23/10 ok). Dopo decisione Andrea: avvio R2 (D18 DEFAULT A), apertura CP-006 per R2.
3. F60 gate verde registrato 22/10 20:06 (60/50 Hz OK, replay 2x IDENTICO, 6/6 percorsi OK); manca solo review incrociata developer→developer-junior e eventuale CP-006. Poi R2 dopo ok Andrea Mac.
4. G1 @grafico: catalogo 100 PNG, `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0" (handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` consultato).

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
