# ULTIMO_TURNO developer
- Data/ora: 12/11/2026 09:32 (Turno 1)
- Blocco: V47 V9 ∞ fluidita' a giro (terzo giro)
- Passo: lv_06_desktop percorso + smoke misurati; nessuna modifica a codice/dati di gioco (max frame 4.747 ms < 8 ms, resa invariata).
- File cambiati: `UFFICIO/bacheche/developer/diario.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md`
- Comandi ed exit code:
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno3" lv_06_desktop percorso` -> exit 0 ("GATE PERCORSO OK", 2114 frame, 35.2 s, fuori 0.0%)
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno3_smoke" lv_06_desktop smoke` -> exit 0 ("GATE OK", ms/frame medio 1.268 max 4.747, replay IDENTICO)
- Gate: gate_t1.sh non eseguito. Terzo giro V9 completo (lv_01-lv_06).
- Prossimo: V9 ∞ fluidita' lv_01_neon (quarto giro).