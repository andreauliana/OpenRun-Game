# REVIEW V47 (riscritta) — @developer-junior
Turno 12/11 09:11, VJ5 ∞.

## Cambio rivisto
- File: `motore_nuovo/src/road/road.h` (file diverso dai turni prima: partenza.cpp, salvataggi.cpp, fasi.cpp, libretro.cpp, game.cpp/player.*). Review statica; ultimo cambio @developer in src non verificabile via git (shell non disponibile).

## Rilievi
1. **road.h:107 — membro pubblico `Road::acqua` inutilizzato/duplicato:** `class Road` dichiara `f32 acqua = 0.0f;` (riga 107) con commento duplicato alle righe 104-106, ma il campo acqua di tracciato e' gestito da `SegmentDesc::acqua` (riga 70) e dai campi dei segmenti/proiezione (`seg.acqua`, `ps.acqua`); nessun uso di `Road::acqua` come membro dell'istanza nel sorgente. Semplificazione: rimuovere membro e commento morto per evitare confusione con `SegmentDesc::acqua` e `acquaAt()`.
2. **road.h:97-99, 129-132 — API incroci/acquaAt coerenti:** `incrocioD` usa indice segmento + `incrociZ_` con sentinel 1e9f; `acquaAt/sospesoAt/galleriaAt` delegano a `indiceSegmento` (ritorno -1 gestito). Nessun errore evidente letto.
3. **Nota correlata (road.cpp:339):** formattazione compatta `if (bivio_) {        const i32 inizio...` su una riga sola (stile), non funzionale.

## Prova
- `sh UFFICIO/Officina/TOOL/run_tests.sh` non eseguito: shell non disponibile in questo turno (ricompilazione VJ5 non verificabile).
- gate_t1.sh: non eseguito.
