# RIASSUNTO @developer-junior (19/11 11:40)
## Stato
- V47 VJ4 soak chiuso (lv_01-lv_06 OK). VJ5 ∞ in corso: review dell'ultimo diff src ogni turno.

## Ultimo turno (VJ5, turno 9, 19/11 11:40)
- Diff src: M `motore_nuovo/src/render/renderer.cpp` + M `renderer.h` — NUOVO: `colCerca_[c] = tx1 - tx0 > 1` nel ciclo colonne di `drawSprite` (`renderer.cpp:1804`, uso nella condizione di ricerca `:1838`), nuovo membro `bool colCerca_[Framebuffer::WIDTH]` (`renderer.h:503-507`) al posto del termine a ogni pixel (`git status/diff --stat/diff` exit 0; unici src modificati).
- Nessun errore: il precalc usa gli stessi `tx0/tx1` (clamp `:1793`, specchio `:1794`) che il ciclo pixel rilegge da `colTx0_`/`colTx1_` (`:1795-1796,1829-1830`) -> stesso bool; `c ∈ [0,nCol)` con `nCol <= WIDTH` (`:1759-1760,1788`) e return `:1766` prima del ciclo colonne -> tutti gli elementi scritti prima della lettura; OR identico -> replay identico; +320 byte, -1 sub+cmp per pixel.
- Semplificazioni: (nuova) droppare `colCerca_` e usare `tx1-tx0>1` dai locali `:1829-1830` (costo +1 sub+cmp/pixel, misurare V9) o riordinare `:1838` in `cercaRiga || colCerca_[c]`; aperte da prima: `screenYRow` duplica `y0+sy` (uso solo `inTubo` `:1850` -> passare `y0+sy`), base `rigaCerca0` (`:1843`), init morta `rigaTex=px` (`:1782`).
- Prove: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27, 262.19 s); `prova_gate "UFFICIO/Officina/Test area/vj5_lv05_1911t2" lv_05_spray smoke` exit 0 (GATE OK, medio 0.975 max 3.768 ms frame 0, replay IDENTICO, caricamento 3.2-4.4 ms). `gate_t1.sh` non eseguito.
## Rilievi aperti (sintesi)
- Invariati: `mascheraGraffito` cache per puntatore (`renderer.cpp:1201,1247,1303-1304`); `incrocioSprites_` 2 push_back/tipo con `Sprite*` stabili (`renderer.h:258-271`); reserve fisse/accessor a puntatore; `spriteOrd_`/`terraOrdine_` riallocabili >2048; `hline` duplica il clamp di `fillRect` (`framebuffer.cpp:21-45`); `drawLuceGalleria` idiv per pixel (`renderer.cpp:1385`); `copertura_` resize di pari passo (`:188`).
- Backlog warning/fragilità: `game.cpp`, `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
