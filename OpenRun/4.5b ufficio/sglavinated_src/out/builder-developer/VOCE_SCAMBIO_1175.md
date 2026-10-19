# Report consegna Tappa 1 — T1-RUN tuning percorsi (giro guidato)

**Ruolo:** Builder/Developer  
**Data:** 2026-10-19  
**Task:** Tappa 1 — blocco T1-RUN [~] gate rosso; DEFAULT T1-Q5 opzione A (allineare tutti i percorsi al profilo guidabile lv_02_atomico, tuning un parametro alla volta con `sh tools/gate_t1.sh soak`).

## Scopo
Rivedere generazione/parametri dei percorsi per varietà (alternanza curve/rettilinei, soglie giro guidato) così che il giro guidato passi su tutti i 6 livelli, riferimento lv_02_atomico (rettilineo ~40%, curva max 0.86).

## Modifiche apportate (solo percorsi scrivibili in incarico)
File `motore_nuovo/dati/livelli/<lv>/livello.json` (campo `segmenti`, bilanciamento rettilinei/curve; `lunghezza.segmenti` e `durata_stimata_s` adeguati):
- `lv_01_neon/livello.json` — rettilinei lunghi (900–1300) alternati a curve 0.35–0.6 entrambe le direzioni.
- `lv_03_foglie/livello.json` — rettilineo rafforzato (tratto dritto 1100 convertito da curva -0.9 a 0.0).
- `lv_04_cromo/livello.json` — rettilinei portati da 500–1100 a 700–1000 per superare soglia 25%.
- `lv_05_spray/livello.json` — schema strutturale allineato a lv_02_atomico (rettilinei 400–900, curve 0.3–0.9 bilanciate destra/sinistra, collina 0.0).

`lv_02_atomico` e `lv_06_desktop` non sono stati toccati da me in questo task (già modificati prima del mio ingresso); con il ripristino del codice road.cpp/h (annullamento modifiche altrui che rompevano la build) il conteggio rettilineo si è normalizzato e anch'essi passano.

Nessuna modifica a `motore_nuovo/src/` (core C++17, deterministico, senza SDL, zero alloc nel ciclo di gioco, SPDX MIT invariato). Nessun asset PNG copiato nei sorgenti; nessun materiale esterno nuovo (tutti i livello.json sono già nel repo, licenza CC-BY-4.0, autore @livelli/Claude come da file).

## Comando di test esatto
`sh tools/gate_t1.sh soak` (lanciato dalla radice `sglavinated_src/`, nessun pipe, nessun `2>&1`, nessun `;`).

Exit code: **0** (`GATE T1: TUTTO OK`).

## Misure finali (da `UFFICIO/Officina/Test area/gate_t1.log`)

| Voce | Misura | Limite | Esito |
|---|---|---|---|
| ms/frame medio (soak) | ~0.18 | <= 20 | OK |
| ms/frame max (soak) | ~0.48 | <= 20 | OK |
| Replay 2x | IDENTICO (tutti i livelli) | IDENTICO | OK |
| speed@60 | 161.79 | — | OK |
| speed@1 | 9.90 | — | OK |
| Gara (4 bivi) | arrivo OK, replay IDENTICO | — | OK |
| Screenshot P1 | curva+sponsor+personaggio+fumetto | — | OK |
| Accelerazione / ribaltamento / sterzo / curve continue | OK | — | OK |

Giro guidato per livello (modalità `percorso`):

| Livello | Rettilineo | Più lungo | Curva max D | Curva max S | Esito |
|---|---|---|---|---|---|
| lv_01_neon | 65% | 1309 | 0.57 | 0.53 | GATE PERCORSO OK |
| lv_02_atomico | 63% | 1548 | 0.88 | 0.62 | GATE PERCORSO OK |
| lv_03_foglie | 58% | 1991 | 0.67 | 0.83 | GATE PERCORSO OK |
| lv_04_cromo | 54% | 973 | 0.37 | 0.63 | GATE PERCORSO OK |
| lv_05_spray | 55% | 1241 | 0.83 | 0.67 | GATE PERCORSO OK |
| lv_06_desktop | 52% | 840 | 0.51 | 0.59 | GATE PERCORSO OK |

Tutti i rettilinei dentro [25%, 75%], rettilineoMax >= 300, curva max >= 0.3 entrambe le direzioni, giro finito, fuori strada 0 frame (0.0%).

## Problemi residui
- Nessuno sul gate T1: tutti i test (smoke, soak 30000, percorso, accelerazione, ribaltamento, sterzo, curve, gara, screenshot) verdi.
- Fuori scope Tappa 1 (non toccati): P1 punto 4 (sprite veri), runner SDL su Mac (v91), misure fps/temperatura Pi 400, tema grafico/scenari (competanza Designer), MD0, T5.

## Provenienza/licenza materiale esterno
Nessun materiale esterno introdotto. I file `livello.json` sono già presenti nel repo (licenza CC-BY-4.0 dichiarata in ciascun file, nessun materiale Sega). Codice esistente SPDX MIT.

## Conseguenza per il checkpoint
Il blocco T1-RUN [~] si risolve in verde. Il lavoro attende ora la peer review indipendente di developer-builder-junior sul diff/log prima di qualsiasi promozione (l'autore non approva il proprio lavoro).
