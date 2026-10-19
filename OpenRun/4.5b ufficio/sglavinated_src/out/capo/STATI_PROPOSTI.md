# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`).
Fonti lette: TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| T1-RUN | [~] | `sh tools/gate_t1.sh soak` exit 0; replay 2x IDENTICO; ms/frame <= 20. Evidenza: SCAMBIO_RECENTE "GATE T1: TUTTO OK"; gate OK 17 ott 14:51; ms/frame medio 0.185 max 0.518 (limite 20); replay 2x IDENTICO; speed@60=161.79 speed@1=9.90. Log `UFFICIO/Officina/Test area/gate_t1.log`. Residue non bloccanti: colline, wrap sprite, densità cartelloni (T1-RUN 2-4 in RICHIESTA_APERTA). Risposta Andrea T1-Q1 non registrata → DEFAULT applicato. | builder-developer → developer-builder-junior | out/builder-developer/ (fase 1: motore_nuovo/src/, tools/, dati/livelli/ assegnati) |
| P1 | [~] | 3 screenshot in `out/sviluppatore/screenshot_p1/` (curva, cartelloni sponsor, personaggio+fumetto; `GATE SCREENSHOT P1 OK`). Nuova prova bloccata (shell), promozione differita. DEFAULT T1-Q2: sì. | builder-developer → developer-builder-junior | out/builder-developer/ |
| H | [~] | Andrea prova `sglavinated_runner` su Mac (SDL2) e registra v91 in ANDREA_REVIEW.md; fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`). Non eseguibile qui (regole 9/21). DEFAULT T1-Q3: sì, appena possibile. | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [ ] | `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0". Oggi: catalogo 52 elementi, 100 PNG non classificati. Non attivare in T1 (DEFAULT T1-Q4: no, fuori scope). | designer → capo | out/designer/ (fase 1: assets_originali/, libreria/, dati UI/testo assegnati) |
| M | [~] | Tappa 3 (HUD/font) non avviata. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 non avviata. | — | — |
| C | [x] | Baseline 4.6 chiusa (CP-001). | capo | Checkpoint Revision/ |
| J | [ ] | Escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il Capo non è revisore tecnico del proprio lavoro. Review ultima (SCAMBIO_RECENTE): saltata, nessun file di codice/dati/asset testuale modificato dall'ultimo checkpoint; quota open 99.5% (soglia 50%); nessuna ROM Sega.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review indipendenti e prove. Non scrive codice/asset.
- **builder-developer** (out/builder-developer/; fase 1 anche motore_nuovo/ e tools/): motore_nuovo/src/ (player/, road/, render/, game/), CMakeLists.txt, dati/livelli/. Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Shell solo `sh tools/gate_t1.sh` [soak] e `cmake --build build --target prova_gate` (D9). Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1).
- **developer-builder-junior** (out/developer-builder-junior/): test, gate T1, replay deterministico, diagnosi, report. Legge diff, log ed esiti di builder-developer; revisione indipendente su T1-RUN e P1. Non avviare nuove tappe.
- **designer** (out/designer/): catalogo grafico G1 (assets_originali/CATALOGO_GRAFICO.json, check_catalogo_grafico.py). Non attivato in questo turno (DEFAULT no, fuori scope T1).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- T1-Q1 chiusura gate T1: DEFAULT sì — chiudi T1-RUN a [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20. (Risposta: non registrata → DEFAULT applicato.)
- T1-Q2 promozione P1: DEFAULT sì — screenshot bloccati da shell negata, prova differita. (Risposta: non registrata.)
- T1-Q3 prova Mac/Pi 400: DEFAULT sì — Andrea esegue appena possibile; v91 + misure chiudono H. (Risposta: non registrata.)
- T1-Q4 attivazione G1: DEFAULT non attivare — fuori scope T1. (Risposta: non registrata.)

## Esito turno

Non tutti i blocchi della tappa sono [x] (G1 [ ], H [~], M [~], MD0 [-], T5 [-], J [ ]): **nessun FINITO**. Nessun blocco hard (CONTROLLO.md: decisioni aperte per Andrea 0; nessun PER_CLAUDE.md; agenti aperti builder-developer e developer-builder-junior). Si prosegue con i DEFAULT: prove P1/H differite → chiusura Tappa 1, poi review e Tappa 2.
