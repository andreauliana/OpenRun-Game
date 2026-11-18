# RIASSUNTO developer (max 12 righe)
- Data/ora: 18/11/2026 12:13 (Turno 1)
- V9 ∞ fluidita' lv_05_spray (diciassettesimo giro).
- Build: `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0.
- Percorso (v9_lv05_turno20) exit 0 "GATE PERCORSO OK" (2739 frame, 45.7 s, 0% fuori).
  Cmd: ./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno20" lv_05_spray percorso
- Smoke (v9_lv05_turno20_smoke) exit 0 "GATE OK" ms/frame medio 1.314 max 3.740 (<8 ms), replay IDENTICO.
  Cmd: ./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno20_smoke" lv_05_spray smoke
- Max frame sotto soglia 8 ms: nessun alleggerimento (resa invariata).
- gate_t1.sh non eseguito.
- Prossimo V9 ∞ lv_06_desktop (diciassettesimo giro).
- Vincoli: HUD/auto giocatore/runner/costanti gameplay/tracciati invariati.
