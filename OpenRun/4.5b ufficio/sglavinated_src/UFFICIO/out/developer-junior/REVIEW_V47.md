# REVIEW V47 (riscritta) — @developer-junior
Turno 11/11 12:07, VJ5 ∞.

## Cambio rivisto
- `motore_nuovo/src/render/renderer.cpp` commit da1c8c6 (V47 V7): riserve in `Renderer::Renderer` per buffer per-frame.

## Rilievo
- `renderer.cpp:29-30`: `terraOrdine_.reserve(64)` e `spriteOrd_.reserve(512)` riducono allocazioni iniziali, ma `spriteOrd_` viene `resize(sprites.size())` a ogni `drawSprites` (`renderer.cpp:1325`): se gli sprite visibili superano 512 (rivali+traffico+incroci+fissi) si rialloca nel ciclo, riducendo il beneficio V7. Suggerimento: alzare la riserva al caso peggiore misurato o tracciare il massimo; analogo rischio minore per `terraOrdine_` se gli oggetti a terra crescono oltre 64.

## Prova
- `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (dopo edit `motore_nuovo/test/prova_gate.cpp` nota VJ5 12:07).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_renderer_1207" lv_01_neon caricamento` exit 0, GATE CARICAMENTO OK (init 24.5 ms, avviaGara 10.2/9.0 ms). gate_t1.sh non eseguito.
