# REVIEW V47 — VJ5 ∞ scrittura diretta `riga[x]`/`rigaSoff[x]` in galleria (23/11/2026 07:04)

- **Ruolo**: @developer-junior (review codice @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞. `git status` / `git diff -- motore_nuovo/src/render/renderer.cpp`: unico M in src. Ultimi cambi non ancora rivisti = le scritture dirette nel framebuffer al posto di `fb_.put` nelle due lambda della galleria (`parete`, `soffittoRiga`), commenti "V9 ∞ 23/11, lv_06_desktop giro 31"; gli altri hunk del diff (`segnaTubo(a0,b0,y)`, `yf`, `ceilDelta`/`terraDelta`, riuso soffitto) sono gia' rivisti (06:02/06:17/06:33/06:47).

## VJ5 — review `renderer.cpp` parete/soffitto galleria, scrittura diretta
- **File/riga**: `motore_nuovo/src/render/renderer.cpp:911` (`u32* riga = fb_.data() + (size_t)y * Framebuffer::WIDTH;` in `parete`), usi :967 (`riga[x] = tuboColore`, ramo tubo) e :987 (`riga[x] = c`, fondo loop); `soffittoRiga` :1003 (`u32* rigaSoff`) e :1020 (`rigaSoff[x] = c`). Prima: `fb_.put(x, y, colore)` in tutti e tre i punti.
- **Nessun errore comportamentale**: `Framebuffer::put` (`framebuffer.cpp:17-20`) fa solo `if (x<0 || x>=WIDTH || y<0 || y>=HEIGHT) return; buf_[y*WIDTH+x]=color;`. La scrittura diretta e' `buf_[y*WIDTH+x]=color` (`data()` e' `buf_.data()`, `framebuffer.h:25-26`) — identica se le coordinate sono in bounds. E' il caso: il loop e' `for (i32 y=y0; y<=y1; ++y)` con `y0=max(0,(i32)ceilN)>=0` e `y1=min(HEIGHT-1, ny-1)<=HEIGHT-1` (:797-798), e in `parete`/`soffittoRiga` `a0=max(0,(i32)xa)>=0`, `b0=min(WIDTH-1,(i32)xb)<=WIDTH-1` (:905, :1004), ciclo `x` in [a0,b0]. Quindi i guard di `put` sono sempre false e il bounds-check e' ridondante: stessi pixel, stessi valori, stessa resa. Se l'intervallo e' vuoto (a0>b0 o y1<y0) i loop non girano in entrambe le forme -> no-op. `segnaTubo(a0,b0,y)` resta immutato e con i suoi clamp (:990, :1022), `tubo_` bit-identico. `frameHash` invariato, resa e replay IDENTICI. Vero risparmio: si tolgono 4 guard per pixel nelle pareti/soffitti della galleria; lo stesso schema `u32* riga = fb_.data() + ...` e' gia' usato altrove nel file (:307, :320, :531, :636, :780, :2135) e in `Framebuffer::hline` — coerenza di stile.
- **Semplificazione**: nessuna ulteriore. Nota minore: i commenti :906-910 e :1004-1006 (5 e 3 righe) sono lunghi per una riga di codice, coerenti pero' con lo stile del file; il commento :1028-1031 (4 righe) e' altrettanto verboso. Rilievo doc persistente :495 "riga 408" (dichiarazione `zAvanti` :499), fuori hunk, gia' segnalato piu' volte.

## Prove (23/11 07:04, eseguite in questo turno)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — **exit 0** (ctest **27/27**, 159.95 s, prova_gate ricompilato, `gate_decorazioni` passa).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_atomico_direttawrite_2311_t1" lv_02_atomico percorso` — **exit 0** (GATE PERCORSO OK, 2476 frame/41.3 s @ 60 Hz, fuori strada 0; lv_02 ha galleria -> esercita il codice cambiato, diverso da lv_05_spray del turno prima).
- `gate_t1.sh` non eseguito.

## Stato
- VJ5 ∞ prosegue: prossimo turno rileggere l'ultimo cambio `src/` (hunk/file diverso da parete/soffitto direct-write). Nessun fail aperto noto: ctest 27/27 verde.
