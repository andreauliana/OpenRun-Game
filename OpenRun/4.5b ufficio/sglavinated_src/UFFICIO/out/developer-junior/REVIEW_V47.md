# REVIEW V47 (@developer-junior)
Riscritto 19/11/2026 ~09:50 (turno 4, passo VJ5).

## VJ4 soak (completato 17/11)
- lv_01-lv_06: tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash).

## VJ5 ∞ review src — turno 19/11 09:32-09:50 (ultimo diff: M `render/renderer.cpp` 19+/7-, unico file src)
- Diff: in `Renderer::drawSprite` il lambda `texel(tx,ty)` e' diventato `coloreIdx(idx)` (`renderer.cpp:1772-1777`) e la riga sorgente viene calcolata una volta per riga schermo: `const u8* rigaTex = px` (`:1782`), `rigaTex = px + (size_t)ty*sw` (`:1804`), uso `coloreIdx(rigaTex[(tx0+tx1)/2])` (`:1814`); nel fallback anti-sparizione `rigaCerca = px + (size_t)ry*sw` (`:1822-1825`). Prima `px[ty*sw + tx]` a ogni texel. (`git status --short`, `git diff --stat -- motore_nuovo/src`, `git diff -- motore_nuovo/src` exit 0.)
- Esito: nessun errore. Stessi indici: `rigaTex[(tx0+tx1)/2] == px[ty*sw + (tx0+tx1)/2]` e `rigaCerca[rx] == px[ry*sw + rx]`; corpo del lambda identico a `texel` (`spPal[idx % nPalette]` o `idx==0 ? 0 : pal_[paletteBase+idx]`), `spPal` resta nullptr se la palette e' vuota → nessuna divisione per 0. Bounds: `tx0 <= sw-1` (con specchia `tx1 <= sw`, `tx0 >= 0`) e `(tx0+tx1)/2 <= (2*sw-1)/2 = sw-1`, `rx < tx1 <= sw`, `ty < sh` → nessun accesso fuori `pixels`. Scrittura `riga[x0+sx]` e clipp invariati (review turno precedente).
- Semplificazione (piccola): `const u8* rigaTex = px` (`renderer.cpp:1782`) e' un'inizializzazione morta — la variabile e' riassegnata a `:1804` prima di ogni uso a `:1814` — si puo' dichiarare dentro il ciclo sy (`const u8* rigaTex = px + (size_t)ty * sw;`), togliendo la variabile mutabile fuori ciclo.
- Prove di questo turno (19/11 09:35-10:02): `sh UFFICIO/Officina/TOOL/run_tests.sh` -> exit 0 **due volte**: 09:35 pre-modifica test (ctest 27/27 pass, 275.25 s) e 10:02 sul binario finale col nuovo `msMaxFrame` (ctest **27/27 pass**, 289.97 s); `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv04_1911" lv_04_cromo percorso` -> exit 0 (**GATE PERCORSO OK**, 2125 frame, fuori strada 0); smoke lv_04 (`vj5_lv04_1911c/d`) -> exit 0 (**GATE OK**, medio 1.597/1.142 ms, max 9.728 frame 230 / 3.614 frame 0, replay 2x IDENTICO + avvolgimento/inattivita' IDENTICO). Prima del fix due run sotto carico di build avevano max 21.257/22.894 ms: col nuovo indice del picco si vede che non si riproduce a sistema tranquillo (stessi frameHash di tutti gli altri run) -> rumore di macchina, non il codice. `gate_t1.sh` non eseguito.
- Nota VJ1: il passo V9 lv_04_cromo e' coperto da percorso + smoke lv_04 sopra; il limite dichiarato 20 ms e' controllato solo sulla media (`prova_gate.cpp:4885`), il max resta informativo.

## VJ5 turni precedenti (19/11)
- 09:05 `renderer.cpp` (6+/1-): scrittura diretta `riga[x0+sx] = col` (`:1804,1823`) al posto di `fb_.put`: nessun errore (clipp `:1757-1766` garantiscono i range, `put` era bounds+scrittura opaca); residuo: sicurezza tutta nei clipp (eventuale assert). Semplificazioni: base riga fuori dal ciclo sy + offset `x0+sxIni`. Prove: run_tests 27/27, percorso lv_03 2392 frame, smoke lv_02 medio 1.011 max 4.045.
- 08:45 `renderer.cpp` tx per colonna (`colTx0_`/`colTx1_`, `:1784-1793`, `renderer.h:497-498`): nessun errore (`nCol <= WIDTH`, return `:1766` prima del precalc, formula identica -> stessi texel; replay A/B stesso binario, resa garantita dall'ispezione). Semplificazione aperta: `drawLuceGalleria` idiv per pixel (`:1385`).
- 08:15 `framebuffer.cpp` base riga in hline/fillRect: nessun errore; `hline` (`:21-31`) duplica il clamp di `fillRect` (`:33-45`) -> delegare con misura V9.

## Rilievi aperti (invariati 18/11)
- `mascheraGraffito` cache per identita puntatore `m.testo == testo` (`renderer.cpp:1201,1247`, ref da `graffiti_` `:37,1303-1304`); `incrocioSprites_.reserve(16)` con 2 `push_back`/tipo e accessor `Sprite*` stabili (`renderer.h:258-271`, usi `game.cpp:3682,3843`); reserve fisse/accessor a puntatore sponsor/oggetto/rivali/traffico; `spriteOrd_`/`terraOrdine_` riallocabili oltre 2048 (`renderer.cpp:29-30,173-181,1667`); `copertura_` resize di pari passo (`:188`) -> high-water-mark.

## Rilievi aperti (backlog)
- `game.cpp` (`:734-736,1026-1048,3201-3256,3693,3719`), `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `core/json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
