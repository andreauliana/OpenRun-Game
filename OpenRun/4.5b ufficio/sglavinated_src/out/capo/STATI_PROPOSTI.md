# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 1 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh UFFICIO/Officina/TOOL/gate_t1.sh soak`). Fonti: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md; consultati STATO_CORRENTE.md, REGISTRO_REVISIONI.md, CP-005, STATUS_OFFICINA.md (09:39 + report 20/10), STATUS_GRAFICO_PER_CAPO.md. Percorsi relativi, mai cd.

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (CP-001 chiuso come riferimento documentale; nessuna approvazione di release). | capo → — | Checkpoint Revision/ |
| T1-RUN | [~] | `gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio <= 20. Ultimo esito: `sh UFFICIO/Officina/TOOL/gate_t1.sh` 20/10 ~14:05 exit 0, 6/6 OK (rettilineo 55-71%, curva max 0.72-0.98, giro finito, fuori strada 0%), ms/frame ~0.18-0.20 max < 2, replay IDENTICO (log `UFFICIO/Officina/Test area/gate_t1.log`; `out/developer/ESITO_GATE_T1_20OTT_1405.md`). Chiusura a [x] solo dopo: CP-005 chiuso con review incrociata indipendente positiva, gate verde riconfermato, ctest eseguito. Il via Mac di Andrea (D12 DEFAULT A) condiziona solo Pi 400/RetroPie, non T1-RUN. | developer → developer-junior | out/developer/; out/developer-junior/ (report di test); motore_nuovo/ e UFFICIO/Officina/TOOL/ (fase 1) |
| CP-005 / R1 | [ ] APERTO | Blocco R1: `road.cpp` raccordo entrata/uscita (deriva solo su segmenti con curva != 0) FATTO; `dati/tracciati/tratti.json` (8 tratti) e `regole.json` CREATI; 6 `dati/livelli/*/livello.json` con campo `tracciato` presenti; `prova_gate` legge `regole.json` e stampa regole violate; test espansione tracciati OK 6/6 (segmenti = somma lunghezze). Gate: `gate_t1.sh soak` verde 6/6 piste, `ctest`, replay 2x IDENTICO, ms/frame <= 20, soglie NON abbassate (T1-Q5 DEFAULT A). Review indipendente incrociata NON ancora registrata (developer-junior rivede developer punti 1-3; developer rivede developer-junior punti 4-5). | developer → developer-junior (1-3); developer-junior → developer (4-5) | motore_nuovo/src/road/, motore_nuovo/dati/tracciati/, motore_nuovo/test/, motore_nuovo/dati/livelli/, UFFICIO/Officina/TOOL/ |
| P1 | [x] | Promosso [x] per Andrea (T1-Q2 "sì", prova screenshot differita). Punto 4 sprite veri da `assets_originali` ancora aperto (dipende da G1/designer). | developer → developer-junior | out/developer/ |
| H | [~] | Andrea: v91 `sglavinated_runner` su Mac (SDL2) + guida registrata; fps/temperatura Pi 400 (T1-Q3/T1-Q8). DEFAULT: Andrea esegue appena possibile; nessun lavoro dedicato a Pi 400 finché Andrea non dà ok su Mac (ordine test T1-Q5; D12 DEFAULT A). Gate headless resta verde. | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] | ATTIVATO (Andrea T1-Q4 "attivare"). designer: catalogo unico `assets_originali/CATALOGO_GRAFICO.json`; classifica i 100 PNG non catalogati (scartato o nuovo elemento, senza cancellare file); `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; prepara sprite veri per P1 punto 4. Handoff grafico: `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (aggiornato in place; unico scrittore di `Shared asset/`, non in questo turno). | designer → capo (checkpoint in `Checkpoint Revision/`, review indipendente) | out/designer/; fase 1 `assets_originali/`, `libreria/`, dati UI/testo assegnati |
| M | [~] | Tappa 3 (HUD/font variante B, elemento "tempo") fuori scope Tappa 1. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 fuori scope Tappa 1. | — | — |
| J | [ ] | Escalazione dopo 2 tentativi falliti o fine blocco importante; non serve. | — | — |
| R2 | [-] | Dopo R1 (ciclo di gioco completo). Non avviare prima che CP-005 sia chiuso. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il @capo non è revisore tecnico. CONTROLLO.md: decisione aperta per Andrea: 1 (D12, risposta "A"). Quota open 99,5% (soglia 50%); nessuna ROM Sega.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md, STATO_CORRENTE.md e REGISTRO_REVISIONI.md; verifica review e prove. Non scrive codice/asset.
- **developer** (out/developer/; fase 1 anche motore_nuovo/ e UFFICIO/Officina/TOOL/): CP-005/R1 punti 1-3 conclusi (road.cpp, tratti.json, regole.json); rilancia `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` dalla radice `sglavinated_src/` e riporta comando, exit code, misure; non avviare P2/P3/T5/MD0/R2.
- **developer-junior** (out/developer-junior/; fase 1 test in motore_nuovo/test/): CP-005/R1 punti 4-5 conclusi (prova_gate legge regole.json, stampa regole violate, test espansione 6/6); peer review indipendente dei punti 1-3 del developer. Report in out/developer-junior/.
- **designer** (out/designer/; ruolo grafico, handoff out/grafico/STATUS_GRAFICO_PER_CAPO.md): G1 — catalogo i 100 PNG non catalogati (scartato o nuovo elemento, senza cancellare file), `check_catalogo_grafico.py` exit 0 / "PNG non catalogati: 0"; prepara sprite veri per P1 punto 4. Aggiorna `out/grafico/STATUS_GRAFICO_PER_CAPO.md` in place.

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- **D12** (risposta registrata "A", 2026-10-19): dopo peer review incrociata su R1 e gate soak verde 6/6, Andrea ri-verifica `sglavinated_runner` su Mac (v91, SDL2) prima dei test Pi 400/RetroPie. DEFAULT: A — sì, si attende il via di Andrea su Mac prima di lavoro dedicato a Pi/RetroPie (salvo non rompere la build Linux). R1 si esegue senza aspettare.
- **T1-Q3**: Andrea ha risposto (gate FALLITO exit 13 su 5/6 livelli su Mac, 19/10); azione in carico a @developer (tuning percorsi via R1); gate ora verde su run 20/10 14:05, da riconfermare con Andrea alla ri-verifica Mac.
- **T1-Q5**: DEFAULT A già registrato (allinea tutti i livelli al profilo guidabile lv_02_atomico, tuning un parametro alla volta, soglie invariate). Nessuna attesa di Andrea per avviare/continuare R1.
- **Chiusura T1-RUN**: DEFAULT sì — T1-RUN a [x] quando `gate_t1.sh soak` exit 0, replay 2x IDENTICO, 6/6 percorsi OK, ms/frame medio <= 20, CP-005 chiuso con review indipendente positiva e ctest eseguito. Criterio ctest: CP-005 cita 17/17; STATUS_OFFICINA segnala 7 test scoperti nella build attuale — DEFAULT: si esegue `ctest` e si registra l'esito; se 17/17 non è applicabile, si segnala ad Andrea senza bloccare.
- **Frequenza 60 Hz** (decisione Andrea 19/10): dopo R1, prima di R2 — da registrare nel checkpoint successivo; non blocca Tappa 1.
- Nessuna nuova domanda: tutti i DEFAULT esistenti sono registrati e si procede senza aspettare Andrea.

## Esito turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~], CP-005/R1 [ ] APERTO, H [~], G1 [~], M [~]; MD0/T5/J/R2 fuori scope o da fare): **nessun FINITO**. Nessun blocco hard (nessuna decisione Andrea mancante per procedere; D12 risposta "A" già registrata). Nessun PER_CLAUDE.md. Prossimo: registrare la review incrociata indipendente su CP-005 (developer-junior → developer punti 1-3; developer → developer-junior punti 4-5), riconfermare gate verde, poi chiudere T1-RUN a [x]; designer avvia G1 (catalogo 100 PNG).
