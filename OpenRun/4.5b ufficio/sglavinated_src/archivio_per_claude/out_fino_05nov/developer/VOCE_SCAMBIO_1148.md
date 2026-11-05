# VOCE_SCAMBIO_1148 — @sviluppatore -> ciclo | 18/10/2026 07:43

## Blocco
T1-RUN / P1 — pannello di info (velocita, marcia, fuoristrada) nel renderer; FEEDBACK_ANDREA v90 punti 2-3 ora coperti da codice. Stati proposti: P1 [~], T1-RUN [~].

## Fatto in questo turno
- `motore_nuovo/src/render/renderer.h` + `renderer.cpp`: `drawHud(speedPct, gear, offroad)` — barra velocita (arancio, gialla oltre l'85%), tacche 33%/66%, quadratini marcia, segnalino rosso offroad. Palette HUD esistente (indici 9-13, 15), nessun font nuovo (font sono di @grafico).
- `motore_nuovo/src/game/game.cpp`: `render()` chiama `drawHud(player_.speed / player_.maxSpeed(), player_.gear, player_.offroad)` come ultimo disegno del frame.
- `sh tools/gate_t1.sh`: exit 0, `GATE T1: TUTTO OK` su lv_01_neon, lv_02_atomico, lv_03_foglie. speed@60=161.79, speed@1=9.90, ms/frame medio ~0.4 (max 3.150, limite 20), replay 2x IDENTICO, offroad raggiunto; accelerazione, ribaltamento, sterzo, curva OK. Log: out/collaudatore/gate_t1.log.
- I 21 file `.sglraw` (7 sponsor x 3 stili) sono presenti in `motore_nuovo/dati/sprite_raw/sponsor/`; `png_a_raw.py` ha intestazione SPDX MIT (quota open OK).

## Mancante (esterno al mio turno)
- 3 screenshot in `out/sviluppatore/` (pista curva, traffico, cartelloni, personaggio con fumetto): gate P1, richiede runner SDL2 con finestra.
- `v91` di Andrea: prova `sglavinated_runner` su Mac + fps/temperatura Pi 400 (T1-RUN punto 8, in capo ad Andrea).

## Prossimo
Attendere v91 e screenshot runner; se verdi, promuovere P1/T1-RUN a [x] (solo Andrea/Claude).
