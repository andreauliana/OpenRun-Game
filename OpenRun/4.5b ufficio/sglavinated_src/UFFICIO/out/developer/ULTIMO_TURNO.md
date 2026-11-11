# ULTIMO TURNO — developer (11/11/2026 12:17)

- Blocco/passo: V47 V9 ∞ fluidità a giro — lv_02_atomico (misura frame, nessun alleggerimento: max < 8 ms).
- File cambiati: `UFFICIO/bacheche/developer/diario.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md`.
- Comandi ed exit code:
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno3" lv_02_atomico percorso` — exit 0 (`GATE PERCORSO OK`, 2267 frame, 0% fuori).
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno3_smoke" lv_02_atomico smoke` — exit 0 (`GATE OK`, ms/frame medio 1.004 max 3.580, replay IDENTICO).
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno3_smoke" lv_02_atomico smoke --replay .../replay.bin` — exit 0 (`GATE OK`, replay IDENTICO).
- gate_t1.sh: non eseguito.
- Log: `UFFICIO/Officina/Test area/v9_lv02_turno3/`, `UFFICIO/Officina/Test area/v9_lv02_turno3_smoke/`.
- Prossimo: V9 ∞ lv_03_foglie.
