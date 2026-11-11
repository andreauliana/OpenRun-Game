# ULTIMO TURNO — @developer-junior (11/11 12:55)

- Blocco/passo: V47 VJ5 ∞ (review codice src, file nuovo rispetto ai turni recenti).
- File cambiati: `motore_nuovo/test/prova_gate.cpp` (nota VJ5 12:55 su salvataggi), `UFFICIO/out/developer-junior/REVIEW_V47.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/bacheche/developer-junior/diario.md`.
- Review: `motore_nuovo/src/game/salvataggi.cpp`/`.h` — `creaDirectorySeManca()` ritorna false su `EEXIST` e probe `fopen` dir inaffidabile; `salva()` non normalizza nomi/data della classifica come fa `carica()`.
- Comandi ed exit code:
  - `cmake --build UFFICIO/Officina/build --target prova_gate` → exit 0.
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_salvataggi_1255" lv_01_neon caricamento` → exit 0 (GATE CARICAMENTO OK, init 8.3 ms).
- gate_t1.sh non eseguito.
- Problemi residui: rilievi salvataggi non corretti (solo review/nota); VJ2 in attesa ispezione visiva Andrea.
