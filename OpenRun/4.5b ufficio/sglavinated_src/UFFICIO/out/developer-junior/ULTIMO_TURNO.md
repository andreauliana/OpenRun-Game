# ULTIMO TURNO — @developer-junior (20/11/2026 11:10)

- **Blocco/passo**: V47 VJ4 soak 5/6 su lv_05_spray (+ VJ5: review nuovi hunk src).
- **File cambiati**: nessuno di prodotto. Solo `UFFICIO/out/developer-junior/REVIEW_V47.md`, questo file, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, una riga in `diario.md`.
- **Comandi**: `sh UFFICIO/Officina/TOOL/run_tests.sh` -> exit 0 (ctest 27/27, 226.78 s; prova_gate ricompilato).
- **Comandi**: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv05_soak_2011" lv_05_spray soak` -> exit 0 (GATE OK).
- **Misure soak**: 30000 frame, replay 2x IDENTICO, nessun crash, medio 1.486 ms/frame, max 14.066 ms (frame 25351), caricamento 3.7/3.1 ms, init 11.0/8.8 ms, offroad frame 156, 3441 oggetti, 603 cartelloni.
- **Esito**: PASS (media sotto 20 ms). Max 14.066 ms sopra la soglia V9 di 8 ms: segnalato per V9 lv_05.
- **VJ5**: nessun errore nei nuovi hunk (`centroNear/centroFar` precalcolati, `calcolaIncrocio` fuori dal ciclo righe, LUT `colLut_`, lazy tx0/tx1); invarianti verificate, replay identico. Semplificazione: `fineRiga = rigaCercaBase + pCol->tx1` elimina `largh`.
- **Prossimo**: VJ4 soak lv_06_desktop (6/6); VJ5 sul prossimo diff src.
- `gate_t1.sh`: non eseguito.
