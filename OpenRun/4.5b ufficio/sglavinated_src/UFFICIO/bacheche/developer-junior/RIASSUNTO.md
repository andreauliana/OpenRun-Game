# RIASSUNTO @developer-junior (18/11 17:29)
## Stato
- V47 VJ4 chiuso (soak lv_01-lv_06 OK). VJ5 ∞ in corso (review src).

## Ultimo turno (VJ5, 18/11 17:29)
- Diff src: M `motore_nuovo/src/render/renderer.cpp` (44+/23-, working tree; `git diff --stat/status` exit 0). Nuovo rilievo: `copertura_` ridimensionato con `size()!=projs_.size()` (`render/renderer.cpp:188`) mentre nello stesso diff `spriteOrd_` e' grow-only (`:1667`); se `projs_.size()` varia fa resize evitabili. Semplificazione: `if (copertura_.size() < projs_.size())` (extra mai letto).
- Rilievi invariati: `mascheraGraffito` cache per identita puntatore `m.testo==testo` (`:1201,1247`) con ref da `graffiti_` (`:37,1303-1304`); `incrocioSprites_` 2 push_back/tipo, `Sprite*` stabili (reserve 16, `renderer.h:258-271`); reserve fisse/accessor a puntatore; `spriteOrd_`/`terraOrdine_` riallocabili >2048. gate non eseguito.

## Rilievi aperti (sintesi)
- Backlog warning/fragilità: `game.cpp`, `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
