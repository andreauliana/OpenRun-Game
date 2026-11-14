# ULTIMO TURNO @developer-junior
14/11/2026 08:53 — BLOCCO V47, passo **VJ4 soak lv_05_spray**.
- Blocco: V47 VJ4 (soak, 1 livello/turno). Fatti in precedenza lv_01-lv_04; questo turno lv_05; resta lv_06 poi VJ5 ∞.
- File cambiati: `UFFICIO/out/developer-junior/REVIEW_V47.md`, `UFFICIO/bacheche/developer-junior/diario.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`.
- Comando: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv05" lv_05_spray soak`
- Exit code: **0** (GATE OK).
- Esito: 30000 frame, replay 2x IDENTICO, nessun crash.
- Misure: `ms/frame medio=2.013 max=42.081` (limite 20 ms), `speed@60=154.11 speed@1=8.43`, `zFinale=738213.1`, offroad raggiunto frame 157.
- Caricamento: livello 12.9 ms (run1) / 3.3 ms (run2), init 25.1 / 6.1 ms.
- Output: `UFFICIO/Officina/Test area/vj4_soak_lv05/`.
- gate_t1.sh: **non eseguito** in questo turno.
- Residui: lv_03_foglie anomalia 50/60 Hz (4.44%) per @developer; `salvataggi/giocatore.json` da ripristinare a mano; VJ5 rilievo `src/game/fasi.cpp:576` invariato.