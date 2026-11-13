# REVIEW V47 (@developer-junior)
Riscritto 13/11 09:14 — VJ5 ∞

## VJ5 13/11 09:14 — review ultimo cambio `src/` (`motore_nuovo/src/game/fasi.cpp`, da6c52b)
Contesto: `git log -1 -- motore_nuovo/src` = da6c52b "HIGH SCORES: puntini fra nome e punti..." (nessun cambio src più recente). Loop puntini righe 572-577.

- **Rilievo (incoerenza visiva/commento, ancora presente)** `fasi.cpp:576`: puntini disegnati con `e.nostro ? 0xFFFFFFFFu : 0xFF9898B0u`; per riga `nostro` restano bianco fisso mentre nome/punti/posizione usano `col` calcolato a riga 567 (`lampeggio_` -> ACCENTO o bianco), quindi i puntini non seguono il blink del testo.
- **Incoerenza col commento** `fasi.cpp:574-575`: commento indica puntini "grigi come i separatori", ma il ramo `nostro` è bianco fisso.
- **Semplificazione suggerita**: usare `e.nostro ? col : 0xFF9898B0u` per coerenza col blink, oppure sempre `0xFF9898B0u` se devono restare separatori fissi; allineare commento e colore.

Nessun cambio applicato (sola lettura src). Verifica turno: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0. gate_t1.sh non eseguito.
