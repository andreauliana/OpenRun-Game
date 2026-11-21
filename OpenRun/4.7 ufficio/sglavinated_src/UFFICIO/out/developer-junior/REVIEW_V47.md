# REVIEW V47 — VJ5 ∞ review dell'hoist dei riflessi delle onde (21/11/2026 11:50)

- **Ruolo**: @developer-junior (review del codice del @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞ — ultimo cambio del @developer in `src/`. `git status --short`: unico M in `src/` e' `motore_nuovo/src/render/renderer.cpp` (gli altri M sono documenti di coordinamento); `git diff -- motore_nuovo/src/render/renderer.cpp` exit 0. Il diff ha cinque hunk gia' noti (`sogliaBayerSfuma`, `shiftSchiuma` 11:13, `hshFila` 10:29, `limiteStrisce` 11:37, `incStrisce` 11:26); il NUOVO in questo turno e' l'ultimo, segnato "V9 ∞ (21/11, lv_01_neon giro 28)". Livello/file diverso dal turno prima (`lv_06_desktop`/`drawSegmentTrapezoid` -> `lv_01_neon`/`drawTerraSospesa`).

## VJ5 — review di `renderer.cpp`, hunk "V9 ∞ (21/11, lv_01_neon giro 28)"
- Cambio nuovo in `drawTerraSospesa` (`renderer.cpp:331-348`): il loop dei riflessi sul mare scriveva `fb_.put(x, y, riflesso)` per pixel; ora il guard di y e' fuori dal ciclo (:337), la base della riga `fb_.data() + (size_t)y*WIDTH` e' calcolata una volta (:338) e si scrive `rigaOnde[x] = riflesso` (:346). Stessa idea degli hoist `incStrisce`/hline segnalati in passi precedenti.
- **Nessun errore trovato**: il loop `for (x=0; x<WIDTH; ++x)` (:339) garantisce x in [0,319], quindi i controlli x di `put` (`x<0 || x>=WIDTH`, framebuffer.cpp:18) erano sempre falsi; il guard `y>=0 && y<HEIGHT` (:337) e' esattamente il controllo y di `put` (framebuffer.cpp:18); `data()` (framebuffer.h:26) restituisce la stessa `buf_` che `put` scrive, e `(size_t)y*WIDTH+x` induce lo stesso indice (max 223*320+319=71679 < 71680, nessun overflow). La riga e' gia' dipinta da `fb_.hline(0,WIDTH-1,y,acqua)` (:314) subito sopra, come dice il commento. I calcoli `xw`/`cellaF`/`cella`/`hsh`/`dentro` (:340-345) sono puri e senza effetti collaterali: saltarli quando y e' fuori schermo non cambia stato. Resa e replay IDENTICI, coerente con `hshFila` (:330), `sogliaBayerSfuma` (:286) e `shiftSchiuma` (:292).
- **Semplificazione (con riga)**: `renderer.cpp:331-336` — commento di 6 righe per due righe di codice; accorciabile a 1-2 righe nello stile degli altri commenti V9.
- **Nota sul guard**: `a0 = max((i32)next.acquaY, orizzonte_)` e `a1 = min((i32)cur.acquaY, HEIGHT-1)` (:269-270); `orizzonte_` e' ~112 (HORIZON_Y, renderer.h:399, + beccheggio a :91), quindi y e' normalmente in [~112,223] e il guard non scatta mai. E' ridondante ma innocuo e conserva la semantica esatta di `put` se in futuro i limiti cambiassero. Non funzionale.
- **Rilievo documentale persistente**: il commento all'hoist `zAvanti` (`renderer.cpp:460`) cita ancora "a riga 408 (dichiarazione)"; la dichiarazione e' ora a :464 (:408 e' `grassQui`). Segnalato piu' volte, fuori da questo hunk e non tocca codice eseguito.

## Prove (eseguite in questo turno, 21/11 11:50)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 142.73 s; `prova_gate` ricompilato prima della prova, come richiede VJ5).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv01_neon_onde_riga_2111" lv_01_neon percorso` **exit 0** (GATE PERCORSO OK; 4540 segmenti, giro guidato 1989 frame = 33.2 s @ 60 Hz, fuori strada 0, caricamento 6.8 ms, init 14.8 ms).
- `gate_t1.sh` non eseguito questo turno.

## Stato
- VJ4 soak chiusa 6/6. VJ5 ∞ prosegue: prossimo turno rileggere l'ultimo cambio `src/` se cambia di nuovo (livello/file diverso da `lv_01_neon`/`drawTerraSospesa`). ctest 27/27 verde qui; la flakiness replay nei sottmodi smoke segnalata il 21/11 10:20 resta sotto osservazione (qui non riparsa).
