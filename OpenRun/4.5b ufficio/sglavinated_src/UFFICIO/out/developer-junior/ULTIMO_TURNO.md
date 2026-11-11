# ULTIMO_TURNO @developer-junior — 11/11/2026 08:41 (turno 1)
- **Blocco/passo:** V47 seconda lista **VJ3 50/60 Hz [~]** — fatti **lv_03_foglie** e **lv_04_cromo** (ora 4/6 livelli); più **VJ5** review dell'ultimo cambio src (V7 di @developer).
- **File cambiati:** `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto), `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (riscritto), `UFFICIO/bacheche/developer-junior/RIASSUNTO.md` (riscritto), `UFFICIO/bacheche/developer-junior/diario.md` (1 riga). Nessuna modifica a codice/dati: `git status` su `motore_nuovo/dati/config.json` pulito.
- **Comandi ed exit code (11/11 turno 08:41):**
  1. `cmake --build UFFICIO/Officina/build --target prova_gate` → exit 0 (gia' aggiornato: src più nuovo 08:32, binario 08:33).
  2. `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj3_freq_1111b" lv_03_foglie frequenza 60` → exit 0, 37.88 s, replay IDENTICO.
  3. idem `frequenza 50` → exit 0, 37.88 s. **Diff 0.00%.**
  4. `... lv_04_cromo frequenza 60` → exit 0, 34.47 s, replay IDENTICO; 5. `... frequenza 50` → exit 0, 34.48 s. **Diff 0.03%.**
- **Esito:** tempi di giro uguali a 50 e 60 Hz sui 4 livelli provati; nessun FALLITO; ms/frame medio 1.37-1.87.
- **VJ5 per @developer (V7):** `renderer.cpp:29` `terraOrdine_.reserve(64)` è sotto l'attesa di ~63 oggetti a terra nei 300 segmenti visibili (paesaggio.cpp:244-287) → `push_back` a riga 141 rialloca in loop. Consiglio `reserve(Road::DRAW_DIST + 8)`. Nota minore: `spriteOrd_.reserve(512)` (riga 30) da legare a `game.cpp:3084`.
- **Gate:** `gate_t1.sh` non eseguito questo turno (ultimo exit 0 11/11 ~08:10, @developer).
- **Prossimo passo mio:** VJ3 su lv_05_spray e lv_06_desktop (60 e 50 Hz), poi VJ4 soak.
- **Problemi residui:** J3 [~] attende fix src D2 (`game.cpp:2386/2390`); VJ2 [~] attende ispezione visiva di Andrea.
