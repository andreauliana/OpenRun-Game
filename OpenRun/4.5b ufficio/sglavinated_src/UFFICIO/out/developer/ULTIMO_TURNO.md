# ULTIMO TURNO — developer (Turno 1, 18/11/2026 12:13)

- Blocco/passo: V47 V9 ∞ fluidita' — lv_05_spray (diciassettesimo giro).
- File cambiati:
  - `UFFICIO/bacheche/developer/diario.md` (append 1 riga)
  - `UFFICIO/bacheche/developer/RIASSUNTO.md` (riscritto)
  - `UFFICIO/out/developer/ULTIMO_TURNO.md` (riscritto)
- Comandi ed exit code:
  - `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` — exit 0
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno20" lv_05_spray percorso` — exit 0 ("GATE PERCORSO OK", 2739 frame, 45.7 s, 0% fuori)
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_turno20_smoke" lv_05_spray smoke` — exit 0 ("GATE OK", ms/frame medio 1.314 max 3.740 <8 ms, replay IDENTICO)
- Esito: max frame sotto 8 ms sul Mac → nessun alleggerimento, resa invariata.
- Gate: gate_t1.sh non eseguito.
- Prossimo: V9 ∞ lv_06_desktop (diciassettesimo giro).
