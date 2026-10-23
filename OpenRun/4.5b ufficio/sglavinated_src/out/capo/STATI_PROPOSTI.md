# STATI PROPOSTI — Turno 1 (ora 0 di 12) — @capo

Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**. Regola: una tappa si chiude quando passa il suo gate; poi review, poi Andrea, poi la tappa dopo.
Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.
Handoff reparti citati: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (grafica, unico handoff @grafico) e `UFFICIO/Officina/STATUS_OFFICINA.md` (codice/build/test, unico handoff Officina).

## Stati proposti (tappa 1 / Gate T1)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note DEFAULT Andrea |
|---|---|---|---|---|---|---|
| T1-RUN | [x] proposto — CP-006 PRONTO PER ANDREA | @developer + @developer-junior | incrociata (CP-005 chiusa: developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA) | `out/developer/`, `out/developer-junior/` | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest verde | Gate tecnico verde (20/10 e 21/10: 6/6 GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio <= 0.323 max <= 16.094; ctest 17/17 verde 22/10). CP-005 chiusa con review incrociata indipendente positiva. Prova Andrea: 23/10 Mac runner ok, ctest 17/17 (ANDREA_REVIEW.md ultima riga/D19). D19 [APERTA] con DEFAULT A = chiudi T1-RUN [x], PRONTO PER ANDREA; risposta Andrea mancante → non CHIUSO (regola 8). Nessun lavoro Pi 400/RetroPie dedicato prima di ok Mac formale. Non toccare `src/runner/` (D16). |
| R1 (tracciati 6 piste) | [x] | @developer (punti 1-3) + @developer-junior (punti 4-5) | incrociata (CP-005) | `out/developer/`, `out/developer-junior/` | gate percorso verde su tutte le 6 piste, ctest 17/17, review incrociata registrata in CP-005, soglie invariate | CP-005 CHIUSO 2026-10-22. Dopo R1, prima di R2: F60 (60 Hz). |
| F60 (60 Hz, tempi in secondi) | [x] gate, [~] review | @developer (scrive: `dt` in secondi, 60 Hz default, opzione 50 Hz in `dati/config.json`, `src/libretro/sglavinated_libretro.cpp` dichiara frequenza) | @developer-junior (revisore; prove 50/60 Hz al gate) | `out/developer/`, `out/developer-junior/` | ctest verde a 60 Hz e 50 Hz; giro guidato stessi secondi +/- 2%; replay 2x IDENTICO a ciascuna frequenza | Gate verde 22/10 20:06 (developer-junior: 60/50 Hz OK, replay 2x IDENTICO, 6/6 percorsi OK, ms/frame 0.16-0.275; log `UFFICIO/Officina/Test area/gate_t1.log`, report `out/developer-junior/ESITO_F60_22OTT_2006.md`). Review incrociata developer→developer-junior pendente; nessun CP F60 ancora aperto. Dopo R1, prima di R2 (Andrea 19 ott). Non toccare `src/runner/` prima di ok Andrea Mac (D16). |
| G1 (catalogo grafico) | [~] (check [x]) | @grafico | review indipendente diversa da @grafico (da assegnare, CP-002) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato` | D15 = A, T1-Q4 = attivare. Check eseguito 22/10 21:33: exit 0, 159 elementi, PNG non catalogati: 0 (handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` consultato). CP-002 aperto (review indipendente da assegnare). Residuo: UI/HUD "tempo" D8 in attesa approvazione PNG B/N Andrea (manifest bozza). |
| H (prova Mac / Pi 400) | [~] | Andrea | — | — | v91 runner SDL2 su Mac da registrare; poi fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`) | D12 = A, D14 = A: prima Mac, poi Pi 400/RetroPie; nessun lavoro dedicato Pi 400/RetroPie prima dell'ok Mac. Prova Mac 23/10: runner ok, ctest 17/17 (registrato in ANDREA_REVIEW.md D19). Misure Pi 400 ancora da fare. |
| P1 (promozione livello 1) | [x] | @developer | — | `out/developer/` | build CMake verificata, screenshot gameplay (prova differita, shell negata) | T1-Q2: sì (Andrea). |
| R2 (ciclo di gioco) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per R2) | `out/developer/`, `out/developer-junior/`, `out/grafico/` | ctest verde incluso gate ciclo; screenshot 5 fasi in `UFFICIO/Officina/Test area/`; @grafico rivede schermate | Dopo F60. Subordinato a ok Andrea Mac runner (D18 DEFAULT A applicato, risposta mancante → DEFAULT A). Nessun lavoro Pi 400/RetroPie dedicato prima di ok Mac. |

## Deleghe 4.6 (questa tappa)

- **@developer**: scrittura F60 (`motore_nuovo/src/game/` misure in secondi con `dt`, `dati/config.json` `"frequenza_hz"`, `src/libretro/sglavinated_libretro.cpp` dichiara frequenza scelta); nessun tocco a `src/runner/` prima di ok Andrea Mac (D16). T1-RUN punti aperti ereditati (colline e wrap sprite già nel codice; sprite veri bloccati su G1). Scrittura: percorsi assegnati, `UFFICIO/Officina/TOOL/`, `out/developer/`.
- **@developer-junior**: revisore indipendente di @developer per F60; prove gate a 50/60 Hz; consegna `out/developer-junior/`.
- **@grafico**: G1 catalogo grafico (check verde, 159 voci, PNG non catalogati: 0); risorse grafiche R2 (splash, menu, risultati, HUD variante B) e 3 finali animati quando richiesti. Consegna `out/grafico/`. Unico handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

## Decisioni Andrea (ANDREA_REVIEW.md) — DEFAULT applicato, si prosegue senza aspettare

- T1-Q1: sì; T1-Q2: sì (P1 [x]); D12 = A; D13 = B; D14 = A; D15 = A; D16 in corso (Andrea testa runner Mac v91 — prova 23/10 eseguita, esito registrato in D19); D17 = A; D18 risposta mancante → DEFAULT A applicato (dopo CP-005 chiuso + ctest verde, T1-RUN resta [~] fino a ok Andrea Mac, poi avvio R2; nessun lavoro Pi 400/RetroPie dedicato prima dell'ok).
- Nuova domanda D19 [APERTA] in ANDREA_REVIEW.md (DEFAULT A): conferma chiusura T1-RUN [x] dopo prova Mac 23/10 ok; si procede senza aspettare.

## Checkpoint

- CP-005 (R1 tracciati): **CHIUSO** 2026-10-22 — gate soak verde 6/6 piste e review incrociata indipendente positiva registrati (`REGISTRO_REVISIONI.md`, `STATO_CORRENTE.md`).
- CP-006 (chiusura T1-RUN): **PRONTO PER ANDREA** 2026-10-23 — gate + review registrati, decisione Andrea su D19 in attesa (DEFAULT A applicato). Non CHIUSO: regola 8 richiede decisione di Andrea.
- Nessun altro checkpoint ha gate+review già registrati da chiudere in questo turno (CP-002/CP-003/CP-004 attendono review indipendente; nessun CP F60 ancora aperto).

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (R2 [ ], G1 [~], H [~], F60 review [~]). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente.
