# REVIEW V47 — VJ5 ∞ + VJ1 (26/11/2026, turno 17:00-17:12)

- **Ruolo**: @developer-junior (review/collaudo; nessuna modifica a `src/`).

## Stato del diff in questo turno
- `git status --short`/`git diff --stat`/`git diff HEAD -- motore_nuovo/src`: **nessun M in `src/`** — `renderer.cpp` (mtime 26 Nov **10:11:16**) resta committato in `c146cb5`, working tree = HEAD; `renderer.h` 25/11 11:03:33, `framebuffer.cpp` 21/11 10:50:13.
- Nessun cambio nuovo -> **approfondimento VJ5 ∞ con angolo nuovo**: copertura dei guard da posizioni estreme (auto/effetti fuori dal framebuffer) e casi limite degli clamp.

## VJ5 ∞ — angolo "posizioni estreme + copertura guard" sui 11 hunk
- **Cerchio effetti :2979-2992**: con `py`/`px` fuori dallo schermo (auto parzialmente fuori, effetti lanciati oltre il bordo) gli clamp danno `yMinC>yMaxC` o `xMinC>xMaxC` -> loop vuoto, esattamente come `put` che saltava ogni pixel; se non vuoto l'intervallo e' l'intersezione `[0,W)x[0,H)` = l'insieme che `put` scriveva.
- **Hunk gomme H :3044-3048**: `gx` fuori da `[0,W)` -> intera colonna skippata (come `put` su ogni y); `cy` estremo -> `y0g>y1g` loop vuoto; y sempre in `[0,H-1]` -> indice in `[0,71679]`.
- **Razzo :3011-3016**: `y` e' gia' filtrato dal `continue` :3013 (`y<120 || y>=HEIGHT`) prima della scrittura, quindi il solo guard `x` (come `put` faceva con entrambi) basta; `x` negativo skippato da `(u32)x<W`; indice `y*W+x` con y in [120,223], x in [0,319] -> max 71679, in i32 senza overflow.
- **drawDetrito :2080-2085 / danni->fumo :2936-2940 / polvere J :3089**: clamp estremi (`y-r<0`, `y+r>=H`, o entrambi) -> intersezione vuota o ridotta, stessi pixel di `put`.
- **fumo sprite :2880-2881 / scintille :2913-2918 / zolle I :3064-3070**: guard per-pixel coprono anche `bx/by` negativi (`(u32)` su negativo = enorme >= bound -> skip), identico allo skip di `put`.
- **Riepilogo: nessun errore; tutti i guard coprono i casi limite con la stessa semantica di `Framebuffer::put` (bounds-check + skip). Resa/replay IDENTICI.**

## RILIEVO BLOCCANTE (procedurale, invariato)
- 5 hunk dentro `Renderer::drawPlayerCar` (:2798-2816, :2875-2889, :2890-2905, :2910-2922, :2932-2952): area intoccabile (REGOLA BLOCCATA Andrea 06/11, ANDREA_REVIEW §2) -> serve autorizzazione esplicita o revert dei soli hunk `drawPlayerCar`.
- Area grigia `drawEffettiAuto` (:2966, chiamata da drawPlayerCar :2842 e :2627): segnalata non bloccante (bit-identica), da chiarire con Andrea.

## Possibili semplificazioni non bloccanti (fuori perimetro, per @developer)
1. Helper `rigaScrivibile(y)` in `framebuffer.h` per lo schema guard+`riga[x]` ripetuto (~20 punti di renderer.cpp).
2. Hunk H: iterare solo gli y di parita' giusta (passo 2) invece di testare `((y+frame)&1)==0` a ogni y.
3. Fumo sprite :2883: hoistare `fb_.data()` e usare `(size_t)` come cerchio :2983 e hunk H :3048 (coerenza; razzo :3016 usa anch'esso i32, in bounds).

## VJ1 — passo V9 lv_05_spray giro40 del @developer (26/11 16:58)
- Passo di sola misura (nessun file cambiato); prova dedicata mia: stesso `frameHash 0 4250821778` del run del @developer -> replay confermato.

## Rilievi invariati (non risolti)
- **VJ3**: 6/6 misurati, 5 entro 2.00%; solo **lv_05_spray 2.40% FALLITO** (deterministico, exit 22) -> aperto per @developer.
- **VJ4**: chiuso 6/6 (soak OK).

## Prove eseguite in questo turno (26/11/2026, 17:00-17:12)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` -> **exit 0** (ctest 27/27, 130.38 s; `prova_gate` ricompilato su renderer.cpp 10:11:16).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_sportellata_lv02_atomico_2611_1705" lv_02_atomico sportellata` -> **exit 0** (GATE SPORTELLATA OK MIG1 J3; nota: il modo forza lv_01_neon; colpo a segno SI a 0.73 s frame 44, rivale 5, urti_gara 2, sbandata 3.00 s, ripresa dopo 5.00 s; livello/modo mai usati in questi turni).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj1_v9giro40_lv05_spray_2611_1708" lv_05_spray smoke` -> **exit 0** (GATE OK, VJ1; frameHash 0 4250821778 identico al @developer, replay 2x IDENTICO, offroad frame 156, medio 0.588 max 2.814 ms/frame).
- `gate_t1.sh`: **non eseguito** in questo turno.
