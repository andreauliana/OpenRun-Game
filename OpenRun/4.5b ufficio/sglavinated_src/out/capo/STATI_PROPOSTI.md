# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`). Fonti lette (solo queste): TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Handoff: out/grafico/STATUS_GRAFICO_PER_CAPO.md (inventario 18/10, 52 elementi, 100 PNG non catalogati); UFFICIO/Officina/STATUS_OFFICINA.md (report 19/10 09:39). CP-005 APERTO (blocco R1 tracciati per le 6 piste). Nota: developer ha eseguito `sh tools/gate_t1.sh soak` exit 0 su 6/6 percorsi (rettilineo 52-73%, soglie rispettate) dopo fix road.cpp; restano R1 compiti 2-3 (tratti.json, regole.json) e 4-5 (gate tratto-violazione, 6 livello.json con tracciato) in capo a developer-junior, review incrociata da eseguire.

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (CP-001 riferimento documentale). | capo → — | Checkpoint Revision/ |
| T1-RUN | [~] | `sh tools/gate_t1.sh soak` exit 0 su tutti e 6 i percorsi; replay 2x IDENTICO; ms/frame <= 20; soglie NON abbassate (T1-Q5 DEFAULT A). Dipende da CP-005/R1 (road.cpp, tratti.json, regole.json, 6 livello.json, gate tratto-violazione). | developer → developer-junior (CP-005 review incrociata in attesa) | out/developer/; out/developer-junior/ (report di test) |
| P1 | [~] | Promozione T1-Q2 Andrea: "sì". Punto 4 sprite veri da assets_originali aperto (dipende da G1/designer). | developer → developer-junior | out/developer/ |
| H | [~] | Andrea: v91 `sglavinated_runner` su Mac (SDL2) + guida registrata; fps/temperatura Pi 400. T1-Q3 Andrea: "sì" (esecuzione in carico ad Andrea). Gate headless verde dopo R1. | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] ATTIVATO | T1-Q4 Andrea: "attivare". `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; catalogo unico id/tipo/stato/file/raw/livelli. | designer → capo (CP-002, review indipendente) | out/grafico/; fase 1 assets_originali/, libreria/, dati UI/testo assegnati |
| M | [~] | Tappa 3 (HUD/font variante B, elemento tempo) fuori scope Tappa 1. | designer → capo | out/grafico/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 fuori scope Tappa 1. | — | — |
| J | [ ] | Escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il Capo non è revisore tecnico. CONTROLLO.md: blocchi [x]: 17, [~]: 4, [ ]: 4; agenti aperti: developer, developer-junior; decisioni aperte per Andrea: 1 (D12). Quota open 99,5% (soglia 50%); nessuna ROM Sega.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review e prove. Non scrive codice/asset.
- **developer** (out/developer/; fase 1 anche motore_nuovo/ e tools/): R1 punti 1-3 — `motore_nuovo/src/road/road.cpp` raccordo entrata/uscita (campi `entrata`, `uscita`, default min(n/3, 60); deriva solo su segmenti con curva != 0), `motore_nuovo/dati/tracciati/tratti.json` (libreria tratti nominati), `motore_nuovo/dati/tracciati/regole.json` (regole per pista lette da prova_gate, soglie invariate). Tuning un parametro alla volta con `sh tools/gate_t1.sh soak`. Non avviare P2/P3/T5/MD0.
- **developer-junior** (out/developer-junior/; fase 1 test in motore_nuovo/test/): R1 punti 4-5 — gate `percorso` stampa tratto + segmento iniziale violato per ogni pista; test espansione tracciati (segmenti = somma lunghezze); riscrittura 6 `motore_nuovo/dati/livelli/*/livello.json` con campo `tracciato` (compatibile se assente); peer review indipendente di developer (punti 1-3). Report in out/developer-junior/.
- **designer** (out/grafico/): G1 — `assets_originali/CATALOGO_GRAFICO.json`, classifica i 100 PNG non catalogati (scartato o nuovo elemento, senza cancellare file), `check_catalogo_grafico.py` exit 0 / "PNG non catalogati: 0"; prepara sprite veri per P1 punto 4. È l'unico scrittore di `Shared asset/` (non in questo turno). Aggiorna `out/grafico/STATUS_GRAFICO_PER_CAPO.md` in place.

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- D12 (APERTA): dopo peer review incrociata su R1 e gate soak verde su 6/6 percorsi, Andrea ri-verifica `sglavinated_runner` su Mac (v91, SDL2) prima dei test Pi 400/RetroPie. DEFAULT: A — sì, si attende il via di Andrea su Mac prima di lavoro dedicato a Pi/RetroPie (salvo non rompere la build Linux). Procedo con l'esecuzione R1 senza aspettare.
- T1-Q5: DEFAULT A già registrato (allinea tutti i livelli al profilo guidabile lv_02_atomico, tuning un parametro alla volta, soglie invariate). Nessuna attesa di Andrea per avviare R1.
- T1-Q3: Andrea ha risposto (gate FALLITO exit 13 su 5/6 livelli); azione in carico a developer (tuning percorsi via R1).

## Esito turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~] via CP-005, P1 [~], H [~], G1 [~], M [~]; MD0 [-], T5 [-], J [ ]): **nessun FINITO**. Nessun blocco hard (decisione aperta per Andrea 1, D12 con DEFAULT A; nessun PER_CLAUDE.md). Prossimo: developer esegue R1 punti 1-3 e rilancia `sh tools/gate_t1.sh soak`; developer-junior completa punti 4-5 e peer review; designer esegue G1 (catalogo + classificazione PNG). CP-005 resta APERTO fino a gate verde + entrambe le review indipendenti.
