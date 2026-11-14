# RIASSUNTO @developer-junior
Riscritto 14/11 10:09 (turno V47 VJ5 ∞).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate`; sola lettura `src/`, nessun codice prodotto senza autorizzazione.
- **VJ3 CHIUSO 6/6**: 60/50 Hz replay IDENTICO su tutti; delta ok tranne lv_03 4.44% (segnalato a @developer).
- **VJ4 CHIUSO 6/6 soak**: tutti exit 0, replay IDENTICO, nessun crash; lv_05 medio 2.013 max 42.081, lv_06 medio 1.536 max 8.926.
- **VJ5 ∞ 14/11 10:09**: `git log -1 --oneline -- motore_nuovo/src` exit 0, `git status --short -- motore_nuovo/src` exit 0, ultimo `da6c52b`. Review `fasi.cpp:512-520` (`salva`/tabella HIGH SCORES): se `posizione_>10` dopo sort+resize sostituisce `tabella_.back()` col giocatore, rompendo l'ordine per punteggio in decima riga.
- Rilievi VJ5 aperti: `show.cpp:141-145` off-road reset/notifica ogni frame; `game.cpp:2377-2383` finestra affiancata ampia/asimmetrie; `fasi.cpp:572-576` puntini colore `nostro`; `fasi.cpp:512-520` ordine tabella forzata.
- Da fare a mano (fuori perimetro): `git checkout -- salvataggi/giocatore.json`.
- Prossimo passo ∞: VJ5 review altro file/cambio `src/` in `REVIEW_V47.md`.
- gate_t1.sh non eseguito in questo turno.