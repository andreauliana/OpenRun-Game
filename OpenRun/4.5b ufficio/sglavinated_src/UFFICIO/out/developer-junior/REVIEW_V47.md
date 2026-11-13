# REVIEW V47 (@developer-junior)
Riscritto 13/11 09:57 — VJ5 ∞

## VJ5 13/11 09:57 — review ultimo cambio `src/` (`motore_nuovo/src/game/fasi.cpp`, da6c52b)
Contesto: `git log -5 --oneline -- motore_nuovo/src` (exit 0) conferma da6c52b "HIGH SCORES: puntini fra nome e punti..." come ultimo cambio in `src/` (nessun nuovo commit). Analisi `disegnaPunteggi`.

- **Rilievo (incoerenza visiva, invariato)** `fasi.cpp:576`: puntini disegnati con `e.nostro ? 0xFFFFFFFFu : 0xFF9898B0u`; per riga `nostro` restano bianco fisso mentre nome/posizione/punti usano `col` lampeggiante calcolato a riga 567 (`std::fmod(passoS_, 0.5f) < 0.3f ? ACCENTO : 0xFFFFFFFFu`), quindi i puntini non seguono il blink.
- **Incoerenza commento** `fasi.cpp:574-575`: indica puntini "grigi come i separatori", ma il ramo `nostro` usa bianco fisso.
- **Semplificazione suggerita**: usare `e.nostro ? col : 0xFF9898B0u` per coerenza col blink, oppure sempre `0xFF9898B0u` se separatori fissi; allineare commento.

Nessun cambio applicato (sola lettura `src/`). Comando turno: `git log -5 --oneline -- motore_nuovo/src` exit 0. gate_t1.sh non eseguito. Nessun cambio src/test/dati.
