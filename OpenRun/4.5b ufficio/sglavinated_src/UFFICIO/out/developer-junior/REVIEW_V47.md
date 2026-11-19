# REVIEW V47 (@developer-junior)
Riscritto 19/11/2026 ~11:40 (turno 9, passo VJ5).

## VJ4 soak (completato 17/11)
- lv_01-lv_06: tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash).

## VJ5 ∞ review src — turno 19/11 11:31-11:40 (ultimo diff: M `render/renderer.cpp` + `renderer.h`, nuovo: `colCerca_` precalcolata per colonna)
- Diff: in `Renderer::drawSprite` il termine di colonna `tx1 - tx0 > 1` e' invariante per colonna ed e' precalcolato: `colCerca_[c] = tx1 - tx0 > 1;` nel ciclo colonne (`renderer.cpp:1804`, commento `:1801-1803`), uso nella condizione di ricerca `if ((col >> 24) == 0 && (colCerca_[c] || cercaRiga))` (`:1838`) al posto di `tx1 - tx0 > 1 || ty1 - ty0 > 1` a ogni pixel; nuovo membro `bool colCerca_[Framebuffer::WIDTH];` (`renderer.h:503-507`). Il diff cumulato contiene anche i passi 6-8 gia' revisati (`cercaRiga` `:1826`, base `rigaCerca` `:1843`, `screenYRow`/`riga` `:1806-1808`). (`git status --porcelain`, `git diff --stat`, `git diff -- motore_nuovo/src/render/renderer.cpp motore_nuovo/src/render/renderer.h` exit 0; unici src modificati: `renderer.cpp`, `renderer.h`.)
- Esito: nessun errore. Equivalenza: il ciclo colonne (`:1789-1805`) scrive `colTx0_`/`colTx1_` (`:1795-1796`) e `colCerca_` (`:1804`) dagli stessi `tx0`/`tx1` calcolati a `:1791-1793` con lo specchio gia' applicato (`:1794`), mentre il ciclo pixel rilegge proprio `colTx0_[c]`/`colTx1_[c]` (`:1829-1830`) -> stesso bool, stessa decisione; lo specchio non cambia la differenza (`sw-tx1'` - `sw-tx0'`) e dopo il fix `tx1 = tx0+1` (`:1793`) vale anche mirrorato -> `colCerca_` false solo per area di 1 texel, come prima. Bounds: `c` va su `[0, nCol)` con `nCol = sxFin - sxIni <= WIDTH` (`:1759-1760,1788`) -> indice in bounds, stesso invariante di `colTx0_`/`colTx1_`/`colTxMid_`; il return `:1766` e' PRIMA del ciclo colonne -> tutti gli `nCol` elementi scritti prima di ogni lettura `:1838`. `cercaRiga` (`:1826`) resta identico al turno 7 -> OR binario identico a prima, replay identico. Costo: +320 byte membro, uno store booleano per colonna in meno di un sub+cmp per pixel.
- Semplificazioni: (1) NUOVA: `colCerca_` (`renderer.h:503-507`, assegnazione `renderer.cpp:1804`) duplica `colTx1_[c] - colTx0_[c] > 1` e il ciclo pixel carica gia' `tx0`/`tx1` (`:1829-1830`) -> si puo' droppare il membro e tenere `const bool cercaCol = tx1 - tx0 > 1;` da quelli locali, costo +1 sub+cmp per pixel (misurare col gate V9 prima di tenere); (2) NUOVA: riordinare la condizione `:1838` in `cercaRiga || colCerca_[c]` -> quando la riga ha gia' area > 1 texel (tipico con sprite rimpicciolito) salta il load del membro; (3) aperta dal turno 8: `screenYRow` duplica `y0 + sy` (uso solo in `inTubo` `:1850`) -> passare `y0 + sy` li e tenere solo l'incremento di `riga` (`:1806-1808`); (4) aperta dal turno 6: base `rigaCerca = px + ty0*sw` resta dentro il ciclo sx (`:1843`) -> `rigaCerca0` accanto a `rigaTex` (`:1814`); (5) aperta dal turno 4: init morta `const u8* rigaTex = px` (`:1782`), riassegnata a `:1814`.
- Prove di questo turno (19/11 11:31-11:40): `sh UFFICIO/Officina/TOOL/run_tests.sh` -> exit 0 (ctest **27/27 pass**, 262.19 s); `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv05_1911t2" lv_05_spray smoke` -> exit 0 (**GATE OK**, medio 0.975 ms, max 3.768 ms frame 0, replay 2x IDENTICO + avvolgimento/inattivita' IDENTICO, accelerazione reattiva, offroad frame 156, caricamento livello 3.2-4.4 ms / init 8.9-15.7 ms). `gate_t1.sh` non eseguito.
- Nota VJ1: il passo V9 lv_05_spray (giro 22, `colCerca_`) e' coperto dallo smoke lv_05; media 0.975 ms ben sotto 8 ms / 12 ms.

## VJ5 turni precedenti (19/11)
- 11:18 puntatore riga incrementale `screenYRow`/`riga` (diff 25 righe): nessun errore (bounds `:1757-1766`, stesso indirizzo di prima); semplificazioni `screenYRow` e `tx1-tx0>1` (poi fatte dal @developer).
- 10:57 `cercaRiga = ty1 - ty0 > 1` fuori dal ciclo sx (13+/3-): nessun errore (ty0/ty1 solo da sy).
- 10:35 base `rigaCerca` fuori dal ciclo ry (6+/2-): nessun errore (stessi indici, early-exit non riletto).
- 10:10 `colTxMid_` (6+/1- + `renderer.h` 4+): nessun errore; semplificazioni: init morta `rigaTex` e base `rigaCerca`.
- 09:50 `texel(tx,ty)` -> `coloreIdx(idx)` + `rigaTex` per riga schermo (19+/7-): nessun errore; init morta `rigaTex = px`.
- 09:05 scrittura diretta `riga[x0+sx] = col` (6+/1-): nessun errore (clipp `:1757-1766`); base riga fuori dal ciclo sy.
- 08:45 tx0/tx1 per colonna in `colTx0_`/`colTx1_` (18+/4- + `.h`): nessun errore (`nCol <= WIDTH`, return prima del precalc); semplificazione aperta: `drawLuceGalleria` idiv per pixel (`renderer.cpp:1385`).
- 08:25 `framebuffer.cpp` base riga in hline/fillRect (9+/3-): nessun errore; `hline` (`:21-31`) duplica il clamp di `fillRect` (`:33-45`) -> delegare con misura V9.

## Rilievi aperti (invariati 18/11)
- `mascheraGraffito` cache per identita puntatore `m.testo == testo` (`renderer.cpp:1201,1247`, ref da `graffiti_` `:37,1303-1304`); `incrocioSprites_.reserve(16)` con 2 `push_back`/tipo e accessor `Sprite*` stabili (`renderer.h:258-271`, usi `game.cpp:3682,3843`); reserve fisse/accessor a puntatore sponsor/oggetto/rivali/traffico; `spriteOrd_`/`terraOrdine_` riallocabili oltre 2048 (`renderer.cpp:29-30,173-181,1667`); `copertura_` resize di pari passo (`:188`) -> high-water-mark.

## Rilievi aperti (backlog)
- `game.cpp` (`:734-736,1026-1048,3201-3256,3693,3719`), `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `core/json_mini.h`, `replay.cpp`, `paesaggio.cpp`.
