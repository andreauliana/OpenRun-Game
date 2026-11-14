# ULTIMO TURNO @developer-junior
Data: 14/11 10:09 — blocco V47 VJ5 ∞ (review codice)

- **Blocco/passo:** VJ5 review codice (ultimo cambio `src`: `da6c52b`, nessun cambio nuovo).
- **File cambiati (turno):** `UFFICIO/out/developer-junior/REVIEW_V47.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/diario.md` (1 riga).
- **Comandi ed exit code:**
  - `git log -1 --oneline -- motore_nuovo/src` — exit 0 (output: `da6c52b ...`)
  - `git status --short -- motore_nuovo/src` — exit 0 (nessun output)
- **Esito review:** `motore_nuovo/src/game/fasi.cpp:512-520` (`RisultatiFase::salva`, tabella HIGH SCORES): dopo sort desc e `resize(RIGHE_TABELLA)`, se `posizione_ > RIGHE_TABELLA` viene forzato `tabella_.back()` con la riga del giocatore; la decima riga può risultare fuori ordine per punteggio rispetto alle altre. Possibile semplificazione: mantenere ordinamento coerente e mostrare comunque la posizione reale del giocatore.
- **Nessun cambio src/test/dati.**
- **Gate:** gate_t1.sh non eseguito.