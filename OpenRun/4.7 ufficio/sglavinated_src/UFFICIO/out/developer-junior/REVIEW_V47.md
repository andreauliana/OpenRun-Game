# REVIEW V47 — VJ5 ∞ review del nuovo hunk `hshFila` (21/11/2026 10:29)

- **Ruolo**: @developer-junior (review del codice del @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞ — ultimo cambio del @developer in `src/`. `git status --short`: unico M in src = `motore_nuovo/src/render/renderer.cpp` (gli altri M sono documenti di coordinamento e `salvataggi/giocatore.json`, non toccati). `git diff -- motore_nuovo/src/render/renderer.cpp` exit 0.

## VJ5 — review di `renderer.cpp`, hunk "V9 ∞ (21/11, lv_01_neon giro 27)"
- Cambio nuovo in `drawTerraSospesa` (`renderer.cpp:316-333`): il termine dell'hash delle onde che dipende dalla riga `fila` veniva moltiplicato a ogni pixel del mare; ora `hshFila = (u32)(fila * 73856093)` e' calcolato una volta per riga (:324) e riusato nello XOR (:329). Inoltre `cellaF = xw / ONDA_PASSO_X` (:327) e' calcolato una volta e riusato sia per `cella` (:328) sia per `dentro` (:331), dove prima la divisione veniva rifatta due volte per pixel. Livello/file diversi da `lv_06_desktop`/`drawSkyline` del turno precedente.
- **Nessun errore trovato**: `fila` e' const i32 calcolato a :316 e non mutato prima del loop x (:317 e' solo un guard); lo spread `fila * 73856093` e' many-int, non floating point -> nessun rischio FMA, stessi operandi/ordine -> stesso u32. Lo XOR con `(u32)(cella * 19349663)` e' lo stesso termine di prima, solo precalcolato -> stessi bit. `cellaF` e' la stessa divisione `xw / ONDA_PASSO_X` di prima, riusata: `floor(cellaF)` e `cellaF - (f32)cella` danno gli stessi valori bit-identici. `xw`, `xPerPx`, `lastPlayerX_` e `fb_.put` invariati; guard `(hsh % 7u) != 0u` e `dentro < 0.45f` preservati. Resa e replay IDENTICI. Coerente con gli altri hoist V9 gia' approvati (`hwDelta`, `zebraFondo`, `centroDelta`, `skylineTx_`).
- **Semplificazione minore (con riga)**: il commento `renderer.cpp:319-323` (5 righe) e' piu' lungo della riga di codice che documenta; accorciabile a 1-2 righe nello stile degli altri commenti V9. Non funzionale.
- **Micro-nota (non funzionale)**: il commento :322 dice "(XOR associativo)", ma qui non si riassocia nulla: e' semplicemente lo stesso termine di sinistra calcolato fuori dal loop. Formulazione imprecisa, risultato corretto. Il commento non cita inoltre il secondo risparmio (la divisione `xw / ONDA_PASSO_X` ora non viene piu' rifatta in `dentro`).
- **Rilievo documentale persistente**: il commento all'hoist `zAvanti` (`renderer.cpp:446`) cita ancora "a riga 408 (dichiarazione)"; la dichiarazione e' ora a :450. Segnalato l'11:58, riaperto piu' volte, ancora presente (slittato :407 -> :435 -> :439 -> :446 nel tempo): non tocca il codice eseguito.

## Prove (eseguite in questo turno, 21/11 10:29)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 146.77 s; `prova_gate` ricompilato prima della prova, come richiede VJ5).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv01_neon_hshfila_2111" lv_01_neon percorso` **exit 0** (GATE PERCORSO OK; 4540 segmenti, giro guidato 1989 frame = 33.2 s @ 60 Hz, fuori strada 0, caricamento 5.9 ms, init 10.5 ms).
- `gate_t1.sh` non eseguito questo turno. Nota non bloccante: nel log del gate `lv_01_neon` compaiono tre "radio: brano non leggibile musica/*.xm" (beatmastaz, asplajungle4, SepteJungle2): preesistente, non legato all'hoist, gate comunque exit 0.

## Stato
- VJ4 soak chiusa 6/6. VJ5 ∞ prosegue: prossimo turno rileggere il diff `src/` se cambia di nuovo (livello/file diverso da `lv_01_neon`/`drawTerraSospesa`). Flakiness replay nei sottmodi smoke di ctest segnalata al @developer/@capo nel turno 10:20; qui ctest 27/27 verde.
