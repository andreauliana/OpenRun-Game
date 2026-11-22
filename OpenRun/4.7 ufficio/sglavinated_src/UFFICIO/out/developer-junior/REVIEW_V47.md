# REVIEW V47 — VJ5 ∞ luceEsterna/soffittoRiga (22/11/2026 11:52)

- **Ruolo**: @developer-junior (review codice @developer, nessuna modifica a src).
- **Blocco**: V47 VJ5 ∞. `git diff --unified=3 -- motore_nuovo/src/render/renderer.cpp` (ultimo cambio src, hunks V9 galleria); nuovo rilievo rispetto al turno 11:40 (imbocco bayerRiga).

## VJ5 — review `renderer.cpp` galleria luceEsterna/soffittoRiga
- **File/riga**: `motore_nuovo/src/render/renderer.cpp:848-850` definisce `luceEsterna(c,g,x,bayerRiga)`; unico uso residuo `:967` in `parete`; `soffittoRiga` `:971-992` inlinea la stessa logica con `sogliaLuceG=g*12.0f` `:985-988` senza chiamare `luceEsterna`.
- **Nessun errore comportamentale**: la condizione/lerp in soffittoRiga è identica (`bayerRiga[x&3] < g*12.0f ? lerpColore(c,tema_.cielo,0.30f) : c`), `bayerRiga` impostata per riga `:994` prima delle chiamate. Resa e replay IDENTICI.
- **Semplificazione**: duplicazione logica luce esterna; `luceEsterna` ha ora un solo call site. Possibile inline a `:967` oppure riuso del helper in soffittoRiga per ridurre duplicazione. Rilievo doc persistente `:477` "riga 408" (fuori hunk).

## Prove (22/11 11:52)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — **exit 8** (ctest 26/27, 151.11 s, prova_gate ricompilato). FAIL unico preesistente: `gate_decorazioni` lv_04_cromo `skyline.futurista` mancante (`GRAFICA/scenari/lv_04_cromo/skyline.png`).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_lv02_atomico_luceesterna_sogliag_2211_t2" lv_02_atomico percorso` — **exit 0** (GATE PERCORSO OK, 2476 frame/41.3 s @60 Hz, fuori strada 0).
- `gate_t1.sh` non eseguito.

## Stato
- VJ5 ∞ prosegue: prossimo turno rileggere ultimo cambio `src/` (hunk/file diverso da luceEsterna/soffittoRiga galleria). Fail gate_decorazioni skyline lv_04_cromo preesistente.