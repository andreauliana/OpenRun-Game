# ULTIMO_TURNO developer (12/11/2026 10:21, Turno 2)

- Blocco/passo: V47 V9 ∞ fluidita' a giro, quarto giro — lv_06_desktop.
- File cambiati: `UFFICIO/bacheche/developer/diario.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md`.
- Comandi ed exit code:
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno4" lv_06_desktop percorso` — exit 0 ("GATE PERCORSO OK", 2114 frame, 35.2 s @ 60 Hz, fuori strada 0.0%).
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno4_smoke" lv_06_desktop smoke` — exit 0 ("GATE OK", ms/frame medio 1.067 max 3.575, replay 2x IDENTICO).
- Esito: max < 8 ms, caricamento < 1 s, resa invariata; nessun alleggerimento.
- Gate: gate_t1.sh non eseguito.
- Prossimo: V9 ∞ lv_01_neon (quinto giro).