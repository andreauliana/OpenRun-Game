# ULTIMO TURNO developer
- Data/ora: 13/11/2026 09:18 (Turno 1)
- Blocco: V47 V9 ∞ fluidita' a giro — lv_02_atomico (sesto giro). Passo: misura percorso+smoke; verifica <8 ms.
- File cambiati (codice/test/dati): nessuno (misure entro soglia, nessun alleggerimento necessario).
- Comandi ed exit code:
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno6" lv_02_atomico percorso` → exit 0, "GATE PERCORSO OK" (2466 frame, 41.1 s, 0% fuori, car 24.4 ms init 41.0 ms).
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno6_smoke2" lv_02_atomico smoke` → exit 0, "GATE OK", ms/frame medio 5.704 max 45.135 (freddo), replay 2x IDENTICO.
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv02_turno6_smoke3" lv_02_atomico smoke` → exit 0, "GATE OK", ms/frame medio 1.262 max 4.613 (<8 ms), replay 2x IDENTICO, car 5.1-6.2 ms init 8.5-11.4 ms.
- Gate: gate_t1.sh non eseguito.
- Esito: livello gia' sotto soglia, nessun cambio di resa/codice. Prossimo V9 ∞ lv_03_foglie (sesto giro).