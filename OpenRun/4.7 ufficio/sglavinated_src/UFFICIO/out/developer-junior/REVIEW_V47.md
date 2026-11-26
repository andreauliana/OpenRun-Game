# REVIEW V47 — VJ5 ∞ (hunk nuovo 26/11 09:05:44, drawLuceGalleria) (26/11/2026, turno 09:07)

- **Ruolo**: @developer-junior (review/collaudo; nessuna modifica a `src/`).

## VJ5 ∞ — hunk nuovo in `renderer.cpp` (mtime 26/11 09:05:44)
- Unico file M in `src/` = `renderer.cpp`; `renderer.h` invariato 25/11 11:03:33. Nuovo rispetto al turno delle 08:53:41 (rivisto alle 09:00): `Renderer::drawLuceGalleria()` :1837-1870 — plafoniere del tunnel, commento "V9 lv_02_atomico giro39". Gli altri hunk del diff (`drawOggettoTerra` absGiro :1319-1333, `drawImboccoGalleria` :1934/:1957/:1978, `drawRoadsideMarkers` colPy :2051-2058, `drawDetrito` hw2/hh2/topLy :2070-2085, tre `drawPlayerCar` :2646/+2730) sono gia' rivisti (08:01/08:55/09:00/07:58); lo spostamento di righe +10 e' esattamente quello del nuovo hunk -> nessun hunk precedente ritoccato.
- Vecchio: doppio loop `y` in [0,h), `x` in [0,w) con `if (py<0||py>=H) continue` / `if (px<0||px>=W) continue`, indice `s->pixels[ty*s->width + x*s->width/w]`, `fb_.put`.
- Nuovo: clamp dei limiti a [0,W-1]x[0,H-1] (`xA/xB/yA/yB` + early return se fuori), `riga = fb_.data()+py*WIDTH`, `src = s->pixels.data()+ty*s->width`, `riga[px] = col`, `palS/palSN` hoistati.
- **Correttezza tecnica: nessun errore, bit-identico.** Verificato `framebuffer.cpp:17-20` (`put` = bounds-check + `buf[y*WIDTH+x]`, fuori range = skip, nessuna write) e `framebuffer.h:25-29` (`data()` = lo stesso `buf_`, row-major, stride WIDTH). Il clamp riproduce esattamente l'insieme di pixel scritti da `put`: quelli fuori [0,W-1]x[0,H-1] venivano scartati, ora non vengono iterati. `x = px-x0` in [0,w-1] e `ty` in [0,h-1] (y=py-y0) -> `src[x*s->width/w]` e' lo stesso indice di prima e resta in bounds. `usaPalettaSprite/palS/palSN` replicano `!palette.empty()/data()/size()`; l'operatore ternario non valuta il ramo non scelto -> niente `% 0` con paletta vuota. Stessi operandi/ordine in `ty`, `idx`, `c`, `lerpColore` -> stessi pixel, resa/replay IDENTICI. Casi limite: `w<=0` (xB<xA, loop vuoto, come il vecchio `for x<w`), sprite vuoto (early return `drawLampada`, ramo non toccato), coordinate fuori schermo (early return = zero write).
- **Non tocca l'area bloccata**: `drawLuceGalleria` e' illuminazione della galleria, non `drawPlayerCar` -> nessun nuovo rilievo procedurale da REGOLA BLOCCATA.
- Nit non bloccanti: clamp e `palS/palSN` calcolati anche se il loop non itera; commento :1854-1855 dice "come drawPlayerCar" ma e' solo lo schema direct-write; guadagno reale solo dal `put` inline.
- **RILIEVO BLOCCANTE procedurale (invariato)**: i tre hunk `drawPlayerCar` elencati sopra toccano area intoccabile (REGOLA BLOCCATA Andrea 06/11, `ANDREA_REVIEW.md` §2) -> serve autorizzazione esplicita o revert; il nuovo hunk `drawLuceGalleria` e' fuori area.

## VJ3 — 50/60 Hz (resta aperto per @developer)
- 6/6 livelli misurati: 5 dentro il limite 2.00%, solo **lv_05_spray 2.40% FALLITO** (60 Hz 48.67 s / 50 Hz 47.50 s, ripetuto identico -> deterministico, exit 22).

## Prove eseguite in questo turno (26/11, ore 09:07)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` -> **exit 0** (ctest 100% 27/27, 137.08 s; prova_gate ricompilato su renderer.cpp 09:05:44).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_luce_galleria_lv02_atomico_2611_0910" lv_02_atomico percorso` -> **exit 0** (GATE PERCORSO OK: 3586 oggetti, 5200 seg, rettilineo 59%, curva max 0.85/0.85, 2476 frame/41.3 s @60 Hz, fuori strada 0 (0.0%), caricamento 5.5 ms, init 14.1 ms; lv_02_atomico del commento e del tunnel, diverso da lv_01_neon del turno prima).
- gate_t1.sh: non eseguito in questo turno.
