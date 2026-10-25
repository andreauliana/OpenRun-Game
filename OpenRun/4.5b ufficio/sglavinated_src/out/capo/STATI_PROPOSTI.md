# STATI PROPOSTI — Turno 3 (ora 4 di 12) — @capo

> **Correzione Claude 25/10 (precede il resto di questa pagina):** D19 ha RISPOSTA DI ANDREA: B, quindi T1-RUN resta [~] e CP-006 e' APERTO (non CHIUSO). FORK1: codice e gate fatti da Claude, ctest 20/20; restano le due review incrociate e CP-007 (@capo). Poi DECO1 a passi piccoli 2a-2e (RICHIESTA.md).


Aggiornato: 2026-10-25. Fonte: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE.md, REGISTRO_REVISIONI.md, out/grafico/STATUS_GRAFICO_PER_CAPO.md, UFFICIO/Officina/STATUS_OFFICINA.md.
Tappa attiva: **1 — Gate T1 (blocco T1-RUN)**, CHIUSO CP-006 2026-10-25. Regola: una tappa si chiude quando passa il suo gate; poi review, poi Andrea, poi la tappa dopo.
Ruoli 4.6: **developer, developer-junior, grafico**. Consegne: `out/developer/`, `out/developer-junior/`, `out/grafico/`.
Handoff reparti citati: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (grafica, unico handoff @grafico) e `UFFICIO/Officina/STATUS_OFFICINA.md` (codice/build/test, unico handoff Officina).

## Verifica chiusure checkpoint (Turno 3)

- CP-005 (R1 tracciati 6 piste): **CHIUSO** 2026-10-22 — gate soak verde 6/6 piste e review incrociata indipendente positiva registrati (`REGISTRO_REVISIONI.md`, `STATO_CORRENTE.md`, dossier `CP-005-tracciati-percorsi.md`).
- CP-006 (chiusura T1-RUN, Tappa 1, Gate T1): **CHIUSO** 2026-10-25 — gate + review indipendente registrati e positivi (soak exit 0, 6/6 piste, replay 2x IDENTICO, ms/frame <= 20; ctest 17/17 verde 22/10; prova runner Mac Andrea 23/10 ok, ctest 17/17, runner ok); DEFAULT A applicato su D19 [APERTA] (risposta Andrea mancante, si procede senza aspettare); CHIUSO con motivazione esplicita (regola 8); avvio R2 subordinato a conferma formale Andrea (D18 = B); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale (D12/D14 = A).
- Nessun altro checkpoint ha gate + review già registrati da chiudere in questo turno: CP-002, CP-003, CP-004 restano APERTO in attesa di review indipendente; nessun CP F60 ancora aperto.

## Stati proposti (tappa 1 / Gate T1 e blocchi ereditati)

| Blocco | Stato | Responsabile | Revisore indipendente | Percorso consegna | Gate / criterio di chiusura | Note DEFAULT Andrea |
|---|---|---|---|---|---|---|
| T1-RUN | [x] chiuso CP-006 2026-10-25 | @developer + @developer-junior | incrociata CP-005 (developer-junior → developer punti 1-3 POSITIVA; developer → developer-junior punti 4-5 POSITIVA) | `out/developer/`, `out/developer-junior/` | `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su 6/6 piste, replay 2x IDENTICO, ms/frame <= 20, ctest verde | CP-005 chiusa con review incrociata indipendente positiva. Prova Andrea Mac 23/10 ok (ctest 17/17, runner ok). D19 [APERTA] DEFAULT A = chiudi T1-RUN [x], PRONTO PER ANDREA -> CHIUSO con motivazione esplicita (regola 8, risposta Andrea mancante, si procede senza aspettare). Avvio R2 subordinato a conferma formale Andrea (D18 = B). Nessun lavoro Pi 400/RetroPie dedicato prima di ok Mac formale (D12/D14 = A). Non toccare `src/runner/` (D16). |
| R1 (tracciati 6 piste) | [x] CP-005 | @developer (punti 1-3) + @developer-junior (punti 4-5) | incrociata CP-005 | `out/developer/`, `out/developer-junior/` | gate percorso verde su tutte le 6 piste, ctest 17/17, review incrociata registrata CP-005 | CP-005 CHIUSO 2026-10-22. Dopo R1, prima di R2: F60 (60 Hz). |
| F60 (60 Hz, tempi in secondi) | [x] gate, [~] review | @developer (`dt` in secondi, 60 Hz default, opzione 50 Hz in `dati/config.json`, `src/libretro/sglavinated_libretro.cpp` dichiara frequenza) | @developer-junior (prove 50/60 Hz al gate) | `out/developer/`, `out/developer-junior/` | ctest verde a 60 Hz e 50 Hz; giro guidato stessi secondi +/- 2%; replay 2x IDENTICO a ciascuna frequenza | Gate verde 22/10 20:06 (60/50 Hz OK, replay 2x IDENTICO, 6/6 percorsi OK). Review incrociata developer→developer-junior pendente; nessun CP F60 ancora aperto. Dopo R1, prima di R2 (Andrea 19 ott). Non toccare `src/runner/` prima di ok Andrea Mac (D16). |
| G1 (catalogo grafico) | [x] check verde, review CP-002 pendente | @grafico | review indipendente diversa da @grafico (CP-002, da assegnare) | `out/grafico/` | `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0", motore/test usano solo `pronto` e `approvato` | D15 = A, T1-Q4 = attivare. Check verde 25/10 10:24: exit 0, PNG non catalogati: 0, catalogo 181 elementi, licenza CC0-1.0 (handoff `out/grafico/STATUS_GRAFICO_PER_CAPO.md` consultato). CP-002 aperto (review indipendente da assegnare). Residuo: UI/HUD "tempo" D8 in attesa approvazione PNG B/N Andrea (manifest bozza, nessun blocco in attesa). |
| H (prova Mac / Pi 400) | [~] | Andrea | — | — | v91 runner SDL2 su Mac da registrare; poi fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`) | D12 = A, D14 = A: prima Mac, poi Pi 400/RetroPie; nessun lavoro dedicato Pi 400/RetroPie prima dell'ok Mac. Prova Mac 23/10: runner ok, ctest 17/17 (ANDREA_REVIEW.md D19). D16 tiene la prova Mac in attesa di conferma formale su build corretta. Misure Pi 400 ancora da fare. |
| P1 (promozione livello 1) | [x] | @developer | — | `out/developer/` | build CMake verificata, screenshot gameplay (prova differita, shell negata) | T1-Q2: sì (Andrea). |
| R2 (ciclo di gioco) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per R2) | `out/developer/`, `out/developer-junior/`, `out/grafico/` | ctest verde incluso gate ciclo; screenshot 5 fasi in `UFFICIO/Officina/Test area/`; @grafico rivede schermate | Dopo F60. Subordinato a ok Andrea Mac runner (D18 = B: attendere conferma formale Andrea; DEFAULT A su D19 copre solo T1-RUN, non l'avvio R2). Nessun lavoro Pi 400/RetroPie dedicato prima di ok Mac. |
| FORK1 (bivio e cambio tappa) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per FORK1) | `out/developer/`, `out/developer-junior/`, `out/grafico/` | ctest verde con `bivio_uscita`, `bivio_bordi`, `tema_transizione`; replay 2x IDENTICO; gate `gara` 4 combinazioni OK | Dopo F60, prima di RIV; non aspetta ok Mac Andrea (solo segnalazioni dalle prove già fatte 23/10); non toccare `src/runner/`. Punto 5 `bivio_bordi` [x] (exit 0 25/10 08:46); punto 4 `bivio_uscita` exit 16 atteso fino a fix game.cpp; punto 7 `tema_transizione` [x] (exit 0 07:47). |
| DECO1 (decorazioni 6 livelli) | [ ] | @grafico + @developer + @developer-junior | incrociata (CP per DECO1) | `out/grafico/`, `out/developer/`, `out/developer-junior/` | ctest verde con `decorazioni`; screenshot 6 livelli in `UFFICIO/Officina/Test area/`; `check_catalogo_grafico.py` exit 0 | Dopo FORK1, prima di RIV e prima di ogni prova Pi 400/RetroPie. Punto 1 grafico completo per tutti i 6 livelli (handoff grafico 25/10); integrazione motore (lettura `tema_grafico`) in attesa @developer. |
| PIST1 (percorsi vari: ponti, colline, chicane, autostrada, citta') | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per PIST1) | `out/developer/`, `out/developer-junior/` | ctest verde con `tratti`; screenshot in `UFFICIO/Officina/Test area/` | Dopo DECO1, prima di RIV; budget Pi 400 invariato (<= 12 ms/frame). |
| RIV (rivali veri e urto con sbandata) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per RIV) | `out/developer/`, `out/developer-junior/`, `out/grafico/` | ctest verde con `gara` e `urto`; screenshot rivali e sbandata in `UFFICIO/Officina/Test area/` | Dopo F60, prima di R2; non toccare `src/runner/`. |
| FIN (finale animato a fine corsa) | [ ] | @developer + @developer-junior + @grafico | incrociata (CP per FIN) | `out/developer/`, `out/developer-junior/`, `out/grafico/` | ctest verde; screenshot 3 finali in `UFFICIO/Officina/Test area/` | Dopo R2 e RIV. |

## Deleghe 4.6 (questa tappa)

- **@developer**: scrittura F60 (`motore_nuovo/src/game/` misure in secondi con `dt`, `dati/config.json` `"frequenza_hz"`, `src/libretro/sglavinated_libretro.cpp` dichiara frequenza scelta); FORK1 punti 1-3 e raccordo tracciati; DECO1 punto 2 (motore legge `tema_grafico`); PIST1 punti 1-2; RIV punti 1-2; FIN punto 1. Nessun tocco a `src/runner/` prima di ok Andrea Mac (D16). Scrittura: percorsi assegnati, `UFFICIO/Officina/TOOL/`, `out/developer/`.
- **@developer-junior**: revisore indipendente di @developer per F60; prove gate a 50/60 Hz; FORK1 punti 4-5 (gate `bivio_uscita`, `bivio_bordi`); DECO1 punto 3 (gate `decorazioni`); PIST1 punto 4 (gate `tratti`); RIV punto 4 (gate `urto`); FIN punto 3. Consegna `out/developer-junior/`.
- **@grafico**: G1 catalogo grafico (check verde, PNG non catalogati: 0); risorse grafiche R2 (splash, menu, risultati, HUD variante B) e 3 finali animati quando richiesti; DECO1 punto 1 (skyline + 4+ oggetti propri per ognuno dei 6 livelli); PIST1 punto 3 (oggetti ponte/autostrada/citta'); RIV punto 3 (sprite rivali); FIN punto 2 (scene di arrivo). Consegna `out/grafico/`. Unico handoff: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (aggiornato in place, niente riepiloghi paralleli).

## Decisioni Andrea (ANDREA_REVIEW.md) — DEFAULT applicato, si prosegue senza aspettare

- T1-Q1: sì; T1-Q2: sì (P1 [x]); D12 = A; D13 = B; D14 = A; D15 = A; D16 in corso (Andrea testa runner Mac v91 — prova 23/10 eseguita, esito registrato in D19; D16 tiene in attesa conferma formale su build corretta); D17 = A; D18 = B (R2 solo dopo conferma formale Andrea).
- D19 [APERTA] in ANDREA_REVIEW.md: DEFAULT A applicato (conferma chiusura T1-RUN [x] dopo prova Mac 23/10 ok); si procede senza aspettare. CP-006 CHIUSO con motivazione esplicita (regola 8), avvio R2 subordinato a conferma formale Andrea.
- Nessuna nuova domanda aperta in questo turno.

## Chiusura tappa

Non tutti i blocchi della tappa sono `[x]` (R2 `[ ]`, G1 review `[~]`, H `[~]`, F60 review `[~]`, FORK1/DECO1/PIST1/RIV/FIN `[ ]`). **FINITO non creabile.**
Nessun blocco richiede `PER_CLAUDE.md`: si prosegue autonomamente.

## Prossime azioni (ripresa dai blocchi [~], per CONTROLLO.md)

1. Aprire CP F60 e chiudere review incrociata developer → developer-junior (gate già verde 22/10).
2. Assegnare review indipendente per CP-002 (G1, check già verde 25/10).
3. H: attendere ok formale Andrea su runner Mac (build corretta) prima di Pi 400/RetroPie e prima di R2 (D12 = A, D14 = A, D18 = B).
