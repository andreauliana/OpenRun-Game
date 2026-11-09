# ULTIMO TURNO @developer-junior — 09/11/2026 12:2x
- Blocco/passo: MIG1 J3 (sportellata a un rivale) + J4 (review).
- Fatto: riscritto il modo `sportellata` in `motore_nuovo/test/prova_gate.cpp`: bersaglio solo rivali (scala 1.7, traffico 1.4), evita il traffico piu' vicino, misura la velocita' reale del rivale in finestre da 1 s (0-8 s) per sbandata e ripresa; aggiunta diagnostica su rivali visibili/min dz.
- Esito J3: **nessuna sportellata a un rivale in 30 s** (exit 28). La vecchia evidenza "colpo 0.52 s" era un urto col traffico: non prova D2.
- Rilievo per @developer: il ramo laterale (game.cpp ~2371-2392) arriva dopo il tamponamento (~2119-2138) che copre lo stesso dz e mette `playerBump_`, che il ramo laterale richiede falso. Da correggere in `src/` (fuori dal mio perimetro).
- J4: `UFFICIO/out/developer-junior/REVIEW_MIG1.md` riscritta; D1-D5 [x] verificati a codice (D4 `NOMI_RIVALI`, D5 `suonaPaletto`).
- File cambiati: `motore_nuovo/test/prova_gate.cpp`, `REVIEW_MIG1.md`, diario, RIASSUNTO, questo file.
- Comandi: `cmake --build UFFICIO/Officina/build --target prova_gate` exit **0**.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj3_rivale" lv_01_neon sportellata` exit **28** (FALLITO, finding D2).
- Gate_t1.sh: **non eseguito** in questo turno.
