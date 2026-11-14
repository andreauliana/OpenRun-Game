# REVIEW V47 (@developer-junior)
Riscritto 14/11 08:53.

## VJ4 Soak (uno per turno)
| Data | Livello | Comando | Exit | Frame | Replay | ms/frame medio | ms/frame max | Crash | Note |
|---|---|---|---|---|---|---|---|---|---|
| precedenti | lv_01-lv_04 | soak | 0 | 30000 | IDENTICO | — | — | no | esiti registrati nei turni precedenti |
| 14/11 | lv_05_spray | `prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv05" lv_05_spray soak` | 0 | 30000 | IDENTICO | 2.013 | 42.081 | no | GATE OK; out `UFFICIO/Officina/Test area/vj4_soak_lv05/` |

Resta VJ4: lv_06_desktop. Poi VJ5 ∞.

## VJ3 14/11 (chiuso 6/6) — 50/60 Hz
Delta tempi giro: lv_01 0.03%, lv_02 0.19%, lv_04 0.03%, lv_05 0.24%, lv_06 0.09%, **lv_03 4.44%** (anomalia gioco, segnalata a @developer). Replay IDENTICO a entrambe le frequenze su tutti. Buco chiuso: modo `frequenza_confronto` in `prova_gate` (soglia 2.0%, exit 22); `gate_t1.sh` non lo usa. Prove: `UFFICIO/Officina/Test area/vj3_14nov/`.

## VJ5 (invariato, 14/11)
`motore_nuovo/src/game/fasi.cpp:576`: puntini riga `nostro` fissi `0xFFFFFFFFu` vs `col` lampeggiante (567); commento 574-575 incoerente. Proposta: `e.nostro ? col : 0xFF9898B0u` o grigio fisso, commento allineato. Ultimo cambio src: `da6c52b` (nessun nuovo).

## Rilievi aperti
- `salvataggi/giocatore.json` cancellato nel working tree: ripristino manuale (`git checkout -- salvataggi/giocatore.json`, fuori perimetro).
- @developer: lv_03 perde ~1.85 s a 50 Hz; lv_05 livello piu' pesante (~2.4-2.6 ms/frame medi). gate_t1.sh non eseguito oggi.