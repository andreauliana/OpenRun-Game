# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`). Fonti lette (solo queste): TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Percorsi relativi, mai cd.

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (riferimento documentale). | capo → — | Checkpoint Revision/ |
| T1-RUN | [~] | Gate verde da builder-developer 19/10 11:45: `sh tools/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi GATE PERCORSO OK, ms/frame medio 0.187, replay 2x IDENTICO (log `UFFICIO/Officina/Test area/gate_t1.log`). Andrea 09:39 stesso giorno: `GATE T1: FALLITO` exit 13, giro guidato KO 5/6 (solo lv_02_atomico OK) — ANDREA_REVIEW T1-Q3. Chiusura definitiva a [x] solo dopo CP-005 verde, review incrociata R1 registrata e via Andrea su Mac (D12 DEFAULT A, in attesa). Soglie non si abbassano (T1-Q5 DEFAULT A). | developer → developer-junior | out/developer/; out/developer-junior/ (report di test); motore_nuovo/ e tools/ per fase 1 |
| CP-005 / R1 | [~] | APERTO. developer task 1-3: scrive `motore_nuovo/dati/tracciati/tratti.json` e `motore_nuovo/dati/tracciati/regole.json` (contenuto pronto in `out/developer/VOCE_SCAMBIO_1174.md`) e fa leggere `regole.json` da `motore_nuovo/test/prova_gate.cpp` (soglie invariate, non abbassate; `road.cpp` raccordo già fatto; `game.cpp` campo `tracciato` già supporta e 6 livello.json già aggiornati). developer-junior task 4-5: gate `percorso` stampa tratto + segmento iniziale violato per ogni pista; test espansione tracciati (segmenti = somma lunghezze); riscrive 6 `motore_nuovo/dati/livelli/*/livello.json` con campo `tracciato` (compatibile se assente). Gate: `sh tools/gate_t1.sh soak` verde 6/6 piste, `ctest` 17/17. Review incrociata: developer-junior rivede 1-3, developer rivede 4-5. | developer → developer-junior (1-3); developer-junior → developer (4-5) | motore_nuovo/src/road/, motore_nuovo/src/game/, motore_nuovo/test/, motore_nuovo/dati/tracciati/, motore_nuovo/dati/livelli/ |
| P1 | [x] | Promosso [x] per Andrea (T1-Q2 "sì", prova screenshot differita). Punto 4 sprite veri da assets_originali ancora aperto (dipende da G1/designer). | developer → developer-junior | out/developer/ |
| H | [~] | Andrea: v91 `sglavinated_runner` su Mac (SDL2) + guida registrata; fps/temperatura Pi 400 (T1-Q3/T1-Q8). DEFAULT: Andrea esegue appena possibile; gate headless resta verde. Nessun lavoro dedicato a Pi 400 finché Andrea non dà ok su Mac (ordine test T1-Q5). | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] | ATTIVATO (Andrea T1-Q4 "attivare"). designer: `assets_originali/CATALOGO_GRAFICO.json` catalogo unico (id/tipo/stato/file/raw/livelli); classifica i 100 PNG non catalogati (scartato o nuovo elemento, senza cancellare file); `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; prepara sprite veri per P1 punto 4. È l'unico scrittore di `Shared asset/` (non in questo turno). Aggiorna `out/grafico/STATUS_GRAFICO_PER_CAPO.md` in place. | designer → capo (checkpoint da aprire in Checkpoint Revision/, review indipendente) | out/designer/ (handoff grafico: out/grafico/STATUS_GRAFICO_PER_CAPO.md); fase 1 assets_originali/, libreria/, dati UI/testo assegnati |
| M | [~] | Tappa 3 (HUD/font variante B, elemento "tempo") fuori scope Tappa 1. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 fuori scope Tappa 1. | — | — |
| J | [ ] | Escalazione dopo 2 tentativi falliti o fine blocco importante; non serve. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il @capo non è revisore tecnico. CONTROLLO.md: decisioni aperte per Andrea: 1 (D12, risposta "A"). Quota open 99,5% (soglia 50%); nessuna ROM Sega.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review e prove. Non scrive codice/asset.
- **developer** (out/developer/; fase 1 anche motore_nuovo/ e tools/): CP-005/R1 task 1-3 — scrivi `motore_nuovo/dati/tracciati/tratti.json` e `motore_nuovo/dati/tracciati/regole.json` (contenuto pronto in `out/developer/VOCE_SCAMBIO_1174.md`), fai leggere `regole.json` da `motore_nuovo/test/prova_gate.cpp` (soglie invariate). Tuning un parametro alla volta con `sh tools/gate_t1.sh soak` dalla radice `sglavinated_src/`. Non avviare P2/P3/T5/MD0.
- **developer-junior** (out/developer-junior/; fase 1 test in motore_nuovo/test/): CP-005/R1 task 4-5 — gate `percorso` stampa tratto + segmento iniziale violato; test espansione tracciati; riscrittura 6 `motore_nuovo/dati/livelli/*/livello.json` con campo `tracciato`; peer review indipendente del developer (task 1-3). Report in out/developer-junior/.
- **designer** (out/designer/; ruolo grafico, handoff out/grafico/STATUS_GRAFICO_PER_CAPO.md): G1 — `assets_originali/CATALOGO_GRAFICO.json`, classifica i 100 PNG non catalogati (scartato o nuovo elemento, senza cancellare file), `check_catalogo_grafico.py` exit 0 / "PNG non catalogati: 0"; prepara sprite veri per P1 punto 4. Aggiorna `out/grafico/STATUS_GRAFICO_PER_CAPO.md` in place.

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- **D12** (risposta registrata "A", 2026-10-19): dopo peer review incrociata su R1 e gate soak verde 6/6 percorsi, Andrea ri-verifica `sglavinated_runner` su Mac (v91, SDL2) prima dei test Pi 400/RetroPie. DEFAULT: A — sì, si attende il via di Andrea su Mac prima di lavoro dedicato a Pi/RetroPie (salvo non rompere la build Linux). Procedo con l'esecuzione R1 senza aspettare.
- **T1-Q3**: Andrea ha risposto (gate FALLITO exit 13 su 5/6 livelli su Mac); azione in carico a @developer (tuning percorsi via R1).
- **T1-Q5**: DEFAULT A già registrato (allinea tutti i livelli al profilo guidabile lv_02_atomico, tuning un parametro alla volta, soglie invariate). Andrea ha risposto 2026-10-19 (dinamica percorsi da ottimizzare, compito operativo R1). Nessuna attesa di Andrea per avviare R1.
- **Frequenza 60 Hz** (decisione Andrea 19/10): dopo R1, prima di R2 — da registrare nel checkpoint successivo; non blocca Tappa 1.

## Esito turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~], CP-005/R1 [~], H [~], G1 [~]; M/MD0/T5/J fuori scope o da fare): **nessun FINITO**. Nessun blocco hard (decisione aperta per Andrea 1, D12 con risposta "A"). Nessun PER_CLAUDE.md. Prossimo: @developer completa CP-005 task 1-3 e rilancia `sh tools/gate_t1.sh soak`; @developer-junior completa task 4-5 e peer review incrociata; @designer esegue G1 (catalogo + classificazione PNG). CP-005 resta APERTO fino a gate verde + entrambe le review indipendenti.
