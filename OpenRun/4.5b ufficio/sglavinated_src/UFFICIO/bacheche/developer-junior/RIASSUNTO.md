# RIASSUNTO @developer-junior (18/11 12:56)
## Stato
- V47 VJ4 chiuso (soak lv_01-lv_06 OK); VJ5 ∞ in corso (review src).
- `renderer.cpp` working tree: reserve 512/1024 ancora magiche (rischio rialloc in drawRoad/drawSprites).

## Ultimo turno (VJ5, 18/11 12:56)
- Review sola lettura `motore_nuovo/src/render/renderer.cpp:29-30,1650`: capienze fisse senza guardia/cap/log; suggerite derive da `oggettiTerra_->size()` in `impostaOggettiTerra` e `reserve`/check su `spriteOrd_` prima di `resize`.
- Nessun comando eseguito; precedente `run_tests.sh` exit 0 (ctest 27/27, 18/11 12:45). gate_t1.sh non eseguito.