# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`).
Fonti lette (solo queste): TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Handoff reparto: out/designer/STATUS_GRAFICO_PER_CAPO.md, UFFICIO/Officina/STATUS_OFFICINA.md (report 09:39 19/10).

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (CP-001 riferimento documentale, nessuna approvazione release). | capo → — | Checkpoint Revision/ |
| T1-RUN | [~] **BLOCCATO** | `sh tools/gate_t1.sh soak` exit 0; replay 2x IDENTICO; ms/frame <= 20; giro guidato OK su tutti i 6 livelli. **Gate fallito (Andrea 19/10 09:39, exit 13): giro guidato OK solo lv_02_atomico, fallito lv_01/lv_03/lv_04/lv_05/lv_06 ("percorso poco vario o non guidabile").** | builder-developer → developer-builder-junior (peer review diff/log) | out/builder-developer/ (fase 1: motore_nuovo/src/ player/road/render/game, dati/livelli/, tools/) |
| P1 | [~] | Promozione T1-Q2 Andrea: "sì"; screenshot P1 già registrati (SCAMBIO_RECENTE). Punto 4 sprite veri da assets_originali ancora aperto (dipende da G1/Designer). | builder-developer → developer-builder-junior | out/builder-developer/ (report) |
| H | [~] | Andrea: v91 `sglavinated_runner` Mac (SDL2) + guida registrata in ANDREA_REVIEW.md; fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`). T1-Q3 Andrea: "sì", da eseguire. Gate headless verde. | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] **ATTIVATO** | T1-Q4 Andrea: "attivare". `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; catalogo unico con id/tipo/stato/file/raw/livelli. | designer → capo (review indipendente) | out/designer/ (fase 1: assets_originali/, libreria/, dati UI/testo assegnati) |
| M | [~] | Tappa 3 (HUD/font, variante B, elemento tempo) non avviata — fuori scope Tappa 1. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 non avviata — fuori scope Tappa 1. | — | — |
| J | [ ] | Escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il Capo non è revisore tecnico del proprio lavoro. Review ultima (SCAMBIO_RECENTE): saltata, nessun file di codice/dati/asset testuale modificato dall'ultimo checkpoint; quota open 99,5% (soglia 50%); nessuna ROM Sega. CONTROLLO.md: blocchi [x]: 17, [~]: 4, [ ]: 4; agenti aperti: developer-builder-junior, builder-developer; decisioni aperte per Andrea: 0.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review indipendenti e prove. Non scrive codice/asset.
- **builder-developer** (out/builder-developer/; fase 1 anche motore_nuovo/ e tools/): **task T1-RUN — rivedere generazione/parametri dei percorsi** per varietà (alternanza curve/rettilinei, soglie giro guidato) su tutti i 6 livelli, riferimento lv_02_atomico (rettilineo 40%, curva max 0.86 OK). Rilanciare `sh tools/gate_t1.sh soak` dopo tuning (un parametro alla volta). Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Shell solo `sh tools/gate_t1.sh [soak]` e `cmake --build build --target prova_gate` (D9). Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1).
- **developer-builder-junior** (out/developer-builder-junior/): test, gate T1, replay deterministico, diagnosi, report; **revisione indipendente (peer review)** del diff/log di builder-developer su T1-RUN e P1. Non avviare nuove tappe.
- **designer** (out/designer/): **G1 catalogo grafico attivato** (assets_originali/CATALOGO_GRAFICO.json, check_catalogo_grafico.py), sprite veri (punto 4 T1-RUN/P1), UI/font (M). È l'unico scrittore di `Shared asset/` (non in questo turno).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

Vedi nuova voce `T1-Q5` in ANDREA_REVIEW.md (soglie giro guidato e tuning percorsi, DEFAULT A). Nessuna decisione di Andrea necessaria per procedere col tuning builder-developer.

## Esito turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~] bloccato, P1 [~], H [~], G1 [~], M [~], MD0 [-], T5 [-], J [ ]): **nessun FINITO**. Nessun blocco hard (decisioni aperte per Andrea 0; nessun PER_CLAUDE.md). Si riprende da T1-RUN [~]: builder-developer esegue tuning percorsi e rilancia gate soak; developer-builder-junior fa peer review indipendente.
