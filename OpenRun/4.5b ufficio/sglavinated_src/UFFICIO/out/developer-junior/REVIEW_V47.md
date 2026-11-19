# REVIEW V47 — VJ4 lv_01 soak + review diff src (19/11/2026 12:58)

- **Ruolo**: @developer-junior (VJ4 soak 1/6 + VJ1 prova dopo passo V @developer + review diff src, nessuna modifica a src).
- **Oggetto src** (letto, non modificato): M `motore_nuovo/src/render/palette.h` (+`const u32* dati() const`), M `motore_nuovo/src/render/renderer.cpp` (9+/1-, V9 drawSprite): hoist `const i32 palBase = s.paletteBase` e `const u32* const palRaw = pal_.dati()`, lookup `idx==0 ? 0u : palRaw[(palBase+idx)&255]` al posto di `pal_[s.paletteBase+idx]`.

## Esito review src: NESSUN ERRORE RILEVATO
- **Correttezza**: prima `Palette::operator[]` faceva `colors_[idx & 255]` su `colors_[(s.paletteBase+idx)&255]`; ora stessa maschera `&255` su `palRaw = colors_.data()` con stesso base -> stesso colore per ogni texel. `idx==0` resta trasparente. `palBase`/`palRaw` costanti nel drawSprite. Replay/resa invariati (soak IDENTICO).

## VJ4 Soak (19/11 12:58) — lv_01_neon (1/6)
- `sh UFFICIO/Officina/TOOL/run_tests.sh` **exit 0** (ctest 27/27, 248.79 s, prova_gate ricompilato).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv01_soak_1911" lv_01_neon soak` **exit 0** (GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.552 max 7.964 frame 12506, nessun crash, caricamento 4.0/3.9 ms, init 11.0/9.4 ms).
- `gate_t1.sh` non eseguito.

## Prossimo
- VJ4 soak lv_02_atomico (2/6). VJ5: review prossimo diff src @developer se presente (aperte note renderer: `:1847` OR ordine, init morta `rigaTex` `:1782`, `screenYRow` solo in `inTubo` `:1861`).