# RIEPILOGO MATTINA — OpenRun 4.6 (08/11, Turno 1, ora 0 di 12)

## Fatto per blocco/agente
- @developer + @developer-junior: gate T1-RUN verde 08/11 — `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio 1.079-1.758 max 17.003 (<=20), screenshot P1 personaggio+fumetto frame 446 > 0 (era -1). Fix: `prova_gate.cpp:969` (@developer-junior, review @developer POSITIVA) e `gate_t1.sh` righe 50-84 `."`→`./` (@developer, review @developer-junior POSITIVA). Review indipendente registrata (autore != revisore).
- @grafico: G1 [x] 220 elementi, 0 non catalogati, CC0-1.0, `check_catalogo_grafico.py` exit 0; DECO1 passo 2f FATTO; PIST1 G-P1 [x] (parapetto/guardrail).
- CP-005 CHIUSO (R1 tracciati 6 piste), CP-007 CHIUSO (FORK1). Stati: P1 [x], FORK1 [x], F60 [x] gate/[~] review, G1 [x].
- DECO1 [~]: passi 2a-2d e 2f fatti; 2e parallasse prossimo; CP-008 da aprire dopo review incrociata.
- R2 [~]: passi 1/1b/1c fatti, ctest 22/22 verde gate `ciclo`; prossimo passo 1d (nome a 3 lettere nei Risultati). RIV [~]: codice a-c fatto, grafica rivali in attesa.
- PIST1 [ ]: G-P1 [x]; prossimo @developer P3 (parapetti oggetti ogni ~400 unità su segmenti con `acqua`) + gate_t1.sh.
- H [~]: in attesa v91 Andrea runner Mac + fps/temperatura Pi 400 (ordine: prima Mac, poi Pi).

## Cosa non funziona / blocchi
- T1-RUN [~]: ANDREA_REVIEW D19 = B → CP-006 APERTO; chiusura richiede decisione Andrea (nuova prova runner Mac su build corretta, prima Mac poi Pi 400/RetroPie; DEFAULT non applicabile). Nessun lavoro dedicato Pi prima di ok Mac.
- DECO1 [~]: manca review incrociata + CP-008 (@capo). F60: manca review incrociata developer→developer-junior + CP F60.
- CP-002 (G1): review indipendente da assegnare (revisore diverso da @grafico, cand. @developer-junior).
- Pi 400/RetroPie: non testate; core libretro manca `libretro.h` (BUILD_LIBRETRO=ON non compila).
- Grafica rivali e personaggi (Gianni/Gesù/UFO) mancanti come PNG; HUD bloccato da Andrea (08/11): niente modifiche a hud.json/drawHudDati.

## Decisioni aperte ANDREA_REVIEW.md (3 più importanti)
1. D19 [APERTA] — chiusura T1-RUN dopo prova Mac 23/10: risposta B registrata, T1-RUN [~]; serve nuova prova/decisione Andrea (runner Mac su build corretta) prima di chiudere T1-RUN e di lavoro dedicato Pi 400/RetroPie.
2. T1-Q3 — ordine test Mac→Pi (D12/D14 = A): Andrea ri-verifica runner Mac v91 prima di Pi 400/RetroPie; H [~] in attesa fps/temperatura Pi 400.
3. Blocco lavoro dedicato Pi 400/RetroPie: sospeso fino a ok Mac formale (D12/D14 = A); nessun lavoro Pi oltre a non rompere la build Linux.

## Prossimo passo consigliato
@developer: PIST1 passo P3 (parapetti come oggetti ai due lati ogni ~400 unità su segmenti con `acqua`), poi `sh UFFICIO/Officina/TOOL/gate_t1.sh`. Poi @developer-junior J2/J1 (gate `tratti`), poi @capo apre/chude CP PIST1. In attesa di: decisione Andrea su D19 (CP-006), review incrociata DECO1 (CP-008) e F60, review indipendente CP-002 (G1).
