# RIASSUNTO @developer-junior
Riscritto 15/11 10:43 (turno VJ5 ∞).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate`; sola lettura `src/`, nessun codice prodotto senza autorizzazione.
- VJ3 CHIUSO 6/6 (lv_03 4.44% segnalato). VJ4 CHIUSO 6/6 soak. VJ5 ∞ attivo.
- 15/11 10:43: review `game.cpp:1809-1814` (`lentaCurva` moltiplica per `r.bravura` ma commento dice "bravi soffrono meno in curva": rallentamento invertito). Modificato `motore_nuovo/test/prova_gate.cpp` (commento VJ5). `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0, `prova_gate .../vj5_replay_turno3 lv_01_neon replay` exit 0 (GATE REPLAY OK). gate_t1.sh non eseguito.
- Rilievi aperti: `game.cpp:1809-1814` (bravura/curva rivali), mixer `:40-76/:108-110/:112-116/:127-130/:91-99`, `game.cpp:912/:2234/:2812/:2794`+`replay.cpp:20`, `replay.cpp:9,20`, `rng.h:19`, `salvataggi.cpp:78-83`, `main.cpp:227-233`, `fasi.cpp:512-520/:576`, `show.cpp:141-145`, audio 50 Hz `game.h:49`/`game.cpp:511`/`main.cpp:344,346`.
- Stato src: ultimo `57ac3f5`. Da fare a mano: `git checkout -- salvataggi/giocatore.json`. Prossimo ∞: altra review `src/`.