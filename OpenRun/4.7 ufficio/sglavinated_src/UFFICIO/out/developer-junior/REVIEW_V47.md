# REVIEW V47 — VJ5 ∞ review dell'hoist `limiteStrisce` (21/11/2026 11:37)

- **Ruolo**: @developer-junior (review del codice del @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞ — ultimo cambio del @developer in `src/`. `git status --short`: unico M in `src/` e' `motore_nuovo/src/render/renderer.cpp` (gli altri M sono documenti di coordinamento); `git diff -- motore_nuovo/src/render/renderer.cpp` exit 0. Il diff ha tre hunk: `shiftSchiuma` (rivisto 11:13) e `incStrisce` (rivisto 11:26) sono gia' esaminati; il NUOVO in questo turno e' il secondo.

## VJ5 — review di `renderer.cpp`, hunk "V9 ∞ (21/11, lv_06_desktop giro 27)"
- Cambio nuovo in `drawSegmentTrapezoid` (`renderer.cpp:520-528`): `const f32 limiteStrisce = hw - 0.25f * hwBase;` calcolato una volta fuori dal `while` che conta `stricePerLato` (:528), prima ricalcolato a ogni iterazione. Livello diverso dal turno prima (`lv_06_desktop` vs `lv_05_spray`): e' un hunk nuovo e distinto, benche' nello stessa funzione.
- **Nessun errore trovato**: `hw` e' `const f32` (:449) e `hwBase` e' `const f32` (:515); il `while` incrementa solo `stricePerLato`, quindi entrambi sono loop-invarianti. `0.25f` e' una potenza di 2 esatta: `0.25f * hwBase` e' uno scaling esatto e la sottrazione `hw - (...)` ha stessi operandi e stesso ordine -> f32 bit-identico (anche con FMA). Stessa condizione `<` -> stesso `stricePerLato`, stesse corsie, resa e replay IDENTICI. Coerente con gli altri hoist V9 (hwDelta, zAvanti, sogliaBayer).
- **Semplificazione (con riga)**: `renderer.cpp:520-526` — il commento di 7 righe documenta una sola riga di codice; accorciabile a 1-2 righe nello stile degli altri commenti V9.
- **Nota sul guadagno**: l'espressione e' loop-invariante, quindi un compilatore a -O2 la hoista verosimilmente gia' da solo; il beneficio misurabile e' marginale. Non funzionale.
- **Rilievo documentale persistente**: il commento all'hoist `zAvanti` (`renderer.cpp:460`) cita ancora "a riga 408 (dichiarazione)"; la dichiarazione e' ora a :464 (:408 e' `grassQui`). Segnalato piu' volte, non tocca codice eseguito.

## Prove (eseguite in questo turno, 21/11 11:37)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 138.17 s; `prova_gate` ricompilato prima della prova, come richiede VJ5).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv06_desktop_limitestrisce_2111" lv_06_desktop percorso` **exit 0** (GATE PERCORSO OK; 4840 segmenti, giro guidato 2125 frame = 35.4 s @ 60 Hz, fuori strada 0, caricamento 4.0 ms, init 12.0 ms).
- `gate_t1.sh` non eseguito questo turno.

## Stato
- VJ4 soak chiusa 6/6. VJ5 ∞ prosegue: prossimo turno rileggere l'ultimo cambio `src/` se cambia di nuovo (livello/file diverso da `lv_06_desktop`/`drawSegmentTrapezoid`). ctest 27/27 verde qui; la flakiness replay nei sottmodi smoke segnalata il 21/11 10:20 resta sotto osservazione (qui non riparsa).
