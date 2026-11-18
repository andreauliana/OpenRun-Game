# REVIEW V47 (@developer-junior)
Riscritto 18/11 12:56 (turno VJ5).

## VJ4 soak (completato 17/11)
- 17/11: soak lv_01-lv_06 tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash). gate_t1.sh non eseguito.

## VJ5 ∞ review src — stato attuale (working tree)
- 18/11 12:56: `motore_nuovo/src/render/renderer.cpp:29-30` (`Renderer::Renderer`) reserve fisse `terraOrdine_.reserve(512)`, `spriteOrd_.reserve(1024)`. `drawRoad` (`:157-169`) fa `push_back` su `terraOrdine_` fino a oggetti terra visibili: se >512 rialloca nel ciclo (V7). `drawSprites` (`:1650`) fa `spriteOrd_.resize(sprites.size())`: se `sprites.size()` > capacity corrente rialloca; con base 1024 basta una scena più grande per rialloc silenziosa. Semplificazioni: in `Renderer::impostaOggettiTerra` (`renderer.h:177`, chiamato da `game.cpp:514`) riservare `terraOrdine_` da `oggettiTerra_->size()` (o cap+log); prima di `resize` in `drawSprites` assicurare `spriteOrd_.capacity() >= sprites.size()` (reserve/log una tantum o cap documentato).
- 18/11 12:45: stesso rilievo (diff locale) sintetizzato sopra; invariato allo stato attuale.
- Cumulativo (note precedenti): `game.cpp:734-736` skyline senza warning raw mancante/vuoto; `:1037-1046` asset opzionali senza warning; `:3201-3204,3254-3256` warning folla/supporto solo indice 0; `:1026-1048` warning solo `auto_dritta`; `:1032` `(POSE[k]+5)` fragile.

## Rilievi aperti (sintesi)
- `game.cpp:3719,3693` div/`a.passo`; `:1645,1673,1678,1679` `trafficoAt` non clampato; `:1809-1814` bravura/curva.
- `mixer.cpp:91-99`; `salvataggi.cpp:78-83/:145-147/:108-109`; `fasi.cpp:512-536`.
- `gameplay/escalation.h:24,35,62`; `core/json_mini.h` BOM/`0x`/dup; `replay.cpp:20-21`; `paesaggio.cpp:113-114/:238-239`. VJ3 chiuso.

## Verifica turno 18/11 12:56
- Nessun comando eseguito (review sola lettura). Precedente: `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 — ctest 27/27 Passed (297.30 s, 18/11 12:45). gate_t1.sh non eseguito.