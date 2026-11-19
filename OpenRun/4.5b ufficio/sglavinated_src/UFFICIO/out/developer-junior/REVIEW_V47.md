# REVIEW V47 (@developer-junior)
Riscritto 19/11/2026 ~08:35 (turno 2, passo VJ5).

## VJ4 soak (completato 17/11)
- lv_01-lv_06: tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash).

## VJ5 ∞ review src — turno 19/11 08:35 (ultimo diff: M `render/renderer.cpp` 18+/4-, M `render/renderer.h` 6+; `framebuffer.cpp` invariato dal turno 08:15)
- Diff: in `Renderer::drawSprite` tx0/tx1 della colonna schermo (e lo specchio) sono precalcolati una volta per colonna in `colTx0_`/`colTx1_` (`renderer.cpp:1784-1793`, membri `renderer.h:497-498`); il ciclo riga legge l'array (`renderer.cpp:1800-1803`) invece di fare due divisioni a 64 bit per pixel. (`git status --short`, `git diff --stat -- motore_nuovo/src`, `git diff -- motore_nuovo/src` exit 0.)
- Esito: nessun errore trovato. Indici entro i limiti: `sxIni >= max(0,-x0)` e `sxFin <= min(w, WIDTH-x0)` (`renderer.cpp:1759-1766`), quindi `nCol = sxFin - sxIni <= WIDTH` e `c in [0,nCol)` sta dentro `i32 colTx0_[Framebuffer::WIDTH]`; l'eventuale return `:1766` arriva prima del precalc (nessun uso di colonne non scritte). Formula identica alla vecchia, stesso ordine (clamp `tx1 <= tx0` poi specchio, `:1787-1790`): stessi texel, stessa immagine.
- Nota di metodo: il replay A/B del gate confronta due run dello STESSO binario (`prova_gate.cpp:1089`), quindi non puo' rilevare un cambio di resa introdotto da un passo; `frameHash` copre il framebuffer (`game.cpp:3895-3903`) ma non esiste un hash di riferimento del build precedente, per cui "stessa immagine" resta dimostrato solo dall'ispezione. Utile un frameHash salvato del build prima/dopo per i passi di resa.
- Semplificazione (stesso pattern non coperto): `drawLuceGalleria` fa una divisione intera per pixel `x * s->width / w` (`renderer.cpp:1385`; `ty` e' gia' fuori dal ciclo a `:1381`) — luce di galleria, presente in lv_02 e lv_05. Precalcolare il texel per colonna come in `drawSprite` toglie quell'idiv; guadagno piccolo (sprite piccolo), da misurare con V9 prima di toccarlo.
- Misura: smoke lv_02 ms/frame medio 0.988 / max 3.558, sotto la soglia 8 ms di V9: per la regola "se e' gia' sotto, scrivilo" il passo su lv_02 non era necessario. Nessun run "prima" disponibile in questo turno (ricostruire il codice pre-cambio significherebbe toccare l'albero del @developer), quindi il delta del passo resta non misurato.
- Prove di questo turno (19/11, ~08:36-08:45): `sh UFFICIO/Officina/TOOL/run_tests.sh` -> exit 0 (build `prova_gate` + ctest **27/27 pass**, 295.08 s); `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_1911" lv_02_atomico percorso` -> exit 0 (**GATE PERCORSO OK**, 2476 frame, fuori strada 0); `... vj5_lv02_1911 lv_02_atomico smoke` -> exit 0 (**GATE OK**, medio 0.988 max 3.558 ms, replay 2x IDENTICO, avvolgimento e inattivita' IDENTICO). `gate_t1.sh` non eseguito in questo turno.

## VJ5 turno precedente (19/11 08:15, `framebuffer.cpp`)
- `hline`/`fillRect` con base della riga calcolata una volta: nessun errore (clamp `framebuffer.cpp:22-25,34-39` invariati, `y` validato, stessi pixel). Semplificazione aperta: `hline` (`:21-31`) duplica il clamp di `fillRect` (`:33-45`) -> delegare con misura V9; delta del @developer (-0.018 ms) dentro il rumore (mia run 1.081/3.457). Rilievo minore: commento `:26,41` "turno30" vs "ventunesimo giro".

## Rilievi aperti (invariati 18/11)
- `mascheraGraffito` cache per identita puntatore `m.testo == testo` (`renderer.cpp:1201,1247`, ref da `graffiti_` `:37,1303-1304`); `incrocioSprites_.reserve(16)` con 2 `push_back`/tipo e accessor `Sprite*` stabili (`renderer.h:258-271`, usi `game.cpp:3682,3843`); reserve fisse/accessor a puntatore sponsor/oggetto/rivali/traffico; `spriteOrd_`/`terraOrdine_` riallocabili oltre 2048 (`renderer.cpp:29-30,173-181,1667`).

## Rilievi aperti (backlog)
- `game.cpp` (`:734-736,1026-1048,3201-3256,3693,3719`), `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `core/json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
