# REVIEW V47 — VJ5 ∞ + VJ1 (26/11/2026, turno 17:25-17:46)

- **Ruolo**: @developer-junior (review/collaudo; nessuna modifica a `src/`).

## Stato del diff in questo turno
- `git status --short`/`git diff --stat`/`git log --oneline -8 -- motore_nuovo/src`: **nessun M in `src/`** — l'ultimo commit che tocca src e' `c146cb5` (26/11 10:43), working tree = HEAD; `git diff 8ab971a c146cb5 -- renderer.cpp` = i **soliti 11 hunk** put->direct-write gia' review nei turni 09:21-17:12 (nessun hunk nuovo; l'hunk colore paletto :2054 "lv_06_desktop giro39" e' gia' in `8ab971a`, review precedente).
- Nessun cambio nuovo -> **approfondimento VJ5 ∞ con angolo nuovo**: coerenza del layout/stride fra `put`, `data()` e TUTTI i consumatori del framebuffer (present, CRT, libretro, hash, strumenti di test).

## VJ5 ∞ — angolo "layout/stride condiviso" (put -> direct-write -> consumatori)
- **Scrittura**: `Framebuffer::put` = bounds-check + `buf_[y*WIDTH+x]` (`framebuffer.cpp:17-20`); `data()` restituisce lo stesso `buf_` (`std::array<u32,WIDTH*HEIGHT>`, righe fitte, nessun padding) (`framebuffer.h:25-29`). Gli 11 hunk scrivono lo stesso buffer con lo stesso indice -> pixel identici (già provato nei turni precedenti).
- **Present SDL** (`runner/main.cpp:647-656`): legge `game.fb().data()` con stride `Framebuffer::WIDTH` (`src + y*WIDTH`); il pitch della texture SDL e' gestito a parte nella copia (`dst + y*pitch`). Nessun percorso puo' divergere dagli hunk.
- **CRT** (`render/crt.cpp:23-35`): `crtSbava` legge `src + y*w` con lo stesso `w` passato da main.cpp:650 -> stride WIDTH.
- **Libretro** (`libretro/sglavinated_libretro.cpp:276-277`): `video_cb(data, W, H, W*4)` -> pitch compatto = stride del framebuffer.
- **frameHash** (`game/game.cpp:3898-3903`): hash FNV sull'indice piatto `W*H` -> ogni hunk rientra nell'hash esattamente come con `put` (confermato a runtime: frameHash dei gate identici al @developer).
- **Test** (`test/prova_gate.cpp`, salvaPPM/ring a partire da :640, :2424): leggono `data()` + `W*H`, stessa ipotesi.
- **Esito: nessun errore.** Tutti i produttori e i consumatori condividono la stessa ipotesi di layout (righe fitte WIDTH); gli hunk direct-write sono invisibili a present/CRT/libretro/hash/screenshot. Resa/replay IDENTICI.

## Possibile errore (nuovo, NON di gioco): gate `pali` FALLITO su lv_06 (exit 27)
- `./UFFICIO/Officina/build/prova_gate ".../vj5_pali_lv06_desktop_2611_1730" lv_06_desktop pali` -> **FALLITO exit 27**: `hit_rilevati=0, screenshot=0` (MIG1 J2).
- **Causa, con file/riga**: il driver di `verificaPali` (`test/prova_gate.cpp:2604-2613`) mira a x 400-700 e rientra a ogni offroad; su lv_06 l'auto resta al bordo (x 677-710, offroad a f=300/600/2100) in erba profonda (vel 72-92, mai >150) -> non raggiunge mai la zona paletti `|xRel| >= 860` (`game/game.cpp:3641-3642`) e la soglia di rilevazione `spPrima > 150.0f` (`prova_gate.cpp:2624`) non scatta mai -> hitCount 0.
- **Non e' una regressione**: J2 e' costruito e firmato su lv_01 (09/11, screenshot visti dal @grafico); ctest 27/27 verde; nessun cambio in src/; il codice paletti (D1) e' invariato.
- **Rilievo per @capo/@driver**: il gate `pali` non e' portabile su altri livelli cosi' com'e' (driver + soglia tarati su lv_01). Opzioni: dichiararlo lv_01-only oppure estendere il driver (raggiungere |xRel|>=860 con slancio) con prova di regressione su lv_01.

## Possibili semplificazioni non bloccanti (fuori perimetro, per @developer)
1. (invariato) Helper `rigaScrivibile(y)` in `framebuffer.h` per lo schema guard+`riga[x]` ripetuto (~20 punti di renderer.cpp).
2. (invariato) Hunk segni gomme: iterare solo gli y di parita' giusta (passo 2) invece del test `((y+frame)&1)` a ogni y (`renderer.cpp:3047-3048`).
3. (invariato) Fumo sprite `renderer.cpp:2883`: hoistare `fb_.data()` e usare `(size_t)`.
4. **NUOVO**: `renderer.cpp:3016` (razzo) e `:3048` (segni gomme) rileggono `fb_.data()` a ogni pixel; hoistare come fanno cerchio effetti `:2983` e danni fumo `:2940` (coerenza; nessun cambio di resa).

## RILIEVO BLOCCANTE (procedurale, invariato)
- 5 hunk dentro `Renderer::drawPlayerCar` (:2798-2816, :2875-2889, :2890-2905, :2910-2922, :2932-2952): area intoccabile (REGOLA BLOCCATA Andrea 06/11, ANDREA_REVIEW §2) -> serve autorizzazione esplicita o revert.
- Area grigia `drawEffettiAuto`: segnalata non bloccante (bit-identica), da chiarire con Andrea.

## VJ1 — passo V9 lv_01_neon giro40 del @developer (26/11 17:23)
- Passo di sola misura (nessun file cambiato); prova dedicata mia: **stesso `frameHash 0 3594390208`** del run del @developer -> replay confermato.

## Rilievi invariati (non risolti)
- **VJ3**: 6/6 misurati, 5 entro 2.00%; solo **lv_05_spray 2.40% FALLITO** (deterministico, exit 22) -> aperto per @developer.
- **VJ4**: chiuso 6/6 (soak OK).

## Prove eseguite in questo turno (26/11/2026, 17:25-17:46)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` -> **exit 0** (ctest 27/27, 154.05 s; prova_gate ricompilato, nessun cambio src).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj1_v9_lv01_neon_2611_1728" lv_01_neon smoke` -> **exit 0** (VJ1: GATE OK, frameHash 0 3594390208 identico al @developer, replay 2x IDENTICO, offroad frame 156 -> hunk zolle I eseguito, medio 0.603 max 3.583 ms/frame).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_pali_lv06_desktop_2611_1730" lv_06_desktop pali` -> **exit 27** (FALLITO: paletti non verificati; diagnosi sopra; livello/modo non usati in questi turni).
- `gate_t1.sh`: **non eseguito** in questo turno.
