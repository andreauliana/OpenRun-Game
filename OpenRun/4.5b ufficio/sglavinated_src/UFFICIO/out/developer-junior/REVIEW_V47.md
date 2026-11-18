# REVIEW V47 (@developer-junior)
Riscritto 18/11 17:29 (turno VJ5).

## VJ4 soak (completato 17/11)
- lv_01-lv_06: tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash).

## VJ5 ∞ review src — turno 18/11 17:29 (ultimo diff: M `motore_nuovo/src/render/renderer.cpp`)
- Stato: `git diff --stat -- motore_nuovo/src` exit 0 (`1 file changed, 44 insertions(+), 23 deletions(-)`, working tree). `git status --short -- motore_nuovo/src` exit 0 (`M motore_nuovo/src/render/renderer.cpp`). `git diff -- motore_nuovo/src/render/renderer.cpp` exit 0 (ispezionato). Diff: reserve/prealloc, `copertura_` resize condizionale, riuso `distI`/`proj`, grow-only `spriteOrd_` con sort/draw su `nSprite`, ottimizzazioni drawSprite/graffiti.
- Possibile errore/semplificazione (incoerenza high-water-mark): `copertura_` ora fa `if (copertura_.size() != projs_.size()) copertura_.resize(projs_.size());` (`render/renderer.cpp:188`, `copertura_.reserve(Road::DRAW_DIST)` `:26`) ma nello stesso diff `spriteOrd_` e' reso grow-only con `if (spriteOrd_.size() < sprites.size()) spriteOrd_.resize(sprites.size());` (`:1667-1668`). `copertura_` e' indicizzato solo fino a `projs_.size()` (`:191-192,202,237`), quindi se `projs_.size()` oscilla tra frame il resize a scendere e' inutile. Semplificazione: usare `if (copertura_.size() < projs_.size()) copertura_.resize(projs_.size());` per coerenza high-water-mark/zero rialloc evitabili.
- Rilievi invariati: `mascheraGraffito` cache per identita puntatore `m.testo == testo` e salva puntatore (`:1201,1247`), ref da `graffiti_` (`:37,1303-1304`, uso attuale sicuro con letterali `kGraf` `:687-689,703`); `incrocioSprites_.reserve(16)` con 2 `push_back`/tipo (`:46`, `renderer.h:258-266`) e accessor `Sprite*` stabili (`renderer.h:268-271`, usi `game.cpp:559,3682,3843`); reserve fisse/accessor a puntatore per sponsor/oggetto/rivali/traffico (`:33-34,44-45,49-61`, `renderer.h:230-232,239-255,343-345`); `spriteOrd_`/`terraOrdine_` possono riallocare oltre 2048 (`:29-30,173-181,1667`).

## Rilievi aperti (backlog)
- `game.cpp` (`:734-736,1026-1048,3201-3256,3693,3719`), `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `core/json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
- gate_t1.sh non eseguito in questo turno.
