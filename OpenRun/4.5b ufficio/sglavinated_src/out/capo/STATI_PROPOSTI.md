# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`).
Fonti lette (solo queste): TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (CP-001 riferimento documentale, nessuna approvazione release). | capo → — | Checkpoint Revision/ |
| T1-RUN | [x] | `sh tools/gate_t1.sh soak` exit 0; replay 2x IDENTICO; ms/frame <= 20 (0,09 ms/frame registrato). T1-Q1 Andrea: "sì". | builder-developer → developer-builder-junior (peer review diff/log) | out/builder-developer/ (fase 1: motore_nuovo/src/, tools/, dati/livelli/) |
| P1 | [x] | Promozione T1-Q2 Andrea: "sì" (prova differita, screenshot shell-negata già registrati in SCAMBIO_RECENTE). Punto 4 sprite veri → G1 attivato. | builder-developer → developer-builder-junior | out/builder-developer/ (report); out/sviluppatore/screenshot_p1/ solo lettura (legacy) |
| H | [~] | Andrea prova `sglavinated_runner` su Mac (SDL2) e registra v91 in ANDREA_REVIEW.md; fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`). T1-Q3 Andrea: "sì", appena possibile. Gate headless verde. Non eseguibile qui (regole 9/21). | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] | ATTIVATO (T1-Q4 Andrea: "attivare"). `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0". Catalogo 52 elementi, 100 PNG non classificati. | designer → capo | out/designer/ (fase 1: assets_originali/, libreria/, dati UI/testo assegnati) |
| M | [~] | Tappa 3 (HUD/font) non avviata. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 non avviata. | — | — |
| J | [ ] | Escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il Capo non è revisore tecnico del proprio lavoro. Review ultima (SCAMBIO_RECENTE): saltata, nessun file di codice/dati/asset testuale modificato dall'ultimo checkpoint; quota open 99,5% (soglia 50%); nessuna ROM Sega.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review indipendenti e prove. Non scrive codice/asset.
- **builder-developer** (out/builder-developer/; fase 1 anche motore_nuovo/ e tools/): motore_nuovo/src/ (player/, road/, render/, game/), CMakeLists.txt, dati/livelli/. T1-RUN punti 1-4 (colline, wrap sprite, acceleratore fatto, densità cartelloni). Zero alloc nel ciclo di gioco, determinismo (contatore frame, RNG seed, nessun orologio di sistema), core senza SDL, SPDX MIT su file nuovi. Shell solo `sh tools/gate_t1.sh [soak]` e `cmake --build build --target prova_gate` (D9). In questo turno nessun comando eseguito; ultima verifica gate già registrata. Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1).
- **developer-builder-junior** (out/developer-builder-junior/): test, gate T1, replay deterministico, diagnosi, report. Legge diff, log ed esiti di builder-developer; revisione indipendente su T1-RUN e P1. Non avviare nuove tappe.
- **designer** (out/designer/): G1 catalogo grafico ATTIVATO (assets_originali/CATALOGO_GRAFICO.json, check_catalogo_grafico.py), sprite veri (punto 4 T1-RUN), UI/font (M). È l'unico scrittore di `Shared asset/` (non in questo turno).

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- T1-Q1 chiusura gate T1: DEFAULT sì — chiudi T1-RUN a [x] se gate soak exit 0, replay 2x IDENTICO, ms/frame <= 20. (Risposta Andrea: "sì" → applicato, T1-RUN [x].)
- T1-Q2 promozione P1: DEFAULT sì — screenshot bloccati da shell negata, prova differita. (Risposta Andrea: "sì" → applicato, P1 [x].)
- T1-Q3 prova Mac/Pi 400: DEFAULT sì — Andrea esegue appena possibile; v91 + misure chiudono H. (Risposta Andrea: "sì" → H [~] in attesa esecuzione Andrea.)
- T1-Q4 attivazione G1: DEFAULT non attivare — fuori scope T1. (Risposta Andrea: "attivare" → applicato, G1 [~] attivato.)

## Esito turno

Non tutti i blocchi della tappa sono [x] (H [~], G1 [~], M [~], MD0 [-], T5 [-], J [ ]): **nessun FINITO**. Nessun blocco hard (CONTROLLO.md: decisioni aperte per Andrea 0; nessun PER_CLAUDE.md; agenti aperti builder-developer e developer-builder-junior). Si riprende dai blocchi [~] e si procede con i DEFAULT: prove P1/H differite → chiusura Tappa 1, poi review e Tappa 2.
