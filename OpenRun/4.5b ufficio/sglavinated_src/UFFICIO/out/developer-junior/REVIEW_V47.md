# REVIEW V47 (@developer-junior)
Riscritto 19/11/2026 ~12:18 (turno 11, passo VJ5).

## VJ4 soak (completato 17/11)
- lv_01-lv_06: tutti exit 0 (GATE OK, 30000 frame, replay 2x IDENTICO, nessun crash).

## VJ5 ∞ review src — turno 19/11 ~12:18 (ultimo diff: M `render/renderer.cpp`)
- Diff: `motore_nuovo/src/render/renderer.cpp` (10+/1-, `git diff --stat -- motore_nuovo/src/render/renderer.cpp`): in `Renderer::drawSprite` la base della ricerca del texel pieno `rigaCercaBase = px + (size_t)ty0 * sw` e' calcolata una volta per riga schermo fuori dal ciclo colonne (`:1815-1820`) e riusata come `rigaCerca = rigaCercaBase` nel fallback trasparenza (`:1857-1859`), al posto di `px + (size_t)ty0 * sw` ricalcolato per ogni colonna trasparente. Nessuna modifica a `renderer.h`.
- Esito: nessun errore. `ty0`/`ty1` dipendono solo da `sy` (`:1810-1812`) e sono costanti nel ciclo colonne (`:1842+`); la sequenza di righe sorgente resta identica (`rigaCercaBase, rigaCercaBase+sw, ...` = `px+ty0*sw, px+(ty0+1)*sw, ...` per `ry in [ty0,ty1)`, `:1860-1862`), con stessi `rx in [tx0,tx1)` (`:1843-1844`) e stessa condizione di ricerca -> resa e replay invariati.
- Semplificazione: `renderer.cpp:1852` `if ((col >> 24) == 0 && (*pCerca || cercaRiga))` valuta/carica `*pCerca` anche quando `cercaRiga` e' gia' true; riordinare in `(cercaRiga || *pCerca)` cortocircuita il load/test del flag per-colonna quando la riga copre >1 texel sorgente (`ty1-ty0>1`, `:1832`), con identico valore logico.
- Semplificazioni aperte: init morta `const u8* rigaTex = px` `:1782` (sovrascritta `:1814`); `screenYRow` duplica `y0+sy` usato solo in `inTubo` `:1866` (`:1806-1808`); valutare drop di `colCerca_` in favore di `tx1-tx0>1` dai locali `:1843-1844` (misurare V9).
- Prove di questo turno (19/11 ~12:18): `sh UFFICIO/Officina/TOOL/run_tests.sh` -> exit 0 (ctest **27/27 pass**, 270.45 s). `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_1911t11" lv_02_atomico smoke` -> exit 0 (**GATE OK**, medio 1.096 ms, max 34.869 ms frame 97, replay 2x IDENTICO + avvolgimento/inattivita' IDENTICO, caricamento 4.5-6.3 ms / init 10.6-14.7 ms). `gate_t1.sh` non eseguito.

## VJ5 turni precedenti (19/11, sintesi)
- 11:55 t10: review diff colCerca_ invariato, reorder suggerito. 11:40 t9: introdotto/validato `colCerca_`. 11:18 t8: `screenYRow`/`riga` incrementali. 10:57 t7: `cercaRiga` per riga. 10:35 t6: base `rigaCerca` fuori da ry. 10:10 t5: `colTxMid_`. 09:50 t4: `coloreIdx`+`rigaTex`. 09:05 t3: scrittura diretta. 08:45 t2: `colTx0_`/`colTx1_`. 08:25 t1: base riga framebuffer.

## Rilievi aperti (invariati)
- `mascheraGraffito` cache per identita puntatore `m.testo == testo` (`renderer.cpp:1201,1247,1303-1304`), reserve 8; `incrocioSprites_` 2 push_back/tipo con accessor `Sprite*` stabili (`renderer.h:258-271`); `spriteOrd_`/`terraOrdine_` riallocabili oltre 2048 (`renderer.cpp:29-30,173-181,1667`); `hline` duplica clamp di `fillRect` (`framebuffer.cpp:21-45`); `drawLuceGalleria` idiv per pixel (`renderer.cpp:1385`); `copertura_` resize di pari passo (`:188`).

## Rilievi aperti (backlog)
- `game.cpp`, `mixer.cpp:91-99`, `salvataggi.cpp`, `escalation.h`, `core/json_mini.h`, `replay.cpp`, `paesaggio.cpp`.