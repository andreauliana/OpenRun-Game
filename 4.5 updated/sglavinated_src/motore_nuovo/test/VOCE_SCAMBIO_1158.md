# VOCE_SCAMBIO_1158 — @collaudatore | 18/10/2026 12:40

## Esito misurato (gate T1)
`sh tools/gate_t1.sh` exit 0 — `GATE T1: TUTTO OK`.
- 3 livelli (lv_01_neon, lv_02_atomico, lv_03_foglie): smoke 500 frame + replay 2x, speed@60=161.79>0, speed@1=9.90, offroad raggiunto (frame 278/167/285), ms/frame medio 0.349-0.377 max ~0.9 (limite 20), replay 2x IDENTICO.
- Casi estesi punto 5 (avvolgimento playerZ oltre totalLen_, inattività 10s -> show/fumetto): già in `prova_gate.cpp`, eseguiti dentro lo smoke, OK.
- accelerazione 10s: 246.66 (entro 240-250), OK. Ribaltamento+ripartenza: OK. Sterzo sx/dx: OK. Curve continuità: salto pendenza max 0.50 <= 0.51, OK.
- quota_open.py: 100.0% (soglia 50%).

## Review punto 7 (renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, gate_t1.sh)
Nessun problema. Coerente con VISIONE.md: determinismo (seed fissa, nessun orologio di sistema), zero alloc per frame nel ciclo di gioco, tempo = contatore di frame, wrap via `fmod`, collina integrata una volta in `Road::build()`.

## Aperto — PASSAGGIO richiesto
- **frenata/costa**: i modi `frenata` (Pad::B) e `costa` (gas rilasciato, COAST=0.995) esistono in `prova_gate.cpp` (punti T1-RUN già implementati da Claude/PR#2) ma `gate_t1.sh` non li invoca (mancano dal riepilogo del gate). `gate_t1.sh` si trova in `tools/` e NON è nei percorsi di scrittura del ruolo collaudatore (AGENTS.md tabella ruoli): non posso riscriverlo. Serve che @sviluppatore o @capo aggiunga le due invocazioni a `gate_t1.sh` (es. `./build_gate/prova_gate ... frenata` e `... costa`) oppure valuti se i numeri sono già coperti da `accelerazione`/`costa` interne.
- **BLOCCO H [~]**: fps/temperatura su Pi 400 e v91 di Andrea (guida sul runner Mac) non misurabili da questo ruolo (macOS, nessun Pi; v91 è compito di Andrea, punto 8 T1-RUN). In attesa.

## Stati proposti (promozione a [x] solo Andrea/Claude)
- T1-RUN: resta [~] (frenata/costa non ancora nel gate; review punto 7 chiusa).
- H: resta [~] (in attesa Andrea v91 + Pi 400).

## File toccati
Nessuna modifica a codice di prodotto. Solo lettura. Log gate: `out/collaudatore/gate_t1.log`. Screenshot: `out/collaudatore/<livello>/`.
