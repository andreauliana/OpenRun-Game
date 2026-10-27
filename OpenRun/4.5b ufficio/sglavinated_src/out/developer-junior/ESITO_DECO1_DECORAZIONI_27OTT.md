# ESITO DECO1 — gate `decorazioni` (27/10 06:37, developer-junior)

## Cosa è stato fatto
Esteso `motore_nuovo/test/prova_gate.cpp` (DECO1 punto 3) con:
- screenshot PPM per livello (`prova_gate_curva.ppm` in `decorazioni_<livello>/`);
- verifica che lo skyline di lv_01_neon sia disegnato sopra l'orizzonte (passo 2A di @developer completato: `renderer.cpp` `drawSkyline`, `game.cpp` `setSkylineSprite`, `caricaSkyline` già nel gate); conteggio pixel cielo diversi dal colore di fondo, fallimento se 0.

## Comandi ed esiti
- `cmake --build UFFICIO/Officina/build --target prova_gate` → exit 0.
- `UFFICIO/Officina/build/prova_gate UFFICIO/Officina/build/test-results/decorazioni lv_01_neon decorazioni` → exit 0, `GATE DECORAZIONI OK`.

## Misure (limite ms/frame medio <= 20, massimo 12 obiettivo Pi 400)
| Livello | catalogo id risolti | cartelloni | ms/frame medio | ms/frame max | skyline sopra l'orizzonte |
| lv_01_neon | 6/6 | 56 | 0.296 | 0.604 | sì (16292 pixel cielo diversi, cielo 0x0A0A1A) |
| lv_02_atomico | 7/7 | 53 | 0.196 | 0.301 | no (passo 2b in attesa) |
| lv_03_foglie | 6/6 | 61 | 0.200 | 0.347 | no |
| lv_04_cromo | 5/5 | 54 | 0.198 | 0.342 | no |
| lv_05_spray | 5/5 | 35 | 0.195 | 0.301 | no |
| lv_06_desktop | 5/5 | 39 | 0.198 | 0.449 | no |

## Problemi residui
Nessuno. Nessun file `src/` toccato: solo `motore_nuovo/test/prova_gate.cpp` (percorso di ruolo).

## Prossimi passi (non in questo turno)
- DECO1 passi 2b-2e (@developer): skyline tutti i livelli, oggetti bordo strada, densità cartelloni ~20-30%, parallasse leggero in curva; poi estendo il gate con gli oggetti.
- G1 (@grafico): catalogo chiuso, stato pronto/approvato per tutti gli id di `tema_grafico` dei 6 livelli.
- Poi R2 [ ]; FORK1 [~] attende review @developer gate 4/5/7 e CP-007 @capo; T1-RUN [~] (D19 = B).
