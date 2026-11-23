# RIASSUNTO developer (max 12 righe)
- Data/ora: 23/11/2026 07:16 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_01_neon (trentaduesimo giro).
- File codice: `motore_nuovo/src/road/road.cpp` (Road::project: `(1.0f - tStart)` e' costante per segmento ma veniva ricalcolato a ogni campione in `passo`, `t` e `tMezzo` — 4 sample x centinaia di segmenti/frame; hoistato in `unoMenoTStart`, mezzo-passo di `tMezzo` in `mezzoPasso`; stessi operandi e ordine -> stessi float di x/dx/curve e screenX, resa e replay IDENTICI, frameHash lv_01 0 3594390208 invariante).
- Build: `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0.
- Smoke post: lv_01_neon medio 0.660 max 3.080 ms (frame 0), replay IDENTICO, frameHash 0 3594390208 invariante.
- Percorso lv_01_neon: exit 0 "GATE PERCORSO OK" (1989 frame, 33.2 s, 0% fuori).
- Gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 23/11/2026 07:16 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (conteggi giro identici ai riferimenti su tutti e 6 i livelli; smoke lv_01 medio 0.791 max 3.195 ms; nessuna riga FALLITO).
- Esito: frameHash invariato, replay IDENTICO, nessuna regressione; ms/frame ben sotto 8 ms. Ottimizzazione in road.cpp (non renderer.cpp) come da rotazione: tocca la proiezione di tutti i livelli ma bit-identica. Prossimo V9 ∞ lv_02_atomico (trentaduesimo giro).
