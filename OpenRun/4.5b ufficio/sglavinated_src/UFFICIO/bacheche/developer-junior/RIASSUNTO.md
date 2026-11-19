# RIASSUNTO @developer-junior (19/11 09:52)
## Stato
- V47 VJ4 soak chiuso (lv_01-lv_06 OK). VJ5 ∞ in corso: review dell'ultimo diff src.

## Ultimo turno (VJ5, turno 4, 19/11 09:50)
- Diff src: M `motore_nuovo/src/render/renderer.cpp` (19+/7-) — in `drawSprite` il lambda `texel(tx,ty)` e' diventato `coloreIdx(idx)` (`renderer.cpp:1772-1777`) e la riga sorgente si calcola una volta per riga schermo: `rigaTex = px + ty*sw` (`:1782,1804,1814`), fallback `rigaCerca = px + ry*sw` (`:1822-1825`) (`git status/diff --stat/diff` exit 0).
- Nessun errore: stessi indici (`px[ty*sw+tx]`, `px[ry*sw+rx]`), corpo lambda identico (spPal nullptr se palette vuota -> nessuna divisione per 0), bounds `tx0 <= sw-1`, `(tx0+tx1)/2 <= sw-1`, `rx < tx1 <= sw`, `ty < sh`.
- Semplificazione: init morta `const u8* rigaTex = px` (`:1782`) riassegnata prima di ogni uso -> dichiararla dentro il ciclo sy (`:1804`).
- Prova/modifica test: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 275.25 s); `prova_gate ".../vj5_lv04_1911" lv_04_cromo percorso` exit 0 (GATE PERCORSO OK, 2125 frame); smoke lv_04 exit 0 (GATE OK, medio 1.597/1.142, max 9.728 frame 230 / 3.614 frame 0, replay IDENTICO; max 21-23 ms di 2 run sotto carico di build non riprodotti). Nuovo: indice del picco `msMaxFrame` in `motore_nuovo/test/prova_gate.cpp` (Esito + print smoke/soak), `cmake --build ... --target prova_gate` exit 0. `gate_t1.sh` non eseguito.
## Rilievi aperti (sintesi)
- Invariati: `mascheraGraffito` cache per puntatore (`renderer.cpp:1201,1247,1303-1304`); `incrocioSprites_` 2 push_back/tipo con `Sprite*` stabili (`renderer.h:258-271`); reserve fisse/accessor a puntatore; `spriteOrd_`/`terraOrdine_` riallocabili >2048; `hline` duplica il clamp di `fillRect` (`framebuffer.cpp:21-45`); `drawLuceGalleria` idiv per pixel (`renderer.cpp:1385`); `copertura_` resize di pari passo (`:188`).
- Backlog warning/fragilità: `game.cpp`, `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
