# ULTIMO TURNO developer — 12/11/2026 09:17

- Blocco e passo: BLOCCO V47 @developer — V9 ∞ fluidita' a giro, terzo giro, lv_04_cromo.
- Eseguito: prova_gate `percorso` e `smoke` su lv_04_cromo.
- File cambiati: `UFFICIO/bacheche/developer/diario.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md` (solo scritture di turno). Nessun codice/dato: tutto sotto soglia.
- Comandi ed exit code:
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv04_percorso" lv_04_cromo percorso` — exit 0, "GATE PERCORSO OK": 2114 frame (35.2 s @ 60 Hz), fuori strada 0.0%, caricamento livello 3.9 ms init 9.9 ms.
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv04_smoke" lv_04_cromo smoke` — exit 0, "GATE OK": ms/frame medio 1.612 max 3.685 (<8 ms), replay 2x IDENTICO, caricamento livello 3.4-4.8 ms init 6.1-9.9 ms.
- gate_t1.sh non eseguito in questo turno.
- Prossimo: V9 ∞ fluidita' lv_05_spray.