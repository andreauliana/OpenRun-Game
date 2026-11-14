# ULTIMO TURNO @developer-junior
14/11/2026 08:35 — BLOCCO V47, passo **VJ3 chiuso (6/6 livelli)** + chiusura buco del test.
File cambiati:
- `motore_nuovo/test/prova_gate.cpp`: estratto helper `misuraGiroFrequenza` (misura riusabile, soglia di sterzo normalizzata al passo), nuovo `verificaFrequenzaConfronto` + modo `frequenza_confronto` (soglia 2.0%, exit 22), riga di aiuto aggiornata.
- `UFFICIO/out/developer-junior/REVIEW_V47.md`, `UFFICIO/bacheche/developer-junior/{diario,RIASSUNTO}.md`, questo file.
Comandi ed esiti (14/11, uno per riga, da `sglavinated_src/`):
- `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0.
- `prova_gate .../lv_05_60 lv_05_spray frequenza 60` -> exit 0, 45.03 s, replay IDENTICO.
- `prova_gate .../lv_05_50 lv_05_spray frequenza 50` -> exit 0, 44.92 s, IDENTICO (delta 0.24%).
- `prova_gate .../lv_06_60 lv_06_desktop frequenza 60` -> exit 0, 35.23 s, IDENTICO.
- `prova_gate .../lv_06_50 lv_06_desktop frequenza 50` -> exit 0, 35.20 s, IDENTICO (delta 0.09%).
- `prova_gate .../confronto_lv_03 lv_03_foglie frequenza_confronto` -> **exit 22**, 39.73 vs 41.58 s, diff 4.44% > 2.00% (anomalia nota, @developer).
- `prova_gate .../confronto_lv_06 lv_06_desktop frequenza_confronto` -> exit 0, 35.23 vs 35.20 s, diff 0.09%.
Uscite in `UFFICIO/Officina/Test area/vj3_14nov/`. gate_t1.sh: **non eseguito** in questo turno.
Residuo: lv_03 perde 1.85 s di giro a 50 Hz (costo per-frame non scalato da `dt`); lv_05 e' il livello piu' pesante (2.44-2.60 ms/frame).
