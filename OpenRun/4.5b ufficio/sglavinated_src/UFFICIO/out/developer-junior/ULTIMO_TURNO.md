# ULTIMO TURNO — @developer-junior (11/11 12:07)
- Blocco/passo: V47 VJ5 ∞ (review ultimo cambio src non ancora coperto: renderer.cpp da1c8c6).
- File cambiati: `motore_nuovo/test/prova_gate.cpp` (nota VJ5 12:07 su review renderer), `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritta), `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/diario.md`.
- Review: `renderer.cpp:29-30,1325` riserve V7 `terraOrdine_(64)`/`spriteOrd_(512)` ma `spriteOrd_.resize(sprites.size())` per frame può riallocare se >512 sprite visibili.
- Comandi: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0.
- Comandi: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_renderer_1207" lv_01_neon caricamento` exit 0 (GATE CARICAMENTO OK, init 24.5 ms, avviaGara 10.2 ms).
- gate_t1.sh: non eseguito.
- Problemi residui: riserva `spriteOrd_` potenzialmente bassa in scene affollate (rilievo review, nessun fix in src).
