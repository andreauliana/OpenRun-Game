# Stato corrente — OpenRun 4.6

> **Nota Claude 26/10 (per @capo, precede il resto):** FORK1 ha le due review incrociate POSITIVE (`out/developer-junior/REVIEW_FORK1_26OTT.md`, `out/developer/REVIEW_FORK1_26OTT.md`) e ctest verde: aprire e chiudere CP-007 per FORK1 in questo turno e aggiungere FORK1 e DECO1 a STATI_PROPOSTI. DECO1: il prossimo passo (2a skyline lv_01) e' del @developer, non del @grafico. Vedi RICHIESTA.md.

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-10-27 (Turno 1, ora 0 di 12). Fonte dettagliata: [CP-005](./CP-005-tracciati-percorsi.md) [CHIUSO], [CP-006](./CP-006-t1-run-chiusura.md) [APERTO], `out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md`, `UFFICIO/Officina/STATUS_OFFICINA.md`, `out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-005](./CP-005-tracciati-percorsi.md) — blocco R1 tracciati per le 6 piste, **CHIUSO** 2026-10-22 dal @capo: review incrociata indipendente completa positiva, ctest 17/17 verde (`sh UFFICIO/Officina/TOOL/run_tests.sh`, exit 0, log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`), gate soak verde registrato (6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame <= 20). |
| | [CP-006](./CP-006-t1-run-chiusura.md) — chiusura T1-RUN, **APERTO** (riaperto 2026-10-25): gate e review indipendente già registrati e positivi (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10; prova runner Mac Andrea 23/10 ok, ctest 17/17, runner ok), ma `ANDREA_REVIEW.md` D19 ha `RISPOSTA DI ANDREA: B` -> T1-RUN resta [~]. Chiudere CP-006 richiede decisione di Andrea (regola 8); DEFAULT non applicabile (risposta Andrea presente). Si procede senza aspettare; avvio R2 subordinato a conferma formale Andrea (D18 = B); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A).
| | [CP-007](./CP-007-fork1-bivio-tema.md) — FORK1 bivio e transizione tema, **CHIUSO** 2026-10-27: review incrociate indipendenti positive (developer-junior → developer punti 1-3 e 6 POSITIVA 07:14; developer → developer-junior gate 4/5/7 POSITIVA 07:20), ctest 20/20 verde (`sh UFFICIO/Officina/TOOL/run_tests.sh`, exit 0, 26/10/2026 07:00), gate `bivio_uscita`/`bivio_bordi`/`tema_transizione` exit 0, replay 2x IDENTICO, soglie non abbassate. Chiusura operativa del @capo senza decisione di Andrea (non richiesta: nessun D# FORK1 chiusura in `ANDREA_REVIEW.md`). |
| Stato attività | Tappa 1, Gate T1-RUN [~] (D19 = B, CP-006 APERTO): gate verde (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10), CP-005 chiusa con review incrociata indipendente positiva (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA; log `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`), prova runner Mac Andrea 23/10 ok (ctest 17/17, runner ok). R1 [x] (CP-005 chiuso). P1 [x]. G1 [~] check [x], CP-002 in attesa review indipendente (D15 = A, T1-Q4). FORK1 [x] (CP-007 chiuso 2026-10-27: review incrociate indipendenti positive, ctest 20/20 verde). DECO1 [~]: passi 2a-2e FATTO del @developer (`gate_t1.sh` exit 0 27/10 07:55, lv_06 smoke OK 08:42, gate `decorazioni` in ctest 21 test); resta review incrociata + CP DECO1 del @capo; @grafico fornisce asset e rivede screenshot in `Test area/`. DECO1 [x] prima di ogni prova Pi 400/RetroPie. F60 [x] gate, [~] review incrociata developer→developer-junior pendente (nessun CP F60 ancora aperto). Dopo R1, prima di R2 (Andrea 19 ott). H [~] in attesa misure Pi 400 (prova Mac 23/10 registrata; D12 = A). |
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

1. CP-006 APERTO (D19 RISPOSTA DI ANDREA: B): T1-RUN resta [~]; attendere nuova prova/decisione Andrea su runner Mac (D12 = A, D14 = A). DEFAULT non applicabile (risposta Andrea presente). Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale. CP-006 si chiude solo con decisione Andrea (regola 8).
2. CP-007 CHIUSO 2026-10-27 (FORK1): review incrociate indipendenti positive + ctest 20/20 verde. FORK1 [x]. Nessun CP R2 ancora aperto.
3. DECO1 [ ]: prossimo passo 2a (skyline lv_01 sopra l'orizzonte) del @developer, non del @grafico; poi 2b-2e; gate `decorazioni` e CP DECO1 dopo; @grafico fornisce asset e rivede screenshot. DECO1 [x] prima di ogni prova Pi 400/RetroPie.
4. R2 [~] in corso (aggiornato da Claude 27/10 su richiesta di Andrea): R2 NON e' subordinato a D18 o D19. Passi 1, 1b, 1c fatti, ctest 22/22 verde con gate `ciclo`; prossimo @developer passo 1d, poi DECO1 2f, RIV, PIST1 (vedi RICHIESTA, 'Chi tocca adesso' di R2).
5. F60 gate verde registrato 22/10 20:06 (60/50 Hz OK, replay 2x IDENTICO, 6/6 percorsi OK); manca solo review incrociata developer→developer-junior e apertura CP F60.
6. G1 @grafico: catalogo 100 PNG, `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0" (handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` consultato); CP-002 in attesa review indipendente.

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
