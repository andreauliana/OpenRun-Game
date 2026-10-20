# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-10-20 (Turno 1, ora 1 di 12). Fonte dettagliata: [CP-005](./CP-005-tracciati-percorsi.md), `out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md`, `UFFICIO/Officina/STATUS_OFFICINA.md`, `out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-005](./CP-005-tracciati-percorsi.md) — blocco R1 tracciati per le 6 piste, **APERTO**. Modifiche eseguite (developer + developer-junior), gate verde 20/10 ~14:05, **review incrociata indipendente ancora da registrare**. |
| Stato attività | Tappa 1, Gate T1-RUN. `road.cpp` raccordo entrata/uscita, `dati/tracciati/tratti.json` (8 tratti), `regole.json`, 6 `livello.json` con campo `tracciato` eseguiti (developer). `prova_gate` legge `regole.json`, stampa regole violate, test espansione OK 6/6 (developer-junior). `sh UFFICIO/Officina/TOOL/gate_t1.sh` 20/10 ~14:05: **exit 0, `GATE T1: TUTTO OK`**, 6/6 percorsi GATE PERCORSO OK, ms/frame ~0.18-0.20 max < 2, replay 2x IDENTICO (log `UFFICIO/Officina/Test area/gate_t1.log`; `out/developer/ESITO_GATE_T1_20OTT_1405.md`). T1-RUN [~] in attesa review CP-005 + ctest. H [~] in attesa via Andrea su Mac (D12 DEFAULT A). G1 [~] attivato (T1-Q4). |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Dal 2026-10-19 `assets_originali/` e' un symlink a `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` (un solo originale, nella Libreria) |
| Ruoli | Capo, Designer (grafico), Developer, Developer Junior |
| Review | Peer review tra i due sviluppatori; autore distinto dal revisore. CP-005: developer-junior rivede developer (punti 1-3); developer rivede developer-junior (punti 4-5). |
| Build/test | Ultimo gate 2026-10-20 ~14:05 exit 0, `GATE T1: TUTTO OK` (log `UFFICIO/Officina/Test area/gate_t1.log`). Build CMake in `UFFICIO/Officina/build/`. ctest: 7 test scoperti nella build esistente, da rieseguire e registrare (criterio CP-005 da confermare). Runner SDL2 compila; PNG disabilitato (SDL2_image non presente). |
| Limite della prova | Eseguito su macOS; prestazioni e compatibilità su Raspberry Pi 400 non verificate da questo gate |
| Decisioni di Andrea | T1-Q1 sì (chiudi T1-RUN se soak exit 0, replay 2x IDENTICO, ms/frame <= 20); T1-Q2 sì (P1 [x]); T1-Q3 risposta registrata (gate KO 5/6 su Mac 19/10, azione tuning a @developer); T1-Q4 attivare G1; T1-Q5 DEFAULT A (soglie invariabili, tuning un parametro alla volta); D12 DEFAULT A (Andrea ri-verifica runner su Mac prima di Pi 400/RetroPie); nessuna approvazione di release registrata. |

## Handoff grafico

Leggere [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) come unico riepilogo aggiornato del Designer. Inventario dettagliato: [CATALOGO_RISORSE.md](../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md). Catalogo: 52 voci (30 approvate, 16 pronte, 2 in costruzione, 4 mancanti); 100 PNG non catalogati da classificare. Mancano asset approvati per personaggi/concorrenti e i 3 finali animati.

## Handoff Officina

Leggere [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) per l'ultimo report di sviluppo, build, test e blocchi. Nel checkpoint riportare soltanto esito, misure e riferimenti ai log pertinenti; lo status non sostituisce la peer review.

## Prossime azioni

1. Registrare review incrociata indipendente su CP-005 (developer-junior → developer punti 1-3; developer → developer-junior punti 4-5) con esito e prove.
2. Riconfermare gate verde (`gate_t1.sh soak` exit 0, 6/6 percorsi OK, replay 2x IDENTICO, ms/frame <= 20) ed eseguire ctest.
3. Chiudere T1-RUN a [x] solo dopo CP-005 chiuso e prove registrate; poi designer avvia G1 (catalogo 100 PNG).
4. Nessun lavoro dedicato a Pi 400/RetroPie finché Andrea non dà ok su Mac (D12 DEFAULT A, ordine test T1-Q5).

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
