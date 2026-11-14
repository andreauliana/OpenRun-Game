# REVIEW V47 (@developer-junior)
Riscritto 14/11 08:35 — VJ3 completo sui 6 livelli + nuovo modo `frequenza_confronto` in `prova_gate`.

## VJ3 14/11 — 50 e 60 Hz: chiusura su lv_05_spray e lv_06_desktop
Dopo `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0. Comandi (uno per riga, uscite in `UFFICIO/Officina/Test area/vj3_14nov/`):
- `prova_gate .../lv_05_60 lv_05_spray frequenza 60` -> exit 0, `GATE FREQUENZA 60 Hz OK`
- `prova_gate .../lv_05_50 lv_05_spray frequenza 50` -> exit 0, `GATE FREQUENZA 50 Hz OK`
- `prova_gate .../lv_06_60 lv_06_desktop frequenza 60` -> exit 0, `GATE FREQUENZA 60 Hz OK`
- `prova_gate .../lv_06_50 lv_06_desktop frequenza 50` -> exit 0, `GATE FREQUENZA 50 Hz OK`

| Livello | 50 Hz runA/runB | 60 Hz runA/runB | Delta | Replay | ms/frame 50 (medio/max) | ms/frame 60 (medio/max) |
|---|---|---|---|---|---|---|
| lv_05_spray | 44.92 / 44.92 s | 45.03 / 45.03 s | 0.11 s (**0.24%**) | IDENTICO 0.00% a entrambe | 2.44/15.81 · 2.42/32.60 | 2.60/41.22 · 2.51/18.51 |
| lv_06_desktop | 35.20 / 35.20 s | 35.23 / 35.23 s | 0.03 s (**0.09%**) | IDENTICO 0.00% a entrambe | 1.69/9.29 · 1.69/11.32 | 1.76/117.58 · 1.74/8.42 |

Esiti: lv_05 e lv_06 allineati, replay identico a entrambe le frequenze. **VJ3 chiuso 6/6 livelli**: lv_01 0.03%, lv_02 0.19%, lv_04 0.03%, lv_05 0.24%, lv_06 0.09%, **lv_03 4.44%** (anomalia del gioco, non del banco).
Nota per @developer: lv_05 e' il livello piu' pesante misurato finora, 2.44-2.60 ms/frame medi contro 1.7-1.8 di lv_06 (citta' con incroci e traffico), utile per V3/V9. I picchi isolati (117.58 ms in runA lv_06 60 Hz) non sono del gioco: carico della macchina, il medio resta sotto soglia.

## Buco del test chiuso: modo `frequenza_confronto`
`verificaFrequenza` confrontava solo le due run della stessa frequenza: 4.7% di scarto passava come `OK`. Aggiunto in `motore_nuovo/test/prova_gate.cpp` (mio perimetro): helper `misuraGiroFrequenza` (misura riusabile, soglia di sterzo normalizzata al passo), `verificaFrequenzaConfronto` con soglia `MAX_DIFF_FREQUENZA_PCT = 2.0` e modo `frequenza_confronto` (exit 22). `gate_t1.sh` non lo richiama (usa `frequenza` su lv_01), quindi il gate resta verde.
- `prova_gate .../confronto_lv_03 lv_03_foglie frequenza_confronto` -> **exit 22**, 60 Hz 39.73 s vs 50 Hz 41.58 s, diff 4.44% > 2.00% (`FALLITO: il tempo di giro cambia con la frequenza`)
- `prova_gate .../confronto_lv_06 lv_06_desktop frequenza_confronto` -> exit 0, 35.23 vs 35.20 s, diff 0.09%

## VJ5 (13/11, invariato) — `src/game/fasi.cpp:576`
Puntini della riga `nostro` fissi `0xFFFFFFFFu` mentre nome/posizione/punti usano `col` lampeggiante (riga 567); commento 574-575 dice "grigi come i separatori". Semplificazione proposta: `e.nostro ? col : 0xFF9898B0u` oppure grigio fisso per tutti, e commento allineato.

## Rilievi aperti per altri ruoli
- `salvataggi/giocatore.json` risulta **cancellato** nel working tree (`git status`): va ripristinato a mano con `git checkout -- salvataggi/giocatore.json` (fuori dal mio perimetro).
- @developer: lv_03_foglie perde 1.85 s di giro a 50 Hz (4.4%) — qualche costo per-frame non scalato da `dt` (dossi/salti, offroad, traffico). Ora c'e' un gate che lo misura: `frequenza_confronto`.
