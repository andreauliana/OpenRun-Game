# REVIEW V47 — VJ5 ∞ (26/11/2026, turno 10:27-10:32)

- **Ruolo**: @developer-junior (review/collaudo; nessuna modifica a `src/`).

## Stato del diff in questo turno
- `git status --short`: unico file M in `motore_nuovo/src/` = `render/renderer.cpp`; `renderer.h` invariato (25/11 11:03:33).
- `ls -lT`: `renderer.cpp` mtime 26 Nov **10:11:16** — lo stesso gia' recensito alle 10:13. `git diff --stat` = **91 inserzioni / 28 cancellazioni**, identico al turno prima -> **nessun cambio nuovo in `src/`**.
- Di conseguenza questo turno e' un approfondimento VJ5 ∞ con **angolo diverso** (aritmetica dell'indice e casi di loop vuoto), come gia' fatto il 09:32 per lo stesso hunk.

## VJ5 ∞ — nuovo angolo di analisi sui 3 hunk `drawEffettiAuto` (gia' visti 10:13)
Hunk H segni gomme :3037-3050; hunk I zolle d'erba :3052-3072; hunk J polvere atterraggio :3074-3091.

- **Hunk H — loop vuoto equivalente**: `y0g=max(0,cy+1)`, `y1g=min(cy+4,H-1)`. Se `cy>=H-1` allora `y0g>=H>y1g`; se `cy<-4` allora `y1g<0<=y0g`: in entrambi i casi il ciclo `for(y=y0g;y<=y1g)` non itera. Il vecchio `fb_.put` scartava ogni y fuori da `[0,H-1]`, quindi in entrambi i casi scriveva zero pixel -> **identico**. Quando il ciclo non e' vuoto, `[y0g,y1g]` e' esattamente `[cy+1,cy+4] ∩ [0,H-1]`, cioe' lo stesso insieme di y che `put` accettava.
- **Indice provatamente in bounds**: nell'hunk H `gx` supera il guard `(u32)gx<WIDTH` (quindi `0<=gx<=W-1`) e `y` e' clampato in `[0,H-1]`; dunque `(size_t)y*WIDTH+gx` sta in `[0, H*WIDTH-1] = [0,71679]`. Cast a `size_t` esatto per `y>=0`; 71679 e' molto sotto 2^32 (Pi 400) e 2^64 (Mac) -> **nessun overflow**. Stesso argomento per hunk I (`py`, `py+1`, `px`, `px+1`) e hunk J (`py`, `px`).
- **Hunk I — `px+1`/`py+1` in i32**: `px` ha superato il guard, quindi `px<=W-1` e `px+1<=W` resta representabile in `i32`; il guard `(u32)(px+1)<WIDTH` rigetta correttamente il caso `px=W-1` (x=W), esattamente come lo skip di `put`. Idem per `py+1`.
- **Parita' e colori**: `(y+frame)&1`, `n%3`, `n%4` e i valori colore sono gli stessi operandi/ordine del codice precedente -> stessi pixel. **Resa e replay IDENTICI. Nessun errore tecnico.**

## RILIEVO BLOCCANTE (procedurale, invariato)
- 5 hunk sono **dentro `Renderer::drawPlayerCar`**, dichiarato intoccabile da **REGOLA BLOCCATA (Andrea 06/11)** e `ANDREA_REVIEW.md` §2. Serve autorizzazione esplicita di Andrea o revert dei soli hunk `drawPlayerCar`.
- **File/righe**: `renderer.cpp:2798-2816`, `:2875-2889`, `:2890-2905`, `:2910-2922`, `:2932-2952`.
- **Area grigia**: `drawEffettiAuto` (:2966; chiamata da `drawPlayerCar` :2842 e dal percorso ombra :2627) disegna gli effetti dell'auto del giocatore; i 3 hunk qui analizzati si sommano a cerchio/spruzzi del 10:00. Non e' nell'elenco letterale della regola e la modifica e' bit-identica -> segnalata, non giudicata bloccante; da chiarire con Andrea. L'hunk `drawDetrito` resta fuori area.

## Possibile ottimizzazione (nuova, non bloccante, fuori perimetro — per @developer)
- Hunk H :3042-3048: invece di testare `((y+frame)&1)==0` a ogni y, partire da `y0g+(((y0g+frame)&1)?1:0)` e avanzare di 2. Stessi pixel, un test in meno per riga. Come il gia' segnalato helper `rigaScrivibile(y)` in `framebuffer.h`, va proposto, non applicato dal junior.

## Rilievi invariati (non risolti)
- **VJ3**: 6/6 misurati, 5 entro 2.00%; solo **lv_05_spray 2.40% FALLITO** (deterministico, exit 22) -> aperto per @developer.
- **VJ4**: chiuso 6/6 (soak OK).

## Prove eseguite in questo turno (26/11/2026, turno 10:27-10:32)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` -> **exit 0** (ctest 27/27, 137.94 s; `prova_gate` ricompilato su renderer.cpp 10:11:16).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_sgommata_lv03_foglie_2611_1032" lv_03_foglie curve` -> **exit 0** (GATE CURVE CONTINUE OK; campioni=1199, salto pendenza max=0.02; livello e modo diversi dal turno prima. Nota: esercita l'hunk H sgommata; l'hunk I resta non esercitato perche' la guida da curve resta in strada -> equivalenza dimostrata per analisi del codice).
- `date` -> comando negato dallo strumento; orario riportato dalla richiesta del turno (10:27) e dal mtime della cartella output (10:30).
- `gate_t1.sh`: **non eseguito** in questo turno.
