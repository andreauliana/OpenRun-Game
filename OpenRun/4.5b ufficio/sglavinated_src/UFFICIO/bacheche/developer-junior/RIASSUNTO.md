# RIASSUNTO @developer-junior (19/11 08:45)
## Stato
- V47 VJ4 soak chiuso (lv_01-lv_06 OK). VJ5 ∞ in corso: review dell'ultimo diff src.

## Ultimo turno (VJ5, turno 2, 19/11 08:45)
- Diff src: M `motore_nuovo/src/render/renderer.cpp` (18+/4-) + M `render/renderer.h` (6+), `framebuffer.cpp` invariato (`git diff --stat -- motore_nuovo/src` exit 0): in `drawSprite` tx0/tx1 (e specchio) precalcolati una volta per colonna in `colTx0_`/`colTx1_` (`renderer.cpp:1784-1793`, membri `renderer.h:497-498`) invece che due divisioni a 64 bit per pixel.
- Nessun errore: `sxIni >= max(0,-x0)` e `sxFin <= min(w,WIDTH-x0)` (`renderer.cpp:1759-1766`) => `nCol <= WIDTH`, indici dentro gli array; return `:1766` prima del precalc; formula identica (clamp `tx1<=tx0` poi specchio) => stessi texel. Nota: replay A/B = stesso binario (`prova_gate.cpp:1089`), resa garantita solo dall'ispezione (`frameHash` copre il framebuffer, `game.cpp:3895`).
- Semplificazione: `drawLuceGalleria` idiv per pixel `x*s->width/w` (`renderer.cpp:1385`, ty fuori ciclo `:1381`), galleria di lv_02/lv_05 -> precalc per colonna. Misura: smoke lv_02 medio 0.988 / max 3.558 ms, sotto la soglia 8 ms di V9 (delta del passo non misurato, nessun build "prima").
- Prove: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 295.08 s); `prova_gate ".../vj5_lv02_1911" lv_02_atomico percorso` exit 0 (`GATE PERCORSO OK`, 2476 frame); smoke lv_02 exit 0 (`GATE OK`, replay 2x IDENTICO). `gate_t1.sh` non eseguito in questo turno.
## Rilievi aperti (sintesi)
- Invariati: `mascheraGraffito` cache per puntatore (`renderer.cpp:1201,1247,1303-1304`); `incrocioSprites_` 2 push_back/tipo con `Sprite*` stabili (`renderer.h:258-271`); reserve fisse/accessor a puntatore; `spriteOrd_`/`terraOrdine_` riallocabili >2048; `hline` duplica il clamp di `fillRect` (`framebuffer.cpp:21-45`).
- Backlog warning/fragilità: `game.cpp`, `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
